#include "server.h"
#include "mongoose.h"

static const char header_html[] = "content-type: text/html\r\n";
static const char header_js[] = "content-type: text/javascript\r\n";
static const char header_css[] = "content-type: text/css\r\n";

$pageData

// Auto-Generated page response handler.
// Returns poiner to requested content, or null if not available.
void handleHttpFileServe(struct mg_connection *c, void * ev_data)
{

   struct mg_http_message *hm = (struct mg_http_message *) ev_data;

   $pageSwitchyard

}