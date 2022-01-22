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
   }
}

int se_bind(SOCKET* sock, uint16_t port)
{
   struct freertos_sockaddr addr;
   socklen_t addrlen=sizeof(addr);
   *sock = FreeRTOS_socket(
      FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
   if(*sock == FREERTOS_INVALID_SOCKET)
   {
      return -1;
   }
   addr.sin_addr=0x0;
   addr.sin_port = FreeRTOS_htons(port);
   if(FreeRTOS_bind(*sock, &addr, addrlen))
   {
      se_close(sock);
      return -3;
   }
   if(FreeRTOS_listen(*sock, 4))
   {
      se_close(sock);
      return -2;
   }
   return 0;
}

int se_connect(SOCKET* sock, const char* address, uint16_t port)
{
   struct freertos_sockaddr addr;
   socklen_t addrlen=sizeof(addr);
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
   return **outSock == 0 ? 0 : (**outSock < 0 ? -1 : 1);
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

void  SharkSslSha1Ctx_constructor(SharkSslSha1Ctx* ctx){
	return;
}
void  SharkSslSha1Ctx_append(SharkSslSha1Ctx* ctx, const U8* data, U32 len){
	return;
}
void  SharkSslSha1Ctx_finish(SharkSslSha1Ctx* ctx,U8 digest[SHARKSSL_SHA1_HASH_LEN]){
	return;
}


