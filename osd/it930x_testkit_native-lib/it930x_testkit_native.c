#include <sys/time.h>
#include <math.h>
#include "it930x_testkit_native.h"

//#define scanf  
int g_handle;
int g_chipType = 0;
int exit_flag =0;
Dword  AUTO_FREQUENCY ;
int AUTO_BANDWIDTH ;

void freq_init(Dword freq, int Bandwidth)
{
	AUTO_FREQUENCY = freq;
	AUTO_BANDWIDTH = Bandwidth;
}

//===========================for Dibcom diversity only=========================
int Dibcom_Detail_TransmissionParameters(DibcomDiversityData *request, int max_fe)
{
	int i = 0; 
	printf(">>>>> Transmission Parameter\n");
	for(i = 0; i < max_fe; i++ ) {
		printf( " [%d]\n",i);
		if((request->DibDetailData[i].locks.fec_mpeg == 1) && (request->DibDetailData[i].locks.tps_sync == 1)) {
				switch(request->DibDetailData[i].cur_channel.nqam) 
				{
					case 0:	printf("   Constellation            : QPSK\n"); break;
					case 1: printf("   Constellation            : 16QAM\n"); break;
					case 2: printf("   Constellation            : 64QAM\n"); break;
					default: printf("   No such Constellation\n"); return -1;
				}

				switch(request->DibDetailData[i].cur_channel.vit_code_rate_hp) 
				{
					case 1: printf("   Code Rate                : 1/2\n"); break;
					case 2:	printf("   Code Rate                : 2/3\n"); break;
					case 3:	printf("   Code Rate                : 3/4\n"); break;
					case 5:	printf("   Code Rate                : 5/6\n"); break;
					case 7:	printf("   Code Rate                : 7/8\n"); break;
					default: printf("   No such Code Rate\n"); return -1;
				}

				switch(request->DibDetailData[i].cur_channel.guard) 
				{
					case 0: printf("   GuardInterval            : 1/32\n"); break;
					case 1: printf("   GuardInterval            : 1/16\n"); break;
					case 2: printf("   GuardInterval            : 1/8\n"); break;
					case 3: printf("   GuardInterval            : 1/4\n"); break;
					default: printf("   No such GuardInterval\n"); return -1;
				}

				switch(request->DibDetailData[i].cur_channel.nfft) 
				{
					case 0: printf("   Transmission Mode        : 2K\n"); break;
					case 1: printf("   Transmission Mode        : 8K\n"); break;
					case 2: printf("   Transmission Mode        : 4K\n"); break;
					default: printf("No such Transmission Mode:\n"); return -1;
				}
				
		}else {
			printf("   Detect no Lock\n");
		}
		
	}

	return 0;
}

double Logarithm_ADCpowerCalculate(Dword data)
{
	double tmpdata = 0.0; 
	if(data == 0)
		tmpdata = 1.0;
	else
		tmpdata = (double)data;
	tmpdata /= (double) (1<<20);
	return ((10.0) * log10(tmpdata));
}

void Dibcom_Detail_ADCpower(DibcomDiversityData *request, int max_fe)
{	
	int i = 0;
	printf(">>>>> ADC power:\n");

	for(i = 0 ; i < max_fe ; i ++)
	{
		printf( " [%d]\n",i);
		printf("   ADC power I              : %lf\n", Logarithm_ADCpowerCalculate(request->DibDetailData[i].adc_power_i));
		printf("   ADC power Q              : %lf\n", Logarithm_ADCpowerCalculate(request->DibDetailData[i].adc_power_q));
		printf("   ADC LNpower              : %lf\n", ((10.0)*request->DibDetailData[i].adc_lnpower_raw/256.0) );
	}
}

void Dibcom_Detail_IQ_Mismatch(DibcomDiversityData *request, int max_fe)
{
	int i = 0;
	printf(">>>>> IQ mismatch:\n");
	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		printf("   IQ gain mismatch(dB)     : %lf\n",(double)((10.0) * log10(request->DibDetailData[i].iq_misgain / 1024.0)));
		printf("   IQ phase mismatch(degree): %lf\n",(double)((180.0) * (request->DibDetailData[i].iq_misphi/2048.0)));
	}
}

void Dibcom_Detail_AGC_info(DibcomDiversityData *request, int max_fe)
{
	int i = 0;
	printf(">>>>> AGC info:\n");
	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		printf("   AGC WBD                  : %lf\n",(request->DibDetailData[i].agc_wbd_raw/4096.0));
		printf("   AGC global(norm)         : %lf\n",(request->DibDetailData[i].rf_power/65536.0));
		printf("   AGC1 (norm)              : %lf\n",(request->DibDetailData[i].agc1_raw/65536.0));
		printf("   AGC2 (norm)              : %lf\n",(request->DibDetailData[i].agc2_raw/65536.0));
	}
}

void Dibcom_Detail_SignalOffset(DibcomDiversityData *request, int max_fe)
{
	int i = 0;
	printf(">>>>> Signal Offset:\n");
	
	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		request->DibDetailData[i].freq_offset = request->DibDetailData[i].dds_freq-request->DibDetailData[i].p_dds_freq;
		printf("   Freq offset(Hz)          : %d\n",request->DibDetailData[i].freq_offset);
		printf("   Carrier offset(KHz)      : %lf\n",
			  (double)(request->DibDetailData[i].tuner_is_baseband?0:60000)-
			  ((double)request->DibDetailData[i].freq_offset / (double)(1<<26)*(double)request->DibDetailData[i].internal_clk));
		printf("   Time offset (ppm)        : %lf\n", 
		      1000000* (((double)request->DibDetailData[i].timf_current / (double)request->DibDetailData[i].timf_default)-1));
	}
}

