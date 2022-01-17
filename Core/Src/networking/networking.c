#include "networking.h"

uint8_t mac[] = { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
uint8_t ip[] = { 10, 111, 76, 123 };

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE+1];

void NetworkingInit(SPI_HandleTypeDef *spiHandle) {
	/* Ethernet Init*/
	ES_enc28j60SpiInit(spiHandle);
	ES_enc28j60Init(mac);

	uint8_t enc28j60_rev = ES_enc28j60Revision();

	if (enc28j60_rev <= 0) {
		while (1); //
		//error(2, 0);
	}

	ES_init_ip_arp_udp_tcp(mac, ip, MYWWWPORT);
}

void ES_PingCallback(void) {
}

uint16_t http200ok(void)
{
  return(ES_fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n")));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
  uint16_t plen;
  plen=http200ok();
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<html><head><title>Arduino ENC28J60 Ethernet Shield V1.0</title></head><body>"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<center><h1>Welcome to Arduino ENC28J60 Ethernet Shield V1.0</h1>"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<hr><br><h2><font color=\"blue\">-- Put your ARDUINO online -- "));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<br> Control digital outputs"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<br> Read digital analog inputs HERE"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("<br></font></h2>") );
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("</center><hr>"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("V1.0 <a href=\"http://blog.thiseldo.co.uk\">blog.thiseldo.co.uk</a>"));
  plen=ES_fill_tcp_data_p(buf,plen,PSTR("</body></html>"));

  return(plen);
}


void NetworkingPeriodic() {

	uint16_t plen, dat_p;
	dat_p=ES_packetloop_icmp_tcp(buf,ES_enc28j60PacketReceive(BUFFER_SIZE, buf));

    /* dat_p will be unequal to zero if there is a valid
     * http get */
    if(dat_p==0){
      // no http request
      return;
    }


    // tcp port 80 begin
    if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
      // head, post and other methods:
      dat_p=http200ok();
      dat_p=ES_fill_tcp_data_p(buf,dat_p,PSTR("<h1>200 OK</h1>"));
    }
    // just one web page in the "root directory" of the web server
    if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
      dat_p=print_webpage(buf);
    }
    else{
      dat_p=ES_fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
    }

    ES_www_server_reply(buf,dat_p); // send web page data
    // tcp port 80 end


}
