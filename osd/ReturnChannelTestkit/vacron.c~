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
#define LENGTH_OF_LISTEN_QUEUE 20
 
#define RCVBUFSIZE     1000
#define SNDBUFSIZE     150
#define LISTEN_SERVER_PORT   50000
unsigned char rx_buf[RCVBUFSIZE];
unsigned char tx_buf[SNDBUFSIZE];

int exit_flag = False;

extern RCShell shell;
extern int iSelectIndex;
extern RCHostInfo deviceInfoList[16];
extern void funReplyCallBack(void* deviceInfo, Word command);
extern void funFailCallBack(void* deviceInfo, Word command);



  struct sockaddr_in server_addr;
int server_socket;

void socket_init()
{
    //设置一个socket地址结构server_addr,代表服务器internet地址, 端口
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(LISTEN_SERVER_PORT);
 
    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
      server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
    { 
	   int opt =1;
	   setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    }
     
    //把socket和socket地址结构联系起来
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", LISTEN_SERVER_PORT); 
        exit(1);
    }
 
    //server_socket用于监听
    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!"); 
        exit(1);
    }
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

void usb_list( char string[200])
{
	FILE *file;
    char line[20];
	int i= 0, j=0;
	bzero(string, 200);
    file = popen("ls /dev/ | grep usb-it930", "r");
    if (NULL != file)
    {
        while (fgets(line, 20, file) != NULL)
        {
            printf("line=%s\n", line);
		strcat(string, line);
	 
        }

    }
	
    else
    {
        
    }
	printf("usb = %s  %d\n", string , strlen(string));
    pclose(file);
     
}


void Stop(int signo)
{
    printf("oops! stop!!!\n");
    exit_flag = True;

  //关闭监听用的socket
    close(server_socket);

	//close(gRcvSocket);
	//close(gSndSocket);

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

int time_position_parse(char *buff, int len)
{
	int i=0, j=0 ;
	int cnt;
	char num[2];
	int flag =0 ;

	memset(num,0,6);
	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "time_pos=", 9) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+9;
			 printf("find time postion \n");
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
	printf("time position = %s\n", num);
	return  atoi(num);
}

int osd_position_parse(char *buff, int len)
{
	int i=0, j=0 ;
	int cnt;
	char num[2];
	int flag =0 ;

	memset(num,0,6);
	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "osd_pos=", 8) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+8;
			 printf("find osd position \n");
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
	printf("time position = %s\n", num);
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
		if(memcmp(&buff[i], "setosd=", 7) == 0)
		{
			 cnt = i;
			 flag = 1;
			 i=i+7;
			 printf("find osd \n");
		}

		if(flag == 1 && buff[i]== 0x3b && buff[i+1]== 0x3b)
		{
			if(i+1 >= len)
			{
				break;
			}
			else if(i+1 < len && buff[i+2] !=0x3b)
			{
				break;
			}
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


int time_parse(char *buff, int len)
{
	int i=0, j=0 ;

	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "timesetting=ok", 10) == 0)
		{
			printf("time setting \n");
			return 1;
		}

	}
	 
	return 0;
}