void Dibcom_Detail_PacketError(DibcomDiversityData *request, int max_fe)
{
	int i = 0 ;
	printf(">>>>> Packet Error:\n");
	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		printf("   Packet Error Count       : %d\n",request->DibDetailData[i].PacketErrors);
		printf("   Packet Error Cumul       : %d\n",request->DibDetailData[i].PacketErrorCount);
	}
}

void Dibcom_Detail_MER(DibcomDiversityData *request, int max_fe)
{
	int i = 0;
	double mer = 0.0, tmp = 0.0;
	printf(">>>>> MER:\n");

	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		mer = 0.0, tmp = 0.0; 
		if(request->DibDetailData[i].mer_mant == 0)
			mer = 0;
		else
		{
			tmp = (double)request->DibDetailData[i].mer_mant * pow(2.0, ((double)request->DibDetailData[i].mer_exp - 8.0));
			mer = (-10.0) *  log10(tmp);
		}
		printf("   MER mant                 : %d\n", request->DibDetailData[i].mer_mant);
		printf("   MER exp                  : %d\n", request->DibDetailData[i].mer_exp);
		printf("   MER                      : %2.2f\n",mer);
	}
}

void Dibcom_Detail_CN(DibcomDiversityData *request, int max_fe)
{
	double signal_db =  0.0 , noise_db = 0.0;
	int i = 0 ;
	printf(">>>>> CN:\n");

	for(i = 0 ; i < max_fe ; i ++)
	{
		printf(" [%d]\n",i);
		signal_db =  0.0, noise_db =  0.0;
		if(request->DibDetailData[i].equal_signal_mant == 0)
			signal_db = 0.0;
		else
		{
		signal_db =  10 * log10(  ((double)request->DibDetailData[i].equal_signal_mant + 1E-10) * pow(2.0,(double)request->DibDetailData[i].equal_signal_exp) ) ;
		}
		if(request->DibDetailData[i].equal_noise_mant == 0)
			noise_db = 0.0;
		else
		{
		noise_db = 10 * log10(  ((double)request->DibDetailData[i].equal_noise_mant + 1E-10) * pow(2.0,(double)request->DibDetailData[i].equal_noise_exp) );
		}
		printf("   equalSignal mant         : %d\n", request->DibDetailData[i].equal_signal_mant);
		printf("   equalSignal exp          : %d\n", request->DibDetailData[i].equal_signal_exp);
		printf("   equalNoise mant          : %d\n", request->DibDetailData[i].equal_noise_mant);
		printf("   equalNoise exp           : %d\n", request->DibDetailData[i].equal_noise_exp);
		printf("   CN                       : %2.2f\n",signal_db - noise_db);
	}
}

void Dibcom_Detail_SignalQuality(DibcomDiversityData *request, int max_fe)
{
	Dword invBER = 0;
	Byte BER_SQI = 0;
	int i = 0;
	printf(">>>>> Signal Quality:\n");
	for(i = 0 ; i < max_fe ; i ++)
	{

		printf(" [%d]\n",i);
		invBER = 0, BER_SQI = 0;
	if(request->DibDetailData[i].ber_raw == 0)
		invBER = 10000001;
	else
		invBER = 100000000 / request->DibDetailData[i].ber_raw;

	if(invBER < 1000)
		BER_SQI = 0;
	else if(invBER >= 1000 && invBER < 10000000)
	{
		if(invBER < 1245) //3.0
			BER_SQI = 20 ;
		else if (invBER >= 1245 && invBER < 1567) //3.1
			BER_SQI = 22 ;
		else if (invBER >= 1567 && invBER < 1973) //3.2
			BER_SQI = 24 ;
		else if (invBER >= 1973 && invBER < 2484) //3.3
			BER_SQI = 26 ;
		else if (invBER >= 2484 && invBER < 3127) //3.4
			BER_SQI = 28 ;
		else if (invBER >= 3127 && invBER < 3936) //3.5
			BER_SQI = 30 ;
		else if (invBER >= 3936 && invBER < 4955) //3.6
			BER_SQI = 32 ;
		else if (invBER >= 4955 && invBER < 6238) //3.7
			BER_SQI = 34 ;
		else if (invBER >= 6238 && invBER < 7853) //3.8
			BER_SQI = 36 ;
		else if (invBER >= 7853 && invBER < 9886) //3.9
			BER_SQI = 38 ;
		else if (invBER >= 9886 && invBER < 12446) //4.0
			BER_SQI = 40 ;
		else if (invBER >= 12446 && invBER < 15668) //4.1
			BER_SQI = 42 ;
		else if (invBER >= 15668 && invBER < 19725) //4.2
			BER_SQI = 44 ;
		else if (invBER >= 19725 && invBER < 24832) //4.3
			BER_SQI = 46 ;
		else if (invBER >= 24832 && invBER < 31261) //4.4
			BER_SQI = 48 ;
		else if (invBER >= 31261 && invBER < 39356) //4.5
			BER_SQI = 50 ;
		else if (invBER >= 39356 && invBER < 49546) //4.6
			BER_SQI = 52 ;
		else if (invBER >= 49546 && invBER < 62374) //4.7
			BER_SQI = 54 ;
		else if (invBER >= 62374 && invBER < 78524) //4.8
			BER_SQI = 56 ;
		else if (invBER >= 78524 && invBER < 98856) //4.9
			BER_SQI = 58 ;
		else if (invBER >= 98856 && invBER < 124452) //5.0
			BER_SQI = 60 ;
		else if (invBER >= 124452 && invBER < 156676) //5.1
			BER_SQI = 62 ;
		else if (invBER >= 156676 && invBER < 197243) //5.2
			BER_SQI = 64 ;
		else if (invBER >= 197243 && invBER < 248314) //5.3
			BER_SQI = 66 ;
		else if (invBER >= 248314 && invBER < 312609) //5.4
			BER_SQI = 68 ;
		else if (invBER >= 312609 && invBER < 393551) //5.5
			BER_SQI = 70 ;
		else if (invBER >= 393551 && invBER < 495451) //5.6
			BER_SQI = 72 ;
		else if (invBER >= 495451 && invBER < 623735) //5.7
			BER_SQI = 74 ;
		else if (invBER >= 623735 && invBER < 785237) //5.8
			BER_SQI = 76 ;
		else if (invBER >= 785237 && invBER < 988554) //5.9
			BER_SQI = 78 ;
		else if (invBER >= 988554 && invBER < 1244515) //6.0
			BER_SQI = 80 ;
		else if (invBER >= 1244515 && invBER < 1566751) //6.1
			BER_SQI = 82 ;
		else if (invBER >= 1566751 && invBER < 1972422) //6.2
			BER_SQI = 84 ;
		else if (invBER >= 1972422 && invBER < 2483135) //6.3
			BER_SQI = 86 ;
		else if (invBER >= 2483135 && invBER < 3126081) //6.5
			BER_SQI = 88 ;
		else if (invBER >= 3126081 && invBER < 3935502) //6.6
			BER_SQI = 90 ;
		else if (invBER >= 3935502 && invBER < 4954502) //6.7
			BER_SQI = 92 ;                
		else if (invBER >= 4954502 && invBER < 6237347) //6.8
			BER_SQI = 94 ;
		else if (invBER >= 6237347 && invBER < 7852361) //6.9
			BER_SQI = 96 ;
		else if (invBER >= 7852361 && invBER < 9885534) //7.0
			BER_SQI = 98 ;
		else
			BER_SQI = 100 ;
	}       
	else if(invBER > 10000000)
		BER_SQI = 100;

	printf("   Signal Quality           : %d\n", BER_SQI);
	}

}
//===========================for Dibcom diversity only=========================

int kbhit(void)  
{
	struct termios oldt, newt;  
	int ch;  
	int oldf;  
	tcgetattr(STDIN_FILENO, &oldt);  
	newt = oldt;  
	newt.c_lflag &= ~(ICANON | ECHO);  
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
	ch = getchar();  
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
	fcntl(STDIN_FILENO, F_SETFL, oldf);  
	if(ch != EOF) {  
		ungetc(ch, stdin);  
		return 1;  
	}  
	return 0;  
}  

// return lock status 1->locked 0->unlock
int Endeavour_Lock_Channel(int handle)
{
	Dword set_frequency = 0, result = ERR_NO_ERROR;
	Word set_bandwidth = 0;
	Bool is_lock = 0;
	int res = 0;
	struct  timeval start;
    struct  timeval end;
    unsigned long diff;

	
#ifndef AUTO_MOD
	printf("\n=> Please Input Frequency (KHz): ");
	//res = scanf("%lu", &set_frequency);
	
	printf("\n=> Please Input Bandwidth (KHz) (Ex: 8000 KHz): ");
//	res = scanf("%hu",&set_bandwidth);
#else
	set_frequency = AUTO_FREQUENCY;
	set_bandwidth = AUTO_BANDWIDTH;
#endif
	
	printf("\n============== Lock Channel ==============\n");
	printf("Frequency = %lu KHz\n", set_frequency);
	printf("Bandwidth = %u KHz\n", set_bandwidth);
	
    gettimeofday(&start, NULL);
	
	result = DTV_AcquireChannel(handle, set_frequency, set_bandwidth);
	if(result)
		return;
	
    gettimeofday(&end, NULL);

	diff = ((end.tv_sec-start.tv_sec)*1000) + ((end.tv_usec-start.tv_usec)/1000);

    printf("DTV_AcquireChannel done (%ld ms)\n", diff);
        
	if(result == 2){
		printf("This handle reading data now!!!\n");
		return;
	}
	
	if(result == 1){
		printf("DTV_AcquireChannel fail [0x%08lx]\n", result);
		return;
	}
	
    gettimeofday(&start,NULL);
	
	result = DTV_IsLocked(handle, &is_lock);
	if(result){
		printf("DTV_IsLocked fail [0x%08lx]\n", result);
	}
	
    gettimeofday(&end, NULL);

	diff = ((end.tv_sec-start.tv_sec)*1000) + ((end.tv_usec-start.tv_usec)/1000);

	printf("************* Channel %slock (%ld ms) *************\n", (is_lock) ? "" : "un", diff);
	printf("==========================================\n");
	return is_lock;
}

