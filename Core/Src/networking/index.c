#include "server.h"

static const U8 egz_html[] = "\r\ncontent-type: text/html; charset=UTF-8";
static const U8 egz_js[] = "\r\ncontent-type: text/javascript; charset=UTF-8";
static const U8 egz_css[] = "\r\ncontent-type: text/css; charset=UTF-8";

static const U8 index_html[] = "<!DOCTYPE html>\n<html>\n<head>\n<link rel=\"stylesheet\" href=\"main.css\">\n</head>\n<body>\n<h1>I am a headline made with HTML</h1>\n<p>And I am a simple text paragraph. The color of this text is styled with CSS. Click the button below to remove me through the power JavaScript.</p>\n<button>Hide the text above</button>\n<script src=\"main.js\"></script>\n</body>\n</html>";

static const U8 main_css[] = "body {\n\tfont-family: sans-serif;\n\ttext-align: center;\n\tpadding: 3rem;\n\tfont-size: 1.125rem;\n\tline-height: 1.5;\n\ttransition: all 725ms ease-in-out;\n}\n\nh1 {\n\tfont-size: 2rem;\n\tfont-weight: bolder;\n\tmargin-bottom: 1rem;\n}\n\np {\n\tmargin-bottom: 1rem;\n\tcolor: tomato;\n}\n\nbutton {\n\tcursor: pointer;\n\tappearance: none;\n\tborder-radius: 4px;\n\tfont-size: 1.25rem;\n\tpadding: 0.75rem 1rem;\n\tborder: 1px solid navy;\n\tbackground-color: dodgerblue;\n\tcolor: white;\n}";

static const U8 main_js[] = "$('button').on('click', function() {\n\t$('p').css('opacity', 0);\n});\n";



// Auto-Generated page response handler.
// Returns 1 on page found, 0 if not found.
int fetchPage(void* hndl, MST* mst, U8* path)
{

   (void)hndl;
   
   U8* sptr;
   int sblen=MST_getSendBufSize(mst);
   int delta=sblen;
   const U8* dptr=NULL;
   const U8* egz=NULL;
   int size=0;

      if(strcmp(path, "/index.html") == 0) {
      dptr=index_html;
      size=sizeof(index_html);
      egz=egz_html;
   } else if(strcmp(path, "/main.css") == 0) {
      dptr=main_css;
      size=sizeof(main_css);
      egz=egz_css;
   } else if(strcmp(path, "/main.js") == 0) {
      dptr=main_js;
      size=sizeof(main_js);
      egz=egz_js;
   } else {
      //URL Not found
      return 0;
   } 



   size =- 1; // discard trailing null
   
   sptr=msRespCT(MST_getSendBufPtr(mst), &sblen, size, egz);
   delta = delta-sblen;
   while(size)
   {
      if(sblen > size)
         sblen = size;
      memcpy(sptr, dptr, sblen);
      if(MST_write(mst,0,sblen+delta) < 0)
         break;
      size-=sblen;
      dptr+=sblen;
      delta=0;
      sblen=MST_getSendBufSize(mst);
      sptr=MST_getSendBufPtr(mst);
   }
   return 1;
}