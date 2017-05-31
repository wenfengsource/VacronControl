
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <sys/wait.h>
#include <sys/types.h>

#define Ref_BufferSize 64
#define BroadcastWaitTime 3
#define WaitTime 10
#define True  1
#define False 0
//extern int port_init(int num);
//extern void DTV_Close();
#define RCVBUFSIZE     80
#define SNDBUFSIZE     80

int gRcvSocket;
int gSndSocket;
int exit_flag = False;

struct sockaddr_in rcvAddr;
struct sockaddr_in gSndAddr;




static int rcv_socket_init(void)
{
	int sock;
	//struct sockaddr_in rcvAddr;
	sock = socket(AF_INET,SOCK_DGRAM,0); 


	memset(&rcvAddr, 0, sizeof(rcvAddr));
	rcvAddr.sin_family = AF_INET;
	//rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rcvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	rcvAddr.sin_port = htons(5561);
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
	for(i=0;i<rec_size;i++)
	{
	printf("%02x ",rx_buf[i]);
	} 
	printf("\n");
}



void Stop(int signo)
{
    printf("oops! stop!!!\n");
    exit_flag = True;

	close(gRcvSocket);
	//close(gSndSocket);

    _exit(0);
}

void main()
{
	unsigned char rx_buf[RCVBUFSIZE];
	unsigned char tx_buf[SNDBUFSIZE];

	gRcvSocket = rcv_socket_init();

	unsigned char tt[]="中国";
	int i;
	for(i=0; i<sizeof(tt); i++)
	{
		printf("tt = %02x \n" ,tt[i]);
	}
	signal(SIGINT, Stop);
	exit_flag = False;
	while(exit_flag == False)
	{

		memset(rx_buf, 0 ,RCVBUFSIZE);
		receive_packet(rx_buf);
	
	}
	printf("exit \n");


}