void Endeavour_Lock_Multi_Channel(int handle)
{
	Dword frequency_start = 0, frequency_end = 0, set_frequency = 0, result = ERR_NO_ERROR;
	Word set_bandwidth = 0;
	Bool is_lock = 0;
	int channels = 0, res = 0;
	DTVStatistic statisic;
	res += 0;
	printf("\n=> Please Input Start Frequency (KHz): ");
	//res = scanf("%lu", &frequency_start);
	
	printf("\n=> Please Input End Frequency (KHz): ");
	//res = scanf("%lu", &frequency_end);
	
	printf("\n=> Please Input Bandwidth (KHz) (Ex: 8000 KHz): ");
	//res = scanf("%hu",&set_bandwidth);
	
	set_frequency = frequency_start;
	while(set_frequency <= frequency_end){
		printf("Scan %lu KHz\n", set_frequency);
		result = DTV_AcquireChannel(handle, set_frequency, set_bandwidth);
		if(result){
			printf("DTV_AcquireChannel fail [0x%08lx]\n", result);
		}
		
		result = DTV_IsLocked(handle, &is_lock);
		if(result){
			printf("DTV_IsLocked fail [0x%08lx]\n", result);
		}
		
		if(is_lock){
			sleep(1);			
			result = DTV_GetStatistic(handle, &statisic);
			if(result){
				printf("DTV_GetStatistic fail [0x%08lx]\n", result);
			}
			
			channels++;
			printf("*** Channel Statistics: ***\n");
			printf("Frequency    : %lu KHz\n", set_frequency);
			printf("Demod locked : %d\n", statisic.signalLocked);
			printf("TPS locked   : %d\n", statisic.signalPresented);
			printf("Quality      : %d\n", statisic.signalQuality);
			printf("Strength     : %d\n", statisic.signalStrength);

			if(statisic.postVitBitCount != 0)
				printf("BER          : %3.3e\n", ((float)statisic.postVitErrorCount) / statisic.postVitBitCount);
			else
				printf("BER          : %3.3e\n", (double) statisic.postVitBitCount);
			
			printf("Abort Count  : %d\n", statisic.abortCount);
			
			printf("*******************************\n");
		}
		
		set_frequency += set_bandwidth;
	}
	
	printf("============ Analysis ============");
	printf("\n Frequency %lu(KHz) ~  %lu(KHz)", frequency_start, frequency_end);
	printf("\n Scan Channel : %d channels Locked", channels);
	printf("\n==================================\n");
}

void Get_Statistic(int handle, int target_times){
	DTVStatistic statisic;
	int get_times = 0;
	Dword result = ERR_NO_ERROR;
	
	
	while(get_times < target_times){
		result = DTV_GetStatistic(handle, &statisic);
		if(result){
			printf("DTV_GetStatistic fail [0x%08lx]\n", result);
			return;
		}
		else{
			printf("\n*** Channel Statistics %d: ***\n", get_times);
			printf("Demod locked : %d\n", statisic.signalLocked);
			printf("TPS locked   : %d\n", statisic.signalPresented);
			printf("Quality      : %d\n", statisic.signalQuality);
			printf("Strength     : %d\n", statisic.signalStrength);
			
			if(g_chipType == 1) { 			
				if(statisic.postVitBitCount != 0)
					printf("BER          : %3.3e\n", ((float)statisic.postVitErrorCount) / statisic.postVitBitCount);
				else
					printf("BER          : %3.3e\n", (double) statisic.postVitBitCount);

				printf("Abort Count  : %d\n", statisic.abortCount);
			}
			printf("*****************************\n");

		}
		
		sleep(1);
		get_times++;
	}
}

void Endeavour_Dibcom_Detail_Channel_Info(int handle) {
	Dword dwError = 0;
	DibcomDiversityData dibValue;
	int max_fe = 4;
	Bool bIsLock = 0;
	
	dibValue.chip = 0;
		
	dwError = DTV_IsLocked(handle, &bIsLock);
	if(dwError){
		printf("DTV_IsLocked fail [0x%08lx]\n", dwError);
	}	
	
	if(!bIsLock) {
		printf("Signal UNLOCk, should lock particular frequency at first!\n\n");
		return;
	}
	
	
	dwError = DTV_Endeavour_Dibcom_Detail_Channel_Info(handle, &dibValue);	
	if(dwError){
		printf("Get Dibcom channel Info fail...\n");
	} else {
		printf("Get Dibcom channel Info Success...\n");
		
		if(Dibcom_Detail_TransmissionParameters(&dibValue, max_fe))
			return;
		Dibcom_Detail_ADCpower(&dibValue, max_fe);
		Dibcom_Detail_IQ_Mismatch(&dibValue, max_fe);
		Dibcom_Detail_AGC_info(&dibValue, max_fe);
		Dibcom_Detail_SignalOffset(&dibValue, max_fe);
		Dibcom_Detail_PacketError(&dibValue, max_fe);
		Dibcom_Detail_MER(&dibValue, max_fe);
		Dibcom_Detail_CN(&dibValue, max_fe);
		Dibcom_Detail_SignalQuality(&dibValue,max_fe);

	}	
}

