
#include <pthread.h>
#include <stdio.h>
typedef unsigned long Dword;

extern void freq_init(Dword freq, int Bandwidth);
extern int device_open();
extern void DTV_Close();

void main()
{
	int exit;
	pthread_t ts_capture;
	 void *status;
	freq_init(189000, 6000);
	//pthread_create(&ts_capture, NULL,ts_capture_thread,NULL );
	int ret;
	ret = device_open();
	printf("ret = %d\n", ret);
	 while(1)
	{
		printf("input exit 1 \n");
		scanf("%d", exit);
		printf("exit = %d \n", exit);
		 if(exit ==1)
		 {
			//DTV_Close();	
       			exit= 0;
		 }
		 break;
       		sleep(3);
	}
	printf("finished\n");
	 DTV_Close();
	 
	
}
