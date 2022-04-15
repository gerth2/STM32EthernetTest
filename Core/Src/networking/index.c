#include "server.h"
#include "mongoose.h"

static const char header_html[] = "content-type: text/html\r\n";
static const char header_js[] = "content-type: text/javascript\r\n";
static const char header_css[] = "content-type: text/css\r\n";

static const char FourOhFour_html[] = "<!DOCTYPE html><html><head><link rel=stylesheet href=main.css></head><body><h1>ERROR 404</h1> The requested resource was not found. </body></html>";

static const char index_html[] = "<!DOCTYPE html><html><head><link rel=stylesheet href=main.css></head><body><h1>Network IMU</h1><hr><div class=tab><button class=tablinks onclick=\"selectTab(event, 'Status')\" id=defaultOpen>Status</button><button class=tablinks onclick=\"selectTab(event, 'Graphs')\">Graphs</button><button class=tablinks onclick=\"selectTab(event, 'Settings')\">Settings</button></div><div id=Status class=tabcontent><table><tr><th style=width:100px> Data</th><th style=width:200px> Value</th><th style=width:75px> Units</th></tr><tr><td>Time</td><td id=time>****</td><td>s</td></tr><tr><td>X Accel</td><td id=xaccel>****</td><td>g</td></tr><tr><td>Y Accel</td><td id=yaccel>****</td><td>g</td></tr><tr><td>Z Accel</td><td id=zaccel>****</td><td>g</td></tr><tr><td>X Gyro</td><td id=xgyro>****</td><td>deg/sec</td></tr><tr><td>Y Gyro</td><td id=ygyro>****</td><td>deg/sec</td></tr><tr><td>Z Gyro</td><td id=zgyro>****</td><td>deg/sec</td></tr><tr><td>Yaw</td><td id=yaw>****</td><td>deg</td></tr></table><hr></div><div id=Graphs class=tabcontent><h3>Graphs still TODO</h3></div><div id=Settings class=tabcontent><h3>Settings still TODO</h3></div><script src=main.js></script></body></html>";

static const char main_css[] = "\n.outlined{\nborder: 1px solid #FFFFFF;\nmargin: 1px;\n}\nbody {\nbackground: black;\npadding: 0.25em;\nmargin: 0;\nfont-size: 16px;\nfont-family: Verdana, sans-serif;\ncolor: white;\n}\n/* Ensure we use up the full screen space with our content */\nhtml,body {\nheight: 99vh;\n}\ntable, th, td {\n\tborder-bottom: 1px solid #ddd;\n\tborder-collapse: collapse;\n}\nth, td {\n\tpadding: 15px;\n\ttext-align: left;\n}\ntr:hover {background-color: rgb(52, 79, 233);} \n/* Fancy schmancy buttons */\nbutton {\nbackground: #222222;\npadding: 6.5px 10px;\nmargin: 6.5px 10px;\n-webkit-border-radius: 3px;\n-moz-border-radius: 3px;\nborder-radius: 3px;\ncolor: white;\nfont-size: 13px;\nfont-family: \"localMichroma\", monospace;\ntext-decoration: none;\nvertical-align: middle;\ncursor:pointer;\n }\n button:hover {\nbackground: #444;\ncolor: white;\n }\n button:active {\nbackground: #333;\n }\n button:disabled {\nbackground: black;\ncolor: #555;\n }\n/* ------------------- */\n/* Tab Support */\n/* Style the tab */\n.tab {\noverflow: hidden;\nborder: 1px solid #555;\nbackground-color: #111;\n  }\n  \n  /* Style the buttons inside the tab */\n  .tab button {\nbackground-color: inherit;\nfloat: left;\nborder: none;\noutline: none;\ncursor: pointer;\npadding: 14px 16px;\ntransition: 0.3s;\nfont-size: 17px;\n  }\n  \n  /* Change background color of buttons on hover */\n  .tab button:hover {\nbackground-color: #333;\n  }\n  \n  /* Create an active/current tablink class */\n  .tab button.active {\nbackground-color: #444;\n  }\n  \n  /* Style the tab content */\n  .tabcontent {\ndisplay: none;\npadding: 6px 12px;\nborder: 1px solid #222;\nborder-top: none;\nanimation: fadeEffect 0.25s; \n  }\n  \n  /* Go from zero to full opacity */\n  @keyframes fadeEffect {\nfrom {opacity: 0;}\nto {opacity: 1;}\n  }";

static const char main_js[] = "function selectTab(evt,tabName){var i,tabcontent,tablinks;tabcontent=document.getElementsByClassName(\"tabcontent\");for(i=0;i<tabcontent.length;i++){tabcontent[i].style.display=\"none\";}\ntablinks=document.getElementsByClassName(\"tablinks\");for(i=0;i<tablinks.length;i++){tablinks[i].className=tablinks[i].className.replace(\" active\",\"\");}\ndocument.getElementById(tabName).style.display=\"block\";evt.currentTarget.className+=\" active\";}\ndocument.getElementById(\"defaultOpen\").click();url=\"ws://\"+window.location.hostname+\":\"+window.location.port+\"/websocket\"\nwebSocket=new WebSocket(url);webSocket.onopen=function(event){};webSocket.onmessage=function(event){rxData=JSON.parse(event.data);document.getElementById(\"time\").innerHTML=rxData.time;document.getElementById(\"xaccel\").innerHTML=rxData.accelX;document.getElementById(\"yaccel\").innerHTML=rxData.accelY;document.getElementById(\"zaccel\").innerHTML=rxData.accelZ;document.getElementById(\"xgyro\").innerHTML=rxData.gyroX;document.getElementById(\"ygyro\").innerHTML=rxData.gyroY;document.getElementById(\"zgyro\").innerHTML=rxData.gyroZ;document.getElementById(\"yaw\").innerHTML=rxData.yaw;}";

static const char test_html[] = "<!DOCTYPE html><html><head/><body> Just the bare necessities. </body></html>";



// Auto-Generated page response handler.
// Returns poiner to requested content, or null if not available.
void handleHttpFileServe(struct mg_connection *c, struct mg_http_message * hm)
{

   if(mg_http_match_uri(hm, "/")) {
      mg_http_reply(c, 200,  header_html,  index_html );
      printf("[WEBSERVER] Served /\n");
   } else if(mg_http_match_uri(hm, "/FourOhFour.html")) {
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