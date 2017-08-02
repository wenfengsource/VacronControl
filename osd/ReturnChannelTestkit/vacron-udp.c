#include "ReturnChannel_Cmd.h"
#include "ReturnChannel_Shell_Host/ReturnChannel_Shell.h"
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <sys/wait.h>
#include <sys/types.h>
#include<time.h>

#define Ref_BufferSize 64
#define BroadcastWaitTime 3
#define WaitTime 10
#define RCV_PORT   5562
#define SND_PORT   5561
//extern int port_init(int num);
//extern void DTV_Close();
#define RCVBUFSIZE     1000
#define SNDBUFSIZE     150

unsigned char rx_buf[RCVBUFSIZE];
unsigned char tx_buf[SNDBUFSIZE];

int gRcvSocket;
int gSndSocket;
int exit_flag = False;

struct sockaddr_in rcvAddr;
struct sockaddr_in gSndAddr;
struct sockaddr_in src_Addr;

extern RCShell shell;
extern int iSelectIndex;
extern RCHostInfo deviceInfoList[16];
extern void funReplyCallBack(void* deviceInfo, Word command);
extern void funFailCallBack(void* deviceInfo, Word command);


static int rcv_socket_init(void)
{
	int sock;
	//struct sockaddr_in rcvAddr;
	sock = socket(AF_INET,SOCK_DGRAM,0);


	memset(&rcvAddr, 0, sizeof(rcvAddr));
	rcvAddr.sin_family = AF_INET;
	rcvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rcvAddr.sin_port = htons(RCV_PORT);
	if (bind(sock, (struct sockaddr *) &rcvAddr, sizeof(struct sockaddr)) < 0)
	{
		printf("error bind failed");
		close(sock);
		sock = -1;
	}
	return sock;
}

static int snd_socket_init(void)
{
	int sock;
	sock = socket(AF_INET,SOCK_DGRAM,0);


	memset(&gSndAddr, 0, sizeof(gSndAddr));
	gSndAddr.sin_family = AF_INET;
	//gSndAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	gSndAddr.sin_addr.s_addr = src_Addr.sin_addr.s_addr;

	printf("ip= %s",inet_ntoa(src_Addr.sin_addr));

	gSndAddr.sin_port = htons(SND_PORT);

	return sock;
}

void send_packet(unsigned char *tx_buf ,int length)//SNDBUFSIZE=30
{
	gSndSocket = snd_socket_init();
	int i=0;
	printf("tx_buf = %s\n",tx_buf);
	//for(i=0;i<length;i++)
	//{
	//printf("%02x ",tx_buf[i]);
	//}
	//printf("\n");
	sendto(gSndSocket, tx_buf, length, 0, (struct sockaddr*)&gSndAddr, sizeof(struct sockaddr_in));

	close(gSndSocket);
}

//
send_packet_cb()
{
	gSndSocket = snd_socket_init();
	send_packet(tx_buf, 10);

	close(gSndSocket);

}

int receive_packet(unsigned char rx_buf[RCVBUFSIZE])
{
	int rec_size=0;
	int recvaddr_len=0;
	int i=0;

	recvaddr_len=sizeof(struct sockaddr_in);

	rec_size=recvfrom(gRcvSocket, (void *)rx_buf, RCVBUFSIZE, 0, (struct sockaddr *)&src_Addr, &recvaddr_len);
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
	return rec_size;
}


int Vacron_init(int freq, int num)
{

	//scanf("%d", &freq);
	int ret = 0;
	//freq_init(237000, 6000);
	freq_init(freq, 6000);
	ret = device_open(num);
	printf("ret = %d\n", ret);
	if(ret < 0)
	{
		DTV_Close();

		return ret;
	}
	ret = port_init(22);

	if(ret !=0)
	{
		DTV_Close();
		de_init();
	}
	return ret;

}

void Stop(int signo)
{
    printf("oops! stop!!!\n");
    exit_flag = True;

	close(gRcvSocket);
	close(gSndSocket);

    _exit(0);
}

int freq_parse(char *buff, int len)
{
	int i=0, j=0 ;
	int cnt;
	char freq[6];
	int flag =0 ;

	memset(freq,0,6);
	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "freq=", 5) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+5;
			 printf("find freq \n");
		}

		if((flag == 1) && (buff[i]== 0x3b))
		{
			printf(" buff[i] = %02x\n",  buff[i]);
			break;
		}

		if(flag == 1)
		{
			freq[j++] = buff[i];

		}


	}
	printf("j = %d \n", j);
	printf("freq = %s\n", freq);
	return  atoi(freq);
}

