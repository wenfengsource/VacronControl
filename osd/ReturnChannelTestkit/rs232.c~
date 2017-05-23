/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/



#include "rs232.h"

#define RETRY_MAX 1000

#ifdef __linux__   /* Linux */

#define Port_Num 24

int Cport[Port_Num],
    error;

struct termios new_port_settings,
       old_port_settings[Port_Num];

char comports[Port_Num][13]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4",
						"/dev/ttyS5","/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9",
						"/dev/ttyS10","/dev/ttyS11","/dev/ttyS12","/dev/ttyS13","/dev/ttyS14",
						"/dev/ttyS15","/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3",
						"/dev/ttyUSB4","/dev/ttyUSB5","/dev/tnt0","/dev/tts/0"};

 
int OpenComport(int comport_number, int baudrate)
{
  int baudr;

  if((comport_number >= Port_Num)||(comport_number<0))
  {
    printf("\nIllegal COM port number\n");
    return(1);
  }

  switch(baudrate)
  {
    case      50 : baudr = B50;
                   break;
    case      75 : baudr = B75;
                   break;
    case     110 : baudr = B110;
                   break;
    case     134 : baudr = B134;
                   break;
    case     150 : baudr = B150;
                   break;
    case     200 : baudr = B200;
                   break;
    case     300 : baudr = B300;
                   break;
    case     600 : baudr = B600;
                   break;
    case    1200 : baudr = B1200;
                   break;
    case    1800 : baudr = B1800;
                   break;
    case    2400 : baudr = B2400;
                   break;
    case    4800 : baudr = B4800;
                   break;
    case    9600 : baudr = B9600;
                   break;
    case   19200 : baudr = B19200;
                   break;
    case   38400 : baudr = B38400;
                   break;
    case   57600 : baudr = B57600;
                   break;
    case  115200 : baudr = B115200;
                   break;
    case  230400 : baudr = B230400;
                   break;
    case  460800 : baudr = B460800;
                   break;
    case  500000 : baudr = B500000;
                   break;
    case  576000 : baudr = B576000;
                   break;
    case  921600 : baudr = B921600;
                   break;
    case 1000000 : baudr = B1000000;
                   break;
    default      : printf("\nInvalid baudrate\n");
                   return(1);
                   break;
  }

  Cport[comport_number] = open(comports[comport_number], O_RDWR|O_NONBLOCK);
  //Cport[comport_number] = open(comports[comport_number], O_RDWR | O_NOCTTY | O_NDELAY);

  if(Cport[comport_number]==-1)
  {
    perror("\nUnable to open COM port ");
    return(1);
  }

  error = tcgetattr(Cport[comport_number], old_port_settings + comport_number);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("\nUnable to read port settings ");
    return(1);
  }
  memset(&new_port_settings, 0, sizeof(new_port_settings));  /* clear the new struct */

  new_port_settings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
  new_port_settings.c_iflag = IGNPAR;
  new_port_settings.c_oflag = 0;
  new_port_settings.c_lflag = 0;
  new_port_settings.c_cc[VMIN] = 100;      /* block untill n bytes are received */
  new_port_settings.c_cc[VTIME] = 100;     /* block untill a timer expires (n * 100 mSec.) */
  error = tcsetattr(Cport[comport_number], TCSANOW, &new_port_settings);
  if(error==-1)
  {
    close(Cport[comport_number]);
    perror("\nUnable to adjust port settings ");
    return(1);
  }

  return(0);
}


int PollComport(int comport_number, unsigned char *buf, int size)
{
  int n;

#ifndef __STRICT_ANSI__                       /* __STRICT_ANSI__ is defined when the -ansi option is used for gcc */
  if(size>SSIZE_MAX)  size = (int)SSIZE_MAX;  /* SSIZE_MAX is defined in limits.h */
#else
  if(size>4096)  size = 4096;
#endif

  n = read(Cport[comport_number], buf, size);

  return(n);
}


int SendByte(int comport_number, unsigned char byte)
{
  int n;

  n = write(Cport[comport_number], &byte, 1);
  if(n<0)  return(1);

  return(0);
}


int SendBuf(int comport_number, const unsigned char *buf, int size)
{
  return(write(Cport[comport_number], buf, size));
}


void CloseComport(int comport_number)
{
  close(Cport[comport_number]);
  tcsetattr(Cport[comport_number], TCSANOW, old_port_settings + comport_number);
}

/*
Constant  Description
TIOCM_LE  DSR (data set ready/line enable)
TIOCM_DTR DTR (data terminal ready)
TIOCM_RTS RTS (request to send)
TIOCM_ST  Secondary TXD (transmit)
TIOCM_SR  Secondary RXD (receive)
TIOCM_CTS CTS (clear to send)
TIOCM_CAR DCD (data carrier detect)
TIOCM_CD  Synonym for TIOCM_CAR
TIOCM_RNG RNG (ring)
TIOCM_RI  Synonym for TIOCM_RNG
TIOCM_DSR DSR (data set ready)
*/

int IsCTSEnabled(int comport_number)
{
  int status;

  status = ioctl(Cport[comport_number], TIOCMGET, &status);

  if(status&TIOCM_CTS) return(1);
  else return(0);
}


