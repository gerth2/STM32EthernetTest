
#include <string.h>
#include "socket.h"
#include "wn_socket.h"
#include "debugUtils.h"

int errno;

int socket(int domain, int type, int protocol)
{
   int32_t ret;
   int i;
   int internal_protocol;

   if(protocol == IPPROTO_TCP)
	   internal_protocol = Sn_MR_TCP;
   else if(protocol == IPPROTO_UDP)
	   internal_protocol = Sn_MR_UDP;
   else
	   internal_protocol = Sn_MR_TCP;

   for(i=0; i<(_WIZCHIP_SOCK_NUM_+1); i++)
   {
	   if((getSn_RX_RSR(i) == SOCK_CLOSED) && (i<_WIZCHIP_SOCK_NUM_))
	   {
		   break;
	   }
   }

   if(i >= _WIZCHIP_SOCK_NUM_)
	   return -1;

   ret = wn_socket(i, internal_protocol, (DFLT_PORT+i), 0x00);
   threadSafePrintf("\r\nSocket:%d Port:%d\r\n", i, (DFLT_PORT+i));

   return ret;
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
   return 0;
}

int bind(int s, const struct sockaddr *name, socklen_t namelen)
{
   return 0;
}

int close(int s)
{
   int32_t ret;

   ret = wn_close(s);

   return ret;
}

int connect(int s, const struct sockaddr *name, socklen_t namelen)
{
   int32_t ret;
   uint8_t destip[4];
   uint16_t destport;
   struct sockaddr_in sa;
   uint32_t s_addr;

   sa.sin_port = ((struct sockaddr_in *)name)->sin_port;
   sa.sin_addr.s_addr = ((struct sockaddr_in *)name)->sin_addr.s_addr;
   destport = ntohs(sa.sin_port);
   s_addr = ntohl(sa.sin_addr.s_addr);
   destip[3] = (uint8_t)(s_addr & 0xFF);
   destip[2] = (uint8_t)((s_addr >> 8) & 0xFF);
   destip[1] = (uint8_t)((s_addr >> 16) & 0xFF);
   destip[0] = (uint8_t)((s_addr >> 24) & 0xFF);

   ret = wn_connect(s, destip, destport);

   return ret;
}

int listen(int s, int backlog)
{
   int32_t ret;

   ret = wn_listen(s);

   return ret;
}

int recv(int s, void *mem, size_t len, int flags)
{
   int32_t ret;

   ret = wn_recv(s, mem, len);

   return ret;
}

int recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen)
{
   int32_t ret;
   uint8_t destip[4];
   uint16_t destport;
   struct sockaddr_in sa;
   uint32_t s_addr;

   ret = wn_recvfrom(s, mem, len, destip, (uint16_t*)&destport);

   sa.sin_len = (uint16_t)sizeof(sa);
   sa.sin_family = AF_INET;
   sa.sin_port = htons(destport);
   s_addr = (((((destip[0] << 8) | destip[1]) << 8) | destip[2]) << 8) | destip[3];
   sa.sin_addr.s_addr = htonl(s_addr);
   fromlen = (socklen_t *)sizeof(sa);
   memcpy(from, &sa, sa.sin_len);

   return ret;
}

int send(int s, const void *dataptr, size_t size, int flags)
{
   int32_t ret;

   ret = wn_send(s, (uint8_t *)dataptr, size);

   return ret;
}

int sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen)
{
   int32_t ret;
   uint8_t destip[4];
   uint16_t destport;
   struct sockaddr_in sa;
   uint32_t s_addr;

   sa.sin_port = ((struct sockaddr_in *)to)->sin_port;
   sa.sin_addr.s_addr = ((struct sockaddr_in *)to)->sin_addr.s_addr;
   destport = ntohs(sa.sin_port);
   s_addr = ntohl(sa.sin_addr.s_addr);
   destip[3] = (uint8_t)(s_addr & 0xFF);
   destip[2] = (uint8_t)((s_addr >> 8) & 0xFF);
   destip[1] = (uint8_t)((s_addr >> 16) & 0xFF);
   destip[0] = (uint8_t)((s_addr >> 24) & 0xFF);

   ret = wn_sendto(s, (uint8_t *)dataptr, size, destip, destport);

   return ret;
}

int getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
   return 0;
}

int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
   return 0;
}

int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
   return 0;
}


/**
 * Convert a 32bit Address into a Dotted Decimal Format string.
 *
 * @param addr 32bit address.
 * @return Dotted Decimal Format string.
 */
int8_t* inet_ntoa(uint32_t addr)
{
	static int8_t addr_str[16];
	memset(addr_str,0,16);
	threadSafeSPrintf((char*)addr_str,"%d.%d.%d.%d",(int32_t)(addr>>24 & 0xFF),(int32_t)(addr>>16 & 0xFF),(int32_t)(addr>>8 & 0xFF),(int32_t)(addr & 0xFF));
	return addr_str;
}

/**
 * Convert a 32bit Address into a Dotted Decimal Format string.
 * This is differ from inet_ntoa in fixed length.
 *
 * @param addr 32bit address.
 * @return Dotted Decimal Format string.
 */
int8_t* inet_ntoa_pad(uint32_t addr)
{
	static int8_t addr_str[16];
	memset(addr_str,0,16);
	threadSafeSPrintf((char*)addr_str,"%03d.%03d.%03d.%03d",(int32_t)(addr>>24 & 0xFF),(int32_t)(addr>>16 & 0xFF),(int32_t)(addr>>8 & 0xFF),(int32_t)(addr & 0xFF));
	return addr_str;
}

/**
 * Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address.
 *
 * @param addr Dotted Decimal Format string.
 * @return 32bit address.
 */
uint32_t inet_addr(uint8_t* addr)
{
	int8_t i;
	uint32_t inetaddr = 0;
	int8_t taddr[30];
	int8_t * nexttok;
	int32_t num;
	strcpy((char*)taddr,(char*)addr);

	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = (int8_t*)strtok((char*)nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = strtol((char*)nexttok+2, NULL, 16);
		else num = strtol((char*)nexttok, NULL, 10);
		inetaddr = inetaddr << 8;
		inetaddr |= (num & 0xFF);
		nexttok = NULL;
	}
	return inetaddr;
}

/**
 * Swap the byte order of 16bit(short) wide variable.
 *
 * @param i 16bit value to swap
 * @return Swapped value
 */
uint16_t swaps(uint16_t i)
{
	uint16_t ret=0;
	ret = (i & 0xFF) << 8;
	ret |= ((i >> 8)& 0xFF);
	return ret;
}

/**
 * Swap the byte order of 32bit(long) wide variable.
 *
 * @param l 32bit value to convert
 * @return Swapped value
 */
uint32_t swapl(uint32_t l)
{
	uint32_t ret=0;
	ret = (l & 0xFF) << 24;
	ret |= ((l >> 8) & 0xFF) << 16;
	ret |= ((l >> 16) & 0xFF) << 8;
	ret |= ((l >> 24) & 0xFF);
	return ret;
}

/**
 * htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).
 *
 * @param hostshort The value to convert.
 * @return The value in TCP/IP network byte order.
 */
uint16_t htons(uint16_t hostshort)
{
#ifdef SYSTEM_LITTLE_ENDIAN
	return swaps(hostshort);
#else
	return hostshort;
#endif
}


/**
 * htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian).
 *
 * @param hostlong The value to convert.
 * @return The value in TCP/IP network byte order.
 */
uint32_t htonl(uint32_t hostlong)
{
#ifdef SYSTEM_LITTLE_ENDIAN
	return swapl(hostlong);
#else
	return hostlong;
#endif
}


/**
 * ntohs function converts a unsigned short from TCP/IP network byte order
 * to host byte order (which is little-endian on Intel processors).
 *
 * @param netshort The value to convert.
 * @return A 16-bit number in host byte order
 */
uint32_t ntohs(uint16_t netshort)
{
#ifdef SYSTEM_LITTLE_ENDIAN
	return htons(netshort);
#else
	return netshort;
#endif
}

/**
 * converts a unsigned long from TCP/IP network byte order to host byte order
 * (which is little-endian on Intel processors).
 *
 * @param netlong The value to convert.
 * @return A 16-bit number in host byte order
 */
uint32_t ntohl(uint32_t netlong)
{
#ifdef SYSTEM_LITTLE_ENDIAN
	return swapl(netlong);
#else
	return netlong;
#endif
}
/**
 * @}
 */

int select(int nfds, fd_set *restrict readfds,
                  fd_set *restrict writefds, fd_set *restrict exceptfds,
                  struct timeval *restrict timeout){

	return 0; //TODO - use this as part of checking whether the chip is active?
}