int device_num_parse(char *buff, int len)
{
	int i=0, j=0 ;
	int cnt;
	char num[2];
	int flag =0 ;

	memset(num,0,6);
	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "index=", 6) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+6;
			 printf("find freq \n");
		}

		if((flag == 1) && (buff[i]== 0x3b))
		{
			printf(" buff[i] = %02x\n",  buff[i]);
			break;
		}

		if(flag == 1)
		{
			num[j++] = buff[i];
		}


	}
	printf("j = %d \n", j);
	printf("num = %s\n", num);
	return  atoi(num);
}

void translateUnicodeToUtf(char *src,char *des,int size) /*src is source address ，res is destination address*/
{
	int i,j;
	for ( i = 0, j = 0; i < size; i++ , j++)
	{
	   if ( src[i] )
	   printf("src[i]=%2x, src[i+1]=%x, i=%d\n",src[i],src[i+1],i);
	   if ( !( src[i] & (0X0FF) ) ) /* judge ascii code , user one utf code */
	   {
		i++;
		des[j] = src[i];
		des[j] = des[j] & (0X07F);
		printf("des[j]=%X j=%d\n",des[j],j);
	   }
	   else if ( !( src[i] & (0X0F8) ) )/* use two utf code */
	   {
		des[j] = 0X0C0;
		src[i] = src[i] & 0X07;
		des[j] = des[j] | ( src[i] << 2 );
		i++;
		src[i] = src[i] & 0X0FF;
		des[j] = des[j] | ( src[i] >> 6 );
		j++;
		des[j] = 0X080;
		des[j] = des[j] | ( src[i] & 0X03F ) ;
	   }
	   else /* do as three utf code */
	   {
		des[j] = 0X0E0;
		des[j] = des[j] | ( ( src[i] & 0X0FF ) >> 4 );
		j++;
		des[j] = 0X080;
		des[j] = des[j] | ( ( src[i] & 0X0F ) << 2 );
		i++;
		des[j] = des[j] | ( ( src[i] & 0X0FF ) >> 6 );
		j++;
		des[j] = 0x080;
		des[j] = des[j] | ( src[i] & 0X3F );
	   }
	}/* for */
	des[j] = '\0';
}

char osd_unicode[200];
char osd_utf8[200];

  int osd_parse(char *buff, int len)
{
	int i=0, j=0 ;
	int cnt;
	//char osd_unicode[200];
	//char osd_utf8[200];
	int flag =0 ;
	memset(osd_unicode,0,200);
	memset(osd_utf8,0,200);
	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "osd=", 4) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+4;
			 printf("find osd \n");
		}

		if(flag == 1 && buff[i]== 0x3b)
		{
			break;
		}

		if(flag == 1)
		{
			//osd_unicode[j++] = buff[i];
			osd_utf8[j++] = buff[i];
		}


	}
	//translateUnicodeToUtf(osd_unicode,osd_utf8,j);
	printf("osd_utf8 = %s\n", osd_utf8);
	return flag;
}


 struct tm *timenow;         //实例化tm结构指针

void get_system_time()
{

	time_t  now;         //实例化time_t结构



	time(&now);

	//time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now

	timenow   =   localtime(&now);

	//localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)

	printf("Local   time   is   %s/n",asctime(timenow));
	printf("timenow->tm_mon = %d timenow->tm_mday = %d timenow->tm_year= %d \n", timenow->tm_mon, timenow->tm_mday, timenow->tm_year);
	printf("timenow->tm_min = %d timenow->tm_hour = %d timenow->tm_second= %d \n", timenow->tm_min, timenow->tm_hour, timenow->tm_sec);
	//上句中asctime函数把时间转换成字符，通过printf()函数输出

}