void Endeavour_Get_Statistic_Menu(int handle){
	int chose_function = 0, target_times = 0, res = 0;
	res += 0;
	printf("\n1. Display Statistics");
	printf("\n2. Monitor Statistics");
	if(g_chipType == 7 || g_chipType == 10)
		printf("\n3. Dibcom Diversity Detail Statistics");	
	printf("\n0. Return to Main Menu");
	printf("\n=> Please Input Your Choice: ");
	//res = scanf("%d", &chose_function);
	
	switch(chose_function)
	{
		case 1:
			target_times = 1;
			Get_Statistic(handle, target_times);
			break;
		case 2:
			printf("\n=> Monitor Times:");
//			res = scanf("%d", &target_times);
			Get_Statistic(handle, target_times);
			break;
		case 3:
			if(g_chipType == 7 || g_chipType == 10) {
		 		Endeavour_Dibcom_Detail_Channel_Info(handle);
			}
			break;		
		default:
			break;
	}
	
}

void Get_Packet(int handle, int target_times, int is_save){
	FILE *save_file = NULL;
	PIDINFO pidt[PID_SIZE];
	int pid_count = 0, get_times = 0, read_length = 0, return_length = 0, retry = 0, pid = 0, packets = 0;
	Dword result = ERR_NO_ERROR;
	//unsigned char buffer[READBUFFER_SIZE];
	unsigned char *buffer;
	unsigned char BlockBuffer[BLOCK_BUFFER_SIZE];
	struct timeval time_start, time_pre, time_now;
	uint8_t checkbit = 0, adaptation_field = 0, seq = 0;
	uint32_t time_diff;
	char key;
	Bool BufferIsEmpty = True;
	int ReadCounter = 0, ReadTime = BLOCK_BUFFER_SIZE/188;
	
	if(is_save){
		save_file = fopen("DVB.ts", "wb");
		if(!save_file){
			printf("DVB.ts create fail\n");
			is_save = 0;
		}
	}
	
	 
	
	/* Start capture */
	result = DTV_StartCapture(handle);
	if(result){
		printf("DTV_StartCapture fail [0x%08lx]\n", result);
		
		if(save_file){
			fclose(save_file);
		}
		return;
	}

	gettimeofday(&time_start, 0);
	time_pre = time_start;
	
	if(target_times == -1) get_times = -2;
	
	/* Set Read Length */
#ifdef BLOCK_MOD
	read_length = BLOCK_BUFFER_SIZE;//188 /*188*816*/
	printf("======================>>> Block Mod\n");
#else
	read_length = READBUFFER_SIZE;
	printf("======================>>> 188 Mod\n");
#endif
	
	while(exit_flag != 1){

	

		/*188 MOD*/
		//return_length = DTV_GetData(handle, read_length, buffer);	
		// printf("return_length = %d\n",return_length );		
			
		 sleep(1);
		 printf(" packet get \n");
		
		retry = 0;		
 
		 
	}//while
	printf("return packet get \n");
	//DTV_StopCapture(handle);

	 
}

void Endeavour_Get_Packet_Menu(int handle){
	int chose_function = 0, target_times = 0, res = 0;
	res += 0;
	printf("\n1. TS Capture and Analyze (Return Channel)");
	printf("\n2. Save Packet to File");
	printf("\n3. Return to Main Menu");
	printf("\n=> Please Choice : ");
	
#ifndef AUTO_MOD
//	res = scanf("%d", &chose_function);
#else
	chose_function = 1;
#endif
	
	switch(chose_function){
		case 1:
			Get_Packet(handle, -1, 0);
			break;
		
		case 2:
			printf("\n=> File Size (MB): ");
//			res = scanf("%d", &target_times);
			
			Get_Packet(handle, target_times, 1);
			break;	
		
		default:
			return ;
	}
}

void Endeavour_RW_Register(int handle){
	int RW_type = 0, processor_type = 0, res = 0, device = 0;
	uint32_t reg_addr = 0, value = 0;
	Dword result = ERR_NO_ERROR;
	res += 0;
	printf("\n=> Please Choose Control Device(0: Endeavour, 1: RX): ");
//	res = scanf("%d", &device);
	
	printf("\n=> Please Choose Read/Write Register (0: Read, 1: Write): ");
//	res = scanf("%d", &RW_type);
		
	if(device == 1)
	{
		printf("\n=> Please Choose LINK or OFDM (0: LINK, 1: OFDM): ");
//		res = scanf("%d", &processor_type);
	}
	
	printf("\n=> Please Enter Read/Write Register Address (Hex): ");
//	res = scanf("%x", &reg_addr);
	
	if(RW_type == 1){ // Write register
		printf("\n=> Please Enter Write Value (Hex): ");
//		res = scanf("%x", &value);
	}
	
	if(device == 0)
		result = DTV_EndeavourRegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	else
		result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	
	if(result){
		printf("DTV_RegisterControl fail [0x%08lx]\n", result);
	}
	else{
		printf("DTV_RegisterControl %s %s Address[0x%X] Value[0x%x] success\n",
			(RW_type ? "write" : "read"),
			(processor_type ? "OFDM" : "LINK"),
			reg_addr,
			value);
	}
}

