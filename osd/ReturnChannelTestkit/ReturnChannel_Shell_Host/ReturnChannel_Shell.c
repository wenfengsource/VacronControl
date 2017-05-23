#include "ReturnChannel_Shell.h"

THREAD RC_Shell_RecvThread(void* data)
{
	RcCmd *rchead,*rccurrent;
	Byte replyBuffer[188];
	int rxlen;
	Dword error = ModulatorError_NO_ERROR;
	Word tmpRXDeviceID = 0;
	Word tmpTXDeviceID = 0;
	Word tmpCommand = 0;
	int IsFind = 0;
	RCShell* pShell;
	int i;
	int FindDeviceInfoIndex = -1;

#ifdef __linux__   /* Linux */
	pthread_detach(pthread_self());
#endif

	pShell = (RCShell*)data;

	pShell->bIsRcvFinish = False;
	rchead = (RcCmd *)User_memory_allocate(sizeof(RcCmd));
	initRcCmdInfo(rchead);
	RC_Reset_DebugInfo(&pShell->debugInfo);
	rccurrent = rchead;
	
	while(pShell->bIsRun)
	{
		User_returnChannelBusRx(pShell->iRxPort, 184,replyBuffer,&rxlen);

		if(rxlen <= 0)
		{
#ifdef __linux__   /* Linux */
			usleep(10000);
#else
			Sleep(10);
#endif

		}else
		{
			pShell->debugInfo.totalGetBufferCount ++;
			if(replyBuffer[0] !=  '#')
				pShell->debugInfo.leadingTagErrorCount ++;
#if _DEBUG_RC 
			printf("\nReceive Client CMD rxlen = %d\n",rxlen);
#endif
			rccurrent = RcCmdCollect(rccurrent, replyBuffer, rxlen);
#if _DEBUG_RC 
			RcPrint(rchead);
#endif


			if(( rchead->cmdState == RC_CMD_STATE_FINISH) || ( rchead->cmdState == RC_CMD_STATE_OVERFLOW))
			{
				pShell->debugInfo.totalPKTCount ++;

				if(pShell->bIsBroadcast)//Register Rx/TxID when broadcast 
				{
					tmpRXDeviceID = ((rchead->buffer_Cmd[1])<<8) | rchead->buffer_Cmd[2];
					tmpTXDeviceID = (rchead->buffer_Cmd[3]<<8) | rchead->buffer_Cmd[4];
					tmpCommand = (rchead->buffer_Cmd[17]<<8) | rchead->buffer_Cmd[18];

					for(i = 0; i < pShell->iListSize; i ++)	//search List for RxID/TxID
					{
						if(	(pShell->rgDeviceInfo[i].device.clientTxDeviceID == tmpTXDeviceID) && (pShell->rgDeviceInfo[i].device.hostRxDeviceID == tmpRXDeviceID) )
							IsFind = 1;
					}
					if(IsFind == 0)	//Can Not found RxID/TxID
					{
						RC_Init_RCHeadInfo(&pShell->rgDeviceInfo[pShell->iListSize].device, tmpRXDeviceID, tmpTXDeviceID, 0xFFFF);
						RC_Init_TSHeadInfo(&pShell->rgDeviceInfo[pShell->iListSize].device, RETURN_CHANNEL_PID, 0, False);
						RC_Init_CmdConfig(&pShell->rgDeviceInfo[pShell->iListSize].cmdConfig, 0, CMD_GetTxDeviceAddressIDOutput);
						RC_Init_PortConfig(&pShell->rgDeviceInfo[pShell->iListSize].device, pShell->iRxPort, pShell->iTxPort);
						RC_Init_CmdSendConfig(&pShell->rgDeviceInfo[pShell->iListSize].cmdSendConfig);
						

						User_Host_init(&pShell->rgDeviceInfo[pShell->iListSize]);
						pShell->rgDeviceInfo[pShell->iListSize].callBackSet.ReplyCallBack = pShell->cbReply;
						User_askUserSecurity (&pShell->rgDeviceInfo[pShell->iListSize].security);
						pShell->iListSize ++;

					}else
					{
						IsFind = 0;
					}
				}


				if(pShell->iListSize >0)
				{
					//-----------------Mutex Lock For clear RCHostInfo List-----------------
					RC_Shell_mutex_lock(&pShell->cleanList_Mutex);  

					for(i = 0; i < pShell->iListSize; i ++)
					{
						error = RcHeadCheck (rchead->buffer_Cmd, &pShell->rgDeviceInfo[i].device, pShell->rgDeviceInfo[i].cmdConfig.cmd, &pShell->debugInfo, 0, 1);		//	//check RC head and assign to deviceInfo command buffer
						if(error == ModulatorError_NO_ERROR)
						{
							FindDeviceInfoIndex = i;
							break;
						}else
						{
							FindDeviceInfoIndex = -1;
						}
					}
					if(error != ModulatorError_NO_ERROR)
					{
						printf("!!! Return Channel error syntex : %lx !!!\n",error);
						error = ModulatorError_NO_ERROR;    //error init
					}else
					{
						if(FindDeviceInfoIndex >= 0)
						{

							error = parseReadyCmd_Host(&pShell->rgDeviceInfo[FindDeviceInfoIndex]);		//Parse command

							if((error != ReturnChannelError_RerurnCode_Error) &&(error != ModulatorError_NO_ERROR))
							{
								printf("!!! Return Channel Parse error : %lx !!!\n",error);
								error = ModulatorError_NO_ERROR;    //error init
							}
							FindDeviceInfoIndex = - 1;
						}
					}
					//-----------------Mutex UnLock For clear RCHostInfo List-----------------
					RC_Shell_mutex_unlock(&pShell->cleanList_Mutex);  
				}



				rchead = popRcCmd(rchead);	//pop and clear first one command
			}


		}
	}
	releaseRcCmdInfo(rchead);
	pShell->bIsRcvFinish = True; 
	return 0;

}

