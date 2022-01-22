#ifndef SERVER_H
#define SERVER_H

#include <MSLib.h>
/* JSON lib */
#include <JParser.h>
#include <JEncoder.h>

/* Connection Data: this container object stores either a Minnow
   Server or an SMQ connection.
 */
typedef struct {
   union { /* union: Minnow or SMQ */
      MS* ms; /* MS: Minnow Server */
   } u; /* Union: MS or SMQ */
} ConnData;

typedef struct {
   BufPrint super; /* Ref-bp: Used as super class. Buffer needed by JEncoder */
   JErr err;
   JEncoder encoder;
   BaBool committed; /* Send: If a complete JSON message assembled */
} SendData;
/*
  The binary message types sent from the browser to the server.
  See the JavaScript equivalent in WebSocketCon.js
*/
typedef enum {
   BinMsg_Upload = 1,
   BinMsg_UploadEOF
} BinMsg;
/*

  This container object stores data objects used when receiving JSON
  messages and sending response data.
  JParser:
    https://realtimelogic.com/ba/doc/en/C/reference/html/structJParser.html
 */
typedef struct {
   JParser parser; /* JSON parser */
   JParserValFact pv; /* JSON Parser Value (JVal) Factory */
   S32 messages; /* Used for counting messages received from browser */
   BaBool authenticated; /* Set to true if the user is authenticated */
   char maxMembN[12]; /* Buffer for holding temporary JSON member name */
   /* nonce: One time key created and sent to the browser as part of
      logic for preventing relay attacks */
   U8 nonce[12];
   U8 binMsg; /* Holds the binary message type 'BinMsg' (enum BinMsg) */
} RecData;
#define RecData_reset(o) JParserValFact_termFirstVal(&(o)->pv);



#define ConnData_setWS(o,_ms) (o)->u.ms=_ms
#define ConnData_WebSocketMode(o) TRUE


 /* Fetch the SPA. See index.c for details. */
extern int fetchPage(void* hndl, MST* mst, U8* path);



/** Public API */
void serverInit(void);
void serverUpdate(void);
void serverShutdown(void);

#endif
