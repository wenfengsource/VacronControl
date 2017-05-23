#include "DTVAPI.h"
#include <termios.h>


#define TESTKIT_VERSION 0x160705

//#define BLOCK_MOD
#define AUTO_MOD
#define READBUFFER_SIZE 188
#define BLOCK_BUFFER_SIZE 153408 /*188*816 153408/ 188*348 65424*/
#define PID_SIZE 0x2001 //13bit + 1, ref spec
#define SYNC_BYTE 0x47

typedef struct {
	uint32_t count;
	uint32_t ulErrCount;
	uint32_t ulLostCount;
	uint16_t pid;
	uint8_t  sequence1;
	uint8_t  sequence2;
	uint8_t  dup_flag;
} PIDINFO, *PPIDINFO;