THREAD RC_Shell_WaitTimeOut(void* data)
{
	RCShell_ThreadParam* pThreadParam;
	RCShell_ThreadParam threadInfo;

	pThreadParam = (RCShell_ThreadParam*)data;


	threadInfo.pRCShell = pThreadParam->pRCShell;
	threadInfo.iSelectIndex = pThreadParam->iSelectIndex;
	threadInfo.wCommand = pThreadParam->wCommand;
	threadInfo.iWaitTime = pThreadParam->iWaitTime;
	threadInfo.cbFail = pThreadParam->cbFail;
	pThreadParam->bIsSet = True;

	RC_Shell_CheckTimeOut(threadInfo.pRCShell, threadInfo.iSelectIndex, threadInfo.wCommand, threadInfo.iWaitTime, threadInfo.cbFail);

	return 0;
}


void RC_Shell_BroadCastWait(
    IN RCShell*			    pRCShell,
	IN RCHostInfo*			pDeviceInfo,
	IN int						iWaitTime
){
	time_t nowTime, startTime,diffTime;
	int i;
	startTime = time(NULL);

	while(1)
	{
		nowTime = time(NULL);
		diffTime = nowTime - startTime;
		if(diffTime > iWaitTime)
		{
			printf("wait for %d sec!!\n",iWaitTime);
			pDeviceInfo->cmdConfig.IsTimeOut = 1;
			pRCShell->bIsBroadcast = False;
			pDeviceInfo->cmdConfig.IsParseFinish = 0;
			for(i = 0; i < pRCShell->iListSize; i ++)
			{
				User_memory_free(pRCShell->rgDeviceInfo[i].cmdConfig.cmd->buffer_Cmd);
				pRCShell->rgDeviceInfo[i].cmdConfig.IsTimeOut = 1;
				pRCShell->rgDeviceInfo[i].cmdConfig.IsParseFinish = 0;
				pRCShell->rgDeviceInfo[i].cmdConfig.ExpectCmd = CMD_MetadataStreamOutput;

				initCmdInfo(pRCShell->rgDeviceInfo[i].cmdConfig.cmd);
			}
			break;
		}
	}//while end
}

