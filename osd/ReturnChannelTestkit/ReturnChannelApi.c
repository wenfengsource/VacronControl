#include "ReturnChannel_Shell_Host/ReturnChannel_Shell.h"
#include "rs232.h"


#define Ref_BufferSize 64
#define BroadcastWaitTime 3 
#define WaitTime 10 
#define BIT_MASK(A,B) (A & (1 << B)) != 0

#ifdef __linux__   /* Linux */
#define SCANF scanf
#else
#include <conio.h>
#define SCANF scanf_s
#define IDC_MAIN_EDIT	102
HWND hEdit;
HWND Metadata_window;
#endif

Bool IsHostRun = True;

void Scanf_String(Byte* buf, int size )
{
#ifdef __linux__   /* Linux */
	SCANF("%s", buf);
#else
	SCANF("%s", buf, size);
#endif
}
// ===================================================================== //
//                                                            Function for printing Metadata                                                               //
// ===================================================================== //
#ifdef __linux__   /* Linux */
FILE *Terminal_Handle = NULL;

void OpenNewWindow_ForMetadata(int *NewTerminal_DevNum)
{
	char tempString[20] = {0};
	//int res, temp = 0;

	while(1)
	{
		snprintf(tempString, sizeof(tempString), "/dev/pts/%d", *NewTerminal_DevNum);
		Terminal_Handle=fopen(tempString,"w");
		if(Terminal_Handle == NULL)
		{
			printf("Open terminal handle %d (%s)", *NewTerminal_DevNum, tempString);
			break;
		}
		//else
		//	fprintf(Terminal_Handle, "This terminal handle is %s\n", tempString);

		(*NewTerminal_DevNum)++;

		fclose(Terminal_Handle);
	}

	system("gnome-terminal");
	sleep(1); //wait for terminal is activated
	snprintf(tempString, sizeof(tempString), "/dev/pts/%d", *NewTerminal_DevNum);
	Terminal_Handle=fopen(tempString,"w");
	if(Terminal_Handle == NULL)
	{
		printf("Open terminal handle %s fail !!!", tempString);
		return;
	}

	//fprintf(Terminal_Handle, "Open terminal handle %s OK !!!\n", tempString);
}

#else

LRESULT __stdcall WindowProcedure( HWND window, unsigned int msg, WPARAM wp, LPARAM lp )
{
	switch(msg)
	{
	case WM_CREATE:
		{
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
				TEXT("EDIT"),
				TEXT(""),
				WS_CHILD|WS_VISIBLE|WS_VSCROLL|
				ES_MULTILINE|ES_AUTOVSCROLL,
				5,
				5,
				560,
				750,
				window,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);
			break;
		}


	case WM_DESTROY:
		printf("destroying window\n") ;

		PostQuitMessage(0) ;
		return 0L ;

	default:            
		return (long)DefWindowProc( window, msg, wp, lp ) ;
	}

	return 0;
}
void MetadataPrintf(const TCHAR * fmt, ...)  
{  
	TCHAR MyPrintfBuf[65535] = {0};  
	va_list args;
	int nLength = GetWindowTextLength(hEdit);
	int nLines = 0;

	va_start(args, fmt);  
	vswprintf(MyPrintfBuf, sizeof(MyPrintfBuf), fmt, args);  
	va_end(args);	

	nLines = (int)SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);


	if(nLines < 500)
	{
		SendMessage(hEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		SendMessage(hEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)MyPrintfBuf);
	}
	else
	{
		SendMessage(hEdit, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
		SendMessage(hEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)MyPrintfBuf);
	}  

} 

unsigned int __stdcall MetadataThread(void *data)
{
	LPCWSTR myclass = TEXT("myclass"); 

	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
		0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
		LoadCursor(0,IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
		0, myclass, LoadIcon(0,IDI_APPLICATION) } ;
	if( RegisterClassEx(&wndclass) )
	{
		Metadata_window = CreateWindowEx( 0, myclass, TEXT("Metadata"),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			/*CW_USEDEFAULT, CW_USEDEFAULT,*/600, 800, 0, 0, GetModuleHandle(0), 0 ) ;
		if(Metadata_window)
		{
			MSG msg;
			ShowWindow( Metadata_window, SW_SHOW ) ;

			while( GetMessage( &msg, 0, 0, 0 ) ) 
			{
				DispatchMessage(&msg) ;
			}
		}
	}

	return 0;
}
#endif