int readosd_parse(char *buff, int len)
{
	int i=0, j=0 ;

	for(i =0 ;i < len ; i++)
	{
		if(memcmp(&buff[i], "getosd=ok", 9) == 0)
		{
			printf("readosd=ok \n");
			return 1;
		}

	}
	 
	return 0;
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
	int time_flag = 0;
	int getosd =0;
	int num = 0;  // device num index it91370x
	int cnt =0;  // send data length 
	//if(gRcvSocket < 0)
	//return;

	socket_init();
	while(1)
	{

        //定义客户端的socket地址结构client_addr
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
 
        //接受一个到server_socket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
  	printf("waitting connection\n");
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }
         printf("waitting rcv data\n");

	osd_flag = 0;
	num =0;
	freq =0;
	time_flag = 0;
	getosd = 0;
	memset(rx_buf, 0 ,RCVBUFSIZE);
	 
        rec_size = recv(new_server_socket,rx_buf,RCVBUFSIZE,0);
        if (rec_size < 0)
        {
            printf("Server Recieve Data Failed!\n");
            break;
        }
		char p[200];
		if(memcmp(rx_buf, "getdev", 6) == 0)
		{
              	usb_list(p);

			if(send(new_server_socket,p,strlen(p),0)<0)
			{
			    printf("Send fail \n");  
			}

			close(new_server_socket);
			continue;
		}

	

		printf("rec_size = %d \n", rec_size);
		
		getosd = readosd_parse(rx_buf, rec_size);

		freq = freq_parse(rx_buf,rec_size);
		printf("freq = %d\n", freq);

		osd_flag = osd_parse(rx_buf,rec_size);

		num = device_num_parse(rx_buf,rec_size);
		time_flag = time_parse(rx_buf,rec_size);

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

 			if(send(new_server_socket,tx_buf,cnt,0)<0)
		        {
		            printf("Send fail \n");
		            
		        }
		 	 //关闭与客户端的连接
			close(new_server_socket);

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
				 
			}
		 
		}
		if(init_status == 0)
		{
			char buf[] = "init=false;";
			cnt = sprintf(tx_buf,"freq=%d;index=%d;%s",freq,num,buf);
			if(send(new_server_socket,tx_buf,cnt,0)<0)
		        {
		            printf("Send fail \n");
		            
		        }
		 	 //关闭与客户端的连接
			close(new_server_socket);
			continue;
		}
		printf("init finish \n");

		if(time_flag == 1)
		{
			get_system_time();
			command = CMD_SetSystemDateAndTimeInput;
			deviceInfoList[iSelectIndex].systemTime.timeZone = rx_buf[2];
			deviceInfoList[iSelectIndex].systemTime.UTCHour = timenow->tm_hour;
			deviceInfoList[iSelectIndex].systemTime.UTCMinute =timenow->tm_min;
			deviceInfoList[iSelectIndex].systemTime.UTCSecond = timenow->tm_sec;
			deviceInfoList[iSelectIndex].systemTime.UTCYear = timenow->tm_year + 1900;
			deviceInfoList[iSelectIndex].systemTime.UTCMonth = timenow->tm_mon + 1;
			deviceInfoList[iSelectIndex].systemTime.UTCDay = timenow->tm_mday;
			deviceInfoList[iSelectIndex].systemTime.UTCMillisecond  = (((Word)rx_buf[9]) << 8) || (Word)rx_buf[10];
			deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
			error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
			if(error == 0)
			{

				char buf[] = "timesetting=ok;";
				cnt += sprintf(tx_buf+cnt,"%s",buf);			 
			}
			else
			{
				char buf[] = "timesetting=false;";
				cnt += sprintf(tx_buf+cnt,"%s",buf); 
			}
		}
		if(osd_flag == 1)
		{
			command = CMD_SetVideoOSDConfigurationInput;
			printf("osd string setting \n");
			//printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
			deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable = 1;
			//printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");

			deviceInfoList[iSelectIndex].videoOSDConfig.datePosition = time_position_parse(rx_buf,rec_size);
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
			deviceInfoList[iSelectIndex].videoOSDConfig.textPosition = osd_position_parse(rx_buf,rec_size);
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
		if(getosd ==1)
		{
			command =CMD_GetVideoOSDConfigurationInput;
			error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack,funFailCallBack);
			printf("16. text		= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData);

			cnt += sprintf(tx_buf+cnt,"osd=%s;",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData);
		}


		if(init_status == 1)
		{
			//send_packet(tx_buf,cnt);
    			 //发送buffer中的字符串到new_server_socket,实际是给客户端
		        if(send(new_server_socket,tx_buf,cnt,0)<0)
		        {
		            printf("Send fail \n");
		            
		        }
		 	 //关闭与客户端的连接
			close(new_server_socket);

			init_status = 0;  //de-init
			DTV_Close();
			de_init();
			printf("DTV Close \n");
		}

	 
		 

	}
	printf("exit \n");


}