Dword RC_Shell_CheckTimeOut(
	IN RCShell*			    pRCShell,
	int								iSelectIndex,
	Word						wCommand,
	int								iWaitTime,
	void							(*cbFail)(void*, Word)
){
	time_t nowTime, startTime,diffTime;
	CmdConfig* pCmdConfig = &pRCShell->rgDeviceInfo[iSelectIndex].cmdConfig;
	startTime = time(NULL);
	while(1)
	{
		nowTime = time(NULL);
		diffTime = nowTime - startTime;
		if(pCmdConfig->IsParseFinish == 1)
		{
			//-----------------Mutex Lock For clear RCHostInfo List-----------------
			RC_Shell_mutex_lock(&pRCShell->cleanList_Mutex);  

			User_memory_free(pCmdConfig->cmd->buffer_Cmd);
			initCmdInfo(pCmdConfig->cmd);

			//-----------------Mutex UnLock For clear RCHostInfo List-----------------
			RC_Shell_mutex_unlock(&pRCShell->cleanList_Mutex);  

			if(pCmdConfig->IsValid == 1)
			{
				pCmdConfig->IsValid = 0;
				pCmdConfig->IsParseFinish = 0;
				pCmdConfig->IsTimeOut = 1;

				printf("Rcv Command OK!!\n\n");
				return ModulatorError_NO_ERROR;
			}else
			{
				pCmdConfig->IsValid = 0;
				pCmdConfig->IsParseFinish = 0;
				pCmdConfig->IsTimeOut = 1;
				if(cbFail == NULL)
				{
					printf("Rcv Command Invalid But Can Not Found Fail void*!!\n\n");
					return ReturnChannelError_Shell_CALLBACK_NOT_FOUND;
				}
				else
					cbFail( (void *)&pRCShell->rgDeviceInfo[iSelectIndex], wCommand);
				printf("Rcv Command Invalid!!\n\n");
				return ReturnChannelError_Shell_CMD_InValid;
			}
		}
		if(diffTime > iWaitTime)
		{
			//-----------------Mutex Lock For clear RCHostInfo List-----------------
			RC_Shell_mutex_lock(&pRCShell->cleanList_Mutex);  

			User_memory_free(pCmdConfig->cmd->buffer_Cmd);
			initCmdInfo(pCmdConfig->cmd);

			//-----------------Mutex UnLock For clear RCHostInfo List-----------------
			RC_Shell_mutex_unlock(&pRCShell->cleanList_Mutex);  

			pCmdConfig->IsValid = 0;
			pCmdConfig->IsParseFinish = 0;
			pCmdConfig->IsTimeOut = 1;

			if(cbFail == NULL)
			{
				printf("Wait Reply for timeout  %d sec But Can Not Found TimeOut void*!!\n", iWaitTime);
				return ReturnChannelError_Shell_CALLBACK_NOT_FOUND;
			}
			else
				cbFail( (void *)&pRCShell->rgDeviceInfo[iSelectIndex], wCommand);
			printf("Wait Reply for timeout  %d sec!!\n", iWaitTime);
			return ReturnChannelError_Shell_CMD_TimeOut;
		}
#ifdef __linux__   /* Linux */
		usleep(200000);
#else
		Sleep(200);
#endif
	}
}

Dword RC_Shell_Init (
    IN RCShell*			    pRCShell,
	IN RCHostInfo*        pDeviceInfoList,
	IN int						iListMaxSize,
	IN int						iTxPort,
	IN int						iRxPort
){
	Dword	error = ModulatorError_NO_ERROR;

	pRCShell->rgDeviceInfo = pDeviceInfoList;
	pRCShell->iListSize = 0;
	pRCShell->iListMaxSize = iListMaxSize;

	pRCShell->bIsRun = True;
	pRCShell->bIsRcvFinish = True;
	pRCShell->bIsBroadcast = False;
	pRCShell->iTxPort = iTxPort;
	pRCShell->iRxPort = iRxPort;
	pRCShell->cbReply = NULL;

	//-----------------Create Mutex Lock For clear RCHostInfo List-----------------
	RC_Shell_Mutex_Init(&pRCShell->cleanList_Mutex);

	return error;
}

Dword RC_Shell_UnInit (
    IN RCShell*			    pRCShell
 ){
	int i = 0;
	Dword	error = ModulatorError_NO_ERROR;
	 //-----------------Destroy Mutex Lock For clear RCHostInfo List-----------------
	 RC_Shell_Mutex_UnInit(&pRCShell->cleanList_Mutex);


	for(i = 0; i < pRCShell->iListSize; i ++)
	{
		User_Host_uninit(&pRCShell->rgDeviceInfo[i]);
		User_memory_free(pRCShell->rgDeviceInfo[i].cmdConfig.cmd);
	}
	pRCShell->iListSize = 0;

	return error;
}