void Endeavour_Get_Driver_Info(int handle)
{
	Dword result = ERR_NO_ERROR;
	DemodDriverInfo driver_info;
	Word RxDeviceID = 0xFFFF;
	
	result = DTV_GetDeviceID(handle, &RxDeviceID);
	if(result)
	{
		printf("DTV_GetDeviceID fail [0x%08lx]\n", result);
		return ;
	}
		
	result = DTV_GetDriverInformation(handle, &driver_info);
	if(result){
		printf("DTV_GetVersion fail [0x%08lx]\n", result);
	}
	else{
		printf("======== ITEtech Linux DTV Information ========\n");
		printf("DriverVerion  = %s\n", driver_info.DriverVersion);
		printf("APIVerion     = %s\n", driver_info.APIVersion);
		printf("FWVerionLink  = %s\n", driver_info.FWVersionLink);
		printf("FWVerionOFDM  = %s\n", driver_info.FWVersionOFDM);
		printf("Company       = %s\n", driver_info.Company);
		printf("SupportHWInfo = %s\n", driver_info.SupportHWInfo);
		printf("RxDeviceID    = 0x%x\n", RxDeviceID);
		printf("Board ID      = 0x%02X\n", driver_info.BoardId);
		printf("chip_Type     = 0x%02X\n", driver_info.chip_Type);		
		printf("===============================================\n");
		
		g_chipType = driver_info.chip_Type;
	}
}

void Endeavour_Set_PID_Filter(int handle)
{
	Dword result = ERR_NO_ERROR;
	int chose_function = 0, res = 0, pid_index = 0;
	Word program_id = 0;
    res += 0;
    printf("\n1. Enable PID Filer");
	printf("\n2. Disable PID Filer");
	printf("\n3. Add PID to Filter");
	printf("\n4. Remove PID to Filter");
	printf("\n5. Reset PID Filter");
	printf("\n6. Return to Main Menu");
	printf("\n=> Please Input Your Choice: ");
//	res = scanf("%d", &chose_function);
    
    switch(chose_function){
		case 1:
			result= DTV_EnablePIDTable(handle);
			if(result){
				printf("DTV_EnablePIDTable fail [0x%08lx]\n", result);
			}
			break;
		
		case 2:
			result = DTV_DisablePIDTable(handle);
			if(result){
				printf("DTV_DisablePIDTable fail [0x%08lx]\n", result);
			}
			break;
		
		case 3:
			printf("\n=> Please Input set PID index (0-31): ");
	//		res = scanf("%d", &pid_index);
			
			printf("\n=> Please Input Program ID: ");
	//		res = scanf("%hx", &program_id);
			
			result = DTV_AddPID(handle, (Byte)pid_index, program_id);
			if(result){
				printf("DTV_AddPID fail [0x%08lx]\n", result);
			}
			break;
		
		case 4:
			printf("\n=> Please Input set PID index (0-31): ");
	//		res = scanf("%d", &pid_index);
			
			printf("\n=> Please Input Program ID: ");
	//		res = scanf("%hx", &program_id);
			
			result = DTV_RemovePID(handle, (Byte)pid_index, program_id);
			if(result){
				printf("DTV_RemovePID fail [0x%08lx]\n", result);
			}
			break;
		
		case 5:
			result = DTV_ResetPIDTable(handle);
			if(result){
				printf("DTV_DisablePIDTable fail [0x%08lx]\n", result);
			}
			break;
		
		default:
			return ;
	}
}

void Endeavour_NULLPacket_Filter(int handle, int flag)
{
	Dword result = ERR_NO_ERROR;
	PIDFilter PIDfilter;
	
	PIDfilter.oldPID[0] = 0x1FFF;
	PIDfilter.newPID[0] = 0x1FFF;	
	
	PIDfilter.tableLen = (flag) ? 1 : 0;
	
	result = DTV_NULLPacket_Filter(handle, &PIDfilter);
	
	if(result == 0)
			printf("\n=> %s NULL Packet Filter success !\n", (PIDfilter.tableLen) ? "Enable" : "Disable");
		else
			printf("\n=> %s NULL Packet Filter Fail.....\n", (PIDfilter.tableLen) ? "Enable" : "Disable");
		
	return;
}

void Endeavour_EEPROM_read(int handle, Byte *rBuffer, Word wAddress){
	int res = 0;
	Word wRegAddr;
	
	wRegAddr = wAddress;
		
	res = DTV_ReadEEPROM(handle, wRegAddr, rBuffer);
	
	if(res){
		printf("Read EEPROM Fail...\n");
	}		
		//printf("[%d] = %d, ", i, readbuffer);
		//if(i % 10 == 0) printf("\n");
	
	return;
}

void Endeavour_EEPROM_write(int handle, Byte *wBuffer){
	int res = 0;
	
	res = DTV_WriteEEPROM(handle, wBuffer);
	
	if(res){
		printf("Write EEPROM Fail...\n");
	} else {
		printf("Write EEPROM Success. \n");
	}
	
	return;
}

