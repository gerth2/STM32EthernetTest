#include "selib.h"


int se_sockValid(SOCKET* sock)
{
   return *sock != FREERTOS_INVALID_SOCKET;
}


void se_close(SOCKET* sock)
{
   TickType_t start_time;
   U8 buffer[10];

   if(*sock != FREERTOS_INVALID_SOCKET)
   {
	  printf("[MINNOWSERVER INF] Starting socket close...\n");
      FreeRTOS_shutdown(*sock, FREERTOS_SHUT_RDWR);
      start_time = xTaskGetTickCount();


      do
      {
         int rc = FreeRTOS_recv(*sock, buffer, sizeof(buffer), 0);
         if(rc < 0 && rc != -pdFREERTOS_ERRNO_EAGAIN)
         {
            break;
         }
      } while((xTaskGetTickCount() - start_time) < pdMS_TO_TICKS(1000));

      FreeRTOS_closesocket(*sock);
      *sock=FREERTOS_INVALID_SOCKET;
      printf("[MINNOWSERVER INF] Socket Close Completed.\n");
   }
}

int se_bind(SOCKET* sock, uint16_t port)
{
   struct freertos_sockaddr addr;
   socklen_t addrlen=sizeof(addr);

   printf("[MINNOWSERVER INF] Binding socket to port %d\n", port);

   *sock = FreeRTOS_socket(
      FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
   if(*sock == FREERTOS_INVALID_SOCKET)
   {
      return -1;
   }
   addr.sin_addr= 0x0;
   addr.sin_port = FreeRTOS_htons(port);
   if(FreeRTOS_bind(*sock, &addr, addrlen))
   {
      se_close(sock);
      return -3;
   }
   if(FreeRTOS_listen(*sock, 10))
   {
      se_close(sock);
      return -2;
   }
   printf("[MINNOWSERVER INF] Socket Bind Success\n", port);
   return 0;
}

int se_connect(SOCKET* sock, const char* address, uint16_t port)
{
   struct freertos_sockaddr addr;
   socklen_t addrlen=sizeof(addr);

   printf("[MINNOWSERVER INF] Socket Connect on port %d started\n", port);

   uint32_t ip = FreeRTOS_gethostbyname(address);
   if( ! ip )
   {
      ip = FreeRTOS_inet_addr(address);
      if( ! ip )
      {
         *sock = FREERTOS_INVALID_SOCKET;
         return -2;
      }
   }
   *sock = FreeRTOS_socket(
      FREERTOS_AF_INET,FREERTOS_SOCK_STREAM,FREERTOS_IPPROTO_TCP);
   if( ! *sock )
   {
      return -1;
   }
   if(*sock == FREERTOS_INVALID_SOCKET)
   {
      return -1;
   }
   addr.sin_addr=ip;
   addr.sin_port = FreeRTOS_htons(port);
   if(FreeRTOS_connect(*sock, &addr, sizeof(addr)))
   {
      se_close(sock);
      return -3;
   }

   printf("[MINNOWSERVER INF] Socket Connect Success\n");
   return 0;
}


int se_accept(SOCKET** listenSock, U32 timeout, SOCKET** outSock)
{
   struct freertos_sockaddr addr;
   socklen_t addrlen=sizeof(addr);
   TickType_t tickTmo;
   tickTmo = timeout==INFINITE_TMO ? portMAX_DELAY : timeout/portTICK_PERIOD_MS;
   FreeRTOS_setsockopt(**listenSock,0,FREERTOS_SO_RCVTIMEO,&tickTmo,0);
   **outSock=FreeRTOS_accept(**listenSock,&addr,&addrlen);

   int retval = **outSock == 0 ? 0 : (**outSock < 0 ? -1 : 1);

   if(retval > 0){
	   printf("[MINNOWSERVER INF] Socket Accept occurred\n");
   }

   return retval;
}


S32 se_recv(SOCKET* sock, void* buf, U32 len, U32 timeout)
{
   TickType_t tickTmo;
   tickTmo = timeout==INFINITE_TMO ? portMAX_DELAY : timeout/portTICK_PERIOD_MS;
   FreeRTOS_setsockopt(*sock,0,FREERTOS_SO_RCVTIMEO,&tickTmo,0);
   return FreeRTOS_recv(*sock,buf,len,0);
}

S32 se_send(SOCKET* sock, const void* buf, U32 len)
{
   return FreeRTOS_send(*sock, buf, len, 0);
}


#define GET_U32_BE(w,a,i)                 \
{                                         \
   (w) = ((U32)(a)[(i)] << 24)            \
       | ((U32)(a)[(i) + 1] << 16)        \
       | ((U32)(a)[(i) + 2] <<  8)        \
       | ((U32)(a)[(i) + 3]);             \
}


#define PUT_U32_BE(w,a,i)                 \
{                                         \
   (a)[(i)]     = (U8)((w) >> 24);        \
   (a)[(i) + 1] = (U8)((w) >> 16);        \
   (a)[(i) + 2] = (U8)((w) >>  8);        \
   (a)[(i) + 3] = (U8)((w));              \
}



void SharkSslSha1Ctx_constructor(SharkSslSha1Ctx *ctx)
{
   /* ASSERT(((unsigned int)(ctx->buffer) & (sizeof(int)-1)) == 0); */

   ctx->total[0] = 0;
   ctx->total[1] = 0;

   ctx->state[0] = 0x67452301;
   ctx->state[1] = 0xEFCDAB89;
   ctx->state[2] = 0x98BADCFE;
   ctx->state[3] = 0x10325476;
   ctx->state[4] = 0xC3D2E1F0;
}


/**
 *  SHA-1 implementation based on FIPS PUB 180-3
 *  http://csrc.nist.gov/publications/fips/fips180-3/fips180-3_final.pdf
 */
static void SharkSslSha1Ctx_process(SharkSslSha1Ctx *ctx, const U8 data[64])
{
   U32 a, b, c, d, e, temp;
   unsigned int i;
   U32 W[16];

   GET_U32_BE(W[0],  data,  0);
   GET_U32_BE(W[1],  data,  4);
   GET_U32_BE(W[2],  data,  8);
   GET_U32_BE(W[3],  data, 12);
   GET_U32_BE(W[4],  data, 16);
   GET_U32_BE(W[5],  data, 20);
   GET_U32_BE(W[6],  data, 24);
   GET_U32_BE(W[7],  data, 28);
   GET_U32_BE(W[8],  data, 32);
   GET_U32_BE(W[9],  data, 36);
   GET_U32_BE(W[10], data, 40);
   GET_U32_BE(W[11], data, 44);
   GET_U32_BE(W[12], data, 48);
   GET_U32_BE(W[13], data, 52);
   GET_U32_BE(W[14], data, 56);
   GET_U32_BE(W[15], data, 60);

   #define ROTL(x,n) ((U32)((U32)x << n) | ((U32)x >> (32 - n)))

   #define Ft1(x,y,z) ((x & (y ^ z)) ^ z)  /* equivalent to ((x & y) ^ ((~x) & z)) */
   #define Ft2(x,y,z) (x ^ y ^ z)
   #define Ft3(x,y,z) ((x & y) | ((x | y) & z))
   #define Ft4(x,y,z) (x ^ y ^ z)

   #define K1 0x5A827999
   #define K2 0x6ED9EBA1
   #define K3 0x8F1BBCDC
   #define K4 0xCA62C1D6

   a = ctx->state[0];
   b = ctx->state[1];
   c = ctx->state[2];
   d = ctx->state[3];
   e = ctx->state[4];

   for (i = 0; i < 80; i++)
   {
      if (i >= 16)
      {
         temp = W[i & 0xF] ^ W[(i + 2) & 0xF] ^ W[(i + 8) & 0xF] ^ W[(i + 13) & 0xF];
         W[i & 0xF] = temp = ROTL(temp, 1);
      }
      temp = W[i & 0xF];
      temp += e + ROTL(a, 5);
      if (i < 20)
      {
         temp += Ft1(b,c,d) + K1;
      }
      else if (i < 40)
      {
         temp += Ft2(b,c,d) + K2;
      }
      else if (i < 60)
      {
         temp += Ft3(b,c,d) + K3;
      }
      else
      {
         temp += Ft4(b,c,d) + K4;
      }
      e = d;
      d = c;
      c = ROTL(b, 30);
      b = a;
      a = temp;
   }

   ctx->state[0] += a;
   ctx->state[1] += b;
   ctx->state[2] += c;
   ctx->state[3] += d;
   ctx->state[4] += e;

   #undef K4
   #undef K3
   #undef K2
   #undef K1

   #undef Ft4
   #undef Ft3
   #undef Ft2
   #undef Ft1

   #undef ROTL
}


void SharkSslSha1Ctx_append(SharkSslSha1Ctx *ctx, const U8 *in, U32 len)
{
   unsigned int left, fill;

   left = (unsigned int)(ctx->total[0]) & 0x3F;
   fill = 64 - left;

   ctx->total[0] += len;
   if (ctx->total[0] < len)
   {
      ctx->total[1]++;
   }

   if((left) && (len >= fill))
   {
      memcpy((ctx->buffer + left), in, fill);
      SharkSslSha1Ctx_process(ctx, ctx->buffer);
      len -= fill;
      in  += fill;
      left = 0;
   }

   while (len >= 64)
   {
      SharkSslSha1Ctx_process(ctx, in);
      len -= 64;
      in  += 64;
   }

   if (len)
   {
      memcpy((ctx->buffer + left), in, len);
   }
}


static const U8 sharkSslHashPadding[64] =
{
   0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void SharkSslSha1Ctx_finish(SharkSslSha1Ctx *ctx, U8 digest[20])
{
   U32 last, padn;
   U32 high, low;
   U8  msglen[8];

   high = (ctx->total[0] >> 29) | (ctx->total[1] <<  3);
   low  = (ctx->total[0] <<  3);

   PUT_U32_BE(high, msglen, 0);
   PUT_U32_BE(low,  msglen, 4);

   last = ctx->total[0] & 0x3F;
   padn = (last < 56) ? (56 - last) : (120 - last);

   SharkSslSha1Ctx_append(ctx, (U8*)sharkSslHashPadding, padn);
   SharkSslSha1Ctx_append(ctx, msglen, 8);

   PUT_U32_BE(ctx->state[0], digest,  0);
   PUT_U32_BE(ctx->state[1], digest,  4);
   PUT_U32_BE(ctx->state[2], digest,  8);
   PUT_U32_BE(ctx->state[3], digest, 12);
   PUT_U32_BE(ctx->state[4], digest, 16);
}