Dword RC_Shell_Stop_RcvThread (
    IN RCShell*			    pRCShell
){
	Dword	error = ModulatorError_NO_ERROR;

	pRCShell->bIsRun = False;
	printf("Stop Rcv Thread!!!\n");
	while(!pRCShell->bIsRcvFinish)
	{
#ifdef __linux__   /* Linux */
		usleep(1000);
#else
		Sleep(1);
#endif
	}
	return error;
}
Dword RC_Shell_Start_RcvThread (
    IN RCShell*			    pRCShell
){
	Dword	error = ModulatorError_NO_ERROR;
#ifdef __linux__   /* Linux */
	pthread_t tid;
	pthread_create(&tid,NULL,RC_Shell_RecvThread, (void*) pRCShell);
#else
	_beginthreadex(NULL, 0, RC_Shell_RecvThread, pRCShell, 0, NULL);
#endif
	return error;
}

Dword RC_Shell_FactoryResetBroadCast(
	IN RCShell*			    pRCShell,
	IN	Byte				bCommand,
	IN void (*cbReply)(void*, Word),
	IN int						iWaitTime
){	
	int i = 0;
	Dword	error = ModulatorError_NO_ERROR; 
	RCHostInfo deviceInfo;
	RC_Init_RCHeadInfo(&deviceInfo.device, 0, 0, 0xFFFF);
	RC_Init_TSHeadInfo(&deviceInfo.device, RETURN_CHANNEL_PID, 0, False);
	RC_Init_CmdConfig(&deviceInfo.cmdConfig, 1, CMD_MetadataStreamOutput);
	RC_Init_PortConfig(&deviceInfo.device, pRCShell->iRxPort, pRCShell->iTxPort);
	RC_Init_CmdSendConfig( &deviceInfo.cmdSendConfig);

	User_Host_init(&deviceInfo);
	User_askUserSecurity (&deviceInfo.security);


	deviceInfo.cmdSendConfig.bIsCmdBroadcast = True;
	deviceInfo.systemDefault.factoryDefault = bCommand;
	error = Cmd_Send(&deviceInfo, CMD_SetSystemFactoryDefaultBroadcastInput);

	User_Host_uninit(&deviceInfo);
	User_memory_free(deviceInfo.cmdConfig.cmd);

	return error;
}

