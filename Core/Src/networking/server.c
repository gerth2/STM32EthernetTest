#include "server.h"



//////////////////////////////////
// Minnowserver interface
static WssProtocolHandshake wph={0};
static ConnData cd;
static RecData rd;
static MS ms; /* The Minnow Server */
static SOCKET listenSock;
static SOCKET sock;
SOCKET* listenSockPtr = &listenSock;
SOCKET* sockPtr = &sock;

struct{
   U8 rec[1500];
   U8 send[1500];
} msBuf;

/*
  Construct the RecData object used when receiving JSON messages and
  sending response data.

  Notice how we use the static allocators if USE_STATIC_ALLOC is
  set. See JsonStaticAlloc.c for details.

  https://realtimelogic.com/ba/doc/en/C/reference/html/structJParserValFact.html
  https://realtimelogic.com/ba/doc/en/C/reference/html/structJParser.html
 */
static void
RecData_constructor(RecData* o)
{
   memset(o, 0, sizeof(RecData));

   /* Use dynamic allocation */
   JParserValFact_constructor(&o->pv, AllocatorIntf_getDefault(),
                              AllocatorIntf_getDefault());
   JParser_constructor(&o->parser, (JParserIntf*)&o->pv, o->maxMembN,
                       sizeof(o->maxMembN), AllocatorIntf_getDefault(),0);
}


static int
SendData_wsSendJSON(BufPrint* bp, int sizeRequired)
{
   MS* ms;
   SendData* o = (SendData*)bp; /* (Ref-bp) */
   (void)sizeRequired; /* not used */
   /* From SendData_constructor >  BufPrint_setBuf */
   ms = (MS*)BufPrint_getUserData(bp);
   if( ! o->committed )
   {
      xprintf(("ERR: WebSocket send buffer too small\n"));
      baAssert(0);/* This is a 'design' error */
      return -1;
   }

   /* Minnow Server (MS) in large mode. Pad with spaces if size less than
      128 (Ref-Size).
      https://realtimelogic.com/ba/doc/en/C/shark/group__MSLib.html
    */
   while(bp->cursor < 128)
      bp->buf[bp->cursor++] = ' '; /* cursor is current bufsize */
   if(MS_sendText(ms, bp->cursor) < 0)
   {
      xprintf(("WebSocket connection closed on send\n"));
      return -1;
   }
   return 0;
}
/* Construct the SendData container object used for sending formatted JSON.
   We use a BufPrint instance as the output buffer for JEncoder (Ref-bp).
   https://realtimelogic.com/ba/doc/en/C/reference/html/structBufPrint.html
   https://realtimelogic.com/ba/doc/en/C/reference/html/structJEncoder.html
*/
static void
SendData_constructor(SendData* o, ConnData* cd)
{
   int sendBufSize;
   U8* buf;
   if(ConnData_WebSocketMode(cd)) /* Always true if USE_SMQ not set */
   {
      BufPrint_constructor(&o->super, cd->u.ms, SendData_wsSendJSON);
       /* Minnow Server: second arg=TRUE: Set size > 128 (Ref-Size) */
      buf = MS_prepSend(cd->u.ms, TRUE, &sendBufSize);
      /* JEncoder is formatting data via BufPrint directly into MS buffer */
      BufPrint_setBuf(&o->super, (char*)buf, sendBufSize);
   }
   JErr_constructor(&o->err);
   JEncoder_constructor(&o->encoder, &o->err, &o->super);
   o->committed=FALSE;
}

/* Called when we are done creating a JSON message.
 */
static int
SendData_commit(SendData* o)
{
   if(o->committed) return -1;
   o->committed=TRUE;
   /* Trigger SendData_wsSendJSON() or SendData_smqSendJSON() */
   return JEncoder_commit(&o->encoder);
}




/* All JSON messages start with the following: [messagename, ... */
static void
beginMessage(SendData* sd, const char* messagename)
{
   JEncoder_beginArray(&sd->encoder);
   JEncoder_setString(&sd->encoder, messagename);
}

/* All messages end with: ...] */
static int
endMessage(SendData* sd)
{
   JEncoder_endArray(&sd->encoder);
   return SendData_commit(sd);
}




/* Send the one time nonce key to the browser.
  ["nonce", "the-12-byte-nonce-encoded-as-b64"]
 */
static int
RecData_sendNonce(RecData* o, ConnData* cd)
{
   SendData sd;
#ifdef MS_SEC
   sharkssl_rng(o->nonce, sizeof(o->nonce));
#else
   {
      int i;
      for(i=0; i < sizeof(o->nonce) ; i++)
         o->nonce[i] = (U8)rand()%0xFF;
   }
#endif
   SendData_constructor(&sd, cd);
   beginMessage(&sd, "nonce");
   /* Send the 12 byte binary nonce B64 encoded */
   JEncoder_b64enc(&sd.encoder, o->nonce, sizeof(o->nonce));
   return endMessage(&sd);
}

/* Manage binary SMQ and WebSocket frames sent by browser.
   Note that a complete frame may be longer than the data we
   receive. The argument eom (end of message) is true when all data in
   the frame has been consumed. We implement rudimentary state
   management for a frame that is split up into multiple chunks by
   storing the binary message type in RecData:binMsg
*/
static int
RecData_manageBinFrame(RecData* o,ConnData* cd, U8* data, int len,BaBool eom)
{
   if(o->binMsg == 0) /* Not set if first chunk in a frame */
   {
      o->binMsg=data[0]; /* Save binary message type (first byte in frame) */
      data++; /* Set pointer to start of payload */
      len--;
      if(o->messages==0)
      {
         //no messages, or something? TODO handle it
      }
   }
   switch(o->binMsg)
   {
      case BinMsg_Upload:
      case BinMsg_UploadEOF:
          // TODO define and handle various messages
      default:
         xprintf(("Received unknown binary message: %u\n",(unsigned)data[0]));
   }
   if(eom)
      o->binMsg = 0; /* Reset */

   return 0;
}


/* All messages have the form ["message-name", 'message-body']
   This function extracts the message name and deals with the initial
   state when we are not authenticated.
*/
static int
RecData_openMessage(RecData* o, ConnData* cd)
{
   JErr e;
   JVal* v;
   const char* message; /* The JSON message name */
   JErr_constructor(&e);
    /*  A.1: Get JSON message root obj
        A.2: Get the first element in the array, which should be the
             JSON message name
        B.1: Extract the JSON message name
        B.2: Advance to next element, which should be the message body
    */
   v=JVal_getArray(JParserValFact_getFirstVal(&o->pv),&e);
   if((message=JVal_getString(v,&e)) != 0 && (v=JVal_getNextElem(v)) != 0)
   {
      /* Message OK so far, now let's look at the message content */
      //TODO look at message content
      RecData_reset(o);
      return 0;
   }
   else
   {
      xprintf(("Semantic JSON message error\n"));
   }
   return -1;
}


/*
  Parse the JSON 'data' with 'len' bytes received either from a
  WebSocket connection or SMQ.  A very large message may be in a
  WebSocket frame that is too large for the Minnow Server receive
  buffer (or SMQ buffer). The Minnow server (and SMQ) include state
  information telling the application about these conditions. The
  argument 'eom' (end of message) is false as long as we receive
  chunks that is not the last chunk. We do not use large JSON messages
  thus eom should always be TRUE, however, this code is designed to
  manage receiving chunks. Note that the code uses the parse state and
  'eom' for integrity validation.
*/
static int
RecData_parse(RecData* o,ConnData* cd,U8* data,int len,BaBool eom)
{
   int status;
   status = JParser_parse(&o->parser, data, len);
   if(status)
   {
      if(status < 0 || ! eom)
      {
         xprintf(("JSON: %s\n",status<0 ? "parse error":"expected end of MSG"));
         return -1;
      }
      /* Got a JSON message */
      return RecData_openMessage(o,cd);
   }
   else if(eom)
   {
      xprintf(("Expected more JSON"));
   }
   return 0; /* OK, but need more data */
}


/*
   ["settemp", number]
 */
