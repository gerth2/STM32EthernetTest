#include "server.h"


uint8_t serverIsRunning = 0;

static const char *s_debug_level = "3";
static const char *s_listening_address = "ws://0.0.0.0:5800"; //not sure if the prefix matters or not, but his is both for http and ws currently
struct mg_mgr mgr;

//Forward declaraton from autogen file
void handleHttpFileServe(struct mg_connection *c, void *ev_data);

// Event handler for the listening connection.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message *hm = (struct mg_http_message*) ev_data;

		if (mg_http_match_uri(hm, "/websocket")) {
			// Upgrade to websocket. From now on, a connection is a full-duplex
			// Websocket connection, which will receive MG_EV_WS_MSG events.
			mg_ws_upgrade(c, hm, NULL);

		} else {
			handleHttpFileServe(c, hm);
		}
	} else if (ev == MG_EV_WS_MSG) {
	    // Got websocket frame. Received data is wm->data. Echo it back!
	    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
	    mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
	  }


	(void) fn_data;
}

void periodicWSDataSend(void){

	struct mg_connection *c = mgr.conns;

	//Create the message to tx
	char txString[256];
	double curTime = getCurTime();
	double testVal = 10 + 5.0 * sin(2 * 3.41459 * 0.5 * curTime);
	int strlen;

	strlen = sprintf(txString, "{\"time\":%lf, \"accelX\":%lf}",curTime, testVal);


	//For all connections in list...
	while(c != NULL){

		if(c->is_websocket){
			//For all websockets, send the data
			mg_ws_send(c, txString, strlen, WEBSOCKET_OP_TEXT);
		}


		c = c->next; //move on to next connection
	}

}

void serverInit(void) {
	mg_log_set(s_debug_level);
	mg_mgr_init(&mgr);
	LOG(LL_INFO, ("Starting Mongoose v%s", MG_VERSION));  // Tell the world
	mg_http_listen(&mgr, s_listening_address, cb, &mgr);  // Web listener

	serverIsRunning = 1;
}

void serverUpdate(void) {

	if (serverStatus == SERVER_INIT) {
		serverInit();
		serverStatus = SERVER_NOCHANGE;
	}

	if (serverStatus == SERVER_SHUTDOWN) {
		serverShutdown();
		serverStatus = SERVER_NOCHANGE;
	}

	if (serverIsRunning) {
		mg_mgr_poll(&mgr, 1000);   // Event loop
		periodicWSDataSend();
	}
}

void serverShutdown(void) {

	mg_mgr_free(&mgr);                                    // Unreachable

	serverIsRunning = 0;
}