Dword RC_Shell_BroadCast(
	IN RCShell*			    pRCShell,
	IN void (*cbReply)(void*, Word),
	IN int						iWaitTime
){	
	int i = 0;
	Dword	error = ModulatorError_NO_ERROR; 
	RCHostInfo deviceInfo;
	RC_Init_RCHeadInfo(&deviceInfo.device, 0, 0, 0xFFFF);
	RC_Init_TSHeadInfo(&deviceInfo.device, RETURN_CHANNEL_PID, 0, False);
	RC_Init_CmdConfig(&deviceInfo.cmdConfig, 1, CMD_MetadataStreamOutput);
	RC_Init_PortConfig(&deviceInfo.device, pRCShell->iRxPort, pRCShell->iTxPort);
	RC_Init_CmdSendConfig( &deviceInfo.cmdSendConfig);

	User_Host_init(&deviceInfo);
	User_askUserSecurity (&deviceInfo.security);

	//-----------------Mutex Lock For clear RCHostInfo List-----------------
	RC_Shell_mutex_lock(&pRCShell->cleanList_Mutex);  

	for(i = 0; i < pRCShell->iListSize; i ++)
	{
		User_Host_uninit(&pRCShell->rgDeviceInfo[i]);
		User_memory_free(pRCShell->rgDeviceInfo[i].cmdConfig.cmd);
	}
	pRCShell->iListSize = 0;

	//-----------------Mutex UnLock For clear RCHostInfo List-----------------
	RC_Shell_mutex_unlock(&pRCShell->cleanList_Mutex);  

	deviceInfo.cmdSendConfig.bIsCmdBroadcast = True;
	error = Cmd_Send(&deviceInfo, CMD_GetTxDeviceAddressIDInput);

	pRCShell->bIsBroadcast = True;
	pRCShell->cbReply = cbReply;
	if(!error)
	{
		Cmd_SetCmdConfigTimeOut(&deviceInfo, CMD_GetTxDeviceAddressIDInput, NULL);
		RC_Shell_BroadCastWait(pRCShell, &deviceInfo, iWaitTime);
	}
	pRCShell->cbReply = NULL;

	User_Host_uninit(&deviceInfo);
	User_memory_free(deviceInfo.cmdConfig.cmd);

	return error;
}
Dword RC_Shell_Send (
    IN RCShell*			    pRCShell,
	IN	int						iSelectIndex,
	IN	Word				wCommand,
	IN	Bool					bIsBroadCast,
	IN	Bool					bIsBlock,
	IN	int						iWaitTime,
	IN	void (*cbReply)(void*, Word),
	IN	void (*cbFail)(void*, Word)
) {
	Dword	error = ModulatorError_NO_ERROR;
	RCShell_ThreadParam threadParam[32];
	int i = 0;
	
	TYPE_SHELL_THREAD threadHandle[32];

	if(wCommand == CMD_GetTxDeviceAddressIDInput)
	{
		error = ReturnChannelError_Shell_CMD_NOT_SUPPORTED;
		printf("Shell Error : Can Not Send CMD_GetTxDeviceAddressIDInput Please Send By RC_Shell_BroadCast!!\n");
		return error;
	}

	if(wCommand == CMD_SetSystemFactoryDefaultBroadcastInput)
	{
		error = ReturnChannelError_Shell_CMD_NOT_SUPPORTED;
		printf("Shell Error : Can Not Send CMD_SetSystemFactoryDefaultBroadcastInput Please Send By RC_Shell_FactoryResetBroadCast!!\n");
		return error;
	}

	if(pRCShell->iListSize <= 0)
	{
		error = ReturnChannelError_Shell_NO_DEVICE_ERROR;
		printf("\nShell Error : No Device Info!!\n");
		return error;
	}
	if(iSelectIndex >= pRCShell->iListSize)
	{
		error = ReturnChannelError_Shell_LIST_RANGE_ERROR;
		printf("\nShell Error : SelectIndex Is Out of DeviceList Range!!\n");
		return error;
	}


	pRCShell->rgDeviceInfo[iSelectIndex].cmdSendConfig.bIsCmdBroadcast = bIsBroadCast;

	error = Cmd_Send(&pRCShell->rgDeviceInfo[iSelectIndex], wCommand);

	if(!error)
	{
		printf("\nSend Command = 0x%04x OK...\n\n",wCommand);

		if(wCommand == CMD_SetTxDeviceAddressIDInput)
		{
			if(pRCShell->rgDeviceInfo[iSelectIndex].newTxDevice.IDType != 0)
			{
				pRCShell->rgDeviceInfo[iSelectIndex].device.clientTxDeviceID =  pRCShell->rgDeviceInfo[iSelectIndex].newTxDevice.deviceAddressID;
			}
			else
			{
				printf("\nSet a random TxID please broadcast to get TxID\n");
				//-----------------Mutex Lock For clear RCHostInfo List-----------------
				RC_Shell_mutex_lock(&pRCShell->cleanList_Mutex);  

				for(i = 0; i < pRCShell->iListSize; i ++)
				{
					User_Host_uninit(&pRCShell->rgDeviceInfo[i]);
					User_memory_free(pRCShell->rgDeviceInfo[i].cmdConfig.cmd->buffer_Cmd);
					User_memory_free(pRCShell->rgDeviceInfo[i].cmdConfig.cmd);
				}
				pRCShell->iListSize = 0;

				//-----------------Mutex UnLock For clear RCHostInfo List-----------------
				RC_Shell_mutex_unlock(&pRCShell->cleanList_Mutex);  
				return error;
			}
		}
		if(bIsBlock)
		{
			if(bIsBroadCast)
			{
				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					Cmd_SetCmdConfigTimeOut(&pRCShell->rgDeviceInfo[i], wCommand, cbReply);
					threadParam[i].iSelectIndex = i;
					threadParam[i].iWaitTime = iWaitTime;
					threadParam[i].pRCShell = pRCShell;
					threadParam[i].cbFail = cbFail;
					threadParam[i].wCommand = wCommand;
					threadParam[i].bIsSet = False;
				}

				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					RC_Shell_Start_WaitTimeOutThread(&threadHandle[i], &threadParam[i]);
				}

				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					RC_Shell_WaitForSet_ThreadParam(&threadParam[i]);
#if _DEBUG_RC
					printf("Thread %d Parameter Set OK!\n", i);
#endif
				}
				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					RC_Shell_Join_WaitTimeOutThread(&threadHandle[i]);
				}

			}else
			{
				Cmd_SetCmdConfigTimeOut(&pRCShell->rgDeviceInfo[iSelectIndex], wCommand, cbReply);
				error  = RC_Shell_CheckTimeOut( pRCShell, iSelectIndex, wCommand, iWaitTime, cbFail);

			}

		}else
		{
			if(bIsBroadCast)
			{
				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					Cmd_SetCmdConfigTimeOut(&pRCShell->rgDeviceInfo[i], wCommand, cbReply);
					threadParam[i].iSelectIndex = i;
					threadParam[i].iWaitTime = iWaitTime;
					threadParam[i].pRCShell = pRCShell;
					threadParam[i].cbFail = cbFail;
					threadParam[i].wCommand = wCommand;
					threadParam[i].bIsSet = False;
				}
				
				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					RC_Shell_Start_WaitTimeOutThread(&threadHandle[i], &threadParam[i]);
				}
				for( i = 0; i < pRCShell->iListSize; i ++)
				{
					RC_Shell_WaitForSet_ThreadParam(&threadParam[i]);
#if _DEBUG_RC
					printf("Thread %d Parameter Set OK!\n", i);
#endif
				}

			}else
			{
				Cmd_SetCmdConfigTimeOut(&pRCShell->rgDeviceInfo[iSelectIndex], wCommand, cbReply);

				threadParam[0].iSelectIndex = iSelectIndex;
				threadParam[0].iWaitTime = iWaitTime;
				threadParam[0].pRCShell = pRCShell;
				threadParam[0].cbFail = cbFail;
				threadParam[0].wCommand = wCommand;
				threadParam[0].bIsSet = False;

				RC_Shell_Start_WaitTimeOutThread(&threadHandle[0], &threadParam[0]);

				RC_Shell_WaitForSet_ThreadParam(&threadParam[0]);

#if _DEBUG_RC
					printf("Thread Parameter Set OK!\n");
#endif
			}
			
		}
	}

	return error;
}