void Endeavour_EEPROM_RxID_editor(int handle){
	int mene_flag = 1, res = 0, select_fun = 0;
	int curr_rxid_mapping = 0;
	int curr_rx_dev_id = 0, i = 0;
	Byte rbuf[1];
	Byte eeprom_buf[256];
	
	/* 1: Get EEPROM current value */				
	Endeavour_EEPROM_read(handle, rbuf, 80);
	curr_rxid_mapping = rbuf[0];
	
	Endeavour_EEPROM_read(handle, rbuf, 81);
	curr_rx_dev_id += rbuf[0] << 8;	
	
	Endeavour_EEPROM_read(handle, rbuf, 82);
	curr_rx_dev_id += rbuf[0];
	
	while(mene_flag) {
		printf("\nCurrent RxID mapping = %d, RxID = %d", curr_rxid_mapping, curr_rx_dev_id);
		
		printf("\n============== RxID Editor =============	");
		printf("\n1. RxID Mapping(0: Disable, 1: Enable):	");
		printf("\n2. Set RxID(0~31):							");
		printf("\n3. Write EEPROM Table							");
		printf("\n0. Quit                      				");
		printf("\n========================================\n");
		
		printf("=> Please Input Your Choice: ");
	//	res = scanf("%d", &select_fun);
		switch(select_fun){
			case 1:
				printf("=> Please Input RxID Mapping(0: Disable, 1: Enable): ");
	//			res = scanf("%d", &curr_rxid_mapping);
				
				break;
				
			case 2:
				printf("=> Please Input RxID(0~31): ");
	//			res = scanf("%d", &curr_rx_dev_id);
				
				break;
				
			case 3:
				for(i = 0; i < 256; i++) {
					Endeavour_EEPROM_read(handle, rbuf, i);
					
					eeprom_buf[i] = rbuf[0];
				}
				
				/* 2: Modify EEPROM value */
				if(curr_rxid_mapping)						
					eeprom_buf[80] = 1;
				else 
					eeprom_buf[80] = 0;
					
				eeprom_buf[81] = curr_rx_dev_id >> 8;
				eeprom_buf[82] = curr_rx_dev_id;
				
				/* 3: Write EEPROM Table table to eeprom */	
				Endeavour_EEPROM_write(handle, eeprom_buf);
				break;
				
			case 0:	
				mene_flag = 0;
				break;
				
			default:{
				break;
			}
		}
	}
	
	return;
}

void Endeavour_EEPROM_raw_data_editor(int handle){
	int mene_flag = 1, res = 0, select_fun = 0, i = 0;
	Byte eeprom_buf[256];
	Byte rbuf[0];
	int if_read_eeprom = 0;
	uint32_t input_add = 0, input_val = 0;
	
	while(mene_flag) {
		printf("\n======= Raw Data Editor =======");
		printf("\n1. Read EEPROM Table        	   ");
		printf("\n2. Modify EEPROM Value			");
		printf("\n3. Write EEPROM Table	  			");
		printf("\n0. Quit                      	");
		printf("\n================================\n");
		
		printf("=> Please Input Your Choice: ");
	//	res = scanf("%d", &select_fun);
		
		switch(select_fun){				
			case 1:
				for(i = 0; i < 256; i++) {
					Endeavour_EEPROM_read(handle, rbuf, i);
					
					eeprom_buf[i] = rbuf[0];
				}
				if_read_eeprom = 1;
				
				printf("\n");
				
				for(i = 0; i < 256; i++) {
					printf("0x%02x, ", eeprom_buf[i]);
					if(((i+1) % 16) == 0) printf("\n");
				}
				break;		

			case 2:
				if(!if_read_eeprom) {
					printf("\nPlease read EEPROM first...!\n");
					break;
				}
			
				printf("Please Input address(0x): ");
	//			res = scanf("%x", &input_add);
				
				printf("Please Input Value: ");
	//			res = scanf("%x", &input_val);
				
				eeprom_buf[input_add] = input_val;
				
				printf("\nModify EEPROM value ok! \n");				
				break;					
			
			case 3:	
				if(!if_read_eeprom) {
					printf("\nPlease read EEPROM first...!\n");
					break;
				}
				Endeavour_EEPROM_write(handle, eeprom_buf);
				
				printf("\nWrite EEPROM table...ok!\n");
				break;
			
			case 0:
				mene_flag = 0;
				break;
				
			default:{		
				break;
			}
		}
	}	
	return;
}

void Endeavour_EEPROM_editor_menu(int handle){
	int editor_flag = 1, select_fun = 0, res = 0;
	
	while(editor_flag){
		printf("\n======= EEPROM Editor Menu =======");
		printf("\n1. RxID Editor              ");
		printf("\n2. Raw Data Editor  			");
		printf("\n0. Quit                      ");
		printf("\n==================================\n");
		
		printf("=> Please Input Your Choice: ");
	//	res = scanf("%d", &select_fun);
	
		switch(select_fun){
			case 1:
				Endeavour_EEPROM_RxID_editor(handle);
				break;
		
			case 2:					
				Endeavour_EEPROM_raw_data_editor(handle);
				break;
								
			case 0:
				editor_flag = 0;
				break;
				
			default:{
				break;
			}
		}			
	}
	return;
}

void Endeavour_Get_Board_Input_Power(int handle) {
	Byte value;
	int res = 0;
	
	res = DTV_GetBoardInputValue(handle, &value);	
	
	if(res){
		printf("Read board input power fail...\n");
	} else {
		printf("Read board input power Success \n");
				
		printf("\n======== Board Input Power ========\n");
		printf(" %d dbm \n", (value - 100));
	}	
}

void Endeavour_RX_Reset(int handle) {
	Byte value;
	int res = 0;
	
	value = 1;	//NO USE
		
	res = DTV_RX_Reset(handle, &value);
	
	if(res){
		printf("RX RESET fail...\n");
	} else {
		printf("RX RESET Success \n");		
	}	
}