// ===================================================================== //
//                                                          Return Channel CallBack Function                                                            //
// ===================================================================== //

void funBroadReplyCallBack(void* deviceInfo, Word command)
{
	RCHostInfo* pRCHostInfo = (RCHostInfo*) deviceInfo;

	printf("\n===========User Define Get BroadCast Reply CallBack===========\n");
	printf("    Command = 0x%04x  ",command);
	printf("    TxID = 0x%04x  ",pRCHostInfo->device.clientTxDeviceID);
	printf("    RxID = 0x%04x!!!\n",pRCHostInfo->device.hostRxDeviceID);
	printf("===========User Define Get BroadCast Reply CallBack===========\n");
}


void funReplyCallBack(void* deviceInfo, Word command)
{
	RCHostInfo* pRCHostInfo = (RCHostInfo*) deviceInfo;

	printf("\n===========User Define Get Reply CallBack===========\n");
	printf("    Command = 0x%04x  ",command);
	printf("    TxID = 0x%04x  ",pRCHostInfo->device.clientTxDeviceID);
	printf("    RxID = 0x%04x!!!\n",pRCHostInfo->device.hostRxDeviceID);
	printf("===========User Define Get Reply CallBack===========\n");
}

void funFailCallBack(void* deviceInfo, Word command)
{
	RCHostInfo* pRCHostInfo = (RCHostInfo*) deviceInfo;

	printf("\n===========User Define Get Reply Fail CallBack===========\n");
	printf("    Command = 0x%04x  ",command);
	printf("    TxID = 0x%04x  ",pRCHostInfo->device.clientTxDeviceID);
	printf("    RxID = 0x%04x!!!\n",pRCHostInfo->device.hostRxDeviceID);
	printf("===========User Define Get Reply Fail CallBack===========\n");
}

void funEventReplyCallBack(void* deviceInfo, Word command)
{
	RCHostInfo* pRCHostInfo = (RCHostInfo*) deviceInfo;

	printf("\n===========User Define Get Event Reply CallBack===========\n");
	printf("    Command = 0x%04x  ",command);
	printf("    TxID = 0x%04x  ",pRCHostInfo->device.clientTxDeviceID);
	printf("    RxID = 0x%04x!!!\n",pRCHostInfo->device.hostRxDeviceID);
	printf("===========User Define Get Event Reply CallBack===========\n");
}

// ===================================================================== //
//                                                         Return Channel Host Main Function                                                           //
// ===================================================================== //

RCShell shell;
RCHostInfo deviceInfoList[16];
int iSelectIndex;
int port_num = 22;
int bIsOpenPort = True;
int  port_init(int num)
{	
	int error ;
	port_num = num;
	printf("port_num = %d baudrate= %d\n ", port_num, RETURN_CHANNEL_BAUDRATE);
	error = User_returnChannelBusOpen(port_num, RETURN_CHANNEL_BAUDRATE);  // Port select
	if(error)
	{
		bIsOpenPort =False;
		return error;
	}
    error = RC_Shell_Init( &shell, deviceInfoList, 16, port_num, port_num);
	//------------------Start Recive Command Thread------------------
	RC_Shell_Start_RcvThread(&shell);

	error =RC_Shell_BroadCast(&shell, funBroadReplyCallBack, BroadcastWaitTime);
	if(error != 0)
	{
		 
		return error;
	}
	printf("\nDevice Index (%x):  TX ID: %04x RX ID: %04x \n",0,deviceInfoList[0].device.clientTxDeviceID,deviceInfoList[0].device.hostRxDeviceID);
	printf("device num = %d \n", shell.iListSize);
	if(shell.iListSize < 1)
		return -1;

	iSelectIndex = shell.iListSize -1;

	return error;
	
}

