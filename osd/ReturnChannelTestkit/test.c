#include "ReturnChannel_Cmd.h"

#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */

#define Ref_BufferSize 64

//extern int port_init(int num);
//extern void DTV_Close();
#define RCVBUFSIZE     15

int gRcvSocket;
struct sockaddr_in rcvAddr;

static int rcv_socket_init(void)
{
	int sock;
	//struct sockaddr_in rcvAddr;
	sock = socket(AF_INET,SOCK_DGRAM,0); 


	memset(&rcvAddr, 0, sizeof(rcvAddr));
	rcvAddr.sin_family = AF_INET;
	rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rcvAddr.sin_port = htons(5561+1); 
	if (bind(sock, (struct sockaddr *) &rcvAddr, sizeof(struct sockaddr)) < 0)
	{
		printf("error bind failed");
		close(sock);
		sock = -1;
	}
	return sock;
}



void receive_packet(unsigned char rx_buf[RCVBUFSIZE])
{
	int rec_size=0;
	int recvaddr_len=0;
	int i=0;

	recvaddr_len=sizeof(struct sockaddr_in);

	rec_size=recvfrom(gRcvSocket, (void *)rx_buf, RCVBUFSIZE, 0, (struct sockaddr *)&rcvAddr, &recvaddr_len);
	//if(rec_size!=RCVBUFSIZE)
	//{
	printf("rec_size = %d\n", rec_size);
	//}

	printf("rx_buf data:");
	for(i=0;i<RCVBUFSIZE;i++)
	{
	printf("%02x ",rx_buf[i]);
	} 
	printf("\n");
}


void main()
{
	int freq;
	printf("please input freq \n");
	//scanf("%d", &freq);
	int ret;
	//freq_init(237000, 6000);
	//freq_init(189000, 6000);
	//ret = device_open();
	//printf("ret = %d\n", ret);
	//if(ret <= 0)
	//return;

	ret = port_init(22);
	if(ret !=0)
	return ;

	SystemTime time;
	OSDInfo osdinfo;
	Byte byRefStringBuffer[Ref_BufferSize]="goldsun1";
	
	
	time.UTCHour = 10;
	time.UTCMillisecond = 10;
	time.UTCMonth = 5;
	time.UTCYear = 2017;
	systemtime_set(time);
	
	//scanf("%d", ret);

	osdinfo.dateEnable		= 1;
	osdinfo.datePosition		= 5;
	osdinfo.dateFormat		= 0;
	osdinfo.timeEnable		= 1;
	osdinfo.timePosition		= 5;
	osdinfo.dateFormat		= 0;
	osdinfo.logoEnable		= 0;
	osdinfo.logoPosition		= 0;
	osdinfo.logooption		= 0;
	osdinfo.detailInfoEnable	= 0;
	osdinfo.detailInfoPosition	= 0;
	osdinfo.detailInfooption	= 0;
	osdinfo.textEnable		= 1;
	osdinfo.textPosition	= 3;

	//Scanf_String(byRefStringBuffer, Ref_BufferSize);
	 //Cmd_StringClear(&osdinfo.text);
	  Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &osdinfo.text);	
							
	set_osdinfo(osdinfo);

	de_init();
	DTV_Close();
	printf("exit \n");

}
 