void Endevour_Print_registers(int handle){
	int RW_type = 0, processor_type = 0;
	uint32_t reg_addr = 0, value = 0;
	Dword result = ERR_NO_ERROR;	
	DTVStatistic statisic;
	
	//OFDM
	processor_type = 1;
	
	reg_addr = 0xF27C;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("LNA  = %d \n", value);
	
	reg_addr = 0xF27D;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("PGC  = %d \n", value);
	
	reg_addr = 0xF2A6;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("PGA1  = %d \n", value);
	
	reg_addr = 0xF2A7;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("PGA2  = %d \n", value);
	
	result = DTV_GetStatistic(handle, &statisic);	
	if(statisic.postVitBitCount != 0)
		printf("BER  = %3.3e \n", ((float)statisic.postVitErrorCount) / statisic.postVitBitCount);
	else
		printf("BER  = %3.3e \n", (double) statisic.postVitBitCount);
		
	printf("RSD abort  = %d \n", statisic.abortCount);
	
	reg_addr = 0x00F7;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("P_inband  = %d \n", value);
	
	reg_addr = 0x0150;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("P_total  = %d \n", value);
	
	reg_addr = 0xF110;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("CCI0  = %d \n", value);
	
	reg_addr = 0xF114;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("CCI1  = %d \n", value);
	
	reg_addr = 0x0043;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("aci0  = %d \n", value);
	
	reg_addr = 0x0044;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("aci1  = %d \n", value);
	
	reg_addr = 0x00C1;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("aci2  = %d \n", value);
	
	reg_addr = 0x00C7;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("aaci  = %d \n", value);
	
	reg_addr = 0x005E;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("channel  = %d \n", value);
	
	reg_addr = 0xFD37;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("FDI type  = %d \n", value);
	
	reg_addr = 0xF279;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("Rssi_power  = %d \n", value);
	
	reg_addr = 0x00AF;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("Signal_level  = %d \n", value);
	
	reg_addr = 0xF02B;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("Mobile  = %d \n", value);
	
	reg_addr = 0xFDA2;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("tdi_lms  = %d \n", value);
	
	reg_addr = 0x003C;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("tps lock  = %d \n", value);
	
	reg_addr = 0xF999;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("mpeg lock  = %d \n", value);
	
	reg_addr = 0x0046;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("tuner ID  = %d \n", value);
	
	reg_addr = 0x0128;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("LNA_max  = %d \n", value);
	
	reg_addr = 0x0129;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("P_total_Max  = %d \n", value);
	
	reg_addr = 0x00A7;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("timging_error  = %d \n", value);
	
	reg_addr = 0xF10F;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("ccif0_fcwccif_13_8  = %d \n", value);
	
	reg_addr = 0xF10E;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("ccif0_fcwccif_7_0  = %d \n", value);
	
	reg_addr = 0xF113;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("ccif1_fcwccif_13_8  = %d \n", value);
	
	reg_addr = 0xF112;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("ccif1_fcwccif_7_0  = %d \n", value);

//LINK	
	processor_type = 0;
	
	reg_addr = 0xD8C7;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("gpioh6_o  = %d \n", value);
	
	reg_addr = 0xF6AF;
	result = DTV_RegisterControl(handle, RW_type, processor_type, (Word)reg_addr, (Byte *)&value);
	printf("ofdm2lnk_63_56  = %d \n", value);	
	
	
	printf("=========================================> \n\n");


}

void Endeavour_Get_Board_ID(int handle) {
	Byte byValue;
	int iRes = 0;

	iRes = DTV_Endeavour_GetBoardID(handle, &byValue);	

	if(iRes){
		printf("Get Board ID fail...\n");
	} else {
		printf("Get Board ID Success \n");

		printf("Board ID =  %u\n", byValue);
	}	
}

int device_open(int num)
//int main(int argc, char **argv)
{
	Byte handle_number = 0;
	int handle = 0, menu_flag = 1, chose_function = 0, res = 0, ctrlflag = 0;
	HandleType handle_type;
	res += 0;
	printf("======== IT930x Testkit Information ========\n");
	printf("Testkit Version = 0x%x\n", TESTKIT_VERSION);
	printf("DTVAPI Version  = 0x%x\n", DTVAPI_VERSION);
	printf("============================================\n\n");
	
	/* Check handle number */
	/*if(argv[1] == NULL){
		printf("================== Open default device handle ====================\n");
		printf("=== To chose another driver handle. Please input handle number ===\n");
		printf("== Example: ./it930x_testkit_native 1 -> for usb-it930x1 handle ==\n");
		printf("==================================================================\n\n");
		handle_number = 0;
	} 
	else{
		handle_number = atoi(argv[1]);
		if(atoi(argv[1]) < 0){
			printf("================ Bad handle number! Please input again! ==========\n");
			printf("================== To chose driver handle sample =================\n");
			printf("== Example: ./it930x_testkit_native   -> for usb-it930x0 handle ==\n");
			printf("== Example: ./it930x_testkit_native 1 -> for usb-it930x1 handle ==\n");
			printf("==================================================================\n\n");
			return 0;
		}
	}*/
	
	/* Device init & open handle */
	handle_type = ENDEAVOUR;
	handle = DTV_DeviceOpen(handle_type, num);
	if(handle < 0){
		printf("DTV_DeviceOpen(%d) fail\n", num);
		return -1;
		//goto EXIT;
	}
	
	g_handle = handle;
	 
	/* Read driver information */
	//Endeavour_Get_Driver_Info(handle);
	
	/* Endeavour NULL Packet PID Filter (Default Enable)*/
	//Endeavour_NULLPacket_Filter(handle, 1);
	
	//
	if(Endeavour_Lock_Channel(handle) == 0)
	{
		return -2;
	}
 	if(DTV_StartCapture(handle) !=0)
	{
		return -3;
	}
	return 0;
}

void DTV_Close()
{
        exit_flag =1;
	//DTV_StopCapture(1);
	DTV_DeviceClose(g_handle);
}