void de_init()
{

	if(bIsOpenPort)
	{
		RC_Shell_Stop_RcvThread(&shell);
		printf("RcvThread exit \n");
		RC_Shell_UnInit(&shell);
		printf("RC_Shell_UnInit exit \n");
	}
	User_returnChannelBusClose(port_num);
	printf("User_returnChannelB)\n");

}

int systemtime_set(SystemTime time)
{
	if(iSelectIndex<0)
	return False;

	int error;
	int get_Command;
	int command = CMD_SetSystemDateAndTimeInput;
	int bIsBroadCast = False;
	int bIsBlock = True;

	
	deviceInfoList[iSelectIndex].systemTime = time;
	deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
	error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);


//	printf("Send GetSystemDateAndTime For Getting Information.....\n");
//
//	get_Command = CMD_GetSystemDateAndTimeInput;
//	error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
//	if(!error)
//	{
//
//	}else
//	{
//		printf("Can Not GetSystemDateAndTime Before Set!\n");
//	}
//
//	printf("\n------------Default Value List------------\n");
//	printf("1. countryCode			= %c%c%c\n",deviceInfoList[iSelectIndex].systemTime.countryCode[0],deviceInfoList[iSelectIndex].systemTime.countryCode[1],deviceInfoList[iSelectIndex].systemTime.countryCode[2]);
//	printf("2. countryRegionID		= %u\n",deviceInfoList[iSelectIndex].systemTime.countryRegionID);
//	printf("3. daylightSavings		= %u\n",deviceInfoList[iSelectIndex].systemTime.daylightSavings);
//	printf("4. timeZone			= 0x%02x\n",deviceInfoList[iSelectIndex].systemTime.timeZone);
//	printf("5. UTCHour			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCHour);
//	printf("6. UTCMinute			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMinute);
//	printf("7. UTCSecond			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCSecond);
//	printf("8. UTCYear			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCYear);
//	printf("9. UTCMonth			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMonth);
//	printf("10. UTCDay			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCDay);
//	printf("11. UTCMillisecond		= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMillisecond);
//	printf("12. timeAdjustmentMode		= %u\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentMode);
//	printf("13. timeAdjustmentCriterionMax	= %lu\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMax);
//	printf("14. timeAdjustmentCriterionMin	= %lu\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMin);
//	printf("15. timeSyncDuration		= %u\n",deviceInfoList[iSelectIndex].systemTime.timeSyncDuration);


}

