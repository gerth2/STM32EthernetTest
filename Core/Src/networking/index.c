#include "server.h"
#include "mongoose.h"

static const char header_html[] = "content-type: text/html\r\n";
static const char header_js[] = "content-type: text/javascript\r\n";
static const char header_css[] = "content-type: text/css\r\n";

static const char FourOhFour_html[] = "<!DOCTYPE html>\n<html>\n<head>\n<link rel=\"stylesheet\" href=\"main.css\">\n</head>\n<body>\n<h1>ERROR 404</h1>\nThe requested resource was not found.\n</body>\n</html>";

static const char index_html[] = "<!DOCTYPE html>\n<html>\n\n<head>\n    <link rel=\"stylesheet\" href=\"main.css\">\n</head>\n\n<body>\n    <h1>Network IMU</h1>\n    <hr>\n\n    <h3>Status</h3>\n    <table>\n        <tr>\n            <th>Data</th>\n            <th>Value</th>\n            <th>Units</th>\n        </tr>\n        <tr>\n            <td>Time</td>\n            <td id=\"time\">****</td>\n            <td>s</td>\n        </tr>\n        <tr>\n            <td>X Accel</td>\n            <td id=\"xaccel\">****</td>\n            <td>g</td>\n        </tr>\n        <tr>\n            <td>Y Accel</td>\n            <td id=\"yaccel\">****</td>\n            <td>g</td>\n        </tr>\n        <tr>\n            <td>Z Accel</td>\n            <td id=\"zaccel\">****</td>\n            <td>g</td>\n        </tr>\n        <tr>\n            <td>X Gyro</td>\n            <td id=\"xgyro\">****</td>\n            <td>deg/sec</td>\n        </tr>\n        <tr>\n            <td>Y Gyro</td>\n            <td id=\"ygyro\">****</td>\n            <td>deg/sec</td>\n        </tr>\n        <tr>\n            <td>Z Gyro</td>\n            <td id=\"zgyro\">****</td>\n            <td>deg/sec</td>\n        </tr>\n        <tr>\n            <td>Yaw</td>\n            <td id=\"yaw\">****</td>\n            <td>deg</td>\n        </tr>\n    </table>\n    <hr>\n\n    <h3>Network</h3>\n    <hr>\n\n    <script src=\"main.js\"></script>\n</body>\n\n</html>";

static const char main_css[] = "\n.outlined{\n    border: 1px solid #FFFFFF;\n    margin: 1px;\n}\n\nbody {\n    background: black;\n    padding: 0.25em;\n    margin: 0;\n    font-size: 16px;\n    font-family: Verdana, sans-serif;\n    color: white;\n}\n\n/* Ensure we use up the full screen space with our content */\nhtml,body {\n    height: 99vh;\n}\n\ntable, th, td {\n\tborder-bottom: 1px solid #ddd;\n\tborder-collapse: collapse;\n}\n\nth, td {\n\tpadding: 15px;\n\ttext-align: left;\n}\n\ntr:hover {background-color: rgb(52, 79, 233);} \n\n/* Fancy schmancy buttons */\nbutton {\n    background: #222222;\n    padding: 6.5px 10px;\n    margin: 6.5px 10px;\n    -webkit-border-radius: 3px;\n    -moz-border-radius: 3px;\n    border-radius: 3px;\n    color: white;\n    font-size: 13px;\n    font-family: \"localMichroma\", monospace;\n    text-decoration: none;\n    vertical-align: middle;\n    cursor:pointer;\n }\n button:hover {\n    background: #444;\n    color: white;\n }\n button:active {\n    background: #333;\n }\n button:disabled {\n    background: black;\n    color: #555;\n }\n\n\n";

static const char main_js[] = "function myFunction() {\n\tdocument.getElementById(\"demo\").style.color = \"red\";\n}\n\nurl = \"ws://\" + window.location.hostname +\":\" + window.location.port + \"/websocket\"\nwebSocket = new WebSocket(url);\n\nwebSocket.onopen = function (event) {\n\n};\n\nwebSocket.onmessage = function (event) {\n\trxData = JSON.parse(event.data);\n\tdocument.getElementById(\"time\").innerHTML = rxData.time;\n\tdocument.getElementById(\"xaccel\").innerHTML = rxData.accelX;\n\tdocument.getElementById(\"yaccel\").innerHTML = rxData.accelY;\n\tdocument.getElementById(\"zaccel\").innerHTML = rxData.accelZ;\n\tdocument.getElementById(\"xgyro\").innerHTML = rxData.gyroX;\n\tdocument.getElementById(\"ygyro\").innerHTML = rxData.gyroY;\n\tdocument.getElementById(\"zgyro\").innerHTML = rxData.gyroZ;\n\tdocument.getElementById(\"yaw\").innerHTML = rxData.yaw;\n\n}";

static const char test_html[] = "<!DOCTYPE html>\n<html>\n<head/>\n<body>\nJust the bare necessities.\n</body>\n</html>";



// Auto-Generated page response handler.
// Returns poiner to requested content, or null if not available.
void handleHttpFileServe(struct mg_connection *c, struct mg_http_message * hm)
{

   if(mg_http_match_uri(hm, "/FourOhFour.html")) {
      mg_http_reply(c, 200,  header_html,  FourOhFour_html );
      printf("[WEBSERVER] Served /FourOhFour.html\n");
   } else if(mg_http_match_uri(hm, "/index.html")) {
      mg_http_reply(c, 200,  header_html,  index_html );
      printf("[WEBSERVER] Served /index.html\n");
   } else if(mg_http_match_uri(hm, "/main.css")) {
      mg_http_reply(c, 200,  header_css,  main_css );
      printf("[WEBSERVER] Served /main.css\n");
   } else if(mg_http_match_uri(hm, "/main.js")) {
      mg_http_reply(c, 200,  header_js,  main_js );
      printf("[WEBSERVER] Served /main.js\n");
   } else if(mg_http_match_uri(hm, "/test.html")) {
      mg_http_reply(c, 200,  header_html,  test_html );
      printf("[WEBSERVER] Served /test.html\n");
   } else {
      //URL Not found
      printf("[WEBSERVER] Could not find requested resource!\n");
      mg_http_reply(c, 404,  header_html,  FourOhFour_html );
   } 



}