#ifndef _IT930x_H_
#define _IT930x_H_

#define _GNU_SOURCE

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/stddef.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/reboot.h>
#include <linux/notifier.h>

#include <asm/uaccess.h>
#include "iocontrol.h"
#include "IT9300.h"
#include "IT9133.h"
#include "userdef.h"
#include "type.h"
#include "brType.h"

#include "firmware.h"
#include "firmware_V2.h"
#include "firmware_V2I.h"
#include "version.h"
#include "brfirmware.h"
#include "brRegister.h"

#include "usb2impl.h"

#include "dibx09x_common.h"

#include <linux/stddef.h>
#include <linux/delay.h>
#include <linux/types.h>

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 39)
	#include <linux/smp_lock.h>
#endif

/* Get a minor range for your devices from the usb maintainer */
#ifdef CONFIG_USB_DYNAMIC_MINORS
	#define USB_it930X_MINOR_BASE	0
#else
	#define USB_it930X_MINOR_BASE	192
#endif

#define DRIVER_RELEASE_VERSION	"v16.08.05.1"

/*********************Rx_ID_Length***************/
#define RX_ID_LENGTH 5

/***************** customization *****************/

#define URB_COUNT   4
#define BR_URB_COUNT   4

#define DEBUG 1
#if DEBUG 
	#define deb_data(args...)   printk(KERN_NOTICE args)
#else
	#define deb_data(args...)
#endif

/***************** from compat.h *****************/

#if LINUX_VERSION_CODE <=  KERNEL_VERSION(2,6,18)
typedef int bool;
#define true	1
#define false	0
#endif

/***************** from device.h *****************/
#define SLAVE_DEMOD_2WIREADDR		0x3A

#define TS_PACKET_SIZE				188
#define TS_PACKET_COUNT_FU			21

/***************** from driver.h *****************/
#define TS_FRAMES_HI 128
#define TS_FRAMES_FU 128
#define MAX_USB20_IRP_NUM  5
#define MAX_USB11_IRP_NUM  2

/***************** from afdrv.h *****************/
//#define GANY_ONLY 0x42F5
#define EEPROM_FLB_OFS  8

#define EEPROM_IRMODE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x10)	//00:disabled, 01:HID
#define EEPROM_SELSUSPEND	(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28)	//Selective Suspend Mode
#define EEPROM_TSMODE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 1)	//0:one ts, 1:dual ts
#define EEPROM_2WIREADDR		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 2)	//MPEG2 2WireAddr
#define EEPROM_SUSPEND		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 3)	//Suspend Mode
#define EEPROM_IRTYPE		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 4)	//0:NEC, 1:RC6
#define EEPROM_SAWBW1		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 5)
#define EEPROM_XTAL1			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 6)	//0:28800, 1:20480
#define EEPROM_SPECINV1		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x28 + 7)
#define EEPROM_TUNERID		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 4)
#define EEPROM_IFFREQL		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30) 
#define EEPROM_IFFREQH		(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 1)   
#define EEPROM_IF1L			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 2)   
#define EEPROM_IF1H			(OVA_EEPROM_CFG + EEPROM_FLB_OFS + 0x30 + 3)
#define EEPROM_SHIFT			(0x10)	//EEPROM Addr Shift for slave front end
#define EEPROM_EVBNUM		(OVA_EEPROM_CFG + 0x38)	//0x4994 + 0x38
#define EEPROM_CHIPTYPE_1stNLC_1	(OVA_EEPROM_CFG + 0x31)
#define EEPROM_CHIPTYPE_1stNLC_2	(OVA_EEPROM_CFG + 0x32)
#define EEPROM_CHIPTYPE_1stNLC_3	(OVA_EEPROM_CFG + 0x35)
#define EEPROM_CHIPTYPE_1stNLC_4	(OVA_EEPROM_CFG + 0x36)
#define EEPROM_CHIPTYPE_otherNLC	(OVA_EEPROM_CFG + 0x37)
#define EEPROM_BOARDID	(OVA_EEPROM_CFG + 0x54)

#define to_afa_dev(d) container_of(d, IT930x_Device it930x_dev, kref)

//belowd enum is for recognizing different receiver from EEPROM
/**********************************************************************
* 
*             Chip Type                   |    enum name 
* ----------------------------------------|--------------------
*  chip_Type[][] = 1 : IT913X             |    EEPROM_IT913X
*                = 5 : Si2168B            |    EPROM_Si2168B
*                = 6 : MxL608 + Atbm782x  |    EEPROM_MxL608_Atbm782x
*                = 7 : Dibcom9090         |    EEPROM_Dibcom9090
*                = a : Dibcom9090_4chan   |    EEPROM_Dibcom9090_4chan
*
************************************************************************/
typedef enum{
	EEPROM_IT913X	= 1,
	EEPROM_Si2168B = 5,
	EEPROM_MxL608_Atbm782x,
	EEPROM_Dibcom9090,
	EEPROM_Dibcom9090_4chan = 10,
}EEPROM_RECEIVER_TYPE;


/***************** debug message *****************/
//define EEPROM_DEBUG_MESSAGE
//define PROBE_DEBUG_MESSAGE

//#define CHECK_SHIFT	//check buffer shift

extern struct usb_device *udevs;

/* AirHD no use, RC, after kernel 38 support */
struct it930x_config {
	u8  dual_mode:1;
	u16 mt2060_if1[2];
	u16 firmware_size;
	u16 firmware_checksum;
	u32 eeprom_sum;
};

struct it930x_ofdm_channel {
	u32 RF_kHz;
	u8  Bw;
	s16 nfft;
	s16 guard;
	s16 nqam;
	s16 vit_hrch;
	s16 vit_select_hp;
	s16 vit_alpha;
	s16 vit_code_rate_hp;
	s16 vit_code_rate_lp;
	u8  intlv_native;
};

/* AirHD no use, RC, after kernel 38 support */
struct tuner_priv {
	struct tuner_config *cfg;
	struct i2c_adapter *i2c;
	
	u32 frequency;
	u32 bandwidth;
	u16 if1_freq;
	u8  fmfreq;
};

typedef struct _TUNER_INFO {
    Bool bTunerInited;
    Bool bSettingFreq;
    Byte TunerId;
    Bool bTunerOK;
	
	Bool bTunerLock;//AirHD
	
    //Tuner_struct MXL5005_Info;

} TUNER_INFO, *PTUNER_INFO;

typedef struct _FILTER_CONTEXT_HW {
    Dword ulCurrentFrequency;
    Word  ucCurrentBandWidth;  
    Dword ulDesiredFrequency;
    Word  ucDesiredBandWidth;     
    Bool bTimerOn;
    Byte GraphBuilt;
    TUNER_INFO tunerinfo; 
    int  bEnPID;
    Bool bApOn;
    int bResetTs;
    Byte OvrFlwChk;
} FILTER_CONTEXT_HW, *PFILTER_CONTEXT_HW;

struct DIB9090_DEVICE {
	struct dibDataBusHost *i2c;
	struct dibFrontend frontend[4]; //dibFrontend
	struct dibDemodMonitor monitor[4]; //dibDemodMonitor
	Dword frequency;
	Word bandwidth;
};

typedef struct _DEVICE_CONTEXT{
    FILTER_CONTEXT_HW fc[2];
    struct usb_device *usb_dev;
    
    Byte DeviceNo;
    Bool bBootCode;
    Bool bEP12Error;
    Bool bEP45Error;
    Bool bDualTs;
    Bool bIrTblDownload;    
    Byte BulkOutData[256];
    u32 WriteLength;
    Bool bSurpriseRemoval;
    Bool bDevNotResp;
    Bool bEnterSuspend;
    Bool bSupportSuspend;
    Bool bSupportSelSuspend;
    u16 regIdx; 
    Byte eepromIdx;
    u16 UsbMode;
    u16 MaxPacketSize;
    u32 MaxIrpSize;
    u32 TsFrames;
    u32 TsFrameSize;
    u32 TsFrameSizeDw;
    u32 TsPacketCount;
    Bool bSelectiveSuspend;
    u32 ulActiveFilter;
    Architecture architecture;
    StreamType StreamType;
    Bool bDCAPIP;
    Bool bSwapFilter;
  
    Bool bTunerPowerOff;
    Byte UsbCtrlTimeOut;
	
	//Endeavour endeavour;
	Byte it9300_Number;
	Byte rx_number;
	Endeavour it9300;
	DefaultDemodulator it9133[4][5];
	Byte Si2168B_standardName[4][5];
	Dword *Si2168B_FE[5];
	Byte is_rx_init[4][5];
	Byte chip_Type[4][5];
	
	Word rx_device_id[8];
	
    Bool ForceWrite;
    Byte chip_version;
    Bool bProprietaryIr;
    Byte bIrType;
	Byte *temp_read_buffer;
	Byte *temp_write_buffer;
	Byte *temp_urb_buffer;
	bool power_state[2];
	Dword parsebufflen;
	Dword device_id;
	Dword board_id;
	
	char filter_name[16][40];
	bool map_enalbe;
	Byte hihg_byte;
	Byte low_byte;	
	
	atomic_t filter_count;
	bool auto_regist;
	
	struct mutex dev_mutex;
	
	int if_degug;

	struct DIB9090_DEVICE dib9090;
} Device_Context;

/* Structure for urb context */
typedef struct _RX_URB_CONTEXT{
	//struct file *file;
	void *br_chip;
	Byte index;
} Rx_Urb_Context;

/* Structure to hold all of our device specific stuff */
typedef struct _CHIP{
	Byte chip_index;
	
	bool urb_streaming;
	struct urb *urbs[BR_URB_COUNT];	
	Rx_Urb_Context urb_context[BR_URB_COUNT];	
	Byte urbstatus[BR_URB_COUNT];
	Byte *pRingBuffer;
	Byte *br_pRingBuffer[BR_URB_COUNT];
	Dword dwTolBufferSize;
	Byte urb_count;
	Dword CurrBuffPointAddr;
	Dword ReadBuffPointAddr;
	Dword dwReadRemaingBufferSize;			
	
	spinlock_t read_buf_lock;
	void *dev;
	
	///for WriteRingBuffer	
	Dword TxWriteBuffPointAddr;
	Dword dwTxRemaingBufferSize;
	
	int handle_startcapture[4];		//chip open flag
	bool if_chip_start_capture;		//chip open flag	
	Byte chip_open_count;
	Byte acq_transfer_count;	//acq_transfer_count
	
	bool if_search_syncbyte;
	
	wait_queue_head_t read_wait_queue;
	bool g_if_wait_read;
	int read_wait_len;
} Chip;

typedef struct _IT930x_DEVICE{
	struct usb_interface *interface;
	struct kref kref;
	struct file *file[16];
	Device_Context *DC;
    struct notifier_block reboot_notifier;  /* default mode before reboot */
	Chip *chip[16];
	Chip *br_chip;
	Byte minor[16];
	bool Tuner_inited[4];
	Byte syncByte;
	bool disconnect;
	bool SetNULLPacket;
	
	///for returnchannel
	unsigned long DealWith_ReturnChannelPacket;
} IT930x_Device;

typedef struct _FILE_INFORMATION{
	IT930x_Device *dev;
	Byte chip_index;
	bool if_file_start_capture; //ap open flag
} File_Information;


extern int it930x_device_count;
extern int dvb_usb_it930x_debug;
extern Dword IsLock(Device_Context *DC, Bool* locked, int br_idx, int ts_idx);
//extern void UsbBus(Device_Context *DC);
//extern Dword Device_init(Device_Context *DC, Bool bBoot);
//extern Dword DL_ApCtrl(Device_Context *DC, Byte chip_index, Bool bOn);
//extern Dword DL_CheckTunerInited(Device_Context *DC, Byte chip_index, bool *Tuner_inited);
extern Dword DL_GetEEPROMConfig(Device_Context *DC);
extern Dword DL_device_communication_test(Device_Context *DC);
extern Dword DL_IT930x_device_init(Device_Context *DC);
extern Dword DL_IT930x_device_deinit(Device_Context *DC);



extern Dword DL_DemodIOCTLFun(void* demodulator, Dword IOCTLCode, unsigned long pIOBuffer, Byte chip_index);

extern Dword DL_Demodulator_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, int br_idx, int ts_idx);
extern Dword DL_Demodulator_isLocked (Device_Context *DC, Bool* locked, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getStatistic(Device_Context *DC, Statistic* statistic, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getChannelStatistic (Device_Context *DC, Byte chip, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DL_Demodulator_writeRegisters(Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx);
extern Dword DL_Demodulator_readRegisters(Device_Context *DC, Processor processor, Dword registerAddress, Byte bufferLength, Byte* buffer, int br_idx, int ts_idx);
extern Dword DL_Demodulator_resetPidFilter(Device_Context *DC, int br_idx, int ts_idx);
extern Dword DL_Demodulator_controlPidFilter(Device_Context *DC, Byte control, int br_idx, int ts_idx);
extern Dword DL_Demodulator_addPidToFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx);
extern Dword DL_Demodulator_removePidFromFilter(Device_Context *DC, Byte index, Word value, int br_idx, int ts_idx);
extern Dword DL_Demodulator_getBoardInputPower(Device_Context *DC, Byte* boardInputPower, int br_idx, int ts_idx);

extern Dword DRV_IT913x_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx);


extern Dword DRV_Si2168B_Initialize(Device_Context *DC, Byte br_idx, Byte tsSrcIdx);
extern Dword DRV_Si2168B_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_getStatistic(Device_Context *DC, Statistic *statistic, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_getChannelStatistic (Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DRV_Si2168B_isLocked(Device_Context *DC, Bool *locked, int br_idx, int ts_idx);



extern Dword DL_ResetRxPort(Device_Context *DC, int is_open);

extern Dword DL_Dib9090_getDiversityDetailData(Device_Context *DC, DibcomDiversityData* data, Byte byChipType);

extern Dword DRV_Dib9090_Initialize(Device_Context *DC, Byte byChipType);
extern Dword DRV_Dib9090_acquireChannel(Device_Context *DC, Word bandwidth, Dword frequency, Byte byChipType);
extern Dword DRV_Dib9090_getStatistic(Device_Context *DC, Statistic *statistic, Byte byChipType, Bool bIsRescan);
extern Dword DRV_Dib9090_getChannelStatistic (Device_Context *DC, ChannelStatistic* channelStatistic, int br_idx, int ts_idx);
extern Dword DRV_Dib9090_isLocked(Device_Context *DC, Bool *locked, Byte byChipType);
extern Dword DRV_Dib9090_DeInitialize(Device_Context *DC, Byte byChipType);
extern Dword DRV_Dib9090_getDiversityDetailData(Device_Context *DC, DibcomDiversityData* data, Byte byChipType);

extern Dword DRV_ResetRxPort(Device_Context *DC, int is_open);

#endif
