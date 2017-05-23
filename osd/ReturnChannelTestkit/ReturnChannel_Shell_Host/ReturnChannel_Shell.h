#ifndef RETURN_CHANNEL_SHELL_H
#define RETURN_CHANNEL_SHELL_H


#include "../ReturnChannel_API_Core/ReturnChannel_Cmd.h"
#include "../ReturnChannel_API_Host/ReturnChannel_Cmd_Host.h"
#include "../ReturnChannel_API_Core/ReturnChannel_User.h"
#include "../ReturnChannel_API_Host/ReturnChannel_User_Host.h"



#define ReturnChannelError_Shell_CMD_NOT_SUPPORTED				0x05100001ul
#define ReturnChannelError_Shell_NO_DEVICE_ERROR						0x05100002ul
#define ReturnChannelError_Shell_LIST_RANGE_ERROR						0x05100003ul
#define ReturnChannelError_Shell_CALLBACK_NOT_FOUND			0x05100004ul

#define ReturnChannelError_Shell_CMD_InValid										0x05100006ul
#define ReturnChannelError_Shell_CMD_TimeOut									0x05100007ul

#define ReturnChannelError_Shell_Thread_TimeOut									0x05100008ul


#ifdef WIN32
#define TYPE_SHELL_THREAD				HANDLE
#define HANDLE_SHELL_THREAD			HANDLE*
#define TYPE_SHELL_MUTEX					HANDLE
#define HANDLE_SHELL_MUTEX			HANDLE*
#else
#define TYPE_SHELL_THREAD				pthread_t
#define HANDLE_SHELL_THREAD			pthread_t*
#define TYPE_SHELL_MUTEX					pthread_mutex_t
#define HANDLE_SHELL_MUTEX			pthread_mutex_t*
#endif

typedef struct {
	RCHostInfo* rgDeviceInfo;
	DebugInfo debugInfo;
	int iListSize;
	int iListMaxSize;
	Bool bIsRun;
	Bool bIsRcvFinish;
	Bool bIsBroadcast;

	int iTxPort;
	int iRxPort;

	void (*cbReply)(void*, Word);
	//-------------Mutex For Clean RCHostInfo List when Set Random Tx ID-------------
	TYPE_SHELL_MUTEX cleanList_Mutex;

} RCShell;


typedef struct {
	RCShell* pRCShell;
	int iSelectIndex;
	Word wCommand;
	int iWaitTime;
	void	(*cbFail)(void*, Word);

	Bool bIsSet;
} RCShell_ThreadParam;

void RC_Shell_BroadCastWait(
    IN RCShell*			    pRCShell,
	IN RCHostInfo*			pDeviceInfo,
	IN int						iWaitTime
);

Dword RC_Shell_CheckTimeOut(
	IN RCShell*			    pRCShell,
	int								iSelectIndex,
	Word						wCommand,
	int								iWaitTime,
	void							(*cbFail)(void*, Word)
);

Dword RC_Shell_Init (
    IN RCShell*			    pRCShell,
	IN RCHostInfo*        deviceInfoList,
	IN int						iListMaxSize,
	IN int						iTxPort,
	IN int						iRxPort
);

Dword RC_Shell_UnInit (
    IN RCShell*			    pRCShell
 );

Dword RC_Shell_FactoryResetBroadCast(
	IN RCShell*			    pRCShell,
	IN	Byte				bCommand,
	IN void (*cbReply)(void*, Word),
	IN int						iWaitTime
);

Dword RC_Shell_BroadCast(
	IN RCShell*			    pRCShell,
	IN void (*cbReply)(void*, Word),
	IN int						iWaitTime
);

Dword RC_Shell_Stop_RcvThread (
    IN RCShell*			    pRCShell
);
Dword RC_Shell_Start_RcvThread (
    IN RCShell*			    pRCShell
);

Dword RC_Shell_Send (
    IN RCShell*			    pRCShell,
	IN	int						iSelectIndex,
	IN	Word				wCommand,
	IN	Bool					bIsBroadCast,
	IN	Bool					bIsBlock,
	IN	int						iWaitTime,
	IN	void (*cbReply)(void*, Word),
	IN	void (*cbFail)(void*, Word)
);

Dword RC_Shell_Set_EventCallBack (
    IN RCShell*			    pRCShell,
	IN	int						iSelectIndex,
	IN	void (*cbEvent)(void*, Word)
);

void RC_Shell_WaitForSet_ThreadParam (
	IN RCShell_ThreadParam*	pThreadParam
);


Dword RC_Shell_Start_WaitTimeOutThread (
	IN HANDLE_SHELL_THREAD	handle,
    IN RCShell_ThreadParam*			    pThreadParam
);

Dword RC_Shell_Join_WaitTimeOutThread (
	IN HANDLE_SHELL_THREAD	handle
);

void RC_Shell_Mutex_Init(
	IN HANDLE_SHELL_MUTEX handle
);

void RC_Shell_Mutex_UnInit(
	IN HANDLE_SHELL_MUTEX handle
);

void RC_Shell_mutex_lock(
	IN HANDLE_SHELL_MUTEX handle
);

void RC_Shell_mutex_unlock(
	IN HANDLE_SHELL_MUTEX handle
);


#endif