static int sendSampleMessage(ConnData* cd, int temp)
{
   SendData sd;
   SendData_constructor(&sd, cd);
   beginMessage(&sd, "settemp");
   JEncoder_setInt(&sd.encoder, temp);
   return endMessage(&sd);
}

/* The idle function, which simulates events in the system, is called
 * when not receiving data.
 */
static int clientSendPeriodic(ConnData* cd)
{
   //Returns 0 if all sends to client were ok (and we should keep talking to the client),
   // or -1 if any sends failed (and we should probably disconnect)
   return sendSampleMessage(cd, 42.0);
}


/*
  ["DeviceName", ["the-name"]
 */
static int
sendDeviceName(ConnData* cd)
{
   SendData sd;
   SendData_constructor(&sd, cd);
   beginMessage(&sd, "devname");
   JEncoder_beginArray(&sd.encoder);
   JEncoder_setString(&sd.encoder,"FartyMcFartFace");
   JEncoder_endArray(&sd.encoder);
   return endMessage(&sd);
}


static void
RecData_runServer(RecData* rd, ConnData* cd, WssProtocolHandshake* wph)
{
   MS* ms=cd->u.ms;
   /* MS_webServer: Manage HTTP GET or upgrade WebSocket request */
   if( ! MS_webServer(ms,wph) && ! sendDeviceName(cd))
   { /* We get here if HTTP(S) was upgraded to a WebSocket con. */
      int rc;
      U8* msg;
      /* We send the nonce to the browser so the user can
       * safely authenticate.
       */
      RecData_sendNonce(rd, cd);
      while((rc=MS_read(ms,&msg,50)) >= 0)
      {
         if(rc) /* incomming data from browser */
         {
            if(ms->rs.frameHeader[0] == WSOP_Text)
            {  /* All text frames should contain JSON */
               if(RecData_parse( rd,cd,msg,rc,ms->rs.frameLen-ms->rs.bytesRead==0))
               {
                  break; /* err */
               }
            }
            else /* Manage binary WebSocket frames */
            {
               if(RecData_manageBinFrame(rd,cd,msg,rc,ms->rs.frameLen-ms->rs.bytesRead==0))
               {
                  break; /* err */
               }
            }
         }
         else /* timeout (Ref-D) */
         {
            if(rd->authenticated && clientSendPeriodic(cd))
               break; /* on sock error */
         }
      }
      rd->authenticated=FALSE;
      xprintf(("Closing WS connection: ecode = %d\n",rc));
   }
}


static int openServerSock(SOCKET* sock)
{
   int status;
   U16 port;
   port=80;
   status = se_bind(sock, port);
   if(!status)
   {
      xprintf(("WebSocket server listening on %d\n", (int)port));
   }
   return status;
}

uint8_t serverIsRunning = FALSE;

void serverInit(void){

   RecData_constructor(&rd);
   ConnData_setWS(&cd, &ms); /* Set default setup */
   MS_constructor(&ms);


   SOCKET_constructor(listenSockPtr, NULL);
   SOCKET_constructor(sockPtr, NULL);

   wph.fetchPage = fetchPage;

   if(openServerSock(listenSockPtr))
   {
      return;
   }

	//MS_setSocket(&ms, sockPtr, msBuf.rec, sizeof(msBuf.rec), msBuf.send, sizeof(msBuf.send));
	//RecData_runServer(&rd, &cd, &wph);

	serverIsRunning = TRUE;
}

void serverUpdate(void){

	if(serverIsRunning){
		switch(se_accept(&listenSockPtr, 50, &sockPtr))
		{
		 case 1: /* Accepted new client connection i.e. new browser conn. */

			MS_setSocket(&ms, sockPtr, msBuf.rec, sizeof(msBuf.rec), msBuf.send, sizeof(msBuf.send));
			RecData_runServer(&rd, &cd, &wph);
			se_close(sockPtr);

		 break;

		 case 0: /* se_accept 50ms timeout */
			break;

		 default:
			/* We get here if 'accept' fails. This is probably where you reboot. */
			se_close(listenSockPtr);
			return; /* Must do system reboot */ //TODO actually reboot?
		}
	}
}

void serverShutdown(void){
	serverIsRunning = FALSE;
}
