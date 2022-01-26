#include "server.h"

static const U8 egz_html[] = "\r\ncontent-type: text/html; charset=UTF-8";
static const U8 egz_js[] = "\r\ncontent-type: text/javascript; charset=UTF-8";
static const U8 egz_css[] = "\r\ncontent-type: text/css; charset=UTF-8";

$pageData

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

   $pageSwitchyard
   
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