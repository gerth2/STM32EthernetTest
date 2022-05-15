
#ifndef _SOCKET_H
#define _SOCKET_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

#include "socket.h"
#include <time.h>

#define u8_t uint8_t
#define u32_t uint32_t
#define BACKLOG		4

#define DATA_BUF_SIZE   128
#define DFLT_PORT 5000


/* For setsockopt(2) */
#define SOL_SOCKET        1

#define SO_DEBUG        1
#define SO_REUSEADDR        2
#define SO_TYPE                3
#define SO_ERROR        4
#define SO_DONTROUTE        5
#define SO_BROADCAST        6
#define SO_SNDBUF        7
#define SO_RCVBUF        8
#define SO_SNDBUFFORCE        32
#define SO_RCVBUFFORCE        33
#define SO_KEEPALIVE        9
#define SO_OOBINLINE        10
#define SO_NO_CHECK        11
#define SO_PRIORITY        12
#define SO_LINGER        13
#define SO_BSDCOMPAT        14



/* Socket types. */
#define SOCK_STREAM	1		/* stream (connection) socket	*/
#define SOCK_DGRAM	2		/* datagram (conn.less) socket	*/
#define SOCK_RAW	3		/* raw socket			*/
#define SOCK_RDM	4		/* reliably-delivered message	*/
#define SOCK_SEQPACKET	5		/* sequential packet socket	*/
#define SOCK_PACKET	10		/* linux specific way of	*/
					/* getting packets at the dev	*/
					/* level.  For writing rarp and	*/
					/* other similar things on the	*/
					/* user level.			*/

/* Supported address families. */
#define AF_UNSPEC	0
#define AF_UNIX		1	/* Unix domain sockets 		*/
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_AX25		3	/* Amateur Radio AX.25 		*/
#define AF_IPX		4	/* Novell IPX 			*/
#define AF_APPLETALK	5	/* Appletalk DDP 		*/
#define	AF_NETROM	6	/* Amateur radio NetROM 	*/
#define AF_BRIDGE	7	/* Multiprotocol bridge 	*/
#define AF_AAL5		8	/* Reserved for Werner's ATM 	*/
#define AF_X25		9	/* Reserved for X.25 project 	*/
#define AF_INET6	10	/* IP version 6			*/
#define AF_MAX		12	/* For now.. */


/* Standard well-defined IP protocols.  */
enum
  {
    IPPROTO_IP = 0,           /* Dummy protocol for TCP.  */
#define IPPROTO_IP                IPPROTO_IP
    IPPROTO_HOPOPTS = 0,   /* IPv6 Hop-by-Hop options.  */
#define IPPROTO_HOPOPTS                IPPROTO_HOPOPTS
    IPPROTO_ICMP = 1,           /* Internet Control Message Protocol.  */
#define IPPROTO_ICMP                IPPROTO_ICMP
    IPPROTO_IGMP = 2,           /* Internet Group Management Protocol. */
#define IPPROTO_IGMP                IPPROTO_IGMP
    IPPROTO_IPIP = 4,           /* IPIP tunnels (older KA9Q tunnels use 94).  */
#define IPPROTO_IPIP                IPPROTO_IPIP
    IPPROTO_TCP = 6,           /* Transmission Control Protocol.  */
#define IPPROTO_TCP                IPPROTO_TCP
    IPPROTO_EGP = 8,           /* Exterior Gateway Protocol.  */
#define IPPROTO_EGP                IPPROTO_EGP
    IPPROTO_PUP = 12,           /* PUP protocol.  */
#define IPPROTO_PUP                IPPROTO_PUP
    IPPROTO_UDP = 17,           /* User Datagram Protocol.  */
#define IPPROTO_UDP                IPPROTO_UDP
    IPPROTO_IDP = 22,           /* XNS IDP protocol.  */
#define IPPROTO_IDP                IPPROTO_IDP
    IPPROTO_TP = 29,           /* SO Transport Protocol Class 4.  */
#define IPPROTO_TP                IPPROTO_TP
    IPPROTO_DCCP = 33,           /* Datagram Congestion Control Protocol.  */
#define IPPROTO_DCCP                IPPROTO_DCCP
    IPPROTO_IPV6 = 41,     /* IPv6 header.  */
#define IPPROTO_IPV6                IPPROTO_IPV6
    IPPROTO_ROUTING = 43,  /* IPv6 routing header.  */
#define IPPROTO_ROUTING                IPPROTO_ROUTING
    IPPROTO_FRAGMENT = 44, /* IPv6 fragmentation header.  */
#define IPPROTO_FRAGMENT        IPPROTO_FRAGMENT
    IPPROTO_RSVP = 46,           /* Reservation Protocol.  */
#define IPPROTO_RSVP                IPPROTO_RSVP
    IPPROTO_GRE = 47,           /* General Routing Encapsulation.  */
#define IPPROTO_GRE                IPPROTO_GRE
    IPPROTO_ESP = 50,      /* encapsulating security payload.  */
#define IPPROTO_ESP                IPPROTO_ESP
    IPPROTO_AH = 51,       /* authentication header.  */
#define IPPROTO_AH                IPPROTO_AH
    IPPROTO_ICMPV6 = 58,   /* ICMPv6.  */
#define IPPROTO_ICMPV6                IPPROTO_ICMPV6
    IPPROTO_NONE = 59,     /* IPv6 no next header.  */
#define IPPROTO_NONE                IPPROTO_NONE
    IPPROTO_DSTOPTS = 60,  /* IPv6 destination options.  */
#define IPPROTO_DSTOPTS                IPPROTO_DSTOPTS
    IPPROTO_MTP = 92,           /* Multicast Transport Protocol.  */
#define IPPROTO_MTP                IPPROTO_MTP
    IPPROTO_ENCAP = 98,           /* Encapsulation Header.  */
#define IPPROTO_ENCAP                IPPROTO_ENCAP
    IPPROTO_PIM = 103,           /* Protocol Independent Multicast.  */
#define IPPROTO_PIM                IPPROTO_PIM
    IPPROTO_COMP = 108,           /* Compression Header Protocol.  */
#define IPPROTO_COMP                IPPROTO_COMP
    IPPROTO_SCTP = 132,           /* Stream Control Transmission Protocol.  */
#define IPPROTO_SCTP                IPPROTO_SCTP
    IPPROTO_UDPLITE = 136, /* UDP-Lite protocol.  */
#define IPPROTO_UDPLITE                IPPROTO_UDPLITE
    IPPROTO_RAW = 255,           /* Raw IP packets.  */
#define IPPROTO_RAW                IPPROTO_RAW
    IPPROTO_MAX
  };