Dword RC_Shell_Set_EventCallBack (
    IN RCShell*			    pRCShell,
	IN	int						iSelectIndex,
	IN	void (*cbEvent)(void*, Word)
){
	Dword	error = ModulatorError_NO_ERROR;

	pRCShell->rgDeviceInfo[iSelectIndex].callBackSet.EventCallBack = cbEvent;

	return error;
}

void RC_Shell_WaitForSet_ThreadParam (
	IN RCShell_ThreadParam*	pThreadParam
){
	while(1)
	{
		if(pThreadParam->bIsSet)
			break;
	}
}

Dword RC_Shell_Start_WaitTimeOutThread (
	IN HANDLE_SHELL_THREAD	handle,
    IN RCShell_ThreadParam*			    pThreadParam

){
	Dword	error = ModulatorError_NO_ERROR;

#ifdef __linux__   /* Linux */
	pthread_create(handle,NULL,RC_Shell_WaitTimeOut, (void*) pThreadParam);
#else
	(*handle) = (HANDLE)_beginthreadex(NULL, 0, RC_Shell_WaitTimeOut, pThreadParam, 0, NULL);
#endif

	return error;
}

Dword RC_Shell_Join_WaitTimeOutThread (
	IN HANDLE_SHELL_THREAD	handle
){
	Dword	error = ModulatorError_NO_ERROR;

#ifdef __linux__   /* Linux */
	pthread_join ((*handle), NULL); 
#else
	WaitForSingleObject((*handle), INFINITE);
#endif
	return error;
}

void RC_Shell_Mutex_Init(
	IN HANDLE_SHELL_MUTEX handle
){
#ifdef __linux__   /* Linux */
	pthread_mutex_init (handle,NULL);
#else
	(*handle) = CreateMutex(NULL, FALSE, NULL);
#endif

}

void RC_Shell_Mutex_UnInit(
	IN HANDLE_SHELL_MUTEX handle
){
#ifdef __linux__   /* Linux */
	 pthread_mutex_destroy(handle); 
#else
	 CloseHandle((*handle));
#endif
}

void RC_Shell_mutex_lock(
	IN HANDLE_SHELL_MUTEX handle
){
#ifdef WIN32
		WaitForSingleObject((*handle), INFINITE); 
#else
		pthread_mutex_lock(handle); 
#endif
}
void RC_Shell_mutex_unlock(
	IN HANDLE_SHELL_MUTEX handle
){
#ifdef WIN32
	ReleaseMutex((*handle));  
#else
	pthread_mutex_unlock(handle);  
#endif
}