#else         /* windows */


HANDLE Cport[32];


char comports[32][15]={"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                       "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                       "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                       "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
					   "\\\\.\\COM17",  "\\\\.\\COM18",  "\\\\.\\COM19",  "\\\\.\\COM20",
					   "\\\\.\\COM21",  "\\\\.\\COM22",  "\\\\.\\COM23",  "\\\\.\\COM24",
					   "\\\\.\\COM25",  "\\\\.\\COM26",  "\\\\.\\COM27",  "\\\\.\\COM28",
					   "\\\\.\\COM29",  "\\\\.\\COM30",  "\\\\.\\COM31",  "\\\\.\\COM32"};

char baudr[64];


int OpenComport(int comport_number, int baudrate)
{
  DCB port_settings;
  COMMTIMEOUTS Cptimeouts;
  if((comport_number>31)||(comport_number<0))
  {
    printf("illegal comport number\n");
    return(1);
  }



  switch(baudrate)
  {
    case     110 : strcpy_s(baudr, 64, "baud=110 data=8 parity=N stop=1");
                   break;
    case     300 : strcpy_s(baudr, 64, "baud=300 data=8 parity=N stop=1");
                   break;
    case     600 : strcpy_s(baudr, 64, "baud=600 data=8 parity=N stop=1");
                   break;
    case    1200 : strcpy_s(baudr, 64, "baud=1200 data=8 parity=N stop=1");
                   break;
    case    2400 : strcpy_s(baudr, 64, "baud=2400 data=8 parity=N stop=1");
                   break;
    case    4800 : strcpy_s(baudr, 64, "baud=4800 data=8 parity=N stop=1");
                   break;
    case    9600 : strcpy_s(baudr, 64, "baud=9600 data=8 parity=N stop=1");
                   break;
    case   19200 : strcpy_s(baudr, 64, "baud=19200 data=8 parity=N stop=1");
                   break;
    case   38400 : strcpy_s(baudr, 64, "baud=38400 data=8 parity=N stop=1");
                   break;
    case   57600 : strcpy_s(baudr, 64, "baud=57600 data=8 parity=N stop=1");
                   break;
    case  115200 : strcpy_s(baudr, 64, "baud=115200 data=8 parity=N stop=1");
                   break;
    case  128000 : strcpy_s(baudr, 64, "baud=128000 data=8 parity=N stop=1");
                   break;
    case  256000 : strcpy_s(baudr, 64, "baud=256000 data=8 parity=N stop=1");
                   break;
    default      : printf("invalid baudrate\n");
                   return(1);
                   break;
  }

  Cport[comport_number] = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(Cport[comport_number]==INVALID_HANDLE_VALUE)
  {
    printf("\nUnable to open COM port!");
    return(1);
  }

  //IOCTL_SERIAL_SET_QUEUE_SIZE
  if(!SetupComm(Cport[comport_number], 0, 0))
  {
	  printf("unable to setup comport settings\n");
	  CloseHandle(Cport[comport_number]);
	  return(1);
  }

  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(baudr, &port_settings))
  {
    printf("\nUnable to set COM port dcb settings!");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  if(!SetCommState(Cport[comport_number], &port_settings))
  {
    printf("\nUnable to set COM port cfg settings!");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  

  Cptimeouts.ReadIntervalTimeout         = 10;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 10;
  Cptimeouts.ReadTotalTimeoutConstant    = 10;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport[comport_number], &Cptimeouts))
  {
    printf("\nUnable to set COM port time-out settings!");
    CloseHandle(Cport[comport_number]);
    return(1);
  }



  return(0);
}


int PollComport(int comport_number, unsigned char *buf, int size)
{
  int n;

 // if(size>4096)  size = 4096;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  ReadFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL);

  return(n);
}


int SendByte(int comport_number, unsigned char byte)
{
  int n;

  WriteFile(Cport[comport_number], &byte, 1, (LPDWORD)((void *)&n), NULL);

  if(n<0)  return(1);

  return(0);
}


int SendBuf(int comport_number, const unsigned char *buf, int size)
{
  int n;
  int remainSize = size;
  int retryCount = 0;

  while(1)
  {
	  if(WriteFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
	  {
		  if(n == 0)
		  {
			  retryCount ++;
			  if( retryCount == RETRY_MAX)
			  {
				  printf("\nUnable to write data to COM port !");
				  return(-1);
			  }
		  }
		  remainSize = remainSize - n;
		  if(remainSize != 0)
		  {

			  buf = buf + n;
			  size = size - n;
			  n = 0;
			  continue;
		  }
		  else
			  return(n);
	  }else
	  {
		  printf("\nUnable to write data to COM port !");
		  return(-1);
	  }
  }

  return(-1);
}


void CloseComport(int comport_number)
{
  //IOCTL_SERIAL_PURGE
  PurgeComm(Cport[comport_number], PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  
  CloseHandle(Cport[comport_number]);
}


int IsCTSEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_CTS_ON) return(1);
  else return(0);
}


#endif

/*
void cprintf(int comport_number, const char *text)  // sends a string to serial port 
{
  while(*text != 0)   SendByte(comport_number, *(text++));
}
*/