void main()
{


	gRcvSocket = rcv_socket_init();
//	gSndSocket = snd_socket_init();

	int get_Command;
	Dword error = ModulatorError_NO_ERROR;
	Dword check = ModulatorError_NO_ERROR;
	Byte byRefStringBuffer[Ref_BufferSize]="金阳";
	int command;
	int bIsBroadCast = False;
	int bIsBlock = True;
	int init_status = 0;
	signal(SIGINT, Stop);
	exit_flag = False;
	int rec_size, freq;
	//while(exit_flag == False)
	signal(SIGINT, Stop);
	int osd_flag =0;
	int num = 0;  // device num index it91370x
	int cnt =0;  // send data length 
	if(gRcvSocket < 0)
	return;

	while(1)
	{

		printf("waitting cmd \n");
		osd_flag = 0;
		memset(rx_buf, 0 ,RCVBUFSIZE);
		rec_size = receive_packet(rx_buf);
		printf("rec_size = %d \n", rec_size);
		

		freq = freq_parse(rx_buf,rec_size);
		printf("freq = %d\n", freq);

		osd_flag = osd_parse(rx_buf,rec_size);

		num = device_num_parse(rx_buf,rec_size);

		if(freq == 177000 || freq == 189000 || freq==201000 || freq == 213000 || freq == 225000 || freq==237000 || freq ==249000
 		  || freq==261000 || freq ==273000 || freq== 363000 || freq == 375000 || freq == 387000 || freq==399000 || freq ==411000
                   || freq==423000 || freq == 473000)
		{
			
		}
		else
		{
                        printf("freq is not right \n");
			char buf[] = "freq=false;";
			cnt = sprintf(tx_buf,"%s",buf);
			send_packet(buf,cnt);
			close(gSndSocket);
 			continue;  // the freq is not suitable 
		}

		if(init_status == 0)
		{
			error = Vacron_init(freq, num);
			if(error == 0)
			{
				init_status = 1;
				char buf[] ="init=ok;";
				cnt = sprintf(tx_buf,"freq=%d;index=%d;%s",freq,num,buf);
				//send_packet(buf,sizeof(buf));
			}
		//	tx_buf[0] = error;
		//	tx_buf[1] = 0x01;
		//	send_packet(tx_buf, 2);
		}
		if(init_status == 0)
		{
			char buf[] = "init=false;";
			cnt = sprintf(tx_buf,"freq=%d;index=%d;%s",freq,num,buf);
			send_packet(tx_buf,cnt);
			close(gSndSocket);
			continue;
		}
		printf("init finish \n");

		get_system_time();
		command = CMD_SetSystemDateAndTimeInput;
		deviceInfoList[iSelectIndex].systemTime.timeZone = rx_buf[2];
		deviceInfoList[iSelectIndex].systemTime.UTCHour = timenow->tm_hour;
		deviceInfoList[iSelectIndex].systemTime.UTCMinute =timenow->tm_min;
		deviceInfoList[iSelectIndex].systemTime.UTCSecond = timenow->tm_sec;
		deviceInfoList[iSelectIndex].systemTime.UTCYear = timenow->tm_year + 1900;
		deviceInfoList[iSelectIndex].systemTime.UTCMonth = timenow->tm_mon;
		deviceInfoList[iSelectIndex].systemTime.UTCDay = timenow->tm_mday;
		deviceInfoList[iSelectIndex].systemTime.UTCMillisecond  = (((Word)rx_buf[9]) << 8) || (Word)rx_buf[10];
		deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
		error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
		if(error == 0)
		{

			char buf[] = "timesetting=ok;";
			cnt += sprintf(tx_buf+cnt,"%s",buf);
			//send_packet(buf,sizeof(buf));

		}
		else
		{
			char buf[] = "timesetting=false;";
			cnt += sprintf(tx_buf+cnt,"%s",buf);
			//send_packet(buf,sizeof(buf));
		}

		if(osd_flag == 1)
		{
			command = CMD_SetVideoOSDConfigurationInput;
			printf("osd string setting \n");
			//printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
			deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable = 1;
			//printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");

			deviceInfoList[iSelectIndex].videoOSDConfig.datePosition = 0;
			//printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");

			deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat =2;
			//printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable =deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable;// rx_buf[5];
			//printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.timePosition = deviceInfoList[iSelectIndex].videoOSDConfig.datePosition ;// rx_buf[6];
			//printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat =  1;
			//printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable = 0;//rx_buf[8];
			//printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
			//SCANF("%lu", &tempSetting);
			//deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition = rx_buf[9];
			//printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
			//SCANF("%lu", &tempSetting);
			//deviceInfoList[iSelectIndex].videoOSDConfig.logooption = rx_buf[10];
			//printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
			//SCANF("%lu", &tempSetting);
			//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable = rx_buf[11];
			//printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
			//SCANF("%lu", &tempSetting);
			//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition = rx_buf[12];
			//printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
			//SCANF("%lu", &tempSetting);
			//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption = rx_buf[13];
			//printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.textEnable = 1;
			//printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
			//SCANF("%lu", &tempSetting);
			deviceInfoList[iSelectIndex].videoOSDConfig.textPosition = 5;
			//printf("\nEnter Text :  ");

			printf("2. dateEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable);
			printf("3. datePosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.datePosition);
			printf("4. dateFormat		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat);
			printf("5. timeEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable);
			printf("6. timePosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timePosition);
			printf("7. timeFormat		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat);
			printf("8. logoEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable);
			printf("9. logoPosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition);
			printf("10. logooption		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logooption);
			printf("11. detailInfoEnable	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable);
			printf("12. detailInfoPosition	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition);
			printf("13. detailInfoOption	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption);
			printf("14. textEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.textEnable);
			printf("15. textPosition	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.textPosition);



			//error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
			//printf("error = %d \n", error);

			//break;
		//case 0x06:  //osd info set
		//	command = CMD_SetVideoOSDConfigurationInput;
			check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.text);
			check = Cmd_StringSet(osd_utf8, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);

		//	check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);

			printf("16. text		= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData);
			error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
			if(error ==0)
			{
				char buf[] = "osdsetting=ok;";
				//send_packet(buf,sizeof(buf));
				cnt += sprintf(tx_buf+cnt,"%s",buf);
			}
			else
			{
				char buf[] = "osdsetting=false";
				cnt += sprintf(tx_buf+cnt,"%s",buf);
				//send_packet(buf,sizeof(buf));
			}
		}
		if(init_status == 1)
		{
			send_packet(tx_buf,cnt);
			init_status = 0;  //de-init
			DTV_Close();
			de_init();
			printf("DTV Close \n");
		}

		//send_packet_cb();
		close(gSndSocket);


# if 0
		switch(rx_buf[1])
		{
			case 0x01:
				memset(tx_buf,0,SNDBUFSIZE);
				printf("start service \n");
				int freq;
				freq = ntohl(*((int*)&rx_buf[2]));
				printf("freq = %d\n", freq);
				if(init_status == 0)
				{
					error = Vacron_init(freq);
					if(error == 0)
					{
						init_status = 1;
					}
					tx_buf[0] = error;
					tx_buf[1] = 0x01;
					send_packet(tx_buf, 2);
				}

				break;

				//Time Setting
			case 0x02:
				printf("time setting \n");
//				SystemTime time;
//
//				time.timeZone = rx_buf[2];
//				time.UTCHour = rx_buf[3];
//				time.UTCMillisecond = (short)(rx_buf[4])<<8 || (short)rx_buf[5];
//				time.UTCMonth = rx_buf[6];
//				time.UTCYear = 2000+ rx_buf[7];
//				systemtime_set(time);
				command = CMD_SetSystemDateAndTimeInput;
				deviceInfoList[iSelectIndex].systemTime.timeZone = rx_buf[2];
				deviceInfoList[iSelectIndex].systemTime.UTCHour = rx_buf[3];
				deviceInfoList[iSelectIndex].systemTime.UTCMinute = rx_buf[4];
				deviceInfoList[iSelectIndex].systemTime.UTCSecond = rx_buf[5];
				deviceInfoList[iSelectIndex].systemTime.UTCYear = rx_buf[6] + 1970;
				deviceInfoList[iSelectIndex].systemTime.UTCMonth = rx_buf[7];
				deviceInfoList[iSelectIndex].systemTime.UTCDay = rx_buf[8];
				deviceInfoList[iSelectIndex].systemTime.UTCMillisecond  = (((Word)rx_buf[9]) << 8) || (Word)rx_buf[10];
				deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
				error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);

				tx_buf[0] = error;
				tx_buf[1] = 0x02;
				send_packet(tx_buf, 2);
				break;

				// Time Getting
			case 0x03:
				get_Command = CMD_GetSystemDateAndTimeInput;
				error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
				if(!error)
				{

				}else
				{
					printf("Can Not GetSystemDateAndTime Before Set!\n");
					//continue;
				}

				memset(tx_buf,0,SNDBUFSIZE);
				tx_buf[0] = error;
				tx_buf[1] = CMD_GetSystemDateAndTimeInput;
				tx_buf[2] = deviceInfoList[iSelectIndex].systemTime.timeZone;
				tx_buf[3] = deviceInfoList[iSelectIndex].systemTime.UTCHour;
				tx_buf[4] = deviceInfoList[iSelectIndex].systemTime.UTCMinute;
				tx_buf[5] = deviceInfoList[iSelectIndex].systemTime.UTCSecond;
				tx_buf[6] = deviceInfoList[iSelectIndex].systemTime.UTCYear - 1970;
				tx_buf[7] = deviceInfoList[iSelectIndex].systemTime.UTCMonth;
				tx_buf[8] = deviceInfoList[iSelectIndex].systemTime.UTCDay;
				tx_buf[9] = (Byte)(deviceInfoList[iSelectIndex].systemTime.UTCMillisecond >> 8);
				tx_buf[10] = (Byte)(deviceInfoList[iSelectIndex].systemTime.UTCMillisecond & 0x00ff);

				send_packet(tx_buf, 11);
				break;

			case 0x05:
				printf("osd getting \n");
				memset(rx_buf, 0, SNDBUFSIZE);
				get_Command = CMD_GetVideoOSDConfigurationInput;
				check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
				printf("\nEnter VideoSourceToken :  ");

				check = Cmd_StringSet(byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
				error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
				if(!error)
				{

				}else
				{
					printf("Can Not GetVideoOSDConfiguration Before Set!\n");
				}
				tx_buf[0] = error;
				tx_buf[1] = 0x04;
				tx_buf[2]= (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable;
				tx_buf[3] = (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.datePosition;

				tx_buf[4] = (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat;

			//	deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable =deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable;// rx_buf[5];

			//	deviceInfoList[iSelectIndex].videoOSDConfig.timePosition = deviceInfoList[iSelectIndex].videoOSDConfig.datePosition ;// rx_buf[6];

				tx_buf[5] = (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat ;

				//deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable = 0;//rx_buf[8];

				tx_buf[6] = (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.textEnable;

				tx_buf[7] = (Byte)deviceInfoList[iSelectIndex].videoOSDConfig.textPosition;

				printf("text len =%d\n",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringLength);
				memcpy(&tx_buf[8],deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData, deviceInfoList[iSelectIndex].videoOSDConfig.text.stringLength);

				send_packet(tx_buf, 8+deviceInfoList[iSelectIndex].videoOSDConfig.text.stringLength);
				break;

			case 0x04: // osd format set
				command = CMD_SetVideoOSDConfigurationInput;
				printf("osd string setting \n");
				//printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
				deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable = rx_buf[2];
				//printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");

				deviceInfoList[iSelectIndex].videoOSDConfig.datePosition = rx_buf[3];
				//printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");

				deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat = rx_buf[4];
				//printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable =deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable;// rx_buf[5];
				//printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.timePosition = deviceInfoList[iSelectIndex].videoOSDConfig.datePosition ;// rx_buf[6];
				//printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat =  rx_buf[5];
				//printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable = 0;//rx_buf[8];
				//printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
				//SCANF("%lu", &tempSetting);
				//deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition = rx_buf[9];
				//printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
				//SCANF("%lu", &tempSetting);
				//deviceInfoList[iSelectIndex].videoOSDConfig.logooption = rx_buf[10];
				//printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
				//SCANF("%lu", &tempSetting);
				//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable = rx_buf[11];
				//printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
				//SCANF("%lu", &tempSetting);
				//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition = rx_buf[12];
				//printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
				//SCANF("%lu", &tempSetting);
				//deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption = rx_buf[13];
				//printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.textEnable = rx_buf[6];
				//printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
				//SCANF("%lu", &tempSetting);
				deviceInfoList[iSelectIndex].videoOSDConfig.textPosition = rx_buf[7];
				//printf("\nEnter Text :  ");

				printf("2. dateEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable);
				printf("3. datePosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.datePosition);
				printf("4. dateFormat		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat);
				printf("5. timeEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable);
				printf("6. timePosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timePosition);
				printf("7. timeFormat		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat);
				printf("8. logoEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable);
				printf("9. logoPosition		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition);
				printf("10. logooption		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.logooption);
				printf("11. detailInfoEnable	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable);
				printf("12. detailInfoPosition	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition);
				printf("13. detailInfoOption	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption);
				printf("14. textEnable		= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.textEnable);
				printf("15. textPosition	= %u\n",deviceInfoList[iSelectIndex].videoOSDConfig.textPosition);



				//error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
				//printf("error = %d \n", error);

				//break;
			//case 0x06:  //osd info set
			//	command = CMD_SetVideoOSDConfigurationInput;
				check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.text);
				check = Cmd_StringSet(&rx_buf[8], Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);

			//	check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);

				printf("16. text		= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData);
				error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);

				tx_buf[0] = error;
				tx_buf[1] = 0x04;
				send_packet(tx_buf, 2);
				break;
			case 0xff:
				if(init_status == 1)
 				{
					init_status = 0;  //de-init
					DTV_Close();
					de_init();
					printf("DTV Close \n");
 				}

				break;
		}
		//usleep(5000);
#endif
	}
	printf("exit \n");


}

