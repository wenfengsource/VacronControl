#ifndef RETURN_CHANNEL_USER_H
#define RETURN_CHANNEL_USER_H



Dword User_returnChannelBusOpen (
    IN int comport_number, 
    IN int baudrate
); 

Dword User_returnChannelBusClose (
    IN  int comport_number
) ;

Dword User_returnChannelBusTx (
	IN  int	  portNum,
    IN  Dword     bufferLength,
    IN  const Byte*     buffer,
	OUT int*	  txLen
);

Dword User_returnChannelBusRx (
	IN  int	  portNum,
    IN  Dword     bufferLength,
    OUT Byte*     buffer,
	OUT int*	  rxLen
);


void User_askUserSecurity(
	IN Security* security
);

void *User_memory_allocate(
	IN size_t size
);
void User_memory_free(
	IN void* ptr
);
void *User_memory_copy( 
	OUT void * destination, 
	IN const void * source, 
	IN size_t num 
);
void * User_memory_set ( 
	OUT void * ptr, 
	IN int value, 
	IN size_t num 
);

Bool User_RCString_equal( 
	IN const RCString * str1, 
	IN const RCString * str2
);

void User_DebugInfo_log( 
	IN const DebugInfo* debugInfo 
);
void User_mutex_init();
void User_mutex_lock();
void User_mutex_unlock();
void User_mutex_uninit();

#endif
