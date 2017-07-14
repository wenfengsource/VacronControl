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
int main() {
#ifdef __linux__   /* Linux */
	int NewTerminal_DevNum = 0;
#endif
	Dword error = ModulatorError_NO_ERROR;
	Dword check = ModulatorError_NO_ERROR;
	Word	command = 0,temp = 0;
	Word	advanceSetting = 0;
	Word	get_Command = 0;
	int temp_int = 0;
	Dword tempSelect = 0;
	Dword tempSetting = 0;
	Byte IsCompleted = 0;
	Byte IsSetIndexParamsCompleted = 0;
	int selectNum = 0;
	char tempChar;
	int tempIntSetting = 0;
	short tempShortSetting = 0;
	float tempFloatSetting = 0.0;

	Byte i = 0;
	int ret = 0;
	int SetIndex = 0;

	int IsUserReboot = 0;
	Byte enablePort[32] = {0}; 

	Byte TPSEncryptionKey[8]="TPSKey";
	Byte DataEncryptionKey[64]="DataEncryptionKey";


	Byte networkname[32]="ITE network ";
	Byte byRefStringBuffer[Ref_BufferSize]="User Define String";
	Byte byActiveCellsBuffer[4];
	Dword dwActiveCellSize = 0;


	Rule_LineDetector* ptrRule_LineDetector = NULL;
	Rule_FieldDetector* ptrRule_FieldDetector = NULL;
	Rule_MotionDetector* ptrRule_MotionDetector = NULL;
	Rule_Counting* ptrRule_Counting = NULL;
	Rule_CellMotion* ptrRule_CellMotion = NULL;
	Rule_LoiteringDetector* ptrRule_LoiteringDetector = NULL;

	AnalyticsModule_ObjectTracker* ptrAnalyticsModule_ObjectTracker = NULL;

	int TxPort = 0;
	int RxPort = 0;
	RCShell shell;
	int iSelectIndex = 65535;
	Bool bIsBroadCast = False;
	Bool bIsBlock = True;
	RCHostInfo deviceInfoList[16];
	Bool bIsOpenPort = True;


	int iTempIndex = 0;
	
	// --------------------------------------------------------------- //
	//                                          Checkout valid Bus COM port                                           //
	// --------------------------------------------------------------- //
#ifdef __linux__   /* Linux */
	for(i = 0; i < 24; i ++)
	{
		error = User_returnChannelBusOpen( i,RETURN_CHANNEL_BAUDRATE);
		if(!error)
		{
			User_returnChannelBusClose(i);
			enablePort[i] = 1;
		}else
		{
			error = ModulatorError_NO_ERROR;
		}
	}
	system("clear");

	printf("\n=============== IT950x Return Channel API Test  ================");
	printf("\n= API Spec Version    : %02x.%02x                                  =", RETURN_CHANNEL_API_VERSION>>8, RETURN_CHANNEL_API_VERSION);
	printf("\n= API TestKit Version : %02x.%02x.%02x.%02x                            =", (RETURN_CHANNEL_VERSION>>24) & 0xFF, (RETURN_CHANNEL_VERSION>>16) & 0xFF, (RETURN_CHANNEL_VERSION>>8) & 0xFF, RETURN_CHANNEL_VERSION & 0xFF);
	printf("\n=                                                              =");
	printf("\n= Please start video capture when you test return channel API! =");
	printf("\n================================================================\n");

	printf("=========================== Port List ==========================\n");
	printf("Com Port : ");
	for(i = 0; i < 24; i ++)
	{
		if(enablePort[i] == 1)
		{
			printf("%u, ", i  );
		}
	}
	printf("\n=========================== Port List ==========================\n");

	printf("\nSet Tx Bus COM port, ttyS(0~15) or ttyUSB0 ~ ttyUSB5(16~21) or VirtualCom(22) ? ");
	scanf("%d", &temp_int);
	TxPort = temp_int;

	printf("\nSet Rx Bus COM port, ttyS(0~15) or ttyUSB0 ~ ttyUSB5(16~21) or VirtualCom(22) ? ");
	scanf("%d", &temp_int);
	RxPort = temp_int;

#else

	for(i = 0; i < 32; i ++)
	{
		error = User_returnChannelBusOpen( i,RETURN_CHANNEL_BAUDRATE);
		if(!error)
		{
			User_returnChannelBusClose(i);
			enablePort[i] = 1;
		}else
		{
			error = ModulatorError_NO_ERROR;
		}
	}
	system("cls");

	printf("\n=============== IT950x Return Channel API Test  ================");
	printf("\n= API Spec Version    : %02x.%02x                                  =", RETURN_CHANNEL_API_VERSION>>8, RETURN_CHANNEL_API_VERSION);
	printf("\n= API TestKit Version : %02x.%02x.%02x.%02x                            =", (RETURN_CHANNEL_VERSION>>24) & 0xFF, (RETURN_CHANNEL_VERSION>>16) & 0xFF, (RETURN_CHANNEL_VERSION>>8) & 0xFF, RETURN_CHANNEL_VERSION & 0xFF);
	printf("\n=                                                              =");
	printf("\n= Please start video capture when you test return channel API! =");
	printf("\n================================================================\n");

	printf("=========================== Port List ==========================\n");
	printf("Com Port : ");
	for(i = 0; i < 32; i ++)
	{
		if(enablePort[i] == 1)
		{
			printf("%u, ", i +1 );
		}
	}
	printf("\n=========================== Port List ==========================\n");
	printf("\nSet Tx Bus COM port, COM(1~n)? ");
	scanf_s("%d", &temp_int);
	TxPort = (temp_int-1);

	printf("\nSet Rx Bus COM port, COM(1~n)? ");
	scanf_s("%d", &temp_int);
	RxPort = (temp_int-1);
#endif
	
	// --------------------------------------------------------------- //
	//                                      Open Return Channel Bus COM port                                     //
	// --------------------------------------------------------------- //
	//----------UART
	if(RxPort == TxPort){

		error = User_returnChannelBusOpen(RxPort, RETURN_CHANNEL_BAUDRATE);
		if(error)
		{
#ifdef __linux__   /* Linux */
			getchar();
#else
			_getch();
#endif
			bIsOpenPort = False;
			goto exit;
		}
	}else{

		error = User_returnChannelBusOpen(TxPort, RETURN_CHANNEL_BAUDRATE);
		if(error)
		{
#ifdef __linux__   /* Linux */
			getchar();
#else
			_getch();
#endif
			bIsOpenPort = False;
			goto exit;
		}
		error = User_returnChannelBusOpen(RxPort, RETURN_CHANNEL_BAUDRATE);
		if(error)
		{
#ifdef __linux__   /* Linux */
			getchar();
#else
			_getch();
#endif
			bIsOpenPort = False;
			goto exit;
		}
	}
	// --------------------------------------------------------------- //
	//                                       Return Channel Structure Initialize                                       //
	// --------------------------------------------------------------- //
	error = RC_Shell_Init( &shell, deviceInfoList, 16, TxPort, RxPort);
#if RC_POP_WINDOW_PRINT
#ifdef __linux__   /* Linux */
	printf("\nOpen new terminal to print Metadata(0:No, 1:Yes)? ");
	ret = scanf("%hx", &temp);
	if (ret<=0)
		printf("scanf warning\n");
	if(temp == 1)
		OpenNewWindow_ForMetadata(&NewTerminal_DevNum);
#else
	_beginthreadex(NULL, 0, MetadataThread, 0, 0, NULL); 
#endif
#endif
	//------------------Start Recive Command Thread------------------
	RC_Shell_Start_RcvThread(&shell);

	while (IsHostRun) {

		printf("\n\n\n === Host Device API Test === \n");
		// -------------- Show Device Info List ---------------- //
		printf("\n\n=================00 Device Info List =================");
		for(i = 0; i < shell.iListSize; i ++)
			printf("\nDevice Index (%u):  TX ID: %04x RX ID: %04x",i,deviceInfoList[i].device.clientTxDeviceID,deviceInfoList[i].device.hostRxDeviceID);
		if(shell.iListSize == 0)
			printf("\nNo Device in DeviceList !!");
		printf("\n================= Device Info List =================");
		// ---------- Show Advanced Command Setting ---------- //
		printf("\n================= Advanced Command Setting =================");
		if(shell.iListSize >0)
		{
			if(iSelectIndex >= shell.iListSize)
			{
				printf("\nNo Device is selected !!");
			}else
			{
				printf("\nSend to Device Index (%d)	: TX ID: %04x RX ID: %04x",iSelectIndex,  deviceInfoList[iSelectIndex].device.clientTxDeviceID,deviceInfoList[iSelectIndex].device.hostRxDeviceID);
				if(bIsBroadCast)
					printf("\nBroadcast Setting		: ON");
				else
					printf("\nBroadcast Setting		: OFF");
				if(bIsBlock)
					printf("\nBlock Setting			: ON");
				else
					printf("\nBlock Setting			: OFF");
			}
		}else
		{
			printf("\nNo Device in DeviceList !!");
		}
		printf("\n================= Advanced Command Setting =================");
		printf("\n ");
		// ---------- Show Command Main Menu (Service) ---------- //
		printf("\n0: ccHDtv Service");
		printf("\n1: Device Management Service");
		printf("\n2: Device_IO Service");
		printf("\n3: Imaging Service");
		printf("\n4: Media Service");
		printf("\n5: PTZ Service");
		printf("\n6: Video Analytics Service");
		printf("\nE: Metadata Stream Service Service");
		printf("\nF: Advanced Command Setting");
		printf("\nFF: Exit");
		printf("\nEnter Number:  ");

		ret = SCANF("%lx", &tempSelect);
		if (ret<=0)
			printf("scanf warning\n");
		if(tempSelect> 6)
		{
			if(tempSelect == 0xFF)
				goto exit;
			else if( tempSelect > 0xF )
			{
				printf("\nUnsupported Service!  ");
				continue;
			}
		}
		// ---------- Show Command Main Menu (Detail List) ---------- //
		switch(tempSelect)
		{
		case 0:
			printf("\n======== ccHDtv Service ========");
			printf("\n0x0000. GetTxDeviceAddressID");
			printf("\n0x0001. GetTransmissionParameterCapabilities");
			printf("\n0x0002. GetTransmissionParameters");
			printf("\n0x0003. GetHwRegisterValues");
			printf("\n0x0004. GetAdvanceOptions");
			printf("\n0x0005. GetTPSInformation");
			printf("\n0x0006. GetEncryptionConfigurationOptions");
			printf("\n0x0007. GetEncryptionConfigurations");
			printf("\n0x0010. GetSiPsiTable");
			printf("\n0x0011. GetNitLocation");
			printf("\n0x0012. GetSdtService");
			printf("\n0x0013. GetEITInformation");
			printf("\n0x0080. SetTxDeviceAddressID");
			printf("\n0x0081. SetCalibrationTable");
			printf("\n0x0082. SetTransmissionParameters");
			printf("\n0x0083. SetHwRegisterValues");
			printf("\n0x0084. SetAdvanceOptions");
			printf("\n0x0085. SetTPSInformation");
			printf("\n0x0087. SetEncryptionConfigurations");
			printf("\n0x0090. SetSiPsiTable");
			printf("\n0x0091. SetNitLocation");
			printf("\n0x0092. SetSdtService");
			printf("\n0x0093. SetEITInformation");
			printf("\n======== ccHDtv Service ========");
			break;
		case 1:
			printf("\n======== Device Management Service ========");
			printf("\n0x0100. GetCapabilities");
			printf("\n0x0101. GetDeviceInformation");
			printf("\n0x0102. GetHostname");
			printf("\n0x0103. GetSystemDateAndTime");
			printf("\n0x0104. GetSystemLog");
			printf("\n0x0105. GetOSDInformation");
			printf("\n0x0180. MultipleSetCommandApply");
			printf("\n0x0181. SetSystemFactoryDefault");
			printf("\n0x0182. SetHostname");
			printf("\n0x0183. SetSystemDateAndTime");	
			printf("\n0x0184. SetSystemFactoryDefaultBroadcast");
			printf("\n0x0185. SetOSDInformation");		
			printf("\n0x0186. UpgradeSystemFirmware");	
			printf("\n0x0187. SystemReboot");
			printf("\n0x0188. MultipleSetCommandStart");
			printf("\n======== Device Management Service ========");
			break;
		case 2:
			printf("\n======== Device_IO Service ========");
			printf("\n0x0200. GetDigitalInputs");
			printf("\n0x0201. GetRelayOutputs");
			printf("\n0x0202. GetSerialPorts");
			printf("\n0x0203. GetSerialPortConfigurations");
			printf("\n0x0204. GetSerialPortConfigurationOptions");
			printf("\n0x0280. SetRelayOutputState");
			printf("\n0x0281. SetRelayOutputSettings");
			printf("\n0x0282. SendReceiveSerialCommand");
			printf("\n0x0283. SetSerialPortConfiguration");
			printf("\n======== Device_IO Service ========");
			break;
		case 3:
			printf("\n======== Imaging Service ========");
			printf("\n0x0300. GetImagingSettings");
			printf("\n0x0301. GetStatus");
			printf("\n0x0302. GetOptions");
			printf("\n0x037F. GetUserDefinedSettings");
			printf("\n0x0380. SetImagingSettings");
			printf("\n0x0381. Move");
			printf("\n0x0382. Stop");
			printf("\n0x03FF. SetUserDefinedSettings");
			printf("\n======== Imaging Service ========");
			break;
		case 4:
			printf("\n======== Media Service ========");	
			printf("\n0x0400. GetProfiles");
			printf("\n0x0401. GetVideoSources");
			printf("\n0x0402. GetVideoSourceConfigurations");
			printf("\n0x0403. GetGuaranteedNumberOfVideoEncoderInstances");
			printf("\n0x0404. GetVideoEncoderConfigurations");
			printf("\n0x0405. GetAudioSources");
			printf("\n0x0406. GetAudioSourceConfigurations");
			printf("\n0x0407. GetAudioEncoderConfigurations");
			printf("\n0x0408. GetVideoSourceConfigurationOptions");
			printf("\n0x0409. GetVideoEncoderConfigurationOptions");
			printf("\n0x040A. GetAudioSourceConfigurationOptions");
			printf("\n0x040B. GetAudioEncoderConfigurationOptions");
			printf("\n0x0440. GetVideoOSDConfiguration ");
			printf("\n0x0441. GetVideoPrivateArea");
			printf("\n0x0480. SetSynchronizationPoint");
			printf("\n0x0482. SetVideoSourceConfiguration");
			printf("\n0x0484. SetVideoEncoderConfiguration");
			printf("\n0x0486. SetAudioSourceConfiguration");
			printf("\n0x0487. SetAudioEncoderConfiguration");
			printf("\n0x04C0. SetVideoOSDConfiguration");
			printf("\n0x04C1. SetVideoPrivateArea");
			printf("\n0x04C2. SetVideoSourceControl");
			printf("\n======== Media Service ========");	
			break;
		case 5:
			printf("\n======== PTZ Service ========");	
			printf("\n0x0500. GetConfigurations");			
			printf("\n0x0501. GetStatus");	
			printf("\n0x0502. GetPresets");	
			printf("\n0x0580. GotoPreset");	
			printf("\n0x0581. RemovePreset");	
			printf("\n0x0582. SetPreset");
			printf("\n0x0583. AbsoluteMove");	
			printf("\n0x0584. RelativeMove");	
			printf("\n0x0585. ContinuousMove");	
			printf("\n0x0586. SetHomePosition");	
			printf("\n0x0587. GotoHomePosition");	
			printf("\n0x0588. Stop");	
			printf("\n======== PTZ Service ========");	
			break;
		case 6:
			printf("\n======== Video Analytics Service ========");	
			printf("\n0x0600. GetSupportedRules");	
			printf("\n0x0601. GetRules");	
			printf("\n0x0608. GetSupportedAnalyticsModules");	
			printf("\n0x0609. GetAnalyticsModules");	
			printf("\n0x0680. CreateRule");	
			printf("\n0x0681. ModifyRule");	
			printf("\n0x0682. DeleteRule");
			printf("\n0x0688. CreateAnalyticsModule");	
			printf("\n0x0689. ModifyAnalyticsModule");	
			printf("\n0x068A. DeleteAnalyticsModule");
			printf("\n======== Video Analytics Service ========");	
			break;
		case 0xE:
			printf("\n======== Metadata Stream Service ========");	
			printf("\n0xE001. GetMetadataSettings");	
			printf("\n0xE081. SetMetadataSettings");	
			printf("\n======== Metadata Stream Service ========");	
			break;
		case 0xF:
			printf("\n======== Advanced Command Setting ========");	
			printf("\n0x0001. Select Device to send command ");	
			printf("\n0x0002. Switch Broadcast");	
			printf("\n0x0003. Switch Block for Reply");	
			printf("\n0x0004. Show Packet Statistic");	
			printf("\n0x0005. Reset Packet Statistic");	
			printf("\n======== Advanced Command Setting ========");	
			printf("\n0xFFFE. Back                             ");
			printf("\nEnter Number(Hex):  ");
			SCANF ("%hx", &advanceSetting);
			// ---------- Advanced Command Setting ---------- //
			switch(advanceSetting)
			{
			case 1:
				if(shell.iListSize >0)
				{
					while(1)
					{
						printf("\n=================11 Device Info List =================");
						for(i = 0; i < shell.iListSize; i ++)
							printf("\nDevice Index (%x):  TX ID: %04x RX ID: %04x",i,deviceInfoList[i].device.clientTxDeviceID,deviceInfoList[i].device.hostRxDeviceID);
						printf("\n================= Device Info List =================");
						printf("\n\nEnter Device Index(0 ~ %d): ",shell.iListSize-1);

						ret = SCANF ("%d", &iSelectIndex);
						if(iSelectIndex >= shell.iListSize)
						{
							printf("\nOut of range for device Please select once again!\n");
							continue;
						}else
						{
							//RC_Shell_Set_EventCallBack( &shell, iSelectIndex, funEventReplyCallBack);
							printf("\nYou select Device as following: ");
							printf("\n================= Device Setting =================");
							printf("\nDevice Index (%x):  TX ID: %04x RX ID: %04x",iSelectIndex,deviceInfoList[iSelectIndex].device.clientTxDeviceID,deviceInfoList[iSelectIndex].device.hostRxDeviceID);
							printf("\n================= Device Setting =================\n\n");
							break;
						}
					}
				}else
				{
					printf("\nEnter command 0x0000 broadcast to get TX Device! ");
					continue;
				}
				continue;
			case 2:
				if(shell.iListSize >0)
				{
					if(iSelectIndex >= shell.iListSize)
					{
						printf("\nSelect Device Index (%d ) is Out of range for device \nPlease set select Device Index from (0xF: Advanced Command Setting -> 0x1: Select Device to send command) !", iSelectIndex);
						continue;
					}
					printf("\n================= Broadcast Setting =================");
					if(bIsBroadCast)
						printf("\nSelect Device[%d] Broadcast Setting : ON",iSelectIndex);
					else
						printf("\nSelect Device[%d] Broadcast Setting : OFF",iSelectIndex);
					printf("\n================= Broadcast Setting =================");
					printf("\n\nCommand Broadcast ? Enter 1 for Yes or 0 for No: ");
					ret = SCANF ("%lu", &tempSetting);
					if(tempSetting == 1)
						bIsBroadCast = True;
					else
						bIsBroadCast = False;
				}else
				{
					printf("\nEnter command 0x0000 broadcast to get TX Device! ");
					continue;
				}
				continue;
			case 3:
				if(shell.iListSize >0)
				{
					if(iSelectIndex >= shell.iListSize)
					{
						printf("\nSelect Device Index (%d ) is Out of range for device \nPlease set select Device Index from (0xF: Advanced Command Setting -> 0x1: Select Device to send command) !", iSelectIndex);
						continue;
					}
					printf("\n================= Block Setting =================");
					if(bIsBlock)
						printf("\nSelect Device[%d] Block Setting : ON",iSelectIndex);
					else
						printf("\nSelect Device[%d] Block Setting : OFF",iSelectIndex);
					printf("\n================= Broadcast Setting =================");
					printf("\n\nCommand Block for Reply? Enter 1 for Yes or 0 for No: ");
					ret = SCANF ("%lu", &tempSetting);
					if(tempSetting == 1)
						bIsBlock = True;
					else
						bIsBlock = False;
				}else
				{
					printf("\nEnter command 0x0000 broadcast to get TX Device! ");
					continue;
				}
				continue;
			case 4:
				User_DebugInfo_log(&shell.debugInfo);
				continue;
			case 5:
				RC_Reset_DebugInfo(&shell.debugInfo);
				printf("Reset Packet Statistic OK...\n");
				continue;
			case 0xFFFE:
				continue;
			default:
				printf("\nUnsupported Setting! ");
				continue;
			}
		default:
			printf("\nUnsupported Setting! ");
			continue;
		}
		printf("\n0xFFFE. Back                             ");
		printf("\n0xFFFF. Quit                             ");
		printf("\nEnter Number(Hex):  ");

		// --------------------------------------------------------------- //
		//                                          Get and Check Command                                                //
		// --------------------------------------------------------------- //
		ret = SCANF ("%hx", &command);

		if (ret<=0)
			printf("scanf warning\n");
		if(command == 0xFFFE)	// Back to Command Menu
			continue;

		if((shell.iListSize >0) && (command != 0xFFFF) && (command != 0) && (command != CMD_SetSystemFactoryDefaultBroadcastInput) )
		{
			if(iSelectIndex >= shell.iListSize)
			{
				printf("\nSelect Device Index (%d ) is Out of range for device \nPlease set select Device Index from (0xF: Advanced Command Setting -> 0x1: Select Device to send command) !", iSelectIndex);
				continue;
			}else
			{
				IsCompleted = 0;
				IsSetIndexParamsCompleted = 0;
			}
		}else
		{
			if(command !=0x0000 && command !=CMD_SetSystemFactoryDefaultBroadcastInput)
			{
				if(command != 0xFFFF)
				{
					printf("\nEnter command 0x0000 broadcast to get TX Device! ");
					continue;
				}
			}
		}
		// --------------------------------------------------------------------------- //
		//                                                     Command Begin                                                            //
		// --------------------------------------------------------------------------- //
		switch (command) {

				case CMD_GetTxDeviceAddressIDInput:	// 0x0000 : GetTxDeviceID
					error =RC_Shell_BroadCast(&shell, funBroadReplyCallBack, BroadcastWaitTime);

					printf("\n=================22 Device Info List =================");
					for(i = 0; i < shell.iListSize; i ++)
					{
						printf("\nDevice Index (%x):  TX ID: %04x RX ID: %04x",i,deviceInfoList[i].device.clientTxDeviceID,deviceInfoList[i].device.hostRxDeviceID);
					}
					printf("\n================= Device Info List =================");
					if(!error)
						iSelectIndex = 65535;
					continue;

				case CMD_GetTransmissionParameterCapabilitiesInput:				// 0x0001 : GetTransmissionParameterCapabilities
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetTransmissionParametersInput:			// 0x0002 : GetTransmissionParameters
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetHwRegisterValuesInput:			// 0x0003 : GetHwRegisterValues
					printf("\nEnter Processor  (ex: 0: OFDM, 8: LINK)");
					SCANF("%lu", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.processor = (Byte)tempSetting;
					printf("\nEnter registerAddress(hex)  (ex: FF)");
					SCANF("%lx", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.registerAddress = tempSetting;
					printf("\nEnter read size  ");
					SCANF("%lu", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.valueListSize = (Byte)tempSetting;

					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetAdvanceOptionsInput:				// 0x0004 : GetAdvanceOptions
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetTPSInformationInput:				// 0x0005 : GetTPSInformation
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetEncryptionConfigurationOptionsInput:// 0x0006 : GetEncryptionConfigurationOptions
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetEncryptionConfigurationsInput:		// 0x0007 : GetEncryptionConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSiPsiTableInput:				// 0x0010 : GetSiPsiTable
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetNitLocationInput:				// 0x0011 : GetNitLocation
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSdtServiceInput:				// 0x0012 : GetSdtService
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetEITInformationInput:				// 0x0013 : GetEITInformation
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoEncoder Configuration token	= %s\n",deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);
							printf("\nEnter VideoEncoder Configuration token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetTxDeviceAddressIDInput:				// 0x0080 : SetTxDeviceAddressID

					printf("\nEnter IDType  (ex: 0: Random generate by TX Device 1: Set by parameter.)");
					SCANF("%lu", &tempSetting);
					deviceInfoList[iSelectIndex].newTxDevice.IDType = (Byte)tempSetting;
					if(deviceInfoList[iSelectIndex].newTxDevice.IDType == 1)
					{
						printf("\nEnter newTxDevice  (ex: 1~65535)");
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].newTxDevice.deviceAddressID = (Word)tempSetting;
					}else
					{
						deviceInfoList[iSelectIndex].newTxDevice.deviceAddressID = 0;
					}

					error = RC_Shell_Send(&shell, iSelectIndex, command, False, True, WaitTime, funReplyCallBack, funFailCallBack);

					continue;

				case CMD_SetCalibrationTableInput:				// 0x0081 : SetCalibrationTable
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. accessOption	= %u \n",deviceInfoList[iSelectIndex].calibrationTable.accessOption);
						printf("2. tableType	= %u \n",deviceInfoList[iSelectIndex].calibrationTable.tableType);
						printf("3. tableData	= %s \n",deviceInfoList[iSelectIndex].calibrationTable.tableData.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter accessOption  (ex: 0: Write to EEPROM, 1: Write to file, 2: Write to device only )");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].calibrationTable.accessOption = (Byte)tempSetting;
							break;
						case 2:
							printf("\nEnter tableType  (ex: 0: IQ table, 1: DC table)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].calibrationTable.tableType = (Byte)tempSetting;
							break;
						case 3:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].calibrationTable.tableData);
							printf("\nEnter tableData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet(byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].calibrationTable.tableData);
							break;
						case 255:
							printf("\nEnter accessOption  (ex: 0: Write to EEPROM, 1: Write to file, 2: Write to device only )");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].calibrationTable.accessOption = (Byte)tempSetting;
							printf("\nEnter tableType  (ex: 0: IQ table, 1: DC table)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].calibrationTable.tableType = (Byte)tempSetting;
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].calibrationTable.tableData);
							printf("\nEnter tableData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].calibrationTable.tableData);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}

					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;


				case CMD_SetTransmissionParametersInput:				// 0x0082 : SetTransmissionParameters
					printf("Send GetTransmissionParameters For Getting Information.....\n");
					get_Command = CMD_GetTransmissionParametersInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetTransmissionParameters Before Set!\n");
						continue;
					}
					printf("Send GetTransmissionParameterCapabilities For Getting Information.....\n");
					get_Command = CMD_GetTransmissionParameterCapabilitiesInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetTransmissionParameterCapabilities Before Set!\n");
						continue;
					}

					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. bandwidth			= 0x%x	Support : [1]/[2]/[3]/[4]/[5]/[6]/[7]/[8] M : %u/%u/%u/%u/%u/%u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.bandwidth, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 3), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 4), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 5), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 6), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthOptions, 7)  );
						printf("2. frequency			= %lu	Support : %lu ~ %lu\n",deviceInfoList[iSelectIndex].transmissionParameter.frequency, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.frequencyMin, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.frequencyMax);
						printf("3. constellation		= 0x%x	Support : [QPSK]/[16Q]/[64Q] : %u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.constellation, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.constellationOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.constellationOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.constellationOptions, 2));
						printf("4. FFT				= 0x%x	Support : [2]/[8]/[4] K : %u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.FFT, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.FFTOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.FFTOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.FFTOptions, 2));
						printf("5. codeRate			= 0x%x	Support : [1/2]/[2/3]/[3/4]/[5/6]/[7/8] : %u/%u/%u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.codeRate, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.codeRateOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.codeRateOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.codeRateOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.codeRateOptions, 3), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.codeRateOptions, 4));
						printf("6. interval			= 0x%x	Support : [1/32]/[1/16]/[1/8]/[1/4] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.interval, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.guardInterval, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.guardInterval, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.guardInterval, 2), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.guardInterval, 3));
						printf("7. attenuation			= 0x%x	Support : 0x%x ~ 0x%x\n",deviceInfoList[iSelectIndex].transmissionParameter.attenuation, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.attenuationMin, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.attenuationMax);
						printf("7. attenuation_signed		= %d	Support : %d ~ %d\n",deviceInfoList[iSelectIndex].transmissionParameter.attenuation_signed, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.attenuationMin_signed, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.attenuationMax_signed);
						printf("8. TPSCellID			= 0x%x	Support : 0x%x ~ 0x%x\n",deviceInfoList[iSelectIndex].transmissionParameter.TPSCellID, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.TPSCellIDMin, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.TPSCellIDMax);
						printf("9. channelNum			= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].transmissionParameter.channelNum, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.channelNumMin, deviceInfoList[iSelectIndex].transmissionParameterCapabilities.channelNumMax);
						printf("10. bandwidthStrapping		= %u	Support : [7+8]/[6]/[7]/[8] M : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.bandwidthStrapping, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthStrapping, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthStrapping, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthStrapping, 2), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.bandwidthStrapping, 3));
						printf("11. TVStandard			= %u	Support : [DVB-T]/[ISDB-T] : %u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.TVStandard, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.TVStandard, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.TVStandard, 1) );
						printf("12. segmentationMode		= %u	Support : [Full]/[1+12] : %u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.segmentationMode, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.segmentationMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.segmentationMode, 1));
						printf("13. oneSeg_Constellation	= 0x%x	Support : [QPSK]/[16Q]/[64Q] : %u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_Constellation, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_Constellation, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_Constellation, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_Constellation, 2));
						printf("14. oneSeg_CodeRate		= 0x%x	Support : [1/2]/[2/3]/[3/4]/[5/6]/[7/8] : %u/%u/%u/%u/%u\n",deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_CodeRate, BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_CodeRate, 0), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_CodeRate, 1), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_CodeRate, 2), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_CodeRate, 3), BIT_MASK(deviceInfoList[iSelectIndex].transmissionParameterCapabilities.oneSeg_CodeRate, 4));
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter bandwidth in MHz (ex. 1~8): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.bandwidth = (Byte)(tempSetting);
							break;
						case 2:
							printf("\nEnter frequency in KHz (ex. 666000 ): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.frequency = tempSetting;
							break;
						case 3:
							printf("\nEnter Constellation (0: QPSK, 1: 16QAM, 2: 64QAM): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.constellation = (Byte)(tempSetting & 0xFF);
							break;
						case 4:
							printf("\nEnter Transmission Mode (0: 2K, 1: 8K 2: 4K): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.FFT = (Byte)(tempSetting & 0xFF);
							break;
						case 5:
							printf("\nEnter Code Rate (0: 1/2, 1: 2/3, 2: 3/4, 3:5/6, 4: 7/8): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.codeRate = (Byte)(tempSetting & 0xFF);
							break;
						case 6:
							printf("\nEnter Guard Interval (0: 1/32, 1: 1/16, 2: 1/8, 3: 1/4): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.interval = (Byte)(tempSetting & 0xFF);
							break;
						case 7:
							printf("\nEnter Attenuation (ex: -100~100): ");
							SCANF("%d", &tempIntSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.attenuation_signed = (char)tempIntSetting;
							if(tempIntSetting < 0)
							{
								tempIntSetting = -tempIntSetting;
								deviceInfoList[iSelectIndex].transmissionParameter.attenuation = (Byte)tempIntSetting;					
							}else
							{
								deviceInfoList[iSelectIndex].transmissionParameter.attenuation = (Byte)(tempIntSetting | 0x80);
							}
							break;
						case 8:
							printf("\nEnter TPS Cell ID : ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.TPSCellID = (Word)tempSetting;
							break;
						case 9:
							printf("\nEnter Channel Number (ex: 0~255): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.channelNum = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter Bandwidth Strapping (ex: 0:7+8MHz, 1:6MHz, 2:7MHz, 3:8MHz): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.bandwidthStrapping = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter TV Standard (ex: 0: DVB-T 1: ISDB-T): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.TVStandard = (Byte)tempSetting;
							break;
						case 12:
							printf("\nEnter Segmentation Mode (ex: 0: ISDB-T Full segmentation mode 1: ISDB-T 1+12 segmentation mode): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.segmentationMode = (Byte)tempSetting;
							break;
						case 13:
							printf("\nEnter One-Seg Constellation (ex: 0: QPSK, 1: 16QAM, 2: 64QAM ): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_Constellation = (Byte)tempSetting;
							break;
						case 14:
							printf("\nEnter One-Seg Code Rate (ex: 0: 1/2, 1: 2/3, 2: 3/4, 3: 5/6, 4: 7/8 ): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_CodeRate = (Byte)tempSetting;
							break;
						case 255:
							printf("\nEnter frequency in KHz (ex. 666000): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.frequency = tempSetting;
							printf("\nEnter bandwidth in MHz (ex. 1~8): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.bandwidth = (Byte)(tempSetting);
							printf("\nEnter Constellation (0: QPSK, 1: 16QAM, 2: 64QAM): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.constellation = (Byte)(tempSetting & 0xFF);
							printf("\nEnter Transmission Mode (0: 2K, 1: 8K 2: 4K): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.FFT = (Byte)(tempSetting & 0xFF);
							printf("\nEnter Code Rate (0: 1/2, 1: 2/3, 2: 3/4, 3:5/6, 4: 7/8): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.codeRate = (Byte)(tempSetting & 0xFF);
							printf("\nEnter Guard Interval (0: 1/32, 1: 1/16, 2: 1/8, 3: 1/4): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.interval = (Byte)(tempSetting & 0xFF);
							printf("\nEnter Attenuation (ex: -100~100): ");
							SCANF("%d", &tempIntSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.attenuation_signed = (char)tempIntSetting;
							if(tempIntSetting < 0)
							{
								tempIntSetting = -tempIntSetting;
								deviceInfoList[iSelectIndex].transmissionParameter.attenuation = (Byte)tempIntSetting;					
							}else
							{
								deviceInfoList[iSelectIndex].transmissionParameter.attenuation = (Byte)(tempIntSetting | 0x80);
							}
							printf("\nEnter TPS Cell ID : ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.TPSCellID = (Word)tempSetting;
							printf("\nEnter Channel Number (ex: 0~255): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.channelNum = (Byte)tempSetting;
							printf("\nEnter Bandwidth Strapping (ex: 0:7+8MHz, 1:6MHz, 2:7MHz, 3:8MHz): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.bandwidthStrapping = (Byte)tempSetting;
							printf("\nEnter TV Standard (ex: 0: DVB-T 1: ISDB-T): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.TVStandard = (Byte)tempSetting;
							printf("\nEnter Segmentation Mode (ex: 0: ISDB-T Full segmentation mode 1: ISDB-T 1+12 segmentation mode): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.segmentationMode = (Byte)tempSetting;
							printf("\nEnter One-Seg Constellation (ex: 0: QPSK, 1: 16QAM, 2: 64QAM ): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_Constellation = (Byte)tempSetting;
							printf("\nEnter One-Seg Code Rate (ex: 0: 1/2, 1: 2/3, 2: 3/4, 3: 5/6, 4: 7/8 ): ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].transmissionParameter.oneSeg_CodeRate = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].transmissionParameter.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetHwRegisterValuesInput:				// 0x0083 : SetHwRegisterValues
					printf("\nEnter processor  (ex: 0: OFDM, 8: LINK)");
					SCANF("%lu", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.processor = (Byte)tempSetting;
					printf("\nEnter registerAddress (ex: FF) ");
					SCANF("%lx", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.registerAddress = tempSetting;
					printf("\nEnter valueListSize  ");
					SCANF("%lu", &tempSetting);
					deviceInfoList[iSelectIndex].hwRegisterInfo.valueListSize = (Byte)tempSetting;

					User_memory_free(deviceInfoList[iSelectIndex].hwRegisterInfo.registerValues);

					deviceInfoList[iSelectIndex].hwRegisterInfo.registerValues = (Byte*) User_memory_allocate(deviceInfoList[iSelectIndex].hwRegisterInfo.valueListSize * sizeof(Byte));
					for( i = 0; i < deviceInfoList[iSelectIndex].hwRegisterInfo.valueListSize; i ++)
					{
						printf("\nEnter value[%hx] = ", i);
						SCANF("%lx", &tempSetting);
						deviceInfoList[iSelectIndex].hwRegisterInfo.registerValues[i] = (Byte)tempSetting;
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetAdvaneOptionsInput:				// 0x0084 : SetAdvanceOptions
					printf("Send GetAdvanceOptions For Getting Information.....\n");
					get_Command = CMD_GetAdvanceOptionsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetAdvanceOptions Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. PTS_PCR_delayTime					= %u\n",deviceInfoList[iSelectIndex].advanceOptions.PTS_PCR_delayTime);
						printf("2. Rx Latency Recover Time Interval timeInterval(hr)	= %u\n",deviceInfoList[iSelectIndex].advanceOptions.timeInterval);
						printf("2. Rx Latency Recover Time Interval timeInterval(min)	= %u\n",deviceInfoList[iSelectIndex].advanceOptions.Rx_LatencyRecoverTimeInterval);
						printf("3. Rx Latency Recover Frame Skip Number			= %u\n",deviceInfoList[iSelectIndex].advanceOptions.skipNumber);
						printf("4. Input Frame Rate Over Flow Number			= %u\n",deviceInfoList[iSelectIndex].advanceOptions.overFlowNumber);
						printf("5. Frame encode frame data rate over flow size		= %u\n",deviceInfoList[iSelectIndex].advanceOptions.overFlowSize);
						printf("6. SIPSI Table Duration					= %u\n",deviceInfoList[iSelectIndex].advanceOptions.SIPSITableDuration);
						printf("7. Frame Rate Adjustment				= %d\n",deviceInfoList[iSelectIndex].advanceOptions.frameRateAdjustment);
						printf("8. Repeat Packet Mode					= %u\n",deviceInfoList[iSelectIndex].advanceOptions.repeatPacketMode);
						printf("9. Repeat Packet Number					= %u\n",deviceInfoList[iSelectIndex].advanceOptions.repeatPacketNum);
						printf("10. Repeat Packet Time Interval				= %u\n",deviceInfoList[iSelectIndex].advanceOptions.repeatPacketTimeInterval);
						printf("11. TS_TableDisable					= 0x%x\n",deviceInfoList[iSelectIndex].advanceOptions.TS_TableDisable);
						printf("12. PCR Re-stamp Mode					= %u\n",deviceInfoList[iSelectIndex].advanceOptions.PCRReStampMode);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter PTS_PCR_delayTime  (ms)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.PTS_PCR_delayTime = (Word)tempSetting;
							break;
						case 2:
							printf("\nEnter Rx Latency Recover Time Interval timeInterval  (hrs) (ex: 0: Disable N: Time interval)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.timeInterval = (Word)tempSetting;
							deviceInfoList[iSelectIndex].advanceOptions.Rx_LatencyRecoverTimeInterval = (Word)(tempSetting * 60);
							break;
						case 3:
							printf("\nEnter Rx Latency Recover Frame Skip Number (ex: 0: Disable N: Frame number) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.skipNumber = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter Input Frame Rate Over Flow Number (ex: 0: Disable N: Frame number)   ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.overFlowNumber = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter Frame encode frame data rate over flow size (kbps): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.overFlowSize = (Word)tempSetting;
							break;
						case 6:
							printf("\nEnter SIPSI Table Duration  (min): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.SIPSITableDuration = (Word)tempSetting;
							break;
						case 7:
							printf("\nEnter Frame Rate Adjustment (ex: -99~99 in 0.01 fps) : ");
							SCANF("%d", &tempIntSetting);
							deviceInfoList[iSelectIndex].advanceOptions.frameRateAdjustment = (char)tempIntSetting;
							break;
						case 8:
							printf("\nEnter Repeat Packet Mode (ex: 0: OFF, 1: ON) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketMode = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter Repeat Packet Number (ex: 1 ~ 255) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketNum = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter Repeat Packet Time Interval (ex: 0~255  ms.) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketTimeInterval = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter TOT/TDT table (ex: 0: Enable, 1: Disable.) : ");
							SCANF("%lu", &tempSetting);
							if(tempSetting > 0)
								deviceInfoList[iSelectIndex].advanceOptions.TS_TableDisable = 1;
							else
								deviceInfoList[iSelectIndex].advanceOptions.TS_TableDisable = 0;
						case 12:
							printf("\nEnter PCR Re-stamp Mode (ex: 0: Disable, 1: Enable mode 1, 2: Enable mode 2, 3: Enable mode 3) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.PCRReStampMode = (Byte)tempSetting;
							break;
						case 255:
							printf("\nEnter PTS_PCR_delayTime  (ms)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.PTS_PCR_delayTime = (Word)tempSetting;
							printf("\nEnter Rx Latency Recover Time Interval timeInterval  (hrs) (ex: 0: Disable N: Time interval)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.timeInterval = (Word)tempSetting;
							deviceInfoList[iSelectIndex].advanceOptions.Rx_LatencyRecoverTimeInterval = (Word)(tempSetting * 60);
							printf("\nEnter Rx Latency Recover Frame Skip Number (ex: 0: Disable N: Frame number) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.skipNumber = (Byte)tempSetting;
							printf("\nEnter Input Frame Rate Over Flow Number (ex: 0: Disable N: Frame number)   ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.overFlowNumber = (Byte)tempSetting;
							printf("\nEnter Frame encode frame data rate over flow size (kbps): ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.overFlowSize = (Word)tempSetting;
							printf("\nEnter SIPSI Table Duration  (min)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.SIPSITableDuration = (Word)tempSetting;
							printf("\nEnter Frame Rate Adjustment (ex: -99~99 in 0.01 fps) : ");
							SCANF("%d", &tempIntSetting);
							deviceInfoList[iSelectIndex].advanceOptions.frameRateAdjustment = (char)tempIntSetting;
							printf("\nEnter Repeat Packet Mode (ex: 0: OFF, 1: ON) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketMode = (Byte)tempSetting;
							printf("\nEnter Repeat Packet Number (ex: 1 ~ 255) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketNum = (Byte)tempSetting;
							printf("\nEnter Repeat Packet Time Interval (ex: 0~255  ms.) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.repeatPacketTimeInterval = (Byte)tempSetting;
							printf("\nEnter TOT/TDT table (ex: 0: Enable, 1: Disable.) : ");
							SCANF("%lu", &tempSetting);
							if(tempSetting > 0)
								deviceInfoList[iSelectIndex].advanceOptions.TS_TableDisable = 1;
							else
								deviceInfoList[iSelectIndex].advanceOptions.TS_TableDisable = 0;
							printf("\nEnter PCR Re-stamp Mode (ex: 0: Disable, 1: Enable mode 1, 2: Enable mode 2, 3: Enable mode 3) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].advanceOptions.PCRReStampMode = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].advanceOptions.extensionFlag = 1;// Must 1 for extension
					deviceInfoList[iSelectIndex].advanceOptions.extensionFlag2 = 1;// Must 1 for extension2
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetTPSInformationInput:				// 0x0085 : SetTPSInformation
					printf("Send GetTPSInfo For Getting Information.....\n");
					get_Command = CMD_GetTPSInformationInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetTPSInfo Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. Cell ID		= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.cellID);
						printf("2. High Code Rate	= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.highCodeRate);
						printf("3. Low Code Rate	= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.lowCodeRate);
						printf("4. Transmission Mode	= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.transmissionMode);
						printf("5. Constellation	= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.constellation);
						printf("6. Interval		= 0x%x\n",deviceInfoList[iSelectIndex].tpsInfo.interval);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter Cell ID  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.cellID = (Word)tempSetting;
							break;
						case 2:
							printf("\nEnter High Code Rate  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.highCodeRate = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter Low Code Rate  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.lowCodeRate = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter Transmission Mode  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.transmissionMode = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter Constellation  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.constellation = (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter Interval  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.interval = (Byte)tempSetting;
							break;
						case 255:
							printf("\nEnter Cell ID  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.cellID = (Word)tempSetting;
							printf("\nEnter High Code Rate  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.highCodeRate = (Byte)tempSetting;
							printf("\nEnter Low Code Rate  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.lowCodeRate = (Byte)tempSetting;
							printf("\nEnter Transmission Mode  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.transmissionMode = (Byte)tempSetting;
							printf("\nEnter Constellation  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.constellation = (Byte)tempSetting;
							printf("\nEnter Interval  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].tpsInfo.interval = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetEncryptionConfigurationsInput:			// 0x0087 : SetEncryptionConfigurations
					printf("Send GetEncryptionConfigurations For Getting Configurations.....\n");
					get_Command = CMD_GetEncryptionConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetEncryptionConfigurations Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. TPS Encryption Mode						= 0x%x\n",deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionMode);
						printf("2. TPS Encryption Type						= 0x%x\n",deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionType);
						printf("3. TPS Encryption Key						= %s\n",deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionKey);
						printf("4. Data Encryption Mode						= 0x%x\n",deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionMode);
						printf("5. Data Encryption Type						= 0x%x\n",deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionType);
						printf("6. Data PartialEncryption Skip Length		= 0x%u\n",deviceInfoList[iSelectIndex].encryptConfig.DataPartialEncryptionSkipLength);
						printf("7. Data Encryption Key						= %s\n",deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionKey);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter TPS Encryption Mode :  (ex: 0:Disable, 1:Enable) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionMode = (Byte)tempSetting;
							break;
						case 2:
							printf("\nEnter TPS Encryption Type :  (ex: 0: SDES encryption) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionType = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter TPS Encryption Key :  ");
							Scanf_String(TPSEncryptionKey, sizeof(TPSEncryptionKey));
							User_memory_copy( deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionKey, TPSEncryptionKey, 8);
							break;
						case 4:
							printf("\nEnter Data Encryption Mode :  (ex: 0:Disable, 1:Enable) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionMode = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter Data Encryption Type :  (ex: 0: AES-128 encryption) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionType = (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter Data PartialEncryption Skip Length :  (ex: 0: Encrypt all bytes (except 4-byte header)) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataPartialEncryptionSkipLength = (Word)tempSetting;
							break;
						case 7:
							printf("\nEnter Data Encryption Key :  ");
							Scanf_String(DataEncryptionKey, sizeof(DataEncryptionKey));
							User_memory_copy( deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionKey, DataEncryptionKey, 64);
							break;

						case 255:
							printf("\nEnter TPS Encryption Mode :  (ex: 0:Disable, 1:Enable) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionMode = (Byte)tempSetting;
							printf("\nEnter TPS Encryption Type :  (ex: 0: SDES encryption) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionType = (Byte)tempSetting;
							printf("\nEnter TPS Encryption Key :  ");
							Scanf_String(TPSEncryptionKey, sizeof(TPSEncryptionKey));
							User_memory_copy( deviceInfoList[iSelectIndex].encryptConfig.TPSEncryptionKey, TPSEncryptionKey, 8);
							printf("\nEnter Data Encryption Mode :  (ex: 0:Disable, 1:Enable) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionMode = (Byte)tempSetting;
							printf("\nEnter Data Encryption Type :  (ex: 0: AES-128 encryption) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionType = (Byte)tempSetting;
							printf("\nEnter Data PartialEncryption Skip Length :  (ex: 0: Encrypt all bytes (except 4-byte header)) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].encryptConfig.DataPartialEncryptionSkipLength = (Word)tempSetting;
							printf("\nEnter Data Encryption Key :  ");
							Scanf_String(DataEncryptionKey, sizeof(DataEncryptionKey));
							User_memory_copy( deviceInfoList[iSelectIndex].encryptConfig.DataEncryptionKey, DataEncryptionKey, 64);

							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}

					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetSiPsiTableInput:			// 0x0090 : SetSiPsiTable
					printf("Send GetSiPsiTable For Getting Information.....\n");
					get_Command = CMD_GetSiPsiTableInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetSiPsiTable Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. ONID				= 0x%x\n",deviceInfoList[iSelectIndex].psiTable.ONID);
						printf("2. NID				= 0x%u\n",deviceInfoList[iSelectIndex].psiTable.NID);
						printf("3. TSID				= 0x%u\n",deviceInfoList[iSelectIndex].psiTable.TSID);
						printf("4. networkName			= %s\n",deviceInfoList[iSelectIndex].psiTable.networkName);
						printf("5. Private Data Specifier	= 0x%08lx\n",deviceInfoList[iSelectIndex].psiTable.privateDataSpecifier);
						printf("6. NIT Version			= 0x%02x\n",deviceInfoList[iSelectIndex].psiTable.NITVersion);
						printf("7. Country ID			= %u\n",deviceInfoList[iSelectIndex].psiTable.countryID);
						printf("8. Language ID			= %u\n",deviceInfoList[iSelectIndex].psiTable.languageID);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter ONID :  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.ONID = (Word)tempSetting;
							break;
						case 2:
							printf("\nEnter NID :  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.NID = (Word)tempSetting;
							break;
						case 3:
							printf("\nEnter TSID :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.TSID = (Word)tempSetting;
							break;
						case 4:
							printf("\nEnter Network Name :  ");
							Scanf_String(networkname, sizeof(networkname));
							User_memory_copy( deviceInfoList[iSelectIndex].psiTable.networkName, networkname, 32);
							break;
						case 5:
							printf("\nEnter Private Data Specifier :  (ex: 0x00000029 for Nordig) 0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.privateDataSpecifier = tempSetting;
							break;
						case 6:
							printf("\nEnter NIT Version :  (ex: 0x00) : 0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.NITVersion = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter Country ID :  (ex: 0 for Australia ) :");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.countryID = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter Language ID :  (ex: 0 for English ) :");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.languageID = (Byte)tempSetting;
							break;
						case 255:
							printf("\nEnter ONID :  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.ONID = (Word)tempSetting;
							printf("\nEnter NID :  ");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.NID = (Word)tempSetting;
							printf("\nEnter TSID :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.TSID = (Word)tempSetting;
							printf("\nEnter Network Name :  ");
							Scanf_String(networkname, sizeof(networkname));
							User_memory_copy( deviceInfoList[iSelectIndex].psiTable.networkName, networkname, 32);

							printf("\nEnter Private Data Specifier :  (ex: 0x00000029 for Nordig) 0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.privateDataSpecifier = tempSetting;
							printf("\nEnter NIT Version :  (ex: 0x00) : 0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.NITVersion = (Byte)tempSetting;
							printf("\nEnter Country ID :  (ex: 0 for Australia ) :");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.countryID = (Byte)tempSetting;
							printf("\nEnter Language ID :  (ex: 0 for English ) :");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].psiTable.languageID = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].psiTable.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetNitLocationInput:			// 0x0091 : SetNitLocation
					printf("Send GetNitLocation For Getting Information.....\n");
					get_Command = CMD_GetNitLocationInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetNitLocation Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. latitude		= %u\n",deviceInfoList[iSelectIndex].nitLoacation.latitude);
						printf("2. longitude		= %u\n",deviceInfoList[iSelectIndex].nitLoacation.longitude);
						printf("3. extentLatitude	= %u\n",deviceInfoList[iSelectIndex].nitLoacation.extentLatitude);
						printf("4. extentLongitude	= %u\n",deviceInfoList[iSelectIndex].nitLoacation.extentLongitude);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter latitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.latitude = (Word)tempSetting;
							break;
						case 2:
							printf("\nEnter longitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.longitude = (Word)tempSetting;
							break;
						case 3:
							printf("\nEnter extentLatitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.extentLatitude =  (Word)tempSetting;
							break;
						case 4:
							printf("\nEnter extentLongitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.extentLongitude =  (Word)tempSetting;
							break;
						case 255:
							printf("\nEnter latitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.latitude = (Word)tempSetting;
							printf("\nEnter longitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.longitude = (Word)tempSetting;
							printf("\nEnter extentLatitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.extentLatitude =  (Word)tempSetting;
							printf("\nEnter extentLongitude :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].nitLoacation.extentLongitude =  (Word)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetSdtServiceInput:			// 0x0092 : SetSdtServiceConfiguration
					printf("Send GetSdtService For Getting Information.....\n");
					get_Command = CMD_GetSdtServiceInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetSdtService Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. serviceID				= 0x%x\n",deviceInfoList[iSelectIndex].serviceConfig.serviceID);
						printf("2. enable				= %u\n",deviceInfoList[iSelectIndex].serviceConfig.enable);
						printf("3. LCN					= 0x%x\n",deviceInfoList[iSelectIndex].serviceConfig.LCN);
						printf("4. serviceName				= %s\n",deviceInfoList[iSelectIndex].serviceConfig.serviceName.stringData);
						printf("5. provider				= %s\n",deviceInfoList[iSelectIndex].serviceConfig.provider.stringData);
						printf("6. IDAssignationMode			= %u\n",deviceInfoList[iSelectIndex].serviceConfig.IDAssignationMode);
						printf("7. ISDBT_RegionID			= %u\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RegionID);
						printf("8. ISDBT_BroadcasterRegionID		= %u\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_BroadcasterRegionID);
						printf("9. ISDBT_RemoteControlKeyID		= %u\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RemoteControlKeyID);
						printf("10. ISDBT_ServiceIDDataType_1		= 0x%04x\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_1);
						printf("11. ISDBT_ServiceIDDataType_2		= 0x%04x\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_2);
						printf("12. ISDBT_ServiceIDPartialReception	= 0x%04x\n",deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDPartialReception);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter serviceID :  (hex)");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.serviceID = (Word)tempSetting;
							break;
						case 2:
							printf("\nEnter enable :  (ex: 0:Disable, 1:Enable, 2:Not available)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.enable = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter LCN :  (hex)");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.LCN = (Word)tempSetting;
							break;
						case 4:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serviceConfig.serviceName);
							printf("\nEnter Service Name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serviceConfig.serviceName);	
							break;
						case 5:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serviceConfig.provider);
							printf("\nEnter Provider :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serviceConfig.provider);	
							break;
						case 6:
							printf("\nEnter ISDB-T ID Assignation Mode :  (ex: 0: AUTO, 1: Manual.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.IDAssignationMode = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter ISDB-T Region ID :  (ex: 1.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RegionID = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter ISDB-T Broadcaster Region ID :  (ex: 0.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_BroadcasterRegionID = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter ISDB-T Remote Control Key ID :  (ex: 1.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RemoteControlKeyID = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter ISDB-T Service ID Data Type 1 :  (ex: 0x0480.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_1 = (Word)tempSetting;
							break;
						case 11:
							printf("\nEnter ISDB-T Service ID Data Type 2 :  (ex: 0x0500.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_2 = (Word)tempSetting;
							break;
						case 12:
							printf("\nEnter ISDB-T Service ID Partial reception :  (ex: 0x0580.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDPartialReception = (Word)tempSetting;
							break;
						case 255:
							printf("\nEnter serviceID :  (hex)");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.serviceID = (Word)tempSetting;
							printf("\nEnter enable :  (ex: 0:Disable, 1:Enable, 2:Not available)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.enable = (Byte)tempSetting;
							printf("\nEnter LCN :  (hex)");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.LCN = (Word)tempSetting;
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serviceConfig.serviceName);
							printf("\nEnter Service Name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet(byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serviceConfig.serviceName);	
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serviceConfig.provider);
							printf("\nEnter Provider :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet(byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serviceConfig.provider);	
							printf("\nEnter ISDB-T ID Assignation Mode :  (ex: 0: AUTO, 1: Manual.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.IDAssignationMode = (Byte)tempSetting;
							printf("\nEnter ISDB-T Region ID :  (ex: 1.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RegionID = (Byte)tempSetting;
							printf("\nEnter ISDB-T Broadcaster Region ID :  (ex: 0.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_BroadcasterRegionID = (Byte)tempSetting;
							printf("\nEnter ISDB-T Remote Control Key ID :  (ex: 1.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_RemoteControlKeyID = (Byte)tempSetting;
							printf("\nEnter ISDB-T Service ID Data Type 1 :  (ex: 0x0480.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_1 = (Word)tempSetting;
							printf("\nEnter ISDB-T Service ID Data Type 2 :  (ex: 0x0500.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDDataType_2 = (Word)tempSetting;
							printf("\nEnter ISDB-T Service ID Partial reception :  (ex: 0x0580.)  0x");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].serviceConfig.ISDBT_ServiceIDPartialReception = (Word)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].serviceConfig.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetEITInformationInput:			// 0x0093 : SetEITInformation
					printf("Send GetEITInformation For Getting Information.....\n");
					get_Command = CMD_GetEITInformationInput;
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);
					printf("\nVideoEncoder Configuration token :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetEITInformation Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------All Default Value List------------\n");
						printf("listSize		= %u\n",deviceInfoList[iSelectIndex].eitInfo.listSize);
						printf("videoEncConfigToken	= %s\n",deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken.stringData);
						for( i = 0; i < deviceInfoList[iSelectIndex].eitInfo.listSize; i ++)
						{
							printf("---------------------------------- Index %u -------------------------------------\n",i);
							printf("enable			= %u\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].enable);
							printf("startDate		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].startDate);
							printf("startTime		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].startTime);
							printf("duration		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].duration);
							printf("eventName		= %s\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventName.stringData);
							printf("eventText		= %s\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventText.stringData);
							printf("---------------------------------- Index %u -------------------------------------\n",i);
						}
						printf("------------All Default Value List------------\n");
						printf("0. Set all parameters completed\n");
						printf("1. Set One of parametes set\n");
						printf("2. Set videoEncConfigToken\n");
						printf("3. Clear all parameters and Create new parameters\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							do
							{
								printf("\nInput index for setting parameter: (ex: 0~%u  )", deviceInfoList[iSelectIndex].eitInfo.listSize-1 );
								SCANF("%d", &SetIndex);
							}while(SetIndex > deviceInfoList[iSelectIndex].eitInfo.listSize);
							IsSetIndexParamsCompleted = 0;
							while(IsSetIndexParamsCompleted == 0)
							{
								printf("\n------------Default Value List------------\n");
								printf("1. enable		= %u\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].enable);
								printf("2. startDate		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startDate);
								printf("3. startTime		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startTime);
								printf("4. duration		= 0x%08lx\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].duration);
								printf("5. eventName		= %s\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventName.stringData);
								printf("6. eventText		= %s\n",deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventText.stringData);
								printf("------------Default Value List------------\n");
								printf("255. Set all parameters\n");
								printf("0. Set all parameters completed\n");
								printf("\nInput index for setting parameter: ");
								SCANF("%d", &selectNum);
								switch(selectNum)
								{
								case 0:
									IsSetIndexParamsCompleted = 1;
									break;
								case 1:
									printf("\nEnter Enable :  (ex: 0: Disable, 1: Enable)");
									SCANF("%lu", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].enable = (Byte)tempSetting;
									break;
								case 2:
									printf("\nEnter Start Date :  (ex: 0x20130516 for 2013/05/16)");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startDate = tempSetting;
									break;
								case 3:
									printf("\nEnter Start Time :  (ex: 0x15111200 for 15:11:12 )");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startTime = tempSetting;
									break;
								case 4:
									printf("\nEnter Duration :  (ex: 0x15111200 for 15:11:12 )");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].duration = tempSetting;
									break;
								case 5:
									check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventName);
									printf("\nEnter Event Name :  ");
									Scanf_String(byRefStringBuffer, Ref_BufferSize);
									check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventName);	
									break;
								case 6:
									check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventText);
									printf("\nEnter Event Event Text :  ");
									Scanf_String(byRefStringBuffer, Ref_BufferSize);
									check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventText);	
									break;
								case 255:
									printf("\nEnter Enable :  (ex: 0: Disable, 1: Enable)");
									SCANF("%lu", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].enable = (Byte)tempSetting;
									printf("\nEnter Start Date :  (ex: 0x20130516 for 2013/05/16)");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startDate = tempSetting;
									printf("\nEnter Start Time :  (ex: 0x15111200 for 15:11:12 )");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].startTime = tempSetting;
									printf("\nEnter Duration :  (ex: 0x15111200 for 15:11:12 )");
									SCANF("%lx", &tempSetting);
									deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].duration = tempSetting;
									check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventName);
									printf("\nEnter Event Name :  ");
									Scanf_String(byRefStringBuffer, Ref_BufferSize);
									check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventName);	
									check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventText);
									printf("\nEnter Event Event Text :  ");
									Scanf_String(byRefStringBuffer, Ref_BufferSize);
									check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[SetIndex].eventText);	

									break;
								default:
									printf("Unsupported Setting!\n");
									continue;
								}
							}
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);

							printf("\nEnter Event Event Text :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.videoEncConfigToken);	

							break;
						case 3:
							for( i = 0; i < deviceInfoList[iSelectIndex].eitInfo.listSize; i ++) // clear all EITInfo
							{
								check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventName);
								check = Cmd_StringClear(&deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventText);
							}
							User_memory_free(deviceInfoList[iSelectIndex].eitInfo.eitInfoParam);
							deviceInfoList[iSelectIndex].eitInfo.listSize = 0;
							printf("\nEnter EITInformation List Size :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].eitInfo.listSize = (Byte)tempSetting;
							deviceInfoList[iSelectIndex].eitInfo.eitInfoParam = (EITInfoParam*) User_memory_allocate(deviceInfoList[iSelectIndex].eitInfo.listSize * sizeof(EITInfoParam));
							for( i = 0; i < deviceInfoList[iSelectIndex].eitInfo.listSize; i ++)
							{
								printf("\nEnter Enable :  (ex: 0: Disable, 1: Enable)");
								SCANF("%lu", &tempSetting);
								deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].enable = (Byte)tempSetting;
								printf("\nEnter Start Date :  (ex: 0x20130516 for 2013/05/16)");
								SCANF("%lx", &tempSetting);
								deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].startDate = tempSetting;
								printf("\nEnter Start Time :  (ex: 0x15111200 for 15:11:12 )");
								SCANF("%lx", &tempSetting);
								deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].startTime = tempSetting;
								printf("\nEnter Duration :  (ex: 0x15111200 for 15:11:12 )");
								SCANF("%lx", &tempSetting);
								deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].duration = tempSetting;
								printf("\nEnter Event Name :  ");
								Scanf_String(byRefStringBuffer, Ref_BufferSize);
								check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventName);	
								printf("\nEnter Event Event Text :  ");
								Scanf_String(byRefStringBuffer, Ref_BufferSize);
								check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].eitInfo.eitInfoParam[i].eventText);	

							}
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//-----------Device Management Service----------
				case CMD_GetCapabilitiesInput:	// 0x0100 : GetCapabilities
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetDeviceInformationInput:	// 0x0101 : GetDeviceInformation
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetHostnameInput:	// 0x0102 : GetHostname
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSystemDateAndTimeInput:	// 0x0103 : GetSystemDateAndTime
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSystemLogInput:	// 0x0104 : GetSystemLog
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. logType	= %u\n",deviceInfoList[iSelectIndex].systemLog.logType);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter logType :  (ex: 0: System log, 1: Access log)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemLog.logType = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetOSDInformationInput:	// 0x0105 : GetOSDInformation
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SystemRebootInput:	// 0x0187 : SystemReboot
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. rebootType	= %u\n",deviceInfoList[iSelectIndex].systemReboot.rebootType);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter rebootType :  (EX : 0: Reboot the device 1: Reboot the application only 2:Reboot the system to the device default settings)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemReboot.rebootType = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_MultipleSetCommandApplyInput:	// 0x0180 : MultipleSetCommandApply
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. mode	= %u\n",deviceInfoList[iSelectIndex].multipleSetCommandApply.mode);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter mode :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].multipleSetCommandApply.mode = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_MultipleSetCommandStartInput:	// 0x0188 : MultipleSetCommandStart
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. mode	= %u\n",deviceInfoList[iSelectIndex].multipleSetCommandStart.mode);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter mode :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].multipleSetCommandStart.mode = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetSystemFactoryDefaultInput:	// 0x0181 : SetSystemFactoryDefault
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. factoryDefault	= %u\n",deviceInfoList[iSelectIndex].systemDefault.factoryDefault);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter factoryDefault :  (ex: 0: Hard factory default 1: Soft factory default)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemDefault.factoryDefault = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetHostnameInput:	// 0x0182 : SetHostname
					printf("Send GetHostname For Getting Information.....\n");
					get_Command = CMD_GetHostnameInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetHostname Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. hostName	= %s\n",deviceInfoList[iSelectIndex].hostInfo.hostName.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].hostInfo.hostName);
							printf("\nhostName :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);		
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].hostInfo.hostName);	

							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetSystemDateAndTimeInput:	// 0x0183 : SetSystemDateAndTime
					printf("Send GetSystemDateAndTime For Getting Information.....\n");
					get_Command = CMD_GetSystemDateAndTimeInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetSystemDateAndTime Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
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
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter countryCode :\n");
							for(i = 0; i < 3; i ++)
							{
								printf("countryCode[%u] = ",i);
#ifdef __linux__   /* Linux */
								SCANF(" %c", &tempChar);
#else
								SCANF(" %c", &tempChar,1);
#endif
								deviceInfoList[iSelectIndex].systemTime.countryCode[i] = tempChar;
							}
							break;
						case 2:
							printf("\nEnter countryRegionID :  (EX : 0)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.countryRegionID = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter daylightSavings :  (EX : 0/off 1/On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.daylightSavings = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter timeZone :  (EX : -12~+12)");
							SCANF("%d", &tempIntSetting);
							if(tempIntSetting > 0)
								deviceInfoList[iSelectIndex].systemTime.timeZone = (Byte)tempIntSetting;
							else
							{
								tempIntSetting = - tempIntSetting;
								deviceInfoList[iSelectIndex].systemTime.timeZone = (Byte)(tempIntSetting | 0x80);
							}
							break;
						case 5:
							printf("\nEnter UTCHour :  (EX : 0 ~ 23)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCHour = (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter UTCMinute : (EX : 0 ~ 59)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMinute = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter UTCSecond :  (EX : 0 ~ 59)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCSecond = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter UTCYear :  (EX : 2013)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCYear = (Word)tempSetting;
							break;
						case 9:
							printf("\nEnter UTCMonth :  (EX : 1 ~ 12)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMonth = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter UTCDay :  (EX : 1 ~ 31)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCDay = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter UTC Millisecond :  (EX : 0 ~ 1000.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMillisecond = (Word)tempSetting;
							break;
						case 12:
							printf("\nEnter Time Adjustment Mode :  (EX : 0: Update time to the device directly, 1: Auto mode, 2: Menual mode.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentMode = (Byte)tempSetting;
							break;
						case 13:
							printf("\nEnter Time Adjustment Criterion Max :  (ms) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMax = tempSetting;
							break;
						case 14:
							printf("\nEnter Time Adjustment Criterion Min :  (ms) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMin = tempSetting;
							break;
						case 15:
							printf("\nEnter Time Synchronization Duration :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeSyncDuration = (Word)tempSetting;
							break;
						case 255:
							printf("\nEnter countryCode :\n");
							for(i = 0; i < 3; i ++)
							{
								printf("countryCode[%u] = ",i);
#ifdef __linux__   /* Linux */
								SCANF(" %c", &tempChar);
#else
								SCANF(" %c", &tempChar,1);
#endif
								deviceInfoList[iSelectIndex].systemTime.countryCode[i] = tempChar;
							}
							printf("\nEnter countryRegionID :  (EX : 0)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.countryRegionID = (Byte)tempSetting;
							printf("\nEnter daylightSavings :  (EX : 0/off 1/On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.daylightSavings = (Byte)tempSetting;
							printf("\nEnter timeZone :  (EX : -12~+12)");
							SCANF("%d", &tempIntSetting);
							if(tempIntSetting > 0)
								deviceInfoList[iSelectIndex].systemTime.timeZone = (Byte)tempIntSetting;
							else
							{
								tempIntSetting = - tempIntSetting;
								deviceInfoList[iSelectIndex].systemTime.timeZone = (Byte)(tempIntSetting | 0x80);
							}
							printf("\nEnter UTCHour :  (EX : 0 ~ 23)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCHour = (Byte)tempSetting;
							printf("\nEnter UTCMinute : (EX : 0 ~ 59)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMinute = (Byte)tempSetting;
							printf("\nEnter UTCSecond :  (EX : 0 ~ 59)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCSecond = (Byte)tempSetting;
							printf("\nEnter UTCYear :  (EX : 2013)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCYear = (Word)tempSetting;
							printf("\nEnter UTCMonth :  (EX : 1 ~ 12)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMonth = (Byte)tempSetting;
							printf("\nEnter UTCDay :  (EX : 1 ~ 31)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCDay = (Byte)tempSetting;
							printf("\nEnter UTC Millisecond :  (EX : 0 ~ 1000.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.UTCMillisecond = (Word)tempSetting;
							printf("\nEnter Time Adjustment Mode :  (EX : 0: Update time to the device directly, 1: Auto mode, 2: Menual mode.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentMode = (Byte)tempSetting;
							printf("\nEnter Time Adjustment Criterion Max :  (ms) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMax = tempSetting;
							printf("\nEnter Time Adjustment Criterion Min :  (ms) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeAdjustmentCriterionMin = tempSetting;
							printf("\nEnter Time Synchronization Duration :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemTime.timeSyncDuration = (Word)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].systemTime.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetSystemFactoryDefaultBroadcastInput:	// 0x0184 : SetSystemFactoryDefaultBroadcast
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. factoryDefault	= %u\n",deviceInfoList[0].systemDefault.factoryDefault);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter factoryDefault :  (ex: 0: Hard factory default 1: Soft factory default)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[0].systemDefault.factoryDefault = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}

					error =RC_Shell_FactoryResetBroadCast(&shell, (Byte)tempSetting,funBroadReplyCallBack, BroadcastWaitTime);
					if(!error)
						iSelectIndex = 65535;
					break;

				case CMD_SetOSDInformationInput:	// 0x0185 : SetOSDInformation
					printf("Send GetOSDInformation For Getting Information.....\n");
					get_Command = CMD_GetOSDInformationInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetOSDInformation Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
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
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.dateEnable = (Byte)tempSetting;
							break;
						case 2:
							printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.datePosition = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.dateFormat = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timeEnable = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timePosition = (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timeFormat = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logoEnable = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logoPosition = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logooption = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfoEnable = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfoPosition = (Byte)tempSetting;
							break;
						case 12:
							printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfooption = (Byte)tempSetting;
							break;
						case 13:
							printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.textEnable = (Byte)tempSetting;
							break;
						case 14:
							printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.textPosition = (Byte)tempSetting;
							break;
						case 15:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].osdInfo.text);
							printf("\nOSDTextData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].osdInfo.text);		
							break;
						case 255:
							printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.dateEnable = (Byte)tempSetting;
							printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.datePosition = (Byte)tempSetting;
							printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.dateFormat = (Byte)tempSetting;
							printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timeEnable = (Byte)tempSetting;
							printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timePosition = (Byte)tempSetting;
							printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.timeFormat = (Byte)tempSetting;
							printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logoEnable = (Byte)tempSetting;
							printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logoPosition = (Byte)tempSetting;
							printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.logooption = (Byte)tempSetting;
							printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfoEnable = (Byte)tempSetting;
							printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfoPosition = (Byte)tempSetting;
							printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.detailInfooption = (Byte)tempSetting;
							printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.textEnable = (Byte)tempSetting;
							printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].osdInfo.textPosition = (Byte)tempSetting;
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].osdInfo.text);
							printf("\nOSDTextData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].osdInfo.text);		

							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_UpgradeSystemFirmwareInput:	// 0x0186 : UpgradeSystemFirmware
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. firmwareType	= %u\n",deviceInfoList[iSelectIndex].systemFirmware.firmwareType);
						printf("2. totalDataLength	= %lu\n",deviceInfoList[iSelectIndex].systemFirmware.totalDataLength);
						printf("3. totalDataBlockNumber	= %lu\n",deviceInfoList[iSelectIndex].systemFirmware.totalDataBlockNumber);
						printf("4. dataBlockNumber	= %lu\n",deviceInfoList[iSelectIndex].systemFirmware.dataBlockNumber);
						printf("5. data		= %s\n",deviceInfoList[iSelectIndex].systemFirmware.data.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("\nfirmwareType :  (Ex: 0: General FW upgrade 1: File system FW upgrade 2: Kernel FW upgrade 3: NAND boot FW upgrade 4: Device depend FW upgrade) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.firmwareType = (Byte)tempSetting;
							break;
						case 2:
							printf("\ntotalDataLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.totalDataLength = (Dword)tempSetting;
							break;
						case 3:
							printf("\ntotalDataBlockNumber :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.totalDataBlockNumber = (Dword)tempSetting;
							break;
						case 4:
							printf("\ndataBlockNumber :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.dataBlockNumber = (Dword)tempSetting;
							break;
						case 5:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].systemFirmware.data);
							printf("\ndata :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);		
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].systemFirmware.data);	
							break;
						case 255:
							printf("\nfirmwareType :  (Ex: 0: General FW upgrade 1: File system FW upgrade 2: Kernel FW upgrade 3: NAND boot FW upgrade 4: Device depend FW upgrade) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.firmwareType = (Byte)tempSetting;
							printf("\ntotalDataLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.totalDataLength = (Dword)tempSetting;
							printf("\ntotalDataBlockNumber :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.totalDataBlockNumber = (Dword)tempSetting;
							printf("\ndataBlockNumber :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].systemFirmware.dataBlockNumber = (Dword)tempSetting;
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].systemFirmware.data);
							printf("\ndata :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);		
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].systemFirmware.data);	
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//-----------Device Management Service----------
					//-----------Device IO Service----------
				case CMD_GetDigitalInputsInput:	// 0x0200 : GetDigitalInputs
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetRelayOutputsInput:	//0x0201 : GetRelayOutputs
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSerialPortsInput:	//0x0202 : GetSerialPorts
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSerialPortConfigurationsInput:	//0x0203 : GetSerialPortConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetSerialPortConfigurationOptionsInput:	//0x0204 : GetSerialPortConfigurationOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1 .SerialPort Configuration Token	= %s\n",deviceInfoList[iSelectIndex].serialPortConfigOptions.serialPortConfigToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfigOptions.serialPortConfigToken);
							printf("\nEnter SerialPort Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfigOptions.serialPortConfigToken);	
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetRelayOutputStateInput:	//0x0280 : SetRelayOutputState.
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1 .token	= %s\n",deviceInfoList[iSelectIndex].relayOutputState.token.stringData);
						printf("2. logicalState	= %u\n",deviceInfoList[iSelectIndex].relayOutputState.logicalState);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].relayOutputState.token);
							printf("\nEnter RelayOutputToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].relayOutputState.token);	
							break;
						case 2:
							printf("\nEnter logicalState :  (ex: 0: active, 1: inactive)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputState.logicalState = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].relayOutputState.token);
							printf("\nEnter RelayOutputToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].relayOutputState.token);	
							printf("\nEnter logicalState :  (ex: 0: active, 1: inactive)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputState.logicalState = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetRelayOutputSettingsInput:	//0x0281 : SetRelayOutputSettings
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].relayOutputsSetParam.token.stringData);
						printf("2. mode		= %u\n",deviceInfoList[iSelectIndex].relayOutputsSetParam.mode);
						printf("3. delayTime	= %lu\n",deviceInfoList[iSelectIndex].relayOutputsSetParam.delayTime);
						printf("4. idleState	= %u\n",deviceInfoList[iSelectIndex].relayOutputsSetParam.idleState);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].relayOutputsSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].relayOutputsSetParam.token);		
							break;
						case 2:
							printf("\nEnter mode :  (ex: 0: Monostable, 1: Bistable)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.mode = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter delayTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.delayTime = tempSetting;
							break;
						case 4:
							printf("\nEnter idleState :  (ex: 0: closed, 1: open)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.idleState = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].relayOutputsSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].relayOutputsSetParam.token);		
							printf("\nEnter mode :  (ex: 0: Monostable, 1: Bistable)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.mode = (Byte)tempSetting;
							printf("\nEnter delayTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.delayTime = tempSetting;
							printf("\nEnter idleState :  (ex: 0: closed, 1: open)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].relayOutputsSetParam.idleState = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SendReceiveSerialCommandInput:	//0x0282 : SendReceiveSerialCommand
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. SerialPort Token	= %s\n",deviceInfoList[iSelectIndex].serialCommand.serialPortToken.stringData);
						printf("2. SerialData		= %s\n",deviceInfoList[iSelectIndex].serialCommand.serialData.stringData);
						printf("3. TimeOut		= %u\n",deviceInfoList[iSelectIndex].serialCommand.timeOut);
						printf("4. Receive DataLength	= %lu\n",deviceInfoList[iSelectIndex].serialCommand.receiveDataLength);
						printf("5. Delimiter		= %s\n",deviceInfoList[iSelectIndex].serialCommand.delimiter.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.serialPortToken);
							printf("\nEnter SerialPort Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.serialPortToken);		
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.serialData);
							printf("\nEnter SerialData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.serialData);		
							break;
						case 3:
							printf("\nEnter TimeOut : (sec) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialCommand.timeOut = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter Receive DataLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialCommand.receiveDataLength = tempSetting;
							break;
						case 5:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.delimiter);
							printf("\nEnter Delimiter :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.delimiter);		
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.serialPortToken);
							printf("\nEnter SerialPort Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.serialPortToken);		
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.serialData);
							printf("\nEnter SerialData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.serialData);		
							printf("\nEnter TimeOut : (sec) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialCommand.timeOut = (Byte)tempSetting;
							printf("\nEnter Receive DataLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialCommand.receiveDataLength = tempSetting;
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialCommand.delimiter);
							printf("\nEnter Delimiter :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialCommand.delimiter);		
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetSerialPortConfigurationInput:	//0x0283 : SetSerialPortConfig

					get_Command = CMD_GetSerialPortConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetSerialPortConfig Before Set!\n");
						continue;
					}
					printf("Get SerialPortConfig  from Device\n");
					for( i = 0; i < deviceInfoList[iSelectIndex].serialPortConfigList.listSize; i ++)
					{
						printf("---------------------------------- Index %u -------------------------------------\n",i);
						printf("    Name stringLength			= %lu\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].name.stringLength);
						printf("    Name stringData			= %s\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].name.stringData);
						printf("    Token stringLength			= %lu\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].token.stringLength);
						printf("    Token stringData			= %s\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].token.stringData);
						printf("    SerialPortToken stringLength	= %lu\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].serialPortToken.stringLength);
						printf("    SerialPortToken stringData		= %s\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].serialPortToken.stringData);
						printf("    Type				= %u\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].type);
						printf("    BaudRate				= %lu\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].baudRate);
						printf("    DataBit Length			= %u\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].dataBitLength);
						printf("    ParityBit Check			= %u\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].parityBitCheck);
						printf("    StopBit				= %u\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].stopBit);
						printf("    FlowControl				= %u\n", deviceInfoList[iSelectIndex].serialPortConfigList.configList[i].flowControl);
						printf("---------------------------------- Index %u -------------------------------------\n",i);
					}

					do{
						printf("Load one of SerialPortConfig from SerialPortConfig List Before set (0 ~ %u)\n", deviceInfoList[iSelectIndex].serialPortConfigList.listSize - 1);
						SCANF("%d", &iTempIndex);
						if(iTempIndex >=deviceInfoList[iSelectIndex].serialPortConfigList.listSize)
						{
							printf("The Selected Config is Out of Range!!\n");
						}else
						{
							break;
						}
					}while(1);

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].serialPortToken, &deviceInfoList[iSelectIndex].serialPortConfigOptions.serialPortConfigToken);
					get_Command = CMD_GetSerialPortConfigurationOptionsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetSerialPortConfigOptions Before Set!\n");
						continue;
					}

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].name, &deviceInfoList[iSelectIndex].serialPortConfig.name);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].serialPortConfig.token);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].serialPortToken, &deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken);
					deviceInfoList[iSelectIndex].serialPortConfig.type= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].type;
					deviceInfoList[iSelectIndex].serialPortConfig.baudRate= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].baudRate;
					deviceInfoList[iSelectIndex].serialPortConfig.dataBitLength= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].dataBitLength;
					deviceInfoList[iSelectIndex].serialPortConfig.parityBitCheck= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].parityBitCheck;
					deviceInfoList[iSelectIndex].serialPortConfig.stopBit= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].stopBit;
					deviceInfoList[iSelectIndex].serialPortConfig.flowControl= deviceInfoList[iSelectIndex].serialPortConfigList.configList[iTempIndex].flowControl;

					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. Name			= %s\n",deviceInfoList[iSelectIndex].serialPortConfig.name.stringData);
						printf("2. Token		= %s\n",deviceInfoList[iSelectIndex].serialPortConfig.token.stringData);
						printf("3. SerialPortToken	= %s\n",deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken.stringData);
						printf("4. Type			= %u	Support : [RS232]/[RS422Half]/[RS422Full]/[RS485Half]/[RS485Full]/[Generic] : %u/%u/%u/%u/%u/%u\n",deviceInfoList[iSelectIndex].serialPortConfig.type, \
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 2), \
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 3), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 4), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.typeOptions, 5));
						printf("5. BaudRate		= %lu\n",deviceInfoList[iSelectIndex].serialPortConfig.baudRate);
						printf("6. DataBit Length	= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].serialPortConfig.dataBitLength, deviceInfoList[iSelectIndex].serialPortConfigOptions.dataBitLengthMin, deviceInfoList[iSelectIndex].serialPortConfigOptions.dataBitLengthMax);
						printf("7. ParityBit Check	= %u	Support : [None]/[Even]/[Odd]/[Mark]/[Space]/[Extended] : %u/%u/%u/%u/%u/%u\n",deviceInfoList[iSelectIndex].serialPortConfig.parityBitCheck, \
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 2), \
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 3), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 4), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.parityBitCheckOptions, 5));
						printf("8. StopBit		= %u	Support : [1bit]/[1.5bit]/[2bit] : %u/%u/%u\n",deviceInfoList[iSelectIndex].serialPortConfig.stopBit,\
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.stopBitOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.stopBitOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.stopBitOptions, 2));
						printf("9. FlowControl		= %u	Support : [None]/[Xon/Xoff]/[Hardware] : %u/%u/%u\n",deviceInfoList[iSelectIndex].serialPortConfig.flowControl,\
							BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.flowControlOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.flowControlOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].serialPortConfigOptions.flowControlOptions, 2));
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.name);
							printf("\nEnter Name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.name);		
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.token);		
							break;
						case 3:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken);
							printf("\nEnter SerialPortToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken);		
							break;
						case 4:
							printf("\nEnter Type : (ex: 0: RS232, 1: RS422HalfDuplex, 2: RS422FullDuplex, 3: RS485HalfDuplex, 4: RS485FullDuplex, 5: Generic.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.type = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter BaudRate : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.baudRate = tempSetting;
							break;
						case 6:
							printf("\nEnter DataBit Length : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.dataBitLength = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter ParityBit Check : (ex: 0: None, 1: Even, 2: Odd, 3: Mark, 4: Space, 5: Extended.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.parityBitCheck = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter StopBit : (ex: 0: 1bit, 1: 1.5bit, 2: 2bit.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.stopBit = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter FlowControl : (ex: 0: None, 1: Xon/Xoff, 2: Hardware.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.flowControl = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.name);
							printf("\nEnter Name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.name);		
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.token);		
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken);
							printf("\nEnter SerialPortToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].serialPortConfig.serialPortToken);		
							printf("\nEnter Type : (ex: 0: RS232, 1: RS422HalfDuplex, 2: RS422FullDuplex, 3: RS485HalfDuplex, 4: RS485FullDuplex, 5: Generic.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.type = (Byte)tempSetting;
							printf("\nEnter BaudRate : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.baudRate = tempSetting;
							printf("\nEnter DataBit Length : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.dataBitLength = (Byte)tempSetting;
							printf("\nEnter ParityBit Check : (ex: 0: None, 1: Even, 2: Odd, 3: Mark, 4: Space, 5: Extended.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.parityBitCheck = (Byte)tempSetting;
							printf("\nEnter StopBit : (ex: 0: 1bit, 1: 1.5bit, 2: 2bit.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.stopBit = (Byte)tempSetting;
							printf("\nEnter FlowControl : (ex: 0: None, 1: Xon/Xoff, 2: Hardware.) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].serialPortConfig.flowControl = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

					//-----------Device IO Service----------
					//----------- Imaging Service----------
				case CMD_GetImagingSettingsInput:	//0x0300 : GetImagingSettings
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].imageConfig.videoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_IMG_GetStatusInput:	//0x0301 : GetStatus
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].focusStatusInfo.videoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].focusStatusInfo.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].focusStatusInfo.videoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_IMG_GetOptionsInput:	//0x0302 : GetOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].imageConfigOption.videoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfigOption.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfigOption.videoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetUserDefinedSettingsInput:	//0x037F : GetUserDefinedSettings
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetImagingSettingsInput:	//0x0380 : SetImagingSettings
					printf("Send GetImagingSettings For Getting Information.....\n");
					get_Command = CMD_GetImagingSettingsInput;
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
					printf("\nVideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetImagingSettings Before Set!\n");
						continue;
					}
					get_Command = CMD_IMG_GetOptionsInput;
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfigOption.videoSourceToken);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfigOption.videoSourceToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not Get_IMG_Options Before Set!\n");
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. videoSourceToken stringData	= %s\n",deviceInfoList[iSelectIndex].imageConfig.videoSourceToken.stringData);
						printf("2. backlightCompensationMode	= %u		Support : [OFF]/[ON] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.backlightCompensationMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.backlightCompensationMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.backlightCompensationMode, 1));
						printf("3. backlightCompensationLevel	= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.backlightCompensationLevel, deviceInfoList[iSelectIndex].imageConfigOption.backlightCompensationLevelMin, deviceInfoList[iSelectIndex].imageConfigOption.backlightCompensationLevelMax);
						printf("4. brightness			= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.brightness, deviceInfoList[iSelectIndex].imageConfigOption.brightnessMin, deviceInfoList[iSelectIndex].imageConfigOption.brightnessMax);
						printf("5. colorSaturation		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.colorSaturation, deviceInfoList[iSelectIndex].imageConfigOption.colorSaturationMin, deviceInfoList[iSelectIndex].imageConfigOption.colorSaturationMax);
						printf("6. contrast			= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.contrast, deviceInfoList[iSelectIndex].imageConfigOption.contrastMin, deviceInfoList[iSelectIndex].imageConfigOption.contrastMax);
						printf("7. exposureMode			= %u		Support : [Auto]/[Manual] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.exposureMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.exposureMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.exposureMode, 1));
						printf("8. exposurePriority		= %u		Support : [LowNoise]/[FrameRate] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.exposurePriority, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.exposurePriority, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.exposurePriority, 1));
						printf("9. exposureWindowbottom		= %u\n",deviceInfoList[iSelectIndex].imageConfig.exposureWindowbottom);
						printf("10. exposureWindowtop		= %u\n",deviceInfoList[iSelectIndex].imageConfig.exposureWindowtop);
						printf("11. exposureWindowright		= %u\n",deviceInfoList[iSelectIndex].imageConfig.exposureWindowright);
						printf("12. exposureWindowleft		= %u\n",deviceInfoList[iSelectIndex].imageConfig.exposureWindowleft);
						printf("13. minExposureTime		= %lu		Support : %lu ~ %lu\n",deviceInfoList[iSelectIndex].imageConfig.minExposureTime, deviceInfoList[iSelectIndex].imageConfigOption.minExposureTimeMin, deviceInfoList[iSelectIndex].imageConfigOption.minExposureTimeMax);
						printf("14. maxExposureTime		= %lu		Support : %lu ~ %lu\n",deviceInfoList[iSelectIndex].imageConfig.maxExposureTime, deviceInfoList[iSelectIndex].imageConfigOption.maxExposureTimeMin, deviceInfoList[iSelectIndex].imageConfigOption.maxExposureTimeMax);
						printf("15. exposureMinGain		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureMinGain, deviceInfoList[iSelectIndex].imageConfigOption.exposureMinGainMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureMinGainMax);
						printf("16. exposureMaxGain		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureMaxGain, deviceInfoList[iSelectIndex].imageConfigOption.exposureMaxGainMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureMaxGainMax);
						printf("17. exposureMinIris		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureMinIris, deviceInfoList[iSelectIndex].imageConfigOption.exposureMinIrisMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureMinIrisMax);
						printf("18. exposureMaxIris		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureMaxIris, deviceInfoList[iSelectIndex].imageConfigOption.exposureMaxIrisMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureMaxIrisMax);
						printf("19. exposureTime		= %lu		Support : %lu ~ %lu\n",deviceInfoList[iSelectIndex].imageConfig.exposureTime, deviceInfoList[iSelectIndex].imageConfigOption.exposureTimeMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureTimeMax);
						printf("20. exposureGain		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureGain, deviceInfoList[iSelectIndex].imageConfigOption.exposureGainMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureGainMax);
						printf("21. exposureIris		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.exposureIris, deviceInfoList[iSelectIndex].imageConfigOption.exposureIrisMin, deviceInfoList[iSelectIndex].imageConfigOption.exposureIrisMax);
						printf("22. autoFocusMode		= %u		Support : [Auto]/[Manual] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.autoFocusMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.autoFocusMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.autoFocusMode, 1));
						printf("23. focusDefaultSpeed		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.focusDefaultSpeed, deviceInfoList[iSelectIndex].imageConfigOption.focusDefaultSpeedMin, deviceInfoList[iSelectIndex].imageConfigOption.focusDefaultSpeedMax);
						printf("24. focusNearLimit		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.focusNearLimit, deviceInfoList[iSelectIndex].imageConfigOption.focusNearLimitMin, deviceInfoList[iSelectIndex].imageConfigOption.focusNearLimitMax);
						printf("25. focusFarLimit		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.focusFarLimit, deviceInfoList[iSelectIndex].imageConfigOption.focusFarLimitMin, deviceInfoList[iSelectIndex].imageConfigOption.focusFarLimitMax);
						printf("26. irCutFilterMode		= %u		Support : [ON]/[OFF]/[AUTO] : %u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.irCutFilterMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.irCutFilterMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.irCutFilterMode, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.irCutFilterMode, 2));
						printf("27. sharpness			= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.sharpness, deviceInfoList[iSelectIndex].imageConfigOption.sharpnessMin, deviceInfoList[iSelectIndex].imageConfigOption.sharpnessMax);
						printf("28. wideDynamicRangeMode	= %u		Support : [ON]/[OFF] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.wideDynamicRangeMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.wideDynamicRangeMode, 1));
						printf("29. wideDynamicRangeLevel	= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeLevel, deviceInfoList[iSelectIndex].imageConfigOption.wideDynamicRangeLevelMin, deviceInfoList[iSelectIndex].imageConfigOption.wideDynamicRangeLevelMax);
						printf("30. whiteBalanceMode		= %u		Support : [Auto]/[Manual] : %u/%u\n",deviceInfoList[iSelectIndex].imageConfig.whiteBalanceMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceMode, 1));
						printf("31. whiteBalanceCrGain		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCrGain, deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceCrGainMin, deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceCrGainMax);
						printf("32. whiteBalanceCbGain		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCbGain, deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceCbGainMin, deviceInfoList[iSelectIndex].imageConfigOption.whiteBalanceCbGainMax);
						printf("33. analogTVOutputStandard	= %u		Support : [AUTO]/[NTSC]/[PAL] : %u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.analogTVOutputStandard, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.analogTVOutputStandard, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.analogTVOutputStandard, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.analogTVOutputStandard, 2));
						printf("34. imageStabilizationLevel	= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.imageStabilizationLevel, deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationLevelMin, deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationLevelMax);
						printf("35. flickerControl		= %u		Support : [AUTO]/[50 Hz]/[60 Hz] : %u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.flickerControl, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.flickerControl, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.flickerControl, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.flickerControl, 2));
						printf("36. forcePersistance		= %u\n",deviceInfoList[iSelectIndex].imageConfig.forcePersistence);
						printf("37. imageStabilizationMode	= %u		Support : [OFF]/[ON]/[Auto]/[Extended] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.imageStabilizationMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationMode, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationMode, 2), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.imageStabilizationMode, 3));
						printf("38. deNoiseMode			= %u		Support : [Disable]/[2D]/[3D]/[Manual] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.deNoiseMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.deNoiseMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.deNoiseMode, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.deNoiseMode, 2), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.deNoiseMode, 3));
						printf("39. deNoiseStrength		= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.deNoiseStrength, deviceInfoList[iSelectIndex].imageConfigOption.deNoiseStrengthMin, deviceInfoList[iSelectIndex].imageConfigOption.deNoiseStrengthMax);
						printf("40. backLightControlMode	= %u		Support : [Disable]/[Enable]/[Manual] : %u/%u/%u\n",deviceInfoList[iSelectIndex].imageConfig.backLightControlMode, BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.backLightControlMode, 0), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.backLightControlMode, 1), BIT_MASK(deviceInfoList[iSelectIndex].imageConfigOption.backLightControlMode, 2));
						printf("41. backLightControlStrength	= %f	Support : %f ~ %f\n",deviceInfoList[iSelectIndex].imageConfig.backLightControlStrength, deviceInfoList[iSelectIndex].imageConfigOption.backLightControlStrengthMin, deviceInfoList[iSelectIndex].imageConfigOption.backLightControlStrengthMax);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							break;
						case 2:
							printf("\nEnter backlightCompensationMode :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.backlightCompensationMode = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter backlightCompensationLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.backlightCompensationLevel = tempFloatSetting;
							break;
						case 4:
							printf("\nEnter brightness :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.brightness = tempFloatSetting;
							break;
						case 5:
							printf("\nEnter colorSaturation :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.colorSaturation = tempFloatSetting;
							break;
						case 6:
							printf("\nEnter contrast :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.contrast = tempFloatSetting;
							break;
						case 7:
							printf("\nEnter exposureMode :  (ex: 0: Auto 1: Manual )");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMode =  (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter exposurePriority : (ex: 0: LowNoise, 1:FrameRate) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposurePriority =  (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter exposureWindowbottom :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowbottom = (Word)tempSetting;
							break;
						case 10:
							printf("\nEnter exposureWindowtop :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowtop = (Word)tempSetting;
							break;
						case 11:
							printf("\nEnter exposureWindowright :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowright = (Word)tempSetting;
							break;
						case 12:
							printf("\nEnter exposureWindowleft :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowleft = (Word)tempSetting;
							break;
						case 13:
							printf("\nEnter minExposureTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.minExposureTime = tempSetting;
							break;
						case 14:
							printf("\nEnter maxExposureTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.maxExposureTime = tempSetting;
							break;
						case 15:
							printf("\nEnter exposureMinGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMinGain = tempFloatSetting;
							break;
						case 16:
							printf("\nEnter exposureMaxGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMaxGain = tempFloatSetting;//15;
							break;
						case 17:
							printf("\nEnter exposureMinIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMinIris = tempFloatSetting;
							break;
						case 18:
							printf("\nEnter exposureMaxIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMaxIris = tempFloatSetting;
							break;
						case 19:
							printf("\nEnter exposureTime :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureTime = tempSetting;//18;
							break;
						case 20:
							printf("\nEnter exposureGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureGain = tempFloatSetting;
							break;
						case 21:
							printf("\nEnter exposureIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureIris = tempFloatSetting;
							break;
						case 22:
							printf("\nEnter autoFocusMode :  (ex: 0: AUTO, 1: MANUAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.autoFocusMode =  (Byte)tempSetting;
							break;
						case 23:
							printf("\nEnter focusDefaultSpeed : (float) ");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusDefaultSpeed = tempFloatSetting;
							break;
						case 24:
							printf("\nEnter focusNearLimit : (float) ");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusNearLimit = tempFloatSetting;
							break;
						case 25:
							printf("\nEnter focusFarLimit :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusFarLimit = tempFloatSetting;
							break;
						case 26:
							printf("\nEnter irCutFilterMode :  (ex: 0: ON, 1: OFF, 2: AUTO)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.irCutFilterMode = (Byte)tempSetting;
							break;
						case 27:
							printf("\nEnter sharpness :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.sharpness = tempFloatSetting;
							break;
						case 28:
							printf("\nEnter wideDynamicRangeMode :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeMode = (Byte)tempSetting;
							break;
						case 29:
							printf("\nEnter wideDynamicRangeLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeLevel = tempFloatSetting;
							break;
						case 30:
							printf("\nEnter whiteBalanceMode :  (ex: 0: AUTO, 1: MANUAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceMode = (Byte)tempSetting;
							break;
						case 31:
							printf("\nEnter whiteBalanceCrGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCrGain = tempFloatSetting;
							break;
						case 32:
							printf("\nEnter whiteBalanceCbGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCbGain = tempFloatSetting;
							break;
						case 37:
							printf("\nEnter imageStabilizationMode :  (ex: 0: OFF, 1: ON, 2:AUTO, 3:Extended)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.imageStabilizationMode = (Byte)tempSetting;
							break;
						case 34:
							printf("\nEnter imageStabilizationLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.imageStabilizationLevel =tempFloatSetting;
							break;
						case 35:
							printf("\nEnter FlickerControl :  (ex: 0: AUTO, 1: 50Hz, 2: 60Hz)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.flickerControl = (Byte) tempSetting;
							break;
						case 36:
							printf("\nEnter ForcePersistence :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.forcePersistence = (Byte) tempSetting;
							break;
						case 33:
							printf("\nEnter analogTVOutputStandard :  (ex: 0: AUTO, 1: NTSC, 2: PAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.analogTVOutputStandard = (Byte)tempSetting;
							break;
						case 38:
							printf("\nEnter deNoiseMode :  (ex: 0: Disable, 1: Enable 2D, Enable 3D, 3: Manual)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.deNoiseMode = (Byte)tempSetting;
							break;
						case 39:
							printf("\nEnter deNoiseStrength :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.deNoiseStrength =tempFloatSetting;
							break;
						case 40:
							printf("\nEnter backLightControlMode :  (ex: 0: Disable, 1: Enable, 2: Manual)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.backLightControlMode = (Byte)tempSetting;
							break;
						case 41:
							printf("\nEnter backLightControlStrength :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.backLightControlStrength =tempFloatSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].imageConfig.videoSourceToken);
							printf("\nEnter backlightCompensationMode :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.backlightCompensationMode = (Byte)tempSetting;
							printf("\nEnter backlightCompensationLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.backlightCompensationLevel = tempFloatSetting;
							printf("\nEnter brightness :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.brightness = tempFloatSetting;
							printf("\nEnter colorSaturation :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.colorSaturation = tempFloatSetting;
							printf("\nEnter contrast :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.contrast = tempFloatSetting;
							printf("\nEnter exposureMode :  (ex: 0: Auto 1: Manual )");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMode =  (Byte)tempSetting;
							printf("\nEnter exposurePriority : (ex: 0: LowNoise, 1:FrameRate) ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposurePriority =  (Byte)tempSetting;
							printf("\nEnter exposureWindowbottom :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowbottom = (Word)tempSetting;
							printf("\nEnter exposureWindowtop :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowtop = (Word)tempSetting;
							printf("\nEnter exposureWindowright :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowright = (Word)tempSetting;
							printf("\nEnter exposureWindowleft :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureWindowleft = (Word)tempSetting;
							printf("\nEnter minExposureTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.minExposureTime = tempSetting;
							printf("\nEnter maxExposureTime :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.maxExposureTime = tempSetting;
							printf("\nEnter exposureMinGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMinGain = tempFloatSetting;
							printf("\nEnter exposureMaxGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMaxGain = tempFloatSetting;//15;
							printf("\nEnter exposureMinIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMinIris = tempFloatSetting;
							printf("\nEnter exposureMaxIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureMaxIris = tempFloatSetting;
							printf("\nEnter exposureTime :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureTime = tempSetting;//18;
							printf("\nEnter exposureGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureGain = tempFloatSetting;
							printf("\nEnter exposureIris :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.exposureIris = tempFloatSetting;
							printf("\nEnter autoFocusMode :  (ex: 0: AUTO, 1: MANUAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.autoFocusMode =  (Byte)tempSetting;
							printf("\nEnter focusDefaultSpeed : (float) ");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusDefaultSpeed = tempFloatSetting;
							printf("\nEnter focusNearLimit : (float) ");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusNearLimit = tempFloatSetting;
							printf("\nEnter focusFarLimit :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.focusFarLimit = tempFloatSetting;
							printf("\nEnter irCutFilterMode :  (ex: 0: ON, 1: OFF, 2: AUTO)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.irCutFilterMode = (Byte)tempSetting;
							printf("\nEnter sharpness :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.sharpness = tempFloatSetting;
							printf("\nEnter wideDynamicRangeMode :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeMode = (Byte)tempSetting;
							printf("\nEnter wideDynamicRangeLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.wideDynamicRangeLevel = tempFloatSetting;
							printf("\nEnter whiteBalanceMode :  (ex: 0: AUTO, 1: MANUAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceMode = (Byte)tempSetting;
							printf("\nEnter whiteBalanceCrGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCrGain = tempFloatSetting;
							printf("\nEnter whiteBalanceCbGain :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.whiteBalanceCbGain = tempFloatSetting;
							printf("\nEnter imageStabilizationMode :  (ex: 0: OFF, 1: ON, 2:AUTO, 3:Extended)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.imageStabilizationMode = (Byte)tempSetting;
							printf("\nEnter imageStabilizationLevel :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.imageStabilizationLevel =tempFloatSetting;
							printf("\nEnter FlickerControl :  (ex: 0: AUTO, 1: 50Hz, 2: 60Hz)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.flickerControl = (Byte) tempSetting;
							printf("\nEnter ForcePersistence :  (ex: 0: OFF, 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.forcePersistence = (Byte) tempSetting;
							printf("\nEnter analogTVOutputStandard :  (ex: 0: AUTO, 1: NTSC, 2: PAL)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.analogTVOutputStandard = (Byte)tempSetting;
							printf("\nEnter deNoiseMode :  (ex: 0: Disable, 1: Enable 2D, Enable 3D, 3: Manual)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.deNoiseMode = (Byte)tempSetting;
							printf("\nEnter deNoiseStrength :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.deNoiseStrength =tempFloatSetting;
							printf("\nEnter backLightControlMode :  (ex: 0: Disable, 1: Enable, 2: Manual)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].imageConfig.backLightControlMode = (Byte)tempSetting;
							printf("\nEnter backLightControlStrength :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].imageConfig.backLightControlStrength =tempFloatSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].imageConfig.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_IMG_MoveInput:	//0x0381 : Move
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. videoSourceToken	= %s\n",deviceInfoList[iSelectIndex].focusMoveInfo.videoSourceToken.stringData);
						printf("2. absolutePosition	= %f\n",deviceInfoList[iSelectIndex].focusMoveInfo.absolutePosition);
						printf("3. absoluteSpeed	= %f\n",deviceInfoList[iSelectIndex].focusMoveInfo.absoluteSpeed);
						printf("4. relativeDistance	= %f\n",deviceInfoList[iSelectIndex].focusMoveInfo.relativeDistance);
						printf("5. relativeSpeed	= %f\n",deviceInfoList[iSelectIndex].focusMoveInfo.relativeSpeed);
						printf("6. continuousSpeed	= %f\n",deviceInfoList[iSelectIndex].focusMoveInfo.continuousSpeed);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].focusMoveInfo.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].focusMoveInfo.videoSourceToken);
							break;
						case 2:
							printf("\nEnter absolutePosition :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.absolutePosition = tempFloatSetting;
							break;
						case 3:
							printf("\nEnter absoluteSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.absoluteSpeed = tempFloatSetting;
							break;
						case 4:
							printf("\nEnter relativeDistance :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.relativeDistance = tempFloatSetting;
							break;
						case 5:
							printf("\nEnter relativeSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.relativeSpeed = tempFloatSetting;
							break;
						case 6:
							printf("\nEnter continuousSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.continuousSpeed = tempFloatSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].focusMoveInfo.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].focusMoveInfo.videoSourceToken);
							printf("\nEnter absolutePosition :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.absolutePosition = tempFloatSetting;
							printf("\nEnter absoluteSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.absoluteSpeed = tempFloatSetting;
							printf("\nEnter relativeDistance :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.relativeDistance = tempFloatSetting;
							printf("\nEnter relativeSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.relativeSpeed = tempFloatSetting;
							printf("\nEnter continuousSpeed :  (float)");
							SCANF("%f", &tempFloatSetting);
							deviceInfoList[iSelectIndex].focusMoveInfo.continuousSpeed = tempFloatSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_IMG_StopInput:	//0x0382 : Stop
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].focusStopInfo.videoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].focusStopInfo.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].focusStopInfo.videoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetUserDefinedSettingsInput:	//0x03FF : SetUserDefinedSettings
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken.stringData);
						printf("2. UerDefinedData	= %s\n",deviceInfoList[iSelectIndex].userDefinedSettings.uerDefinedData.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].userDefinedSettings.uerDefinedData);
							printf("\nEnter UerDefinedData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].userDefinedSettings.uerDefinedData);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].userDefinedSettings.videoSourceToken);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].userDefinedSettings.uerDefinedData);
							printf("\nEnter UerDefinedData :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].userDefinedSettings.uerDefinedData);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//----------- Imaging Service----------
					//----------- Media Service----------
				case CMD_GetProfilesInput:	//0x0400 : GetProfiles
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetVideoSourcesInput:	//0x0401 : GetVideoSources
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetVideoSourceConfigurationsInput:	//0x0402 : GetVideoSourceConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetGuaranteedNumberOfVideoEncoderInstancesInput:	//0x0403 : GetGuaranteedNumberOfVideoEncoderInstances
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. ConfigurationToken	= %s\n",deviceInfoList[iSelectIndex].guaranteedEncs.configurationToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].guaranteedEncs.configurationToken);
							printf("\nEnter ConfigurationToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].guaranteedEncs.configurationToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetVideoEncoderConfigurationsInput:	//0x0404 : GetVideoEncoderConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAudioSourcesInput:	//0x0405 : GetAudioSources
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAudioSourceConfigurationsInput:	//0x0406 : GetAudioSourceConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAudioEncoderConfigurationsInput:	//0x0407 : GetAudioEncoderConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetVideoSourceConfigurationOptionsInput:	//0x0408 : GetVideoSourceConfigurationOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSrcConfigToken	= %s\n",deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken.stringData);
						printf("2. ProfileToken		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetVideoEncoderConfigurationOptionsInput:	//0x0409 : GetVideoEncoderConfigurationOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoEncoder configuration Token	= %s\n",deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken.stringData);
						printf("2. ProfileToken				= %s\n",deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAudioSourceConfigurationOptionsInput:	//0x040A : GetAudioSourceConfigurationOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. AudioSource Configuration Token	= %s\n",deviceInfoList[iSelectIndex].audioSrcConfigOptions.audioSrcConfigToken.stringData);
						printf("2. ProfileToken				= %s\n",deviceInfoList[iSelectIndex].audioSrcConfigOptions.profileToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigOptions.audioSrcConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioSrcConfigOptions.audioSrcConfigToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioSrcConfigOptions.profileToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigOptions.audioSrcConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioSrcConfigOptions.audioSrcConfigToken);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioSrcConfigOptions.profileToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAudioEncoderConfigurationOptionsInput:	//0x040B : GetAudioEncoderConfigurationOptions
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. AudioEncoder configuration Token	= %s\n",deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken.stringData);
						printf("2. ProfileToken				= %s\n",deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken);
							printf("\nEnter VideoSource Configuration Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken);
							printf("\nEnter Profile Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_GetVideoOSDConfigurationInput:	//0x0440 : GetVideoOSDConfiguration
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetVideoPrivateAreaInput:	//0x0441 : GetVideoPrivateArea
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetSynchronizationPointInput:	//0x0480 : SetSynchronizationPoint
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. ProfileToken	= %s\n",deviceInfoList[iSelectIndex].syncPoint.profileToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].syncPoint.profileToken);
							printf("\nEnter ProfileToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].syncPoint.profileToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetVideoSourceConfigurationInput:	//0x0482 : SetVideoSourceConfiguration
					printf("Send GetProfiles For Getting Information.....\n");
					get_Command = CMD_GetProfilesInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetProfiles Before Set!\n");
						continue;
					}
					printf("Send GetVideoSourceConfig For Getting Information.....\n");
					get_Command = CMD_GetVideoSourceConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetVideoSourceConfig Before Set!\n");
						continue;
					}
					if( (deviceInfoList[iSelectIndex].videoSrcConfig.configListSize <= 0) || (deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize <= 0) )
					{
						printf("Can Not GetVideoSrcConfigOptions Before Set!\n");
						continue;
					}
					printf("Send GetVideoSrcConfigOptions For Getting Information.....\n");

					get_Command = CMD_GetVideoSourceConfigurationOptionsInput;
					printf("-------------------VideoSourceToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].videoSrcConfig.configListSize; i ++)
					{
						printf("Index : %u  VideoSourceToken = %s\n", i, deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].token.stringData);
					}
					printf("-------------------VideoSourceToken-------------------\n");
					printf("Choose VideoSrcToken to GetVideoSrcConfigOptions\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.videoSrcConfigToken);
					printf("-------------------ProfileToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize; i ++)
					{
						printf("Index : %u  ProfileToken = %s\n", i, deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[i].token.stringData);
					}
					printf("-------------------ProfileToken-------------------\n");
					printf("Choose ProfileToken to GetVideoSrcConfigOptions\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[iTempIndex].token, &deviceInfoList[iSelectIndex].videoSrcConfigOptions.profileToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetVideoSrcConfigOptions Before Set!\n");
					}
					printf("Get VideoSrcConfigList  from Device\n");
					for( i = 0; i <deviceInfoList[iSelectIndex].videoSrcConfig.configListSize; i ++)
					{
						printf("---------------------------------- Index %u -------------------------------------\n", i);
						printf("    name stringData		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].name.stringData);
						printf("    token stringData		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].token.stringData);
						printf("    srcToken stringData		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].srcToken.stringData);
						printf("    useCount			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].useCount);
						printf("    bounds_x			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].bounds_x);
						printf("    bounds_y			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].bounds_y);
						printf("    boundsWidth			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].boundsWidth);
						printf("    boundsHeight		= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].boundsHeight);
						printf("    rotateMode			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].rotateMode);
						printf("    rotateDegree		= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].rotateDegree);
						printf("    mirrorMode			= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].mirrorMode);
						printf("    maxFrameRate		= %u\n",deviceInfoList[iSelectIndex].videoSrcConfig.configList[i].maxFrameRate);
						printf("---------------------------------- Index %u -------------------------------------\n", i);
					}
					do{
						printf("Load one of VideoSrcConfig from VideoSrcConfig List Before set (0 ~ %u)\n", deviceInfoList[iSelectIndex].videoSrcConfig.configListSize - 1);
						SCANF("%d", &iTempIndex);
						if(iTempIndex >=deviceInfoList[iSelectIndex].videoSrcConfig.configListSize)
						{
							printf("The Selected Config is Out of Range!!\n");
						}else
						{
							break;
						}
					}while(1);

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].name, &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].srcToken, &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken);
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.useCount = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].useCount;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_x = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].bounds_x;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_y = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].bounds_y;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsWidth = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].boundsWidth;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsHeight = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].boundsHeight;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateMode = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].rotateMode;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateDegree = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].rotateDegree;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.mirrorMode = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].mirrorMode;
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.maxFrameRate = deviceInfoList[iSelectIndex].videoSrcConfig.configList[iTempIndex].maxFrameRate;

					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. name			= %s\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name.stringData);
						printf("2. token		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token.stringData);
						printf("3. srcToken		= %s\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken.stringData);
						printf("4. useCount		= %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.useCount);
						printf("5. bounds_x		= %u		Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_x, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_X_Min, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_X_Max);
						printf("6. bounds_y		= %u		Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_y, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Y_Min, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Y_Max);
						printf("7. boundsWidth		= %u		Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsWidth, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Width_Min, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Width_Max);
						printf("8. boundsHeight		= %u		Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsHeight, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Height_Min, deviceInfoList[iSelectIndex].videoSrcConfigOptions.boundsRange_Heigh_Max);
						printf("9. rotateMode		= %u		Support : [OFF]/[ON]/[AUTO] : %u/%u/%u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateMode, BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.rotateModeOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.rotateModeOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.rotateModeOptions, 2));
						printf("10. rotateDegree	= %u		Support : %u ~ \n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateDegree, deviceInfoList[iSelectIndex].videoSrcConfigOptions.rotateDegreeMinOption);
						printf("11. mirrorMode		= %u		Support : [OFF]/[Horizontal]/[Vertical]/[Both] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.mirrorMode, BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.mirrorModeOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.mirrorModeOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.mirrorModeOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].videoSrcConfigOptions.mirrorModeOptions, 3));
						printf("12. maxFrameRate	= %u		Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.maxFrameRate, deviceInfoList[iSelectIndex].videoSrcConfigOptions.maxFrameRateMin, deviceInfoList[iSelectIndex].videoSrcConfigOptions.maxFrameRateMax);
						printf("13. forcePersistence	= %u\n",deviceInfoList[iSelectIndex].videoSrcConfigSetParam.forcePersistence);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token);
							break;
						case 3:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken);
							printf("\nEnter SourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken);
							break;
						case 4:
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.useCount = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter bounds_x :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_x = (Word)tempSetting;
							break;
						case 6:
							printf("\nEnter bounds_y :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_y = (Word)tempSetting;
							break;
						case 7:
							printf("\nEnter boundsWidth :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsWidth = (Word)tempSetting;
							break;
						case 8:
							printf("\nEnter boundsHeight :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsHeight = (Word)tempSetting;
							break;
						case 9:
							printf("\nEnter rotateMode :  (ex: 0: OFF, 1: ON, 2: AUTO)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateMode = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter rotateDegree :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateDegree = (Word)tempSetting;
							break;
						case 11:
							printf("\nEnter mirrorMode :  (ex: 0: OFF, 1: Horizontal, 2: Vertical, 3: Both)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.mirrorMode = (Byte)tempSetting;
							break;
						case 12:
							printf("\nEnter maxFrameRate :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.maxFrameRate = (Byte)tempSetting;
							break;
						case 13:
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.forcePersistence = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.name);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.token);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken);
							printf("\nEnter SourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoSrcConfigSetParam.srcToken);
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.useCount = (Byte)tempSetting;
							printf("\nEnter bounds_x :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_x = (Word)tempSetting;
							printf("\nEnter bounds_y :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.bounds_y = (Word)tempSetting;
							printf("\nEnter boundsWidth :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsWidth = (Word)tempSetting;
							printf("\nEnter boundsHeight :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.boundsHeight = (Word)tempSetting;
							printf("\nEnter rotateMode :  (ex: 0: OFF, 1: ON, 2: AUTO)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateMode = (Byte)tempSetting;
							printf("\nEnter rotateDegree :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.rotateDegree = (Word)tempSetting;
							printf("\nEnter mirrorMode :  (ex: 0: OFF, 1: Horizontal, 2: Vertical, 3: Both)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.mirrorMode = (Byte)tempSetting;
							printf("\nEnter maxFrameRate :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.maxFrameRate = (Byte)tempSetting;
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcConfigSetParam.forcePersistence = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].videoSrcConfigSetParam.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;

				case CMD_SetVideoEncoderConfigurationInput:	//0x0484 : SetVideoEncoderConfiguration
					printf("Send GetProfiles For Getting Information.....\n");
					get_Command = CMD_GetProfilesInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetProfiles Before Set!\n");
						continue;
					}
					printf("Send GetVideoEncConfig For Getting Information.....\n");
					get_Command = CMD_GetVideoEncoderConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetVideoEncConfig Before Set!\n");
						continue;
					}
					if( (deviceInfoList[iSelectIndex].videoEncConfig.configListSize <= 0) || (deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize <= 0) )
					{
						printf("Can Not GetVideoEncConfigOption Before Set!\n");
					}
					printf("Send GetVideoEncConfigOption For Getting Information.....\n");
					get_Command = CMD_GetVideoEncoderConfigurationOptionsInput;
					printf("-------------------VideoEncConfigToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].videoEncConfig.configListSize; i ++)
					{
						printf("Index : %u  VideoEncConfigToken = %s\n", i, deviceInfoList[iSelectIndex].videoEncConfig.configList[i].token.stringData);
					}
					printf("-------------------VideoEncConfigToken-------------------\n");
					printf("Choose VideoEncConfigToken to GetVideoEncConfigOption\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].videoEncConfigOptions.videoEncConfigToken);
					printf("-------------------ProfileToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize; i ++)
					{
						printf("Index : %u  ProfileToken = %s\n", i, deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[i].token.stringData);
					}
					printf("-------------------ProfileToken-------------------\n");
					printf("Choose ProfileToken to GetVideoEncConfigOption\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[iTempIndex].token, &deviceInfoList[iSelectIndex].videoEncConfigOptions.profileToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetVideoEncConfigOption Before Set!\n");
					}
					printf("Get VideoEncConfig  from Device\n");
					for( i = 0; i < deviceInfoList[iSelectIndex].videoEncConfig.configListSize; i ++)
					{
						printf("---------------------------------- Index %u -------------------------------------\n",i);
						printf("    name stringData		= %s\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].name.stringData);
						printf("    token stringData		= %s\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].token.stringData);
						printf("    useCount			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].useCount);
						printf("    encoding			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].encoding);
						printf("    width			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].width);
						printf("    height			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].height);
						printf("    quality			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].quality);
						printf("    frameRateLimit		= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].frameRateLimit);
						printf("    encodingInterval		= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].encodingInterval);
						printf("    bitrateLimit		= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].bitrateLimit);
						printf("    rateControlType		= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].rateControlType);
						printf("    govLength			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].govLength);
						printf("    profile			= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].profile);
						printf("    targetBitrateLimit		= %u\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].targetBitrateLimit);
						printf("    aspectRatio			= 0x%x\n", deviceInfoList[iSelectIndex].videoEncConfig.configList[i].aspectRatio);
						printf("---------------------------------- Index %u -------------------------------------\n",i);
					}

					do{
						printf("Load one of VideoEncConfig from VideoEncoderConfig List Before set (0 ~ %u)\n", deviceInfoList[iSelectIndex].videoEncConfig.configListSize - 1);
						SCANF("%d", &iTempIndex);
						if(iTempIndex >=deviceInfoList[iSelectIndex].videoEncConfig.configListSize)
						{
							printf("The Selected Config is Out of Range!!\n");
						}else
						{
							break;
						}
					}while(1);

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].name, &deviceInfoList[iSelectIndex].videoEncConfigSetParam.name);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].videoEncConfigSetParam.token);
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.useCount= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].useCount;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].encoding;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.width= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].width;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.height= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].height;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.quality= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].quality;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.frameRateLimit= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].frameRateLimit;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.encodingInterval= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].encodingInterval;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.bitrateLimit= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].bitrateLimit;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.rateControlType= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].rateControlType;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.govLength= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].govLength;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].profile;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.targetBitrateLimit= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].targetBitrateLimit;
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.aspectRatio= deviceInfoList[iSelectIndex].videoEncConfig.configList[iTempIndex].aspectRatio;

					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. name			= %s\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.name.stringData);
						printf("2. token		= %s\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.token.stringData);
						printf("3. useCount		= %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.useCount);
						printf("4. encoding		= %u	Support : [JPEG]/[MPEG4]/[H264]/[MPEG2] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding, BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingOption, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingOption, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingOption, 2), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingOption, 3));
						printf("5. width x height	= %u x %u	Support : ",deviceInfoList[iSelectIndex].videoEncConfigSetParam.width, deviceInfoList[iSelectIndex].videoEncConfigSetParam.height);
						for( i = 0; i < deviceInfoList[iSelectIndex].videoEncConfigOptions.resolutionsAvailableListSize; i ++)
							printf("%u x %u , ", deviceInfoList[iSelectIndex].videoEncConfigOptions.resolutionsAvailableList[i].width, deviceInfoList[iSelectIndex].videoEncConfigOptions.resolutionsAvailableList[i].height);
						printf("\n");
						printf("6. quality		= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.quality, deviceInfoList[iSelectIndex].videoEncConfigOptions.qualityMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.qualityMax);
						printf("7. frameRateLimit	= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.frameRateLimit, deviceInfoList[iSelectIndex].videoEncConfigOptions.frameRateMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.frameRateMax);
						printf("8. encodingInterval	= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.encodingInterval, deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingIntervalMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.encodingIntervalMax);
						printf("9. bitrateLimit		= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.bitrateLimit, deviceInfoList[iSelectIndex].videoEncConfigOptions.bitrateRangeMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.bitrateRangeMax);
						printf("10. rateControlType	= %u	Support : [CBR]/[VBR]/[Fixed]/[CVBR] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.rateControlType, BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.rateControlTypeOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.rateControlTypeOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.rateControlTypeOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.rateControlTypeOptions, 3));
						printf("11. govLength		= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.govLength, deviceInfoList[iSelectIndex].videoEncConfigOptions.govLengthRangeMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.govLengthRangeMax);
						if(deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding == 0x1) // MPEG4
							printf("12. profile		= %u	Support : [SP]/[ASP] : %u/%u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile, BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 1));
						else if(deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding == 0x2)		//H264
							printf("12. profile		= %u	Support : [Baseline]/[Main]/[Extended]/[High] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile, BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 3));
						else
							printf("12. profile		= %u	Support : [SP/Baseline]/[Main]/[Extended]/[High] : %u/%u/%u/%u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile, BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 0), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 1), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 2), BIT_MASK(deviceInfoList[iSelectIndex].videoEncConfigOptions.profileOptions, 3));
						printf("13. forcePersistance	= %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.forcePersistence);
						printf("14. targetBitrateLimit	= %u	Support : %u ~ %u\n",deviceInfoList[iSelectIndex].videoEncConfigSetParam.targetBitrateLimit, deviceInfoList[iSelectIndex].videoEncConfigOptions.targetBitrateRangeMin, deviceInfoList[iSelectIndex].videoEncConfigOptions.targetBitrateRangeMax);
						printf("15. aspectRatio		= 0x%x	Support : ",deviceInfoList[iSelectIndex].videoEncConfigSetParam.aspectRatio);
						for( i = 0; i < deviceInfoList[iSelectIndex].videoEncConfigOptions.aspectRatioAvailableListSize; i ++)
							printf("0x%x , ",deviceInfoList[iSelectIndex].videoEncConfigOptions.aspectRatioList[i]);
						printf("\n");
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoEncConfigSetParam.name);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoEncConfigSetParam.token);
							break;
						case 3:
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.useCount = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter encoding :  (ex: 0: JPEG, 1: MPEG4, 2: H264, 3: MPEG2)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter width :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.width = (Word)tempSetting;
							printf("\nEnter height :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.height = (Word)tempSetting;
							break;
						case 6:
							printf("\nEnter quality :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.quality = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter frameRateLimit :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.frameRateLimit = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter encodingInterval :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.encodingInterval = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter bitrateLimit :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.bitrateLimit = (Word)tempSetting;
							break;
						case 10:
							printf("\nEnter rateControlType :  (ex: 0: CBR 1: VBR 2: Fixed QP 3: CVBR)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.rateControlType = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter govLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.govLength = (Byte)tempSetting;
							break;
						case 12:
							printf("\nEnter profile :  (ex: 0: SP, 1:ASP (for MPEG4)  0: Baseline, 1: Main, 2; Extended, 3: High (for H264))");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile = (Byte)tempSetting; 
							break;
						case 13:
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.forcePersistence = (Byte)tempSetting; 
							break;
						case 14:
							printf("\nEnter TargetBitrateLimit :  (kbps)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.targetBitrateLimit = (Word)tempSetting; 
							break;
						case 15:
							printf("\nEnter aspectRatio :  (ex: 0x1009, 0x0403 for 16:9 and 4:3 )");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.aspectRatio = (Word)tempSetting; 
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoEncConfigSetParam.name);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoEncConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].videoEncConfigSetParam.token);
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.useCount = (Byte)tempSetting;
							printf("\nEnter encoding :  (ex: 0: JPEG, 1: MPEG4, 2: H264, 3: MPEG2)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.encoding = (Byte)tempSetting;
							printf("\nEnter width :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.width = (Word)tempSetting;
							printf("\nEnter height :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.height = (Word)tempSetting;
							printf("\nEnter quality :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.quality = (Byte)tempSetting;
							printf("\nEnter frameRateLimit :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.frameRateLimit = (Byte)tempSetting;
							printf("\nEnter encodingInterval :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.encodingInterval = (Byte)tempSetting;
							printf("\nEnter bitrateLimit :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.bitrateLimit = (Word)tempSetting;
							printf("\nEnter rateControlType :  (ex: 0: CBR 1: VBR 2: Fixed QP 3: CVBR)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.rateControlType = (Byte)tempSetting;
							printf("\nEnter govLength :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.govLength = (Byte)tempSetting;
							printf("\nEnter profile :  (ex: 0: SP, 1:ASP (for MPEG4)  0: Baseline, 1: Main, 2; Extended, 3: High (for H264))");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.profile = (Byte)tempSetting; 
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.forcePersistence = (Byte)tempSetting; 
							printf("\nEnter TargetBitrateLimit :  (kbps)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.targetBitrateLimit = (Word)tempSetting; 
							printf("\nEnter aspectRatio :  (ex: 0x1009, 0x0403 for 16:9 and 4:3 )");
							SCANF("%lx", &tempSetting);
							deviceInfoList[iSelectIndex].videoEncConfigSetParam.aspectRatio = (Word)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].videoEncConfigSetParam.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetAudioSourceConfigurationInput:	//0x0486 : SetAudioSourceConfiguration
					printf("Send GetAudioSourceConfig For Getting Information.....\n");
					get_Command = CMD_GetAudioSourceConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetAudioSourceConfig Before Set!\n");
						continue;
					}
					printf("Get AudioSrcConfig  from Device\n");
					for( i = 0; i < deviceInfoList[iSelectIndex].audioSrcConfig.configListSize; i ++)
					{
						printf("----------------------------- Index %u ----------------------------------\n",i);
						printf("    name stringData		= %s\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].name.stringData);
						printf("    token stringData		= %s\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].token.stringData);
						printf("    sourceToken stringData	= %s\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].sourceToken.stringData);
						printf("    useCount			= %u\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].useCount);
						if(deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].extensionFlag)
						{
							printf("    inputLevel			= %lu\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].inputLevel);
							printf("    channels			= %u\n",deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[i].channels);
						}

						printf("----------------------------- Index %u ----------------------------------\n",i);
					}

					do{
						printf("Load one of AudioSrcConfig from AudioSrcConfig List Before set (0 ~ %u)\n", deviceInfoList[iSelectIndex].audioSrcConfig.configListSize - 1);
						SCANF("%d", &iTempIndex);
						if(iTempIndex >=deviceInfoList[iSelectIndex].audioSrcConfig.configListSize)
						{
							printf("The Selected Config is Out of Range!!\n");
						}else
						{
							break;
						}
					}while(1);

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[iTempIndex].name, &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[iTempIndex].token, &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[iTempIndex].sourceToken, &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken);
					deviceInfoList[iSelectIndex].audioSrcConfigSetParam.useCount = deviceInfoList[iSelectIndex].audioSrcConfig.audioSrcConfigList[iTempIndex].useCount;
					deviceInfoList[iSelectIndex].audioSrcConfigSetParam.extensionFlag = 1;
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. name			= %s\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name.stringData);
						printf("2. token		= %s\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token.stringData);
						printf("3. sourceToken		= %s\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken.stringData);
						printf("4. useCount		= %u\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.useCount);
						printf("5. forcePersistance	= %u\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.forcePersistence);
						printf("6. inputLevel		= %lu\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.inputLevel);
						printf("7. channels		= %u\n",deviceInfoList[iSelectIndex].audioSrcConfigSetParam.channels);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token);
							break;
						case 3:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken);
							printf("\nEnter SourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken);
							break;
						case 4:
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.useCount= (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.forcePersistence= (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter inputLevel :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.inputLevel= (Dword)tempSetting;
							break;
						case 7:
							printf("\nEnter channels :  (ex: 1: Mono 2: Stereo)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.channels= (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.name);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.token);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken);
							printf("\nEnter SourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioSrcConfigSetParam.sourceToken);
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.useCount= (Byte)tempSetting;
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.forcePersistence= (Byte)tempSetting;
							printf("\nEnter inputLevel :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.inputLevel= (Dword)tempSetting;
							printf("\nEnter channels :  (ex: 1: Mono 2: Stereo)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioSrcConfigSetParam.channels= (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetAudioEncoderConfigurationInput:	//0x0487 : SetAudioEncoderConfiguration
					printf("Send GetProfiles For Getting Information.....\n");
					get_Command = CMD_GetProfilesInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetProfiles Before Set!\n");
						continue;
					}
					printf("Send GetAudioEncConfig For Getting Information.....\n");
					get_Command = CMD_GetAudioEncoderConfigurationsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetAudioEncConfig Before Set!\n");
						continue;
					}
					if( (deviceInfoList[iSelectIndex].audioEncConfig.configListSize <= 0) || (deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize <= 0) )
					{
						printf("Can Not GetAudioEncConfigOptions Before Set!\n");
						continue;
					}
					printf("Send GetAudioEncConfigOptions For Getting Information.....\n");
					get_Command = CMD_GetAudioEncoderConfigurationOptionsInput;
					printf("-------------------AudioEncConfigToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].audioEncConfig.configListSize; i ++)
					{
						printf("Index : %u  AudioEncConfigToken = %s\n", i, deviceInfoList[iSelectIndex].audioEncConfig.configList[i].token.stringData);
					}
					printf("-------------------AudioEncConfigToken-------------------\n");
					printf("Choose AudioEncConfigToken to GetAudioEncConfigOptions\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigToken);
					printf("-------------------ProfileToken-------------------\n");
					for(i = 0; i < deviceInfoList[iSelectIndex].mediaProfiles.profilesListSize; i ++)
					{
						printf("Index : %u  ProfileToken = %s\n", i, deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[i].token.stringData);
					}
					printf("-------------------ProfileToken-------------------\n");
					printf("Choose ProfileToken to GetAudioEncConfigOptions\n");
					SCANF("%d", &iTempIndex);
					check = Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].mediaProfiles.mediaProfilesParam[iTempIndex].token, &deviceInfoList[iSelectIndex].audioEncConfigOptions.profileToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(error)
					{
						printf("Can Not GetAudioEncConfigOptions Before Set!\n");
						continue;
					}
					printf("Get AudioEncConfig  from Device\n");
					for( i = 0; i < deviceInfoList[iSelectIndex].audioEncConfig.configListSize; i ++)
					{
						printf("---------------------------------- Index %u -------------------------------------\n",i);
						printf("    token stringData	= %s\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].token.stringData);
						printf("    name stringData	= %s\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].name.stringData);
						printf("    useCount		= %u\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].useCount);
						printf("    encoding		= %u\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].encoding);
						printf("    bitrate		= %u\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].bitrate);
						printf("    sampleRate		= %u\n",deviceInfoList[iSelectIndex].audioEncConfig.configList[i].sampleRate);
						printf("---------------------------------- Index %u -------------------------------------\n",i);
					}

					do{
						printf("Load one of AudioEncConfig from AudioEncConfig List Before set (0 ~ %u)\n", deviceInfoList[iSelectIndex].audioEncConfig.configListSize - 1);
						SCANF("%d", &iTempIndex);
						if(iTempIndex >=deviceInfoList[iSelectIndex].audioEncConfig.configListSize)
						{
							printf("The Selected Config is Out of Range!!\n");
						}else
						{
							break;
						}
					}while(1);

					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].name, &deviceInfoList[iSelectIndex].audioEncConfigSetParam.name);
					Cmd_StringResetCopy(&deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].token, &deviceInfoList[iSelectIndex].audioEncConfigSetParam.token);
					deviceInfoList[iSelectIndex].audioEncConfigSetParam.useCount = deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].useCount;
					deviceInfoList[iSelectIndex].audioEncConfigSetParam.encoding = deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].encoding;
					deviceInfoList[iSelectIndex].audioEncConfigSetParam.bitrate = deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].bitrate;
					deviceInfoList[iSelectIndex].audioEncConfigSetParam.sampleRate = deviceInfoList[iSelectIndex].audioEncConfig.configList[iTempIndex].sampleRate;

					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. name			= %s\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.name.stringData);
						printf("2. token		= %s\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.token.stringData);
						printf("3. useCount		= %u\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.useCount);
						printf("4. encoding		= %u		Support : ",deviceInfoList[iSelectIndex].audioEncConfigSetParam.encoding);
						for( i = 0; i < deviceInfoList[iSelectIndex].audioEncConfigOptions.configListSize; i ++)
							printf("%u, ", deviceInfoList[iSelectIndex].audioEncConfigOptions.audioEncConfigOptionsParam[i].encodingOption);
						printf("\n");
						printf("5. bitrate		= %u\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.bitrate);
						printf("6. sampleRate		= %u\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.sampleRate);
						printf("7. forcePersistence	= %u\n",deviceInfoList[iSelectIndex].audioEncConfigSetParam.forcePersistence);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioEncConfigSetParam.name);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioEncConfigSetParam.token);
							break;
						case 3:
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.useCount = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter encoding :  (ex: 0: G711, 1: G726, 2: AAC, 3: G729, 4: MPEG1, 5: MPEG2, 6: AC3, 7: PCM, 8: ADPCM)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.encoding = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter bitrate :  (kbps)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.bitrate = (Word)tempSetting;
							break;
						case 6:
							printf("\nEnter sampleRate :  (ex: 441 for 44.1KHz)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.sampleRate = (Word)tempSetting;
							break;
						case 7:
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.forcePersistence = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigSetParam.name);
							printf("\nEnter name :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioEncConfigSetParam.name);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].audioEncConfigSetParam.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].audioEncConfigSetParam.token);
							printf("\nEnter useCount :  ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.useCount = (Byte)tempSetting;
							printf("\nEnter encoding :  (ex: 0: G711, 1: G726, 2: AAC, 3: G729, 4: MPEG1, 5: MPEG2, 6: AC3, 7: PCM, 8: ADPCM)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.encoding = (Byte)tempSetting;
							printf("\nEnter bitrate :  (kbps)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.bitrate = (Word)tempSetting;
							printf("\nEnter sampleRate :  (ex: 441 for 44.1KHz)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.sampleRate = (Word)tempSetting;
							printf("\nEnter ForcePersistence :  (ex: 0: True 1: False)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].audioEncConfigSetParam.forcePersistence = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetVideoOSDConfigurationInput:	//0x04C0 : SetVideoOSDConfiguration
					printf("Send GetVideoOSDConfiguration For Getting Information.....\n");
					get_Command = CMD_GetVideoOSDConfigurationInput;
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetVideoOSDConfiguration Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken.stringData);
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
						printf("16. text		= %s\n",deviceInfoList[iSelectIndex].videoOSDConfig.text.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							break;
						case 2:
							printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable = (Byte)tempSetting;
							break;
						case 3:
							printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.datePosition = (Byte)tempSetting;
							break;
						case 4:
							printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat = (Byte)tempSetting;
							break;
						case 5:
							printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable = (Byte)tempSetting;
							break;
						case 6:
							printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timePosition = (Byte)tempSetting;
							break;
						case 7:
							printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat = (Byte)tempSetting;
							break;
						case 8:
							printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable = (Byte)tempSetting;
							break;
						case 9:
							printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition = (Byte)tempSetting;
							break;
						case 10:
							printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logooption = (Byte)tempSetting;
							break;
						case 11:
							printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable = (Byte)tempSetting;
							break;
						case 12:
							printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition = (Byte)tempSetting;
							break;
						case 13:
							printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption = (Byte)tempSetting;
							break;
						case 14:
							printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.textEnable = (Byte)tempSetting;
							break;
						case 15:
							printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.textPosition = (Byte)tempSetting;
							break;
						case 16:
							printf("\nEnter Text :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.text);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.videoSrcToken);
							printf("\nEnter dateEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.dateEnable = (Byte)tempSetting;
							printf("\nEnter datePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.datePosition = (Byte)tempSetting;
							printf("\nEnter dateFormat :  (EX : 0: D/M/Y, 1: M/D/Y, 2: Y/M/D)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.dateFormat = (Byte)tempSetting;
							printf("\nEnter timeEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timeEnable = (Byte)tempSetting;
							printf("\nEnter timePosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timePosition = (Byte)tempSetting;
							printf("\nEnter timeFormat :  (EX : 0: AM/PM, 1: 24hr)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.timeFormat = (Byte)tempSetting;
							printf("\nEnter logoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logoEnable = (Byte)tempSetting;
							printf("\nEnter logoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logoPosition = (Byte)tempSetting;
							printf("\nEnter logooption :  (EX: 0: Image1, 1: Image2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.logooption = (Byte)tempSetting;
							printf("\nEnter detailInfoEnable :  (EX: 0: Off, 1: On)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoEnable = (Byte)tempSetting;
							printf("\nEnter detailInfoPosition :  (EX: 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoPosition = (Byte)tempSetting;
							printf("\nEnter detailInfooption :  (EX: 0: Information1, 1: Information2.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.detailInfoOption = (Byte)tempSetting;
							printf("\nEnter textEnable :  (EX : 0: OFF 1: ON)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.textEnable = (Byte)tempSetting;
							printf("\nEnter textPosition :  (EX : 0: Left-Top, 1: Left-Center, 2: Left-Down 3: Right-Top, 4: Right-Center, 5: Right-Down)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoOSDConfig.textPosition = (Byte)tempSetting;
							printf("\nEnter Text :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoOSDConfig.text);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoOSDConfig.text);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetVideoPrivateAreaInput:	//0x04C1 : SetVideoPrivateArea
					printf("Send GetVideoPrivateArea For Getting Information.....\n");
					get_Command = CMD_GetVideoPrivateAreaInput;
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetVideoOSDConfiguration Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken.stringData);
						printf("2. Private Polygon\n");
						printf("   polygonListSize	= %u\n",deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize);
						for( i = 0; i < deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize; i ++)
							printf("   [%u]   polygon_x	= %u      polygon_y	= %u\n", i,deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_x, deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_y);
						printf("3. privateAreaEnable	= %u\n",deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaEnable);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoPrivateArea.videoSrcToken);
							break;
						case 2:
							printf("\n1. Clear all and Create New Private Polygon\n");
							printf("2. Modify Private Polygon\n");
							printf("\nInput index for setting Private Polygon: ");
							SCANF("%d", &selectNum);
							if(selectNum == 1)
							{
								User_memory_free(deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon);
								deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon = NULL;
								do
								{
									printf("\nEnter polygonListSize : (must larger than 2)");
									SCANF("%lu", &tempSetting);
								}while(tempSetting < 2);
								deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize = (Byte)tempSetting;
								deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon = (PrivateAreaPolygon *)	User_memory_allocate(deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize * sizeof(PrivateAreaPolygon));
								for( i = 0; i < deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize; i ++)
								{
									printf("\nEnter Polygon %u polygon_x : ", i);
									SCANF("%lu", &tempSetting);
									deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_x = (Word)tempSetting;
									printf("\nEnter Polygon %u polygon_y : ", i);
									SCANF("%lu", &tempSetting);
									deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_y = (Word)tempSetting;
								}
							}else if(selectNum == 2)
							{
								for( i = 0; i < deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize; i ++)
								{
									printf("%u:   polygon_x	= %u      polygon_y	= %u\n", i,deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_x, deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[i].polygon_y);
								}
								printf("\nInput index for setting Private Polygon: (0 ~ %u) ", deviceInfoList[iSelectIndex].videoPrivateArea.polygonListSize-1);
								SCANF("%d", &selectNum);

								printf("\nEnter polygon_x : ");
								SCANF("%lu", &tempSetting);
								deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[selectNum].polygon_x = (Word)tempSetting;
								printf("\nEnter polygon_y : ");
								SCANF("%lu", &tempSetting);
								deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaPolygon[selectNum].polygon_y = (Word)tempSetting;
							}else
							{
								printf("Unsupported Setting!\n");
								continue;
							}
							break;
						case 3:
							printf("\nEnter privateAreaEnable (ex: 0: Off, 1: On) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoPrivateArea.privateAreaEnable = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetVideoSourceControlInput://0x04C2 : SetVideoSourceControl
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].videoSrcControl.videoSrcToken.stringData);
						printf("2. controlCommand	= %u\n",deviceInfoList[iSelectIndex].videoSrcControl.controlCommand);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n"); 
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcControl.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcControl.videoSrcToken);
							break;
						case 2:
							printf("\nEnter controlCommand (ex: 0: None, 1: Up, 2: Down, 3: Left, 4: Right, 5: Enter, 6: Back) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcControl.controlCommand = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].videoSrcControl.videoSrcToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].videoSrcControl.videoSrcToken);
							printf("\nEnter controlCommand (ex: 0: None, 1: Up, 2: Down, 3: Left, 4: Right, 5: Enter, 6: Back) : ");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].videoSrcControl.controlCommand = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//----------- Media Service----------
					//------------ PTZ Service-----------
				case CMD_GetConfigurationsInput:	//0x0500 : GetConfigurations
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_PTZ_GetStatusInput:	//0x0501 : GetStatus
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzStatus.token.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzStatus.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzStatus.token);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetPresetsInput:	//0x0502 : GetPresets
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzPresetsGet.token.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsGet.token);
							printf("\nEnter token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsGet.token);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GotoPresetInput:	//0x0580 : GotoPreset
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzGotoParam.token.stringData);
						printf("2. presetToken	= %s\n",deviceInfoList[iSelectIndex].ptzGotoParam.presetToken.stringData);
						printf("3. panSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzGotoParam.panSpeed);
						printf("4. tiltSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzGotoParam.tiltSpeed);
						printf("5. zoomSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzGotoParam.zoomSpeed);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzGotoParam.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzGotoParam.token);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzGotoParam.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzGotoParam.presetToken);
							break;
						case 3:
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.panSpeed = tempShortSetting;
							break;
						case 4:
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.tiltSpeed = tempShortSetting;
							break;
						case 5:
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.zoomSpeed = tempShortSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzGotoParam.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzGotoParam.token);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzGotoParam.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzGotoParam.presetToken);
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.panSpeed = tempShortSetting;
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.tiltSpeed = tempShortSetting;
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzGotoParam.zoomSpeed = tempShortSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_RemovePresetInput:	//0x0581 : RemovePreset
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzRemoveParam.token.stringData);
						printf("2. presetToken	= %s\n",deviceInfoList[iSelectIndex].ptzRemoveParam.presetToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRemoveParam.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRemoveParam.token);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRemoveParam.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet(byRefStringBuffer , Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRemoveParam.presetToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRemoveParam.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRemoveParam.token);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRemoveParam.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRemoveParam.presetToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetPresetInput:	//0x0582 : SetPreset
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzPresetsSet.token.stringData);
						printf("2. presetName	= %s\n",deviceInfoList[iSelectIndex].ptzPresetsSet.presetName.stringData);
						printf("3. presetToken	= %s\n",deviceInfoList[iSelectIndex].ptzPresetsSet.presetToken.stringData);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.token);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.presetName);
							printf("\nEnter PresetName :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.presetName);
							break;
						case 3:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.presetToken);
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.token);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.presetName);
							printf("\nEnter PresetName :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.presetName);
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzPresetsSet.presetToken);
							printf("\nEnter PresetToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzPresetsSet.presetToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_AbsoluteMoveInput:	//0x0583 : AbsoluteMove
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.token.stringData);
						printf("2. panPosition	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.panPosition);
						printf("3. tiltPosition	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltPosition);
						printf("4. zoomPosition	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomPosition);
						printf("5. panSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.panSpeed);
						printf("6. tiltSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltSpeed);
						printf("7. zoomSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomSpeed);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzAbsoluteMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzAbsoluteMove.token);
							break;
						case 2:
							printf("\nEnter panPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.panPosition = tempShortSetting;
							break;
						case 3:
							printf("\nEnter tiltPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltPosition = tempShortSetting;
							break;
						case 4:
							printf("\nEnter zoomPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomPosition = tempShortSetting;
							break;
						case 5:
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.panSpeed = tempShortSetting;
							break;
						case 6:
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltSpeed = tempShortSetting;
							break;
						case 7:
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomSpeed = tempShortSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzAbsoluteMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzAbsoluteMove.token);
							printf("\nEnter panPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.panPosition = tempShortSetting;
							printf("\nEnter tiltPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltPosition = tempShortSetting;
							printf("\nEnter zoomPosition :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomPosition = tempShortSetting;
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.panSpeed = tempShortSetting;
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.tiltSpeed = tempShortSetting;
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzAbsoluteMove.zoomSpeed = tempShortSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_RelativeMoveInput:	//0x0584 : RelativeMove
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token		= %s\n",deviceInfoList[iSelectIndex].ptzRelativeMove.token.stringData);
						printf("2. panTranslation	= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.panTranslation);
						printf("3. tiltTranslation	= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.tiltTranslation);
						printf("4. zoomTranslation	= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.zoomTranslation);
						printf("5. panSpeed		= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.panSpeed);
						printf("6. tiltSpeed		= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.tiltSpeed);
						printf("7. zoomSpeed		= %hd\n",deviceInfoList[iSelectIndex].ptzRelativeMove.zoomSpeed);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRelativeMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRelativeMove.token);
							break;
						case 2:
							printf("\nEnter panTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.panTranslation = tempShortSetting;
							break;
						case 3:
							printf("\nEnter tiltTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.tiltTranslation = tempShortSetting;
							break;
						case 4:
							printf("\nEnter zoomTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.zoomTranslation = tempShortSetting;
							break;
						case 5:
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.panSpeed = tempShortSetting;
							break;
						case 6:
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.tiltSpeed = tempShortSetting;
							break;
						case 7:
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.zoomSpeed = tempShortSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzRelativeMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzRelativeMove.token);
							printf("\nEnter panTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.panTranslation = tempShortSetting;
							printf("\nEnter tiltTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.tiltTranslation = tempShortSetting;
							printf("\nEnter zoomTranslation :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.zoomTranslation = tempShortSetting;
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.panSpeed = tempShortSetting;
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.tiltSpeed = tempShortSetting;
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzRelativeMove.zoomSpeed = tempShortSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_ContinuousMoveInput:	//0x0585 : ContinuousMove
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzContinuousMove.token.stringData);
						printf("2. panVelocity	= %hd\n",deviceInfoList[iSelectIndex].ptzContinuousMove.panVelocity);
						printf("3. tiltVelocity	= %hd\n",deviceInfoList[iSelectIndex].ptzContinuousMove.tiltVelocity);
						printf("4. zoomVelocity	= %hd\n",deviceInfoList[iSelectIndex].ptzContinuousMove.zoomVelocity);
						printf("5. timeout	= %lu\n",deviceInfoList[iSelectIndex].ptzContinuousMove.timeout);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzContinuousMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzContinuousMove.token);
							break;
						case 2:
							printf("\nEnter panVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.panVelocity = tempShortSetting;
							break;
						case 3:
							printf("\nEnter tiltVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.tiltVelocity = tempShortSetting;
							break;
						case 4:
							printf("\nEnter zoomVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.zoomVelocity = tempShortSetting;
							break;
						case 5:
							printf("\nEnter timeout :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.timeout = tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzContinuousMove.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzContinuousMove.token);
							printf("\nEnter panVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.panVelocity = tempShortSetting;
							printf("\nEnter tiltVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.tiltVelocity = tempShortSetting;
							printf("\nEnter zoomVelocity :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.zoomVelocity = tempShortSetting;
							printf("\nEnter timeout :  (us)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].ptzContinuousMove.timeout = tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetHomePositionInput:	//0x0586 : SetHomePosition
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. Token	= %s\n",deviceInfoList[iSelectIndex].ptzHomePosition.token.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzHomePosition.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzHomePosition.token);	
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GotoHomePositionInput:	//0x0587 : GotoHomePosition
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token	= %s\n",deviceInfoList[iSelectIndex].ptzHomePosition.token.stringData);
						printf("2. panSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzHomePosition.panSpeed);
						printf("3. tiltSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzHomePosition.tiltSpeed);
						printf("4. zoomSpeed	= %hd\n",deviceInfoList[iSelectIndex].ptzHomePosition.zoomSpeed);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzHomePosition.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzHomePosition.token);	
							break;
						case 2:
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.panSpeed = tempShortSetting;
							break;
						case 3:
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.tiltSpeed = tempShortSetting;
							break;
						case 4:
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.zoomSpeed = tempShortSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzHomePosition.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzHomePosition.token);	
							printf("\nEnter panSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.panSpeed = tempShortSetting;
							printf("\nEnter tiltSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.tiltSpeed = tempShortSetting;
							printf("\nEnter zoomSpeed :  (short)");
							SCANF("%hd", &tempShortSetting);
							deviceInfoList[iSelectIndex].ptzHomePosition.zoomSpeed = tempShortSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_PTZ_StopInput:	//0x0588 : Stop
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. token stringData	= %s\n",deviceInfoList[iSelectIndex].ptzStop.token.stringData);
						printf("2. panTiltZoom		= 0x%x\n",deviceInfoList[iSelectIndex].ptzStop.panTiltZoom);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzStop.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzStop.token);	
							break;
						case 2:
							printf("\nEnter panTiltZoom :  (ex: 0x03 for stop PanTilt and Zoom.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].ptzStop.panTiltZoom = (Byte)tempSetting;
							break;
						case 255:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].ptzStop.token);
							printf("\nEnter Token :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize, &deviceInfoList[iSelectIndex].ptzStop.token);	
							printf("\nEnter panTiltZoom :  (ex: 0x03 for stop PanTilt and Zoom.)");
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].ptzStop.panTiltZoom = (Byte)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//------------ PTZ Service-----------
					//------------ Video Analytics Service------------ 
				case CMD_GetSupportedRulesInput:	//0x0600 : GetSupportedRules
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetRulesInput:	//0x0601 : GetRules
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetSupportedAnalyticsModulesInput:	//0x0608 : GetSupportedAnalyticsModules
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_GetAnalyticsModulesInput:	//0x0609 : GetAnalyticsModules
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_CreateRuleInput:	//0x0680 : CreateRule

					if(deviceInfoList[iSelectIndex].totalRule.type == 0x10)
					{
						ptrRule_LineDetector = (Rule_LineDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_LineDetector->detectPolygon);
						User_memory_free(ptrRule_LineDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL; 
						ptrRule_LineDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x11)
					{
						ptrRule_FieldDetector = (Rule_FieldDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_FieldDetector->detectPolygon);
						User_memory_free(ptrRule_FieldDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_FieldDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x12)
					{
						ptrRule_MotionDetector = (Rule_MotionDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_MotionDetector->detectPolygon);
						User_memory_free(ptrRule_MotionDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_MotionDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x13)
					{
						ptrRule_Counting = (Rule_Counting*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_Counting->detectPolygon);
						User_memory_free(ptrRule_Counting);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_Counting = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x14)
					{
						ptrRule_CellMotion = (Rule_CellMotion*) deviceInfoList[iSelectIndex].totalRule.rule;
						check = Cmd_StringClear(&ptrRule_CellMotion->activeCells);
						User_memory_free(ptrRule_CellMotion);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_CellMotion = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x15)
					{
						ptrRule_LoiteringDetector = (Rule_LoiteringDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_LoiteringDetector->detectPolyLine);
						User_memory_free(ptrRule_LoiteringDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL; 
						ptrRule_LoiteringDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.ruleName);
					printf("\nEnter Name :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.ruleName);
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.ruleToken);
					printf("\nEnter RuleToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.ruleToken);
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.videoSrcToken);
					printf("\nSelect Rule Number :  \n(1)Line Detector \n(2)Field  Detector \n(3)Declarative Motion Detector \n(4)Counting Rule \n(5)Cell Motion Detector \n(6)Loitering Detector \n");
					SCANF("%lu", &tempSetting);
					if(tempSetting == 1)//Line Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x10;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_LineDetector*) User_memory_allocate(sizeof(Rule_LineDetector));
						ptrRule_LineDetector = (Rule_LineDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						printf("Enter Direction: (ex: 0: Do not care 1: Right to Left 2: Left to Right 3: Up to Down 4: Down to Up)");
						SCANF("%lu", &tempSetting);
						ptrRule_LineDetector->direction = (Word)tempSetting;

						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrRule_LineDetector->polygonListSize = (Byte)tempSetting;

						ptrRule_LineDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_LineDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_LineDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LineDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LineDetector->detectPolygon[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_LineDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_LineDetector = NULL;
					}else if(tempSetting == 2)//Field  Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x11;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_FieldDetector*) User_memory_allocate(sizeof(Rule_FieldDetector));
						ptrRule_FieldDetector = (Rule_FieldDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_FieldDetector->polygonListSize = (Byte)tempSetting;

						ptrRule_FieldDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_FieldDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_FieldDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_FieldDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_FieldDetector->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_FieldDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_FieldDetector = NULL;
					}else if(tempSetting == 3)//Declarative Motion Detector
					{						
						deviceInfoList[iSelectIndex].totalRule.type = 0x12;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_MotionDetector*) User_memory_allocate(sizeof(Rule_MotionDetector));
						ptrRule_MotionDetector = (Rule_MotionDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						ptrRule_MotionDetector->motionExpression = 0;
						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_MotionDetector->polygonListSize = (Byte)tempSetting;
						ptrRule_MotionDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_MotionDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_MotionDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_MotionDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_MotionDetector->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_MotionDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_MotionDetector = NULL;
					}else if(tempSetting == 4)//Counting Rule
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x13;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_Counting*) User_memory_allocate(sizeof(Rule_Counting));
						ptrRule_Counting = (Rule_Counting*) deviceInfoList[iSelectIndex].totalRule.rule;
						printf("Enter ReportTimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->reportTimeInterval = tempSetting;
						printf("Enter ResetTimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->resetTimeInterval = tempSetting;
						printf("Enter Direction: (ex: 0: Do not care 1: Right to Left 2: Left to Right 3: Up to Down 4: Down to Up)");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->direction = (Word)tempSetting;
						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_Counting->polygonListSize = (Byte)tempSetting;
						ptrRule_Counting->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_Counting->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_Counting->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_Counting->detectPolygon[i].polygon_x =(Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_Counting->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_Counting = NULL;
					}else if(tempSetting == 5)//Cell Motion Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x14;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_CellMotion*) User_memory_allocate(sizeof(Rule_CellMotion));
						ptrRule_CellMotion = (Rule_CellMotion*) deviceInfoList[iSelectIndex].totalRule.rule;
						printf("Enter MinCount :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->minCount = (Word)tempSetting;
						printf("Enter AlarmOnDelay :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->alarmOnDelay = tempSetting;
						printf("Enter AlarmOffDelay :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->alarmOffDelay = tempSetting;
						printf("Enter ActiveCellsSize :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->activeCellsSize = (Word)tempSetting;
						printf("Enter ActiveCells : (ex: 0007 for cells 1 to 3)" );
						SCANF("%lx", &tempSetting);
						dwActiveCellSize = 4;
						byActiveCellsBuffer[0] = (Byte)tempSetting;
						byActiveCellsBuffer[1] = (Byte)(tempSetting >> 8);
						byActiveCellsBuffer[2] = (Byte)(tempSetting >> 16);
						byActiveCellsBuffer[3] = (Byte)(tempSetting >> 24);

						if(byActiveCellsBuffer[3] == 0)
						{
							dwActiveCellSize --;
							if(byActiveCellsBuffer[2] == 0)
							{
								dwActiveCellSize --;
								if(byActiveCellsBuffer[1] == 0)
								{
									dwActiveCellSize --;
									if(byActiveCellsBuffer[0] == 0)
										dwActiveCellSize --;
								}
							}
						}
						check = Cmd_StringSet( byActiveCellsBuffer, dwActiveCellSize, &ptrRule_CellMotion->activeCells);
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->sensitivity = (Byte)tempSetting;
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;
						printf("Enter Layout Bounds x : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_x = (Word)tempSetting;
						printf("Enter Layout Bounds y : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_y = (Word)tempSetting;
						printf("Enter Layout Bounds width : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_width = (Word)tempSetting;
						printf("Enter Layout Bounds height : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_height = (Word)tempSetting;
						printf("Enter Layout Columns : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutColumns = (Byte)tempSetting;
						printf("Enter Layout Rows : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutRows = (Byte)tempSetting;
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;

						ptrRule_CellMotion = NULL;
					}else if(tempSetting == 6)//Loitering Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x15;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_LoiteringDetector*) User_memory_allocate(sizeof(Rule_LoiteringDetector));
						ptrRule_LoiteringDetector = (Rule_LoiteringDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						printf("Enter TimeThreshold: ");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->timeThreshold = (Dword)tempSetting;

						printf("Enter isPeriodicNotify: (ex: 0: Off, 1:On)");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->isPeriodicNotify = (Byte)tempSetting;

						printf("Enter TimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->timeInterval = (Dword)tempSetting;


						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrRule_LoiteringDetector->polylineListSize = (Byte)tempSetting;

						ptrRule_LoiteringDetector->detectPolyLine = (DetectPolygon*) User_memory_allocate(ptrRule_LoiteringDetector->polylineListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_LoiteringDetector->polylineListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LoiteringDetector->detectPolyLine[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LoiteringDetector->detectPolyLine[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_LoiteringDetector = NULL;
					}else
					{
						printf("UnSupported Setting!!!\n");
						continue;
					}
					deviceInfoList[iSelectIndex].totalRule.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_ModifyRuleInput:	//0x0681 : ModifyRule
					if(deviceInfoList[iSelectIndex].totalRule.type == 0x10)
					{
						ptrRule_LineDetector = (Rule_LineDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_LineDetector->detectPolygon);
						User_memory_free(ptrRule_LineDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL; 
						ptrRule_LineDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x11)
					{
						ptrRule_FieldDetector = (Rule_FieldDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_FieldDetector->detectPolygon);
						User_memory_free(ptrRule_FieldDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_FieldDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x12)
					{
						ptrRule_MotionDetector = (Rule_MotionDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_MotionDetector->detectPolygon);
						User_memory_free(ptrRule_MotionDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_MotionDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x13)
					{
						ptrRule_Counting = (Rule_Counting*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_Counting->detectPolygon);
						User_memory_free(ptrRule_Counting);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_Counting = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x14)
					{
						ptrRule_CellMotion = (Rule_CellMotion*) deviceInfoList[iSelectIndex].totalRule.rule;
						check = Cmd_StringClear(&ptrRule_CellMotion->activeCells);
						User_memory_free(ptrRule_CellMotion);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL;
						ptrRule_CellMotion = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}else if(deviceInfoList[iSelectIndex].totalRule.type == 0x15)
					{
						ptrRule_LoiteringDetector = (Rule_LoiteringDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						User_memory_free(ptrRule_LoiteringDetector->detectPolyLine);
						User_memory_free(ptrRule_LoiteringDetector);
						deviceInfoList[iSelectIndex].totalRule.rule = NULL; 
						ptrRule_LoiteringDetector = NULL;
						deviceInfoList[iSelectIndex].totalRule.type = 0x00;
					}

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.ruleName);
					printf("\nEnter Name :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.ruleName);
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.ruleToken);
					printf("\nEnter RuleToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.ruleToken);
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalRule.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalRule.videoSrcToken);
					printf("\nSelect Rule Number :  \n(1)Line Detector \n(2)Field  Detector \n(3)Declarative Motion Detector \n(4)Counting Rule \n(5)Cell Motion Detector \n(6)Loitering Detector \n");
					SCANF("%lu", &tempSetting);
					if(tempSetting == 1)//Line Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x10;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_LineDetector*) User_memory_allocate(sizeof(Rule_LineDetector));
						ptrRule_LineDetector = (Rule_LineDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						printf("Enter Direction: (ex: 0: Do not care 1: Right to Left 2: Left to Right 3: Up to Down 4: Down to Up)");
						SCANF("%lu", &tempSetting);
						ptrRule_LineDetector->direction = (Word)tempSetting;

						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrRule_LineDetector->polygonListSize = (Byte)tempSetting;

						ptrRule_LineDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_LineDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_LineDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LineDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LineDetector->detectPolygon[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_LineDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_LineDetector = NULL;
					}else if(tempSetting == 2)//Field  Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x11;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_FieldDetector*) User_memory_allocate(sizeof(Rule_FieldDetector));
						ptrRule_FieldDetector = (Rule_FieldDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_FieldDetector->polygonListSize = (Byte)tempSetting;

						ptrRule_FieldDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_FieldDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_FieldDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_FieldDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_FieldDetector->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_FieldDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;
						ptrRule_FieldDetector = NULL;
					}else if(tempSetting == 3)//Declarative Motion Detector
					{						
						deviceInfoList[iSelectIndex].totalRule.type = 0x12;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_MotionDetector*) User_memory_allocate(sizeof(Rule_MotionDetector));
						ptrRule_MotionDetector = (Rule_MotionDetector*) deviceInfoList[iSelectIndex].totalRule.rule;
						ptrRule_MotionDetector->motionExpression = 0;
						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_MotionDetector->polygonListSize = (Byte)tempSetting;
						ptrRule_MotionDetector->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_MotionDetector->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_MotionDetector->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_MotionDetector->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_MotionDetector->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_MotionDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;
						ptrRule_MotionDetector = NULL;
					}else if(tempSetting == 4)//Counting Rule
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x13;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_Counting*) User_memory_allocate(sizeof(Rule_Counting));
						ptrRule_Counting = (Rule_Counting*) deviceInfoList[iSelectIndex].totalRule.rule;
						printf("Enter ReportTimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->reportTimeInterval = tempSetting;
						printf("Enter ResetTimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->resetTimeInterval = tempSetting;
						printf("Enter Direction: (ex: 0: Do not care 1: Right to Left 2: Left to Right 3: Up to Down 4: Down to Up)");
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->direction = (Word)tempSetting;
						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);
						}while(tempSetting < 2);
						ptrRule_Counting->polygonListSize = (Byte)tempSetting;
						ptrRule_Counting->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrRule_Counting->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_Counting->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_Counting->detectPolygon[i].polygon_x =(Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_Counting->detectPolygon[i].polygon_y = (Word)tempSetting ;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_Counting->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;
						ptrRule_Counting = NULL;
					}else if(tempSetting == 5)//Cell Motion Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x14;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_CellMotion*) User_memory_allocate(sizeof(Rule_CellMotion));
						ptrRule_CellMotion = (Rule_CellMotion*) deviceInfoList[iSelectIndex].totalRule.rule;
						printf("Enter MinCount :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->minCount = (Word)tempSetting;
						printf("Enter AlarmOnDelay :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->alarmOnDelay = tempSetting;
						printf("Enter AlarmOffDelay :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->alarmOffDelay = tempSetting;
						printf("Enter ActiveCellsSize :" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->activeCellsSize = (Word)tempSetting;
						printf("Enter ActiveCells : (ex: 0007 for cells 1 to 3)" );
						SCANF("%lx", &tempSetting);
						dwActiveCellSize = 4;
						byActiveCellsBuffer[0] = (Byte)tempSetting;
						byActiveCellsBuffer[1] = (Byte)(tempSetting >> 8);
						byActiveCellsBuffer[2] = (Byte)(tempSetting >> 16);
						byActiveCellsBuffer[3] = (Byte)(tempSetting >> 24);

						if(byActiveCellsBuffer[3] == 0)
						{
							dwActiveCellSize --;
							if(byActiveCellsBuffer[2] == 0)
							{
								dwActiveCellSize --;
								if(byActiveCellsBuffer[1] == 0)
								{
									dwActiveCellSize --;
									if(byActiveCellsBuffer[0] == 0)
										dwActiveCellSize --;
								}
							}
						}
						check = Cmd_StringSet( byActiveCellsBuffer, dwActiveCellSize, &ptrRule_CellMotion->activeCells);

						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->sensitivity = (Byte)tempSetting;
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;
						printf("Enter Layout Bounds x : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_x = (Word)tempSetting;
						printf("Enter Layout Bounds y : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_y = (Word)tempSetting;
						printf("Enter Layout Bounds width : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_width = (Word)tempSetting;
						printf("Enter Layout Bounds height : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutBounds_height = (Word)tempSetting;
						printf("Enter Layout Columns : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutColumns = (Byte)tempSetting;
						printf("Enter Layout Rows : " );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->layoutRows = (Byte)tempSetting;
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_CellMotion->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						ptrRule_CellMotion = NULL;
					}else if(tempSetting == 6)//Loitering Detector
					{
						deviceInfoList[iSelectIndex].totalRule.type = 0x15;
						deviceInfoList[iSelectIndex].totalRule.rule = (Rule_LoiteringDetector*) User_memory_allocate(sizeof(Rule_LoiteringDetector));
						ptrRule_LoiteringDetector = (Rule_LoiteringDetector*) deviceInfoList[iSelectIndex].totalRule.rule;

						printf("Enter TimeThreshold: ");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->timeThreshold = (Dword)tempSetting;

						printf("Enter isPeriodicNotify: (ex: 0: Off, 1:On)");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->isPeriodicNotify = (Byte)tempSetting;

						printf("Enter TimeInterval: ");
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->timeInterval = (Dword)tempSetting;


						do{
							printf("Enter Polyline List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrRule_LoiteringDetector->polylineListSize = (Byte)tempSetting;

						ptrRule_LoiteringDetector->detectPolyLine = (DetectPolygon*) User_memory_allocate(ptrRule_LoiteringDetector->polylineListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrRule_LoiteringDetector->polylineListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LoiteringDetector->detectPolyLine[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrRule_LoiteringDetector->detectPolyLine[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter Metadata Stream (ex: 0: Off, 1: On):" );
						SCANF("%lu", &tempSetting);
						ptrRule_LoiteringDetector->metadataStreamSwitch = (Byte)tempSetting;
						printf("Enter Threshold : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.threshold = (Byte)tempSetting;
						printf("Enter Sensitivity : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						deviceInfoList[iSelectIndex].totalRule.motionSensitivity = (Byte)tempSetting;

						ptrRule_LoiteringDetector = NULL;
					}else
					{
						printf("UnSupported Setting!!!\n");
						continue;
					}
					deviceInfoList[iSelectIndex].totalRule.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_DeleteRuleInput:	//0x0682 : DeleteRule
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. ruleToken		= %s\n",deviceInfoList[iSelectIndex].deleteRule.ruleToken.stringData);
						printf("2. VideoSourceToken	= %s\n",deviceInfoList[iSelectIndex].deleteRule.VideoSourceToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].deleteRule.ruleToken);
							printf("\nEnter RuleToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].deleteRule.ruleToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].deleteRule.VideoSourceToken);
							printf("\nEnter VideoSourceToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].deleteRule.VideoSourceToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].deleteRule.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_CreateAnalyticsModuleInput:	//0x0688 : CreateAnalyticsModule

					if(deviceInfoList[iSelectIndex].totalModule.type == 0x30)
					{
						ptrAnalyticsModule_ObjectTracker = (AnalyticsModule_ObjectTracker*) deviceInfoList[iSelectIndex].totalModule.module;
						User_memory_free(ptrAnalyticsModule_ObjectTracker->detectPolygon);
						User_memory_free(ptrAnalyticsModule_ObjectTracker);
						deviceInfoList[iSelectIndex].totalModule.module = NULL; 
						ptrAnalyticsModule_ObjectTracker = NULL;
						deviceInfoList[iSelectIndex].totalModule.type = 0x00;
					}

					
					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.moduleName);
					printf("\nEnter Name :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.moduleName);

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.videoSrcToken);


					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.moduleToken);
					printf("\nEnter ModuleToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.moduleToken);
					
					printf("\nSelect Module Number :  \n(1)Object Tracker \n");
					SCANF("%lu", &tempSetting);
					if(tempSetting == 1)//Object Tracker
					{
						deviceInfoList[iSelectIndex].totalModule.type = 0x30;
						deviceInfoList[iSelectIndex].totalModule.module = (AnalyticsModule_ObjectTracker*) User_memory_allocate(sizeof(AnalyticsModule_ObjectTracker));
						ptrAnalyticsModule_ObjectTracker = (AnalyticsModule_ObjectTracker*) deviceInfoList[iSelectIndex].totalModule.module;

						printf("Enter ObjectID: ");
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectID = (Dword)tempSetting;

						printf("Enter ObjectTracking: (ex: 0: Off, 1: On)");
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectTracking = (Byte)tempSetting;


						do{
							printf("Enter Polygon List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrAnalyticsModule_ObjectTracker->polygonListSize = (Byte)tempSetting;

						ptrAnalyticsModule_ObjectTracker->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrAnalyticsModule_ObjectTracker->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrAnalyticsModule_ObjectTracker->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrAnalyticsModule_ObjectTracker->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrAnalyticsModule_ObjectTracker->detectPolygon[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter ObjectClass (ex: 0: Undefined, 1: Animal, 2: Face, 3: Human, 4: Vehicle, 5: LicensePlate, 6: Group):" );
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectClass = (Byte)tempSetting;
						printf("Enter Likelihood : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->Likelihood = (Byte)tempSetting;
						ptrAnalyticsModule_ObjectTracker->commandLength = 10 + 4*ptrAnalyticsModule_ObjectTracker->polygonListSize;

						ptrAnalyticsModule_ObjectTracker = NULL;
					}else
					{
						printf("UnSupported Setting!!!\n");
						continue;
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_ModifyAnalyticsModuleInput:	//0x0689 : ModifyAnalyticsModule
					if(deviceInfoList[iSelectIndex].totalModule.type == 0x30)
					{
						ptrAnalyticsModule_ObjectTracker = (AnalyticsModule_ObjectTracker*) deviceInfoList[iSelectIndex].totalModule.module;
						User_memory_free(ptrAnalyticsModule_ObjectTracker->detectPolygon);
						User_memory_free(ptrAnalyticsModule_ObjectTracker);
						deviceInfoList[iSelectIndex].totalModule.module = NULL; 
						ptrAnalyticsModule_ObjectTracker = NULL;
						deviceInfoList[iSelectIndex].totalModule.type = 0x00;
					}

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.videoSrcToken);
					printf("\nEnter VideoSourceToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.videoSrcToken);

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.moduleName);
					printf("\nEnter Name :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.moduleName);

					check = Cmd_StringClear(&deviceInfoList[iSelectIndex].totalModule.moduleToken);
					printf("\nEnter ModuleToken :  ");
					Scanf_String(byRefStringBuffer, Ref_BufferSize);
					check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].totalModule.moduleToken);
					
					printf("\nSelect Module Number :  \n(1)Object Tracker \n");
					SCANF("%lu", &tempSetting);
					if(tempSetting == 1)//Object Tracker
					{
						deviceInfoList[iSelectIndex].totalModule.type = 0x30;
						deviceInfoList[iSelectIndex].totalModule.module = (AnalyticsModule_ObjectTracker*) User_memory_allocate(sizeof(AnalyticsModule_ObjectTracker));
						ptrAnalyticsModule_ObjectTracker = (AnalyticsModule_ObjectTracker*) deviceInfoList[iSelectIndex].totalModule.module;

						printf("Enter ObjectID: ");
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectID = (Dword)tempSetting;

						printf("Enter ObjectTracking: (ex: 0: Off, 1: On)");
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectTracking = (Byte)tempSetting;


						do{
							printf("Enter Polygon List Size: ( > 2)");
							SCANF("%lu", &tempSetting);

						}while(tempSetting < 2);
						ptrAnalyticsModule_ObjectTracker->polygonListSize = (Byte)tempSetting;

						ptrAnalyticsModule_ObjectTracker->detectPolygon = (DetectPolygon*) User_memory_allocate(ptrAnalyticsModule_ObjectTracker->polygonListSize * sizeof(DetectPolygon));
						for(i = 0; i < ptrAnalyticsModule_ObjectTracker->polygonListSize; i ++)
						{
							printf("Enter x[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrAnalyticsModule_ObjectTracker->detectPolygon[i].polygon_x = (Word)tempSetting ;
							printf("Enter y[%hd] : ",i );
							SCANF("%lu", &tempSetting);
							ptrAnalyticsModule_ObjectTracker->detectPolygon[i].polygon_y = (Word)tempSetting;
						}
						printf("Enter ObjectClass (ex: 0: Undefined, 1: Animal, 2: Face, 3: Human, 4: Vehicle, 5: LicensePlate, 6: Group):" );
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->objectClass = (Byte)tempSetting;
						printf("Enter Likelihood : (ex: 0~100)" );
						SCANF("%lu", &tempSetting);
						ptrAnalyticsModule_ObjectTracker->Likelihood = (Byte)tempSetting;

						ptrAnalyticsModule_ObjectTracker->commandLength = 10 + 4*ptrAnalyticsModule_ObjectTracker->polygonListSize;

						ptrAnalyticsModule_ObjectTracker = NULL;
					}else
					{
						printf("UnSupported Setting!!!\n");
						continue;
					}

					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_DeleteAnalyticsModuleInput:	//0x068A : DeleteAnalyticsModule
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1. videoSrcToken	= %s\n",deviceInfoList[iSelectIndex].deleteModule.VideoSourceToken.stringData);
						printf("2. moduleToken		= %s\n",deviceInfoList[iSelectIndex].deleteModule.moduleToken.stringData);
						printf("------------Default Value List------------\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].deleteModule.VideoSourceToken);
							printf("\nEnter videoSrcToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].deleteModule.VideoSourceToken);
							break;
						case 2:
							check = Cmd_StringClear(&deviceInfoList[iSelectIndex].deleteModule.moduleToken);
							printf("\nEnter ModuleToken :  ");
							Scanf_String(byRefStringBuffer, Ref_BufferSize);
							check = Cmd_StringSet( byRefStringBuffer, Ref_BufferSize,  &deviceInfoList[iSelectIndex].deleteModule.moduleToken);
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//------------ Video Analytics Service------------ 
					//------------ Metadata Stream Service Service------------ 
				case CMD_GetMetadataSettingsInput:	//0xE001 : GetMetadataSettings
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
				case CMD_SetMetadataSettingsInput:	//0xE081 : SetMetadataSettings
					printf("Send GetMetadataSettings For Getting Information.....\n");
					get_Command = CMD_GetMetadataSettingsInput;
					error = RC_Shell_Send(&shell, iSelectIndex, get_Command, False, True, WaitTime, funReplyCallBack, funFailCallBack);
					if(!error)
					{

					}else
					{
						printf("Can Not GetMetadataSettings Before Set!\n");
						continue;
					}
					while(IsCompleted == 0)
					{
						printf("\n------------Default Value List------------\n");
						printf("1.  deviceInfoEnable		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.deviceInfoEnable);
						printf("2.  deviceInfoPeriod		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.deviceInfoPeriod);
						printf("3.  streamInfoEnable		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.streamInfoEnable);
						printf("4.  streamInfoPeriod		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.streamInfoPeriod);
						printf("5.  motionDetectorEnable	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.motionDetectorEnable);
						printf("6.  motionDetectorPeriod	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.motionDetectorPeriod);
						printf("7.  serialDataEnable		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.serialDataEnable);
						printf("8.  serialDataPeriod		= %u\n",deviceInfoList[iSelectIndex].metadataSettings.serialDataPeriod);
						printf("9.  digitalInputEventEnable	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventEnable);
						printf("10. digitalInputEventPeriod	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventPeriod);
						printf("11. relayOutputEventEnable	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventEnable);
						printf("12. relayOutputEventPeriod	= %u\n",deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventPeriod);
						printf("------------Default Value List------------\n");
						printf("255. Set all parameters\n");
						printf("0. Send command\n");
						printf("\nInput index for setting parameter: ");
						SCANF("%d", &selectNum);
						switch(selectNum)
						{
						case 0:
							IsCompleted = 1;
							break;
						case 1:
							printf("Enter Device Information Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.deviceInfoEnable = (Byte)tempSetting;
							break;
						case 2:
							printf("Enter Device Information Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.deviceInfoPeriod = (Word)tempSetting;
							break;
						case 3:
							printf("Enter Stream Information Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.streamInfoEnable = (Byte)tempSetting;
							break;
						case 4:
							printf("Enter Stream Information Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.streamInfoPeriod = (Word)tempSetting;
							break;
						case 5:
							printf("Enter Motion Detector Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.motionDetectorEnable = (Byte)tempSetting;
							break;
						case 6:
							printf("Enter Motion Detector Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.motionDetectorPeriod = (Word)tempSetting;
							break;
						case 7:
							printf("Enter SerialData Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.serialDataEnable = (Byte)tempSetting;
							break;
						case 8:
							printf("Enter SerialData Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.serialDataPeriod = (Word)tempSetting;
							break;
						case 9:
							printf("Enter DigitalInputEvent Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventEnable = (Byte)tempSetting;
							break;
						case 10:
							printf("Enter DigitalInputEvent Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventPeriod = (Word)tempSetting;
							break;
						case 11:
							printf("Enter RelayOutputEvent Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventEnable = (Byte)tempSetting;
							break;
						case 12:
							printf("Enter RelayOutputEvent Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventPeriod = (Word)tempSetting;
							break;
						case 255:
							printf("Enter Device Information Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.deviceInfoEnable = (Byte)tempSetting;
							printf("Enter Device Information Metadata Period : (ns) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.deviceInfoPeriod = (Word)tempSetting;
							printf("Enter Stream Information Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.streamInfoEnable = (Byte)tempSetting;
							printf("Enter Stream Information Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.streamInfoPeriod = (Word)tempSetting;
							printf("Enter Motion Detector Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.motionDetectorEnable = (Byte)tempSetting;
							printf("Enter Motion Detector Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.motionDetectorPeriod = (Word)tempSetting;
							printf("Enter SerialData Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.serialDataEnable = (Byte)tempSetting;
							printf("Enter SerialData Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.serialDataPeriod = (Word)tempSetting;
							printf("Enter DigitalInputEvent Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventEnable = (Byte)tempSetting;
							printf("Enter DigitalInputEvent Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.digitalInputEventPeriod = (Word)tempSetting;
							printf("Enter RelayOutputEvent Metadata Enable : (ex: 0: OFF, 1: ON) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventEnable = (Byte)tempSetting;
							printf("Enter RelayOutputEvent Metadata Period : (ms) " );
							SCANF("%lu", &tempSetting);
							deviceInfoList[iSelectIndex].metadataSettings.relayOutputEventPeriod = (Word)tempSetting;
							break;
						default:
							printf("Unsupported Setting!\n");
							continue;
						}
					}
					deviceInfoList[iSelectIndex].metadataSettings.extensionFlag = 1;// Must 1 for extension
					error = RC_Shell_Send(&shell, iSelectIndex, command, bIsBroadCast, bIsBlock, WaitTime, funReplyCallBack, funFailCallBack);
					break;
					//------------ Metadata Stream Service Service------------ 
				case 0xFFFF:
					error = 0xFFFF;
					IsHostRun = False;
					break;

				default:
					error = ReturnChannelError_CMD_NOT_SUPPORTED;
					printf("Error CMD_NOT_SUPPORTED\n");
					break;
		}



		// --------------------------------------------------------------------------- //
		//                                                      Send Client Reboot Command                                                     //
		// --------------------------------------------------------------------------- //
		//if (error == ModulatorError_NO_ERROR) 
		//{
		//	printf("\nWait for Client Device Reply ...\n\n");

		//	if((((command & 0x80)>>7) == 0x1) && (command != CMD_SystemRebootInput) && (command != CMD_SetSystemFactoryDefaultBroadcastInput))
		//	{
		//		printf("\nApply & Save all setting to Camera ?(Ex: Yes:1  No: 0) ");
		//		SCANF("%d", &IsUserReboot);
		//		if(IsUserReboot)
		//		{
		//			printf("\nSend Reboot AP Command.....\n");
		//			deviceInfoList[iSelectIndex].systemReboot.rebootType = 1;		//AP Reboot

		//			error = RC_Shell_Send(&shell, iSelectIndex, CMD_SystemRebootInput, False, bIsBlock, 30, funReplyCallBack, funFailCallBack);
		//		}
		//	}

		//}
		// ---------- Command Error Message ---------- //
		if(error)
		{
			printf("\nSend command error = %lx\n",error);
			error = ModulatorError_NO_ERROR;
		}

	}


exit:	
	// --------------------------------------------------------------- //
	//                                      Return Channel Structure DeInitialize                                    //
	// --------------------------------------------------------------- //
	if(bIsOpenPort)
	{
		RC_Shell_Stop_RcvThread(&shell);
		RC_Shell_UnInit(&shell);
	}
	// --------------------------------------------------------------- //
	//                              Close handle for dealing with Metadata information                       //
	// --------------------------------------------------------------- //
#if RC_POP_WINDOW_PRINT
#ifdef __linux__   /* Linux */
	if(Terminal_Handle)
	{
		char tempString[50] = {0};

		fclose(Terminal_Handle);

		snprintf(tempString, sizeof(tempString), "skill -KILL -v /pts/%d", NewTerminal_DevNum);
		system(tempString);
	}
#else
	SendMessage(Metadata_window, WM_DESTROY, 0, 0);
#endif
#endif
	// --------------------------------------------------------------- //
	//                                      Close Return Channel Bus COM port                                     //
	// --------------------------------------------------------------- //
	if(RxPort == TxPort){
		User_returnChannelBusClose(RxPort);
	}else{

		User_returnChannelBusClose(RxPort);
		User_returnChannelBusClose(TxPort);
	}


	return 0;
}