int systemtime_get(SystemTime time)
{
	if(iSelectIndex<0)
	return False;

	int error;
	int get_Command;
	int command = CMD_SetSystemDateAndTimeInput;
	int bIsBroadCast = False;
	int bIsBlock = True;


	//deviceInfoList[iSelectIndex].systemTime = time;
	//deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
	//error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);


	printf("Send GetSystemDateAndTime For Getting Information.....\n");
	
	get_Command = CMD_GetSystemDateAndTimeInput;
	error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
	if(!error)
	{

	}else
	{
		printf("Can Not GetSystemDateAndTime Before Set!\n");
	}

	printf("\n------------Default Value List------------\n");
	printf("1. countryCode			= %c%c%c\n",deviceInfoList[iSelectIndex].systemTime.countryCode[0],deviceInfoList[iSelectIndex].systemTime.countryCode[1],deviceInfoList[iSelectIndex].systemTime.countryCode[2]);
	printf("2. countryRegionID		= %u\n",deviceInfoList[iSelectIndex].systemTime.countryRegionID);
	printf("3. daylightSavings		= %u\n",deviceInfoList[iSelectIndex].systemTime.daylightSavings);
	printf("4. timeZone			= 0x%02x\n",deviceInfoList[iSelectIndex].systemTime.timeZone);
	printf("5. UTCHour			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCHour);
	printf("6. UTCMinute			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMinute);
	printf("7. UTCSecond			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCSecond);
	printf("8. UTCYear			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCYear);
	printf("9. UTCMonth			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMonth);
	printf("10. UTCDay			= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCDay);
	printf("11. UTCMillisecond		= %u\n",deviceInfoList[iSelectIndex].systemTime.UTCMillisecond);
	printf("12. timeAdjustmentMode		= %u\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentMode);
	printf("13. timeAdjustmentCriterionMax	= %lu\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMax);
	printf("14. timeAdjustmentCriterionMin	= %lu\n",deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMin);
	printf("15. timeSyncDuration		= %u\n",deviceInfoList[iSelectIndex].systemTime.timeSyncDuration);

	
}


int set_osdinfo(OSDInfo info)
{
	int error;
	int get_Command;
	int command = CMD_SetOSDInformationInput;
	int bIsBroadCast = False;
	int bIsBlock = True;
	if(iSelectIndex<0)
	return False;

	printf("Send GetOSDInformation For Getting Information.....\n");
	get_Command = CMD_GetOSDInformationInput;
	error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
	if(!error)
	{

	}else
	{
		printf("Can Not GetOSDInformation Before Set!\n");

	}
	if(info.text.stringData != NULL)
	{
		Cmd_StringClear(&deviceInfoList[iSelectIndex].osdInfo.text);
		//deviceInfoList[iSelectIndex].osdInfo.text =  info.test;
	}
	else
	{
		info.text = deviceInfoList[iSelectIndex].osdInfo.text;
	}

	deviceInfoList[iSelectIndex].osdInfo = info;


	printf("1. dateEnable		= %u\n",deviceInfoList[iSelectIndex].osdInfo.dateEnable);
	printf("2. datePosition		= %u\n",deviceInfoList[iSelectIndex].osdInfo.datePosition);
	printf("3. dateFormat		= %u\n",deviceInfoList[iSelectIndex].osdInfo.dateFormat);
	printf("4. timeEnable		= %u\n",deviceInfoList[iSelectIndex].osdInfo.timeEnable);
	printf("5. timePosition		= %u\n",deviceInfoList[iSelectIndex].osdInfo.timePosition);
	printf("6. dateFormat		= %u\n",deviceInfoList[iSelectIndex].osdInfo.timeFormat);
	printf("7. logoEnable		= %u\n",deviceInfoList[iSelectIndex].osdInfo.logoEnable);
	printf("8. logoPosition		= %u\n",deviceInfoList[iSelectIndex].osdInfo.logoPosition);
	printf("9. logooption		= %u\n",deviceInfoList[iSelectIndex].osdInfo.logooption);
	printf("10. detailInfoEnable	= %u\n",deviceInfoList[iSelectIndex].osdInfo.detailInfoEnable);
	printf("11. detailInfoPosition	= %u\n",deviceInfoList[iSelectIndex].osdInfo.detailInfoPosition);
	printf("12. detailInfooption	= %u\n",deviceInfoList[iSelectIndex].osdInfo.detailInfooption);
	printf("13. textEnable		= %u\n",deviceInfoList[iSelectIndex].osdInfo.textEnable);
	printf("14. textPosition	= %u\n",deviceInfoList[iSelectIndex].osdInfo.textPosition);
	printf("15. text		= %s\n",deviceInfoList[iSelectIndex].osdInfo.text.stringData);

	error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);



}
//
//int set_osd_config(VideoOSDConfig osdconfig)
//{
//	int get_Command = CMD_GetVideoOSDConfigurationInput;
//
//	int error;
//
//
//	int bIsBroadCast = False;
//	int bIsBlock = True;
//	if(iSelectIndex<0)
//	return False;
//
//	error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
//	if(!error)
//	{
//
//	}else
//	{
//		printf("Can Not GetVideoOSDConfiguration Before Set!\n");
//
//	}
//	if(osdconfig.text.stringData != NULL)
//	{
//		Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.text);
//	}
//	else
//	{
//		osdconfig.text = deviceInfoList[iSelectIndex].videoOSDConfig.text;
//	}
//
//	deviceInfoList[iSelectIndex].videoOSDConfig = osdconfig;
//
//	Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
//						printf("\nEnter VideoSourceToken :  ");
//						Scanf_String(byRefStringBuffer, Ref_BufferSize);
//						check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
//
//	error = RC_Shell_Send(&shell, iSelectIndex, CMD_SetVideoOSDConfigurationInput, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
//
//
//}
