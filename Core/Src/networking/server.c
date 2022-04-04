#include "server.h"
#include "FreeRTOS_IP_Private.h"

uint8_t serverIsRunning = 0;

static const char *s_debug_level = "4";
static const char *s_listening_address = "http://0.0.0.0:5800";
struct mg_mgr mgr;


// Event handler for the listening connection.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "hello, %s!\n", "world");
  }
  (void) fn_data;
  (void) ev_data;
}


void serverInit(void){
	  mg_log_set(s_debug_level);
	  mg_mgr_init(&mgr);
	  LOG(LL_INFO, ("Starting Mongoose v%s", MG_VERSION));  // Tell the world
	  mg_http_listen(&mgr, s_listening_address, cb, &mgr);  // Web listener


	serverIsRunning = 1;
}

void serverUpdate(void){

	if(serverStatus == SERVER_INIT){
		serverInit();
		serverStatus = SERVER_NOCHANGE;
	}

	if(serverStatus == SERVER_SHUTDOWN){
    	serverShutdown();
		serverStatus = SERVER_NOCHANGE;
	}

	if(serverIsRunning){
		mg_mgr_poll(&mgr, 10);   // Event loop
	}
}

void serverShutdown(void){

	  mg_mgr_free(&mgr);                                    // Unreachable

	serverIsRunning = 0;
}