/*
 * TCP general constants
 */
#define TCP_MSS_DEFAULT      536U   /* IPv4 (RFC1122, RFC2581) */
#define TCP_MSS_DESIRED     1220U   /* IPv6 (tunneled), EDNS0 (RFC3226) */

/* TCP socket options */
#define TCP_NODELAY     1   /* Turn off Nagle's algorithm. */
#define TCP_MAXSEG      2   /* Limit MSS */
#define TCP_CORK        3   /* Never send partially complete segments */
#define TCP_KEEPIDLE        4   /* Start keeplives after this period */
#define TCP_KEEPINTVL       5   /* Interval between keepalives */
#define TCP_KEEPCNT     6   /* Number of keepalives before death */
#define TCP_SYNCNT      7   /* Number of SYN retransmits */
#define TCP_LINGER2     8   /* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT    9   /* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP    10  /* Bound advertised window */
#define TCP_INFO        11  /* Information about this connection. */
#define TCP_QUICKACK        12  /* Block/reenable quick acks */
#define TCP_CONGESTION      13  /* Congestion control algorithm */
#define TCP_MD5SIG      14  /* TCP MD5 Signature (RFC2385) */
#define TCP_COOKIE_TRANSACTIONS 15  /* TCP Cookie Transactions */
#define TCP_THIN_LINEAR_TIMEOUTS 16      /* Use linear timeouts for thin streams*/
#define TCP_THIN_DUPACK         17      /* Fast retrans. after 1 dupack */
#define TCP_USER_TIMEOUT    18  /* How long for loss retry before timeout */
#define TCP_REPAIR      19  /* TCP sock is under repair right now */
#define TCP_REPAIR_QUEUE    20
#define TCP_QUEUE_SEQ       21
#define TCP_REPAIR_OPTIONS  22
#define TCP_FASTOPEN        23  /* Enable FastOpen on listeners */


struct in_addr {
	unsigned long s_addr;          // load with inet_pton()
};

// IPv4 AF_INET sockets:
struct sockaddr_in {
    short            sin_len;
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct sockaddr {
  u8_t sa_len;
  u8_t sa_family;
  char sa_data[14];
};

typedef u32_t socklen_t;

int socket(int domain, int type, int protocol);

int accept(int s, struct sockaddr *addr, socklen_t *addrlen);

int bind(int s, const struct sockaddr *name, socklen_t namelen);

int close(int s);

int connect(int s, const struct sockaddr *name, socklen_t namelen);

int listen(int s, int backlog);

int recv(int s, void *mem, size_t len, int flags);

int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen);

int send(int s, const void *dataptr, size_t size, int flags);

int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen);

int getsockname (int s, struct sockaddr *name, socklen_t *namelen);

int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);

int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);



#define SYSTEM_LITTLE_ENDIAN

int8_t* inet_ntoa(uint32_t addr);
int8_t* inet_ntoa_pad(uint32_t addr);
uint32_t inet_addr(uint8_t* addr);
uint16_t swaps(uint16_t i);
uint32_t swapl(uint32_t l);
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
uint32_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);

#endif
