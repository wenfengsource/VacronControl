#include "ReturnChannel_Cmd.h"
#include "ReturnChannel_User.h"
#include "../rs232.h"


#ifdef __linux__   /* Linux */
pthread_mutex_t mutex;
#else
HANDLE hMutex;
#endif


Dword User_returnChannelBusOpen (
    IN  int comport_number, 
    IN int baudrate
) {
    Dword     error = ModulatorError_NO_ERROR;
#if	UART_TEST	

	int ret;
	ret = OpenComport(comport_number,baudrate);
	if(ret != 0){
		error = ReturnChannelError_BUS_INIT_FAIL;
		printf("\nOpen COM port error = %lx!\n",error);
	}else{
		printf("\nOpen COM port successful!\n");
	}
#endif

    return (error);
}

Dword User_returnChannelBusClose (
    IN  int comport_number
) {
    Dword     error = ModulatorError_NO_ERROR;
#if	UART_TEST	
	CloseComport(comport_number);
#endif
    return (error);
}

Dword User_returnChannelBusTx (
	IN  int	  portNum,
    IN  Dword     bufferLength,
    IN  const Byte*     buffer,
	OUT int*	  txLen
) {
    Dword     error = ModulatorError_NO_ERROR;
#if	UART_TEST	

	*txLen = SendBuf( portNum, (unsigned char*)buffer, bufferLength);

#endif
    return (error);
}

Dword User_returnChannelBusRx (
	IN  int	  portNum,
    IN  Dword     bufferLength,
    OUT Byte*     buffer,
	OUT int*	  rxLen
) {
    Dword     error = ModulatorError_NO_ERROR;
#if	UART_TEST	

	*rxLen = PollComport(portNum ,(unsigned char*)buffer,bufferLength);	

#endif
    return (error);
}
void User_mutex_init()
{
#if	UART_TEST	
#ifdef __linux__   /* Linux */

	pthread_mutex_init (&mutex,NULL);
#else
	hMutex = CreateMutex(NULL, FALSE, NULL);
#endif
#endif
}
void User_mutex_lock()
{
#if	UART_TEST	
#ifdef __linux__   /* Linux */

	pthread_mutex_lock (&mutex);
#else
	WaitForSingleObject(hMutex, INFINITE);  
#endif
#endif
}

void User_mutex_unlock()
{
#if	UART_TEST	
#ifdef __linux__   /* Linux */

	pthread_mutex_unlock(&mutex);
#else
	ReleaseMutex(hMutex);  
#endif
#endif
}


void User_mutex_uninit()
{
#if	UART_TEST	
#ifdef __linux__   /* Linux */

	pthread_mutex_destroy(&mutex); 
#else
	CloseHandle(hMutex);

#endif
#endif
}


void User_askUserSecurity(Security* security)
{
#if	UART_TEST
	Byte userName[20] = "userName";
	Byte password[20] = "password";

	Cmd_StringClear(&security->userName);
	Cmd_StringClear(&security->password);

	Cmd_StringSet(userName, 20, &security->userName);
	Cmd_StringSet(password, 20, &security->password);
#endif
}

void *User_memory_allocate(size_t size)
{
#if	UART_TEST
	void *tmp = NULL;
	(tmp) = malloc(size);
	return (tmp);
#endif
}
void User_memory_free(void* ptr)
{
#if	UART_TEST
	if(ptr!=NULL)
	{
		free(ptr);
		ptr = NULL;
	}
#endif
}

void *User_memory_copy( void * destination, const void * source, size_t num )
{
#if	UART_TEST
	return memcpy ( destination, source, num );
#endif
}
void * User_memory_set ( void * ptr, int value, size_t num )
{
#if	UART_TEST
	return memset ( ptr, value, num );
#endif
}


Bool User_RCString_equal( const RCString * str1, const RCString * str2)
{
#if	UART_TEST
	if(str1->stringLength > str2->stringLength)
		return False;
	if(str1->stringLength < str2->stringLength)
		return False;

	if( strcmp((char*)str1->stringData, (char*)str2->stringData) != 0)
		return False;

	return True;
#endif
}



void User_DebugInfo_log( const DebugInfo* debugInfo )
{
#if	UART_TEST
	printf("\n=== User_DebugInfo_log ===\n");
	printf("Total GetBuffer Count		= %lu\n", debugInfo->totalGetBufferCount);
	printf("Leading Tag Error Count		= %lu\n", debugInfo->leadingTagErrorCount);
	printf("Total PKT Count			= %lu\n", debugInfo->totalPKTCount);
	printf("End Tag Error Count		= %lu\n", debugInfo->endTagErrorCount);
	printf("CheckSum Error Count		= %lu\n", debugInfo->checkSumErrorCount);
	printf("\n=== User_DebugInfo_log ===\n");

#endif
}
