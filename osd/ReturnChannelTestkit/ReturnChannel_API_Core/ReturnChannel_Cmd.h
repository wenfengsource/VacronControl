#ifndef RETURN_CHANNEL_CMD_H
#define RETURN_CHANNEL_CMD_H

#define UART_TEST													1
#define RETURN_CHANNEL_VERSION										0x15120300	// yy/mm/dd/version
#define RETURN_CHANNEL_PID											0x201
#define RETURN_CHANNEL_API_VERSION									0x0018			// Version
#define RETURN_CHANNEL_BAUDRATE										9600			




#ifdef WIN32

#include <windows.h>

#include <process.h>
#endif


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __GNUC__

/* *

 * for Linux 

 */

/* define NULL, true, flase */

#include <linux/stddef.h>

/* define bool */

#include <linux/types.h>

/* include Linux delay */

//#include <linux/delay.h>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <pthread.h>

#endif

#ifndef ModulatorError_NO_ERROR
#define ModulatorError_NO_ERROR	0
#endif

#ifndef __MODULATOR_TYPE_H__

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef __TYPE_H__
/**
 * The type of handle.
 */
typedef void* Handle;


/**
 * The type defination of 8-bits unsigned type.
 */
typedef unsigned char Byte;


/**
 * The type defination of 16-bits unsigned type.
 */
typedef unsigned short Word;

/**
 * The type defination of 32-bits unsigned type.
 */
typedef unsigned long Dword;

/**
 * The type defination of 16-bits signed type.
 */
typedef short Short;


/**
 * The type defination of 32-bits signed type.
 */
typedef long Long;

/**
 * The type defination of Bool
 */

typedef enum {
    False = 0,
    True = 1
} Bool
#ifdef WIN32
#ifndef __cplusplus
,bool
#endif
#endif
;
#endif
#endif

typedef unsigned short SWord;

/**
 * The type defination of 64-bits unsigned type.
 */
typedef unsigned long long Qword;

/**
 * The type defination of 32-bits signed type.
 */
typedef unsigned int Float;


#ifdef __linux__
#define THREAD  void*
#else
#define THREAD   unsigned __stdcall
#endif
//---------for return channel
#define ReturnChannelError_RerurnCode_Error							0x05000000ul // 0x05000000ul | RerurnCode
#define ReturnChannelError_CMD_NOT_SUPPORTED						0x05000001ul
#define ReturnChannelError_BUS_INIT_FAIL							0x05000002ul
#define ReturnChannelError_Reply_WRONG_CHECKSUM						0x05000036ul
#define ReturnChannelError_GET_WRONG_LENGTH							0x05000037ul
#define ReturnChannelError_Reply_WRONG_LENGTH						0x05000038ul
#define ReturnChannelError_CMD_WRONG_LENGTH							0x05000039ul
#define ReturnChannelError_CMD_READ_FAIL							0x05000003ul
#define ReturnChannelError_USER_INVALID								0x05000004ul
#define ReturnChannelError_PASSWORD_INVALID							0x05000005ul
#define ReturnChannelError_UNDEFINED_INVALID						0x05000006ul
#define ReturnChannelError_STRING_WRONG_LENGTH						0x05000007ul
#define ReturnChannelError_EVENT_QUEUE_ERROR						0x05000008ul
#define ReturnChannelError_CMD_REPEAT_ERROR							0x05000009ul
#define ReturnChannelError_CMD_RETURN_INDEX_ERROR					0x0500000aul
#define ReturnChannelError_CMD_CONTENT_ERROR						0x0500000bul

#define ReturnChannelError_CMD_SYNTEX_ERROR							0x05010001ul
#define ReturnChannelError_CMD_RXDEVICEID_ERROR						0x05010002ul
#define ReturnChannelError_CMD_TXDEVICEID_ERROR						0x05010003ul
#define ReturnChannelError_CMD_SEQUENCE_ERROR						0x05010004ul
#define ReturnChannelError_CMD_CHECKSUM_ERROR						0x05010005ul
#define ReturnChannelError_CMD_PKTCHECK_ERROR						0x05010006ul

#define ReturnChannelError_CMD_TS_SYNTEX_ERROR						0x05020001ul
#define ReturnChannelError_CMD_TS_PID_ERROR							0x05020002ul


//=============================================================================
//					ccHDtv Service Definition
//=============================================================================
#define CMD_GetTxDeviceAddressIDInput                               0x0000
#define CMD_GetTxDeviceAddressIDOutput                              0x8000
#define CMD_GetTransmissionParameterCapabilitiesInput               0x0001
#define CMD_GetTransmissionParameterCapabilitiesOutput              0x8001
#define CMD_GetTransmissionParametersInput                          0x0002
#define CMD_GetTransmissionParametersOutput                         0x8002
#define CMD_GetHwRegisterValuesInput                                0x0003
#define CMD_GetHwRegisterValuesOutput                               0x8003
#define CMD_GetAdvanceOptionsInput                                  0x0004
#define CMD_GetAdvanceOptionsOutput                                 0x8004
#define CMD_GetTPSInformationInput                                  0x0005
#define CMD_GetTPSInformationOutput                                 0x8005
#define CMD_GetEncryptionConfigurationOptionsInput                  0x0006
#define CMD_GetEncryptionConfigurationOptionsOutput                 0x8006
#define CMD_GetEncryptionConfigurationsInput                        0x0007
#define CMD_GetEncryptionConfigurationsOutput                       0x8007
#define CMD_GetSiPsiTableInput                                      0x0010
#define CMD_GetSiPsiTableOutput                                     0x8010
#define CMD_GetNitLocationInput                                     0x0011
#define CMD_GetNitLocationOutput                                    0x8011
#define CMD_GetSdtServiceInput                                      0x0012
#define CMD_GetSdtServiceOutput                                     0x8012
#define CMD_GetEITInformationInput                                  0x0013
#define CMD_GetEITInformationOutput                                 0x8013
#define CMD_SetTxDeviceAddressIDInput                               0x0080
#define CMD_SetTxDeviceAddressIDOutput                              0x8080
#define CMD_SetCalibrationTableInput                                0x0081
#define CMD_SetCalibrationTableOutput                               0x8081
#define CMD_SetTransmissionParametersInput                          0x0082
#define CMD_SetTransmissionParametersOutput                         0x8082
#define CMD_SetHwRegisterValuesInput                                0x0083
#define CMD_SetHwRegisterValuesOutput                               0x8083
#define CMD_SetAdvaneOptionsInput                                   0x0084
#define CMD_SetAdvaneOptionsOutput                                  0x8084
#define CMD_SetTPSInformationInput                                  0x0085
#define CMD_SetTPSInformationOutput                                 0x8085
#define CMD_SetEncryptionConfigurationsInput                        0x0087
#define CMD_SetEncryptionConfigurationsOutput                       0x8087
#define CMD_SetSiPsiTableInput                                      0x0090
#define CMD_SetSiPsiTableOutput                                     0x8090
#define CMD_SetNitLocationInput                                     0x0091
#define CMD_SetNitLocationOutput                                    0x8091
#define CMD_SetSdtServiceInput                                      0x0092
#define CMD_SetSdtServiceOutput                                     0x8092
#define CMD_SetEITInformationInput                                  0x0093
#define CMD_SetEITInformationOutput                                 0x8093

//=============================================================================
//						Device Management Service Definition
//=============================================================================
#define CMD_GetCapabilitiesInput                                    0x0100
#define CMD_GetCapabilitiesOutput                                   0x8100
#define CMD_GetDeviceInformationInput                               0x0101
#define CMD_GetDeviceInformationOutput                              0x8101
#define CMD_GetHostnameInput                                        0x0102
#define CMD_GetHostnameOutput                                       0x8102
#define CMD_GetSystemDateAndTimeInput                               0x0103
#define CMD_GetSystemDateAndTimeOutput                              0x8103
#define CMD_GetSystemLogInput                                       0x0104
#define CMD_GetSystemLogOutput                                      0x8104
#define CMD_GetOSDInformationInput                                  0x0105
#define CMD_GetOSDInformationOutput                                 0x8105
#define CMD_MultipleSetCommandApplyInput                            0x0180
#define CMD_MultipleSetCommandApplyOutput                           0x8180
#define CMD_SetSystemFactoryDefaultInput                            0x0181
#define CMD_SetSystemFactoryDefaultOutput                           0x8181
#define CMD_SetHostnameInput                                        0x0182
#define CMD_SetHostnameOutput                                       0x8182
#define CMD_SetSystemDateAndTimeInput                               0x0183
#define CMD_SetSystemDateAndTimeOutput                              0x8183
#define CMD_SetSystemFactoryDefaultBroadcastInput                   0x0184
#define CMD_SetSystemFactoryDefaultBroadcastOutput                  0x8184
#define CMD_SetOSDInformationInput                                  0x0185
#define CMD_SetOSDInformationOutput                                 0x8185
#define CMD_UpgradeSystemFirmwareInput								0x0186
#define CMD_UpgradeSystemFirmwareOutput								0x8186
#define CMD_SystemRebootInput                                       0x0187
#define CMD_SystemRebootOutput                                      0x8187
#define CMD_MultipleSetCommandStartInput                            0x0188
#define CMD_MultipleSetCommandStartOutput                           0x8188
//=============================================================================
//						Device_IO Service Definition
//=============================================================================
#define CMD_GetDigitalInputsInput									0x0200
#define CMD_GetDigitalInputsOutput									0x8200
#define CMD_GetRelayOutputsInput									0x0201
#define CMD_GetRelayOutputsOutput									0x8201
#define CMD_GetSerialPortsInput										0x0202
#define CMD_GetSerialPortsOutput									0x8202
#define CMD_GetSerialPortConfigurationsInput						0x0203
#define CMD_GetSerialPortConfigurationsOutput						0x8203
#define CMD_GetSerialPortConfigurationOptionsInput					0x0204
#define CMD_GetSerialPortConfigurationOptionsOutput					0x8204
#define CMD_SetRelayOutputStateInput								0x0280
#define CMD_SetRelayOutputStateOutput								0x8280
#define CMD_SetRelayOutputSettingsInput								0x0281
#define CMD_SetRelayOutputSettingsOutput							0x8281
#define CMD_SendReceiveSerialCommandInput							0x0282
#define CMD_SendReceiveSerialCommandOutput							0x8282
#define CMD_SetSerialPortConfigurationInput							0x0283
#define CMD_SetSerialPortConfigurationOutput						0x8283
//=============================================================================
//						Imaging Service Definition
//=============================================================================
#define CMD_GetImagingSettingsInput                                 0x0300
#define CMD_GetImagingSettingsOutput                                0x8300
#define CMD_IMG_GetStatusInput                                      0x0301
#define CMD_IMG_GetStatusOutput                                     0x8301
#define CMD_IMG_GetOptionsInput										0x0302
#define CMD_IMG_GetOptionsOutput									0x8302
#define CMD_GetUserDefinedSettingsInput								0x037F
#define CMD_GetUserDefinedSettingsOutput							0x837F
#define CMD_SetImagingSettingsInput                                 0x0380
#define CMD_SetImagingSettingsOutput                                0x8380
#define CMD_IMG_MoveInput											0x0381
#define CMD_IMG_MoveOutput											0x8381
#define CMD_IMG_StopInput                                           0x0382
#define CMD_IMG_StopOutput                                          0x8382
#define CMD_SetUserDefinedSettingsInput								0x03FF
#define CMD_SetUserDefinedSettingsOutput							0x83FF
//=============================================================================
//						Media Service Definition
//=============================================================================
#define CMD_GetProfilesInput										0x0400
#define CMD_GetProfilesOutput										0x8400
#define CMD_GetVideoSourcesInput									0x0401
#define CMD_GetVideoSourcesOutput									0x8401
#define CMD_GetVideoSourceConfigurationsInput						0x0402
#define CMD_GetVideoSourceConfigurationsOutput						0x8402
#define CMD_GetGuaranteedNumberOfVideoEncoderInstancesInput			0x0403
#define CMD_GetGuaranteedNumberOfVideoEncoderInstancesOutput		0x8403
#define CMD_GetVideoEncoderConfigurationsInput						0x0404
#define CMD_GetVideoEncoderConfigurationsOutput						0x8404
#define CMD_GetAudioSourcesInput									0x0405
#define CMD_GetAudioSourcesOutput									0x8405
#define CMD_GetAudioSourceConfigurationsInput						0x0406
#define CMD_GetAudioSourceConfigurationsOutput						0x8406
#define CMD_GetAudioEncoderConfigurationsInput						0x0407
#define CMD_GetAudioEncoderConfigurationsOutput						0x8407
#define CMD_GetVideoSourceConfigurationOptionsInput					0x0408
#define CMD_GetVideoSourceConfigurationOptionsOutput				0x8408
#define CMD_GetVideoEncoderConfigurationOptionsInput				0x0409
#define CMD_GetVideoEncoderConfigurationOptionsOutput				0x8409
#define CMD_GetAudioSourceConfigurationOptionsInput					0x040A
#define CMD_GetAudioSourceConfigurationOptionsOutput				0x840A
#define CMD_GetAudioEncoderConfigurationOptionsInput				0x040B
#define CMD_GetAudioEncoderConfigurationOptionsOutput				0x840B
#define CMD_GetVideoOSDConfigurationInput							0x0440
#define CMD_GetVideoOSDConfigurationOutput							0x8440
#define CMD_GetVideoPrivateAreaInput								0x0441
#define CMD_GetVideoPrivateAreaOutput								0x8441
#define CMD_SetSynchronizationPointInput							0x0480
#define CMD_SetSynchronizationPointOutput							0x8480
#define CMD_SetVideoSourceConfigurationInput						0x0482
#define CMD_SetVideoSourceConfigurationOutput						0x8482
#define CMD_SetVideoEncoderConfigurationInput						0x0484
#define CMD_SetVideoEncoderConfigurationOutput						0x8484
#define CMD_SetAudioSourceConfigurationInput						0x0486
#define CMD_SetAudioSourceConfigurationOutput						0x8486
#define CMD_SetAudioEncoderConfigurationInput						0x0487
#define CMD_SetAudioEncoderConfigurationOutput						0x8487
#define CMD_SetVideoOSDConfigurationInput							0x04C0
#define CMD_SetVideoOSDConfigurationOutput							0x84C0
#define CMD_SetVideoPrivateAreaInput								0x04C1
#define CMD_SetVideoPrivateAreaOutput								0x84C1
#define CMD_SetVideoSourceControlInput								0x04C2
#define CMD_SetVideoSourceControlOutput								0x84C2
//=============================================================================
//						PTZ Service Definition
//=============================================================================
#define CMD_GetConfigurationsInput									0x0500
#define CMD_GetConfigurationsOutput									0x8500
#define CMD_PTZ_GetStatusInput										0x0501
#define CMD_PTZ_GetStatusOutput										0x8501
#define CMD_GetPresetsInput											0x0502
#define CMD_GetPresetsOutput										0x8502
#define CMD_GotoPresetInput											0x0580
#define CMD_GotoPresetOutput										0x8580
#define CMD_RemovePresetInput										0x0581
#define CMD_RemovePresetOutput										0x8581
#define CMD_SetPresetInput											0x0582
#define CMD_SetPresetOutput											0x8582
#define CMD_AbsoluteMoveInput										0x0583
#define CMD_AbsoluteMoveOutput										0x8583
#define CMD_RelativeMoveInput										0x0584
#define CMD_RelativeMoveOutput										0x8584
#define CMD_ContinuousMoveInput										0x0585
#define CMD_ContinuousMoveOutput									0x8585
#define CMD_SetHomePositionInput									0x0586
#define CMD_SetHomePositionOutput									0x8586
#define CMD_GotoHomePositionInput									0x0587
#define CMD_GotoHomePositionOutput									0x8587
#define CMD_PTZ_StopInput											0x0588
#define CMD_PTZ_StopOutput											0x8588
//=============================================================================
//						Video Analytics Service Definition
//=============================================================================
#define CMD_GetSupportedRulesInput									0x0600
#define CMD_GetSupportedRulesOutput									0x8600
#define CMD_GetRulesInput											0x0601
#define CMD_GetRulesOutput											0x8601
#define CMD_GetSupportedAnalyticsModulesInput						0x0608
#define CMD_GetSupportedAnalyticsModulesOutput						0x8608
#define CMD_GetAnalyticsModulesInput								0x0609
#define CMD_GetAnalyticsModulesOutput								0x8609
#define CMD_CreateRuleInput											0x0680
#define CMD_CreateRuleOutput										0x8680
#define CMD_ModifyRuleInput											0x0681
#define CMD_ModifyRuleOutput										0x8681
#define CMD_DeleteRuleInput											0x0682
#define CMD_DeleteRuleOutput										0x8682
#define CMD_CreateAnalyticsModuleInput								0x0688
#define CMD_CreateAnalyticsModuleOutput								0x8688
#define CMD_ModifyAnalyticsModuleInput								0x0689
#define CMD_ModifyAnalyticsModuleOutput								0x8689
#define CMD_DeleteAnalyticsModuleInput								0x068A
#define CMD_DeleteAnalyticsModuleOutput								0x868A
//=============================================================================
//						Video Analytics Service Definition
//=============================================================================
#define CMD_MetadataStreamOutput									0xF000
//=============================================================================
//						Metadata Stream Service Definition
//=============================================================================
#define CMD_GetMetadataSettingsInput								0xE001
#define CMD_GetMetadataSettingsOutput								0xF001
#define CMD_SetMetadataSettingsInput								0xE081
#define CMD_SetMetadataSettingsOutput								0xF081


#define RCMaxSize													169
#define CmdHeadSize													4
#define TsCmdMaxSize												188
#define RcCmdMaxSize												184

#define RC_CMD_STATE_INIT											0					//0 byte
#define RC_CMD_STATE_WAIT											1					//smaller than 184 bytes
#define RC_CMD_STATE_FINISH											2					//equal 184 bytes
#define RC_CMD_STATE_OVERFLOW										3					//larger than 184 bytes

#define RC_CMDINFO_STATE_INIT										0					//init state
#define RC_CMDINFO_STATE_WAIT										1					//smaller than cmd length
#define RC_CMDINFO_STATE_FINISH										2					//equal  cmd length

#define Debug_check_error											0
#define _DEBUG_RC 0

typedef enum {
    Valid = 0,
    UserNameInvalid = 1,
	PasswordInvalid = 2
} SecurityValid;

typedef enum {
    LineDetector = 0,
    FieldDetector,
	DeclarativeMotionDetector,
	CountingRule,
	CellMotionDetector,
	LoiteringDetector,
	ObjectTracker
} Event;
//------------------General-----------------
typedef struct {
	int BusTxComPort;
	int BusRxComPort;
} BusDesc;

typedef struct RcCmdInfo{
	Byte buffer_Cmd[RcCmdMaxSize];   
	Dword cmdSize;
	Byte cmdState;
	struct RcCmdInfo *next;
}RcCmd;

typedef struct {
	Dword totalGetBufferCount;
	Dword totalPKTCount;
	Dword leadingTagErrorCount;
	Dword endTagErrorCount;
	Dword checkSumErrorCount;
}DebugInfo;

typedef struct CmdInfo{
	Byte cmdLength[CmdHeadSize];   
	Byte* buffer_Cmd;   
	Dword cmdRealSize;
	Dword cmdExpectSize;
	Byte cmdState;
	Byte IsBroadCast;
	struct CmdInfo* next;
}Cmd;


typedef struct {
	Dword stringLength;
	Byte* stringData;
} RCString;


typedef struct {
	Byte returnCode;
} ReturnValue;


typedef struct {
	Cmd* cmd;
	Byte IsTimeOut;
	Byte IsParseFinish;
	Word ExpectCmd;
	Byte IsValid;
}CmdConfig;

typedef struct {
	Bool bIsRepeatPacket;
	Byte byRepeatPacketNumber;
	Byte byRepeatPacketTimeInterval;
	Bool bIsCmdBroadcast;
}CmdSendConfig;

typedef struct {

	Word PID;
	Byte TS_sequence;
	Byte TS_sequence_recv;
	Word clientTxDeviceID;
	Word hostRxDeviceID;
	Word RCCmd_sequence;
	Word RCCmd_sequence_recv;
	BusDesc bus;
	Bool bIsDebug;
	Bool TSMode;
	
} Device;
typedef struct {
	RCString userName;
	RCString password;
} Security;

typedef struct {
	Byte queue[10];
	Byte front;
	Byte rear;
} EventQueue;
//------------------General-----------------
//-----------ccHDtv Service----------------
typedef struct {
	Byte IDType;
	Word deviceAddressID;
}NewTxDevice;

typedef struct {
	Byte processor;
	Dword registerAddress;
	Byte* registerValues;
	Byte valueListSize;
}HwRegisterInfo;

typedef struct {

	Word PTS_PCR_delayTime;
	Word timeInterval;
	Byte skipNumber;
	Byte overFlowNumber;
	Word overFlowSize;
	Byte extensionFlag;
	Word Rx_LatencyRecoverTimeInterval;
	Word SIPSITableDuration;
	char frameRateAdjustment;
	Byte repeatPacketMode;
	Byte repeatPacketNum;
	Byte repeatPacketTimeInterval;
	Byte TS_TableDisable;
	Byte extensionFlag2;
	Byte PCRReStampMode;
}AdvanceOptions;

typedef struct {
	Byte highCodeRate;
	Byte lowCodeRate;
	Byte transmissionMode;
	Byte constellation;
	Byte interval;
	Word cellID;
}TPSInfo;

typedef struct {
	Byte TPSEncryption;
	Byte DataEncryption;
}EncryptConfigOption;

typedef struct {
	Byte TPSEncryptionMode;
	Byte TPSEncryptionType;
	Byte TPSEncryptionKey[8];
	Byte DataEncryptionMode;
	Byte DataEncryptionType;
	Word DataPartialEncryptionSkipLength;
	Byte DataEncryptionKey[64];
}EncryptConfig;

typedef struct {
	RCString	manufactureName;
	RCString	modelName;
	RCString	firmwareVersion;
	RCString	serialNumber;
	RCString	hardwareId;
}ManufactureInfo;

typedef struct {
	Byte bandwidthOptions;
	Dword frequencyMin;
	Dword frequencyMax;
	Byte constellationOptions;
	Byte FFTOptions;
	Byte codeRateOptions;
	Byte guardInterval;
	Byte attenuationMin;
	Byte attenuationMax;
	Byte extensionFlag;
	char attenuationMin_signed;
	char attenuationMax_signed;
	Word TPSCellIDMin;
	Word TPSCellIDMax;
	Byte channelNumMin;
	Byte channelNumMax;
	Byte bandwidthStrapping;
	Byte TVStandard;
	Byte segmentationMode;
	Byte oneSeg_Constellation;
	Byte oneSeg_CodeRate;


} TransmissionParameterCapabilities;

typedef struct {
	Byte				bandwidth;
    Dword				frequency;          /** Channel frequency in KHz.                                */
	Byte		constellation;      /** Constellation scheme (FFT mode) in use                   */
    Byte	FFT;				/** Number of carriers used for OFDM signal                  */
    Byte			codeRate;           /** FEC coding ratio of high-priority stream                 */
    Byte			interval;           /** Fraction of symbol length used as guard (Guard Interval) */
	Byte				attenuation;
	Byte				extensionFlag;
	char				attenuation_signed;
	Word			TPSCellID;
	Byte				channelNum;
	Byte				bandwidthStrapping;
	Byte				TVStandard;
	Byte				segmentationMode;
	Byte				oneSeg_Constellation;
	Byte				oneSeg_CodeRate;
} TransmissionParameter;

typedef struct {
	Word	ONID;
	Word	NID;
	Word	TSID;
	Byte	networkName[32];
	Byte extensionFlag;
	Dword privateDataSpecifier;
	Byte NITVersion;
	Byte countryID;
	Byte languageID;

}PSITable;

typedef struct{
	Word	latitude;
	Word	longitude;
	Word	extentLatitude;
	Word	extentLongitude;
}NITLoacation;

typedef struct{
	Word	serviceID;
	Byte	enable;
	Word	LCN;
	RCString	serviceName;
	RCString	provider;
	Byte extensionFlag;
	Byte IDAssignationMode;
	Byte ISDBT_RegionID;
	Byte ISDBT_BroadcasterRegionID;
	Byte ISDBT_RemoteControlKeyID;
	Word ISDBT_ServiceIDDataType_1;
	Word ISDBT_ServiceIDDataType_2;
	Word ISDBT_ServiceIDPartialReception;

}ServiceConfig;


typedef struct{
	Byte accessOption;
	Byte tableType;
	RCString tableData;
}CalibrationTable;

typedef struct{
	Byte enable;
	Dword startDate;
	Dword startTime;
	Dword duration;
	RCString eventName;
	RCString eventText;
}EITInfoParam;

typedef struct{
	RCString videoEncConfigToken;
	Byte listSize;
	EITInfoParam* eitInfoParam;

}EITInfo;

typedef struct{

	Byte listSize;
	EITInfo* infoList;
	Byte returnIndex;
}EITInfoList;
//-----------ccHDtv Service----------------
//-----------Device Management Service----------------

typedef struct {
	Dword supportedFeatures;

} DeviceCapability;

typedef struct{
	RCString hostName;
}HostInfo;

typedef struct{
	Byte countryCode[3];
	Byte countryRegionID;
	Byte daylightSavings;
	Byte timeZone;
	Byte UTCHour;
	Byte UTCMinute;
	Byte UTCSecond;
	Word UTCYear;
	Byte UTCMonth;
	Byte UTCDay;
	Byte extensionFlag;
	Word UTCMillisecond;
	Byte timeAdjustmentMode;
	Dword timeAdjustmentCriterionMax;
	Dword timeAdjustmentCriterionMin;
	Word timeSyncDuration;


}SystemTime;

typedef struct{
	Byte logType;
	RCString logData;

}SystemLog;

typedef struct{
	Byte dateEnable;
	Byte datePosition;
	Byte dateFormat;
	Byte timeEnable;
	Byte timePosition;
	Byte timeFormat;
	Byte logoEnable;
	Byte logoPosition;
	Byte logooption;
	Byte detailInfoEnable;
	Byte detailInfoPosition;
	Byte detailInfooption;
	Byte textEnable;
	Byte textPosition;
	RCString text;
}OSDInfo;

typedef struct{

	Byte factoryDefault;
}SystemDefault;

typedef struct{
	Byte rebootType;
	Dword rebootDuration;
	RCString responseMessage;
}SystemReboot;

typedef struct{
	Byte mode;
	RCString responseMessage;
}MultipleSetCommand;

typedef struct{
	Byte firmwareType;
	Dword totalDataLength;
	Dword totalDataBlockNumber;
	Dword dataBlockNumber;
	RCString data;
	RCString message;
}SystemFirmware;

//-----------Device Management Service----------------
//-----------Device IO Service----------
typedef struct{
	Byte listSize;
	RCString* tokenList;
}DigitalInputsInfo;

typedef struct{
	RCString token;
	Byte mode;
	Dword delayTime;
	Byte idleState;
}RelayOutputsParam;

typedef struct{
	Byte listSize;
	RelayOutputsParam* relayOutputsParam;
}RelayOutputs;

typedef struct{
	Byte serialPortListSize;
	RCString* tokenList;
}SerialPorts;

typedef struct{
	RCString name;
	RCString token;
	RCString serialPortToken;
	Byte type;
	Dword baudRate;
	Byte dataBitLength;
	Byte parityBitCheck;
	Byte stopBit;
	Byte flowControl;
}SerialPortConfig;

typedef struct{
	Byte listSize;
	SerialPortConfig* configList;
}SerialPortConfigList;

typedef struct{
	RCString serialPortConfigToken;

	Byte typeOptions;
	Byte baudRateAvailableListSize;
	Dword* baudRateList;
	Byte dataBitLengthMin;
	Byte dataBitLengthMax;
	Byte parityBitCheckOptions;
	Byte stopBitOptions;
	Byte flowControlOptions;
}SerialPortConfigOptions;

typedef struct{
	Byte listSize;
	SerialPortConfigOptions* configList;
	Byte returnIndex;
}SerialPortConfigOptionsList;

typedef struct{
	RCString token;
	Byte logicalState;
}RelayOutputState;

typedef struct{
	RCString serialPortToken;
	RCString serialData;
	Byte timeOut;
	Dword receiveDataLength;
	RCString delimiter;

	RCString receiveSerialData;
}SerialCommand;
//-----------Device IO Service----------
//-----------Imaging Service---------
typedef struct{

	RCString videoSourceToken;
	Byte backlightCompensationMode;
	float backlightCompensationLevel;
	float brightness;
	float colorSaturation;
	float contrast;
	Byte exposureMode;
	Byte exposurePriority;
	Word exposureWindowbottom;
	Word exposureWindowtop;
	Word exposureWindowright;
	Word exposureWindowleft;
	Dword minExposureTime;
	Dword maxExposureTime;
	float exposureMinGain;
	float exposureMaxGain;
	float exposureMinIris;
	float exposureMaxIris;
	Dword exposureTime;
	float exposureGain;
	float exposureIris;
	Byte autoFocusMode;
	float focusDefaultSpeed;
	float focusNearLimit;
	float focusFarLimit;
	Byte irCutFilterMode;
	float sharpness;
	Byte wideDynamicRangeMode;
	float wideDynamicRangeLevel;
	Byte whiteBalanceMode;
	float whiteBalanceCrGain;
	float whiteBalanceCbGain;
	Byte analogTVOutputStandard;
	float imageStabilizationLevel;
	Byte flickerControl;
	Byte extensionFlag;
	Byte imageStabilizationMode;
	Byte deNoiseMode;
	float deNoiseStrength;
	Byte backLightControlMode;
	float backLightControlStrength;

	Byte forcePersistence;
}ImageConfig;

typedef struct{
	Byte listSize;
	ImageConfig* configList;
	Byte returnIndex;
}ImageConfigList;

typedef struct{
	RCString videoSourceToken;
	Dword position;
	Byte moveStatus;
	RCString error;
}FocusStatusInfo;


typedef struct{
	Byte listSize;
	FocusStatusInfo* infoList;
	Byte returnIndex;
}FocusStatusInfoList;

typedef struct{
	RCString videoSourceToken;
	float absolutePosition;
	float absoluteSpeed;
	float relativeDistance;
	float relativeSpeed;
	float continuousSpeed;
}FocusMoveInfo;

typedef struct{
	Byte listSize;
	FocusMoveInfo* infoList;
}FocusMoveInfoList;

typedef struct{
	RCString videoSourceToken;
}FocusStopInfo;

typedef struct{
	Byte listSize;
	FocusStopInfo* infoList;
}FocusStopInfoList;
typedef struct{

	RCString videoSourceToken;

	Byte backlightCompensationMode;
	float backlightCompensationLevelMin;
	float backlightCompensationLevelMax;
	float brightnessMin;
	float brightnessMax;
	float colorSaturationMin;
	float colorSaturationMax;
	float contrastMin;
	float contrastMax;
	Byte exposureMode;
	Byte exposurePriority;
	Dword minExposureTimeMin;
	Dword minExposureTimeMax;
	Dword maxExposureTimeMin;
	Dword maxExposureTimeMax;
	float exposureMinGainMin;
	float exposureMinGainMax;
	float exposureMaxGainMin;
	float exposureMaxGainMax;
	float exposureMinIrisMin;
	float exposureMinIrisMax;
	float exposureMaxIrisMin;
	float exposureMaxIrisMax;
	Dword exposureTimeMin;
	Dword exposureTimeMax;
	float exposureGainMin;
	float exposureGainMax;
	float exposureIrisMin;
	float exposureIrisMax;
	Byte autoFocusMode;
	float focusDefaultSpeedMin;
	float focusDefaultSpeedMax;
	float focusNearLimitMin;
	float focusNearLimitMax;
	float focusFarLimitMin;
	float focusFarLimitMax;
	Byte irCutFilterMode;
	float sharpnessMin;
	float sharpnessMax;
	Byte wideDynamicRangeMode;
	float wideDynamicRangeLevelMin;
	float wideDynamicRangeLevelMax;
	Byte whiteBalanceMode;
	float whiteBalanceCrGainMin;
	float whiteBalanceCrGainMax;
	float whiteBalanceCbGainMin;
	float whiteBalanceCbGainMax;
	Byte imageStabilizationMode;
	float imageStabilizationLevelMin;
	float imageStabilizationLevelMax;
	Byte flickerControl;
	Byte analogTVOutputStandard;
	Byte deNoiseMode;
	float deNoiseStrengthMin;
	float deNoiseStrengthMax;
	Byte backLightControlMode;
	float backLightControlStrengthMin;
	float backLightControlStrengthMax;

}ImageConfigOption;

typedef struct{
	Byte listSize;
	ImageConfigOption* configList;
	Byte returnIndex;
}ImageConfigOptionList;

typedef struct{
	RCString videoSourceToken;
	RCString uerDefinedData;
}UserDefinedSettings;

typedef struct{
	Byte listSize;
	UserDefinedSettings* settingList;
	Byte returnIndex;
}UserDefinedSettingsList;
//-----------Imaging Service---------
//----------- Media Service----------

typedef struct{
	RCString token;
	RCString name;
	Byte useCount;
	Byte encoding;
	Word bitrate;
	Word sampleRate;

	Byte forcePersistence;
}AudioEncConfigParam;

typedef struct{
	
	Byte configListSize;
	AudioEncConfigParam* configList;
}AudioEncConfig;

typedef struct{

	RCString videoSrcConfigToken;
	RCString profileToken;

	Word boundsRange_X_Min;
	Word boundsRange_X_Max;
	Word boundsRange_Y_Min;
	Word boundsRange_Y_Max;
	Word boundsRange_Width_Min;
	Word boundsRange_Width_Max;
	Word boundsRange_Height_Min;
	Word boundsRange_Heigh_Max;
	Byte videoSrcTokensAvailableListSize;
	RCString* videoSrcTokensAvailableList;
	Byte rotateModeOptions;
	Word rotateDegreeMinOption;
	Byte mirrorModeOptions;
	Byte maxFrameRateMin;
	Byte maxFrameRateMax;

}VideoSrcConfigOptions;


typedef struct{
	Byte listSize;
	VideoSrcConfigOptions* configList;
	Byte returnIndex;
}VideoSrcConfigOptionsList;

typedef struct{
	RCString audioSrcConfigToken;
	RCString profileToken;
	Byte audioSrcTokensAvailableListSize;
	RCString* audioSrcTokensAvailableList;

	Byte extensionFlag;
	Dword inputLevelRangeMin;
	Dword inputLevelRangeMax;

}AudioSrcConfigOptions;

typedef struct{
	Byte listSize;
	AudioSrcConfigOptions* configList;
	Byte returnIndex;
}AudioSrcConfigOptionsList;

typedef struct{

	Byte encodingOption;
	Word bitrateRangeMin;
	Word bitrateRangeMax;
	Byte sampleRateAvailableListSize;
	Word* sampleRateAvailableList;

}AudioEncConfigOptionsParam;
typedef struct{
	RCString audioEncConfigToken;
	RCString profileToken;

	Byte configListSize;
	AudioEncConfigOptionsParam* audioEncConfigOptionsParam;

}AudioEncConfigOptions;

typedef struct{
	Byte listSize;
	AudioEncConfigOptions* configList;
	Byte returnIndex;
}AudioEncConfigOptionsList;

typedef struct{
	Word width;
	Word height;
}ResolutionsAvailableList;

typedef struct{
	RCString videoEncConfigToken;
	RCString profileToken;

	Byte encodingOption;
	Byte resolutionsAvailableListSize;
	ResolutionsAvailableList* resolutionsAvailableList;
	Byte qualityMin;
	Byte qualityMax;
	Byte frameRateMin;
	Byte frameRateMax;
	Byte encodingIntervalMin;
	Byte encodingIntervalMax;
	Word bitrateRangeMin;
	Word bitrateRangeMax;
	Byte rateControlTypeOptions;
	Byte govLengthRangeMin;
	Byte govLengthRangeMax;
	Byte profileOptions;
	Word targetBitrateRangeMin;
	Word targetBitrateRangeMax;
	Byte aspectRatioAvailableListSize;
	Word* aspectRatioList;

}VideoEncConfigOptions;

typedef struct{
	Byte listSize;
	VideoEncConfigOptions* configList;
	Byte returnIndex;
}VideoEncConfigOptionsList;

typedef struct{
	RCString name;
	Byte useCount;
	RCString token;
	RCString outputToken;
	Byte sendPrimacy;
	Dword outputLevel;
}AudioOutputConfigParam;

typedef struct{
	Dword inputLevel;
	Byte channels;
}AudioSrcConfigParam2;

typedef struct{
	RCString name;
	RCString token;
	RCString sourceToken;
	Byte useCount;

	Byte forcePersistence;

	Byte extensionFlag;
	Dword inputLevel;
	Byte channels;
}AudioSrcConfigParam;



typedef struct{
	Byte configListSize;
	AudioSrcConfigParam* audioSrcConfigList;
	Byte extensionFlag;
	Byte configList2Size;
}AudioSrcConfig;

typedef struct{
	RCString audioSourcesToken;
	Byte channels;
}AudioSourcesParam;

typedef struct{
	Byte audioSourcesListSize;
	AudioSourcesParam* audioSourcesList;
}AudioSources;

typedef struct{
	RCString configurationToken;
	Byte TotallNum;
	Byte JPEGNum;
	Byte H264Num;
	Byte MPEG4Num;
	Byte MPEG2Num;
}GuaranteedEncs;

typedef struct{
	Byte listSize;
	GuaranteedEncs* configList;
	Byte returnIndex;
}GuaranteedEncsList;

typedef struct{
	RCString simpleItemName;
	RCString simpleItemValue;
	RCString elementItemName;
	Byte extensionItemListSize;
	RCString* extensionItemList;
	RCString name;
	RCString type;

}MediaProfilesParamsConfig;
typedef struct{
	
	RCString name;
	RCString token;
	RCString videoSrcName;
	RCString videoSrcToken;
	RCString videoSrcSourceToken;
	Byte videoSrcUseCount;

	Word videoSrcBounds_x;
	Word videoSrcBounds_y;
	Word videoSrcBoundsWidth;
	Word videoSrcBoundsHeight;
	Byte videoSrcRotateMode;
	Word videoSrcRotateDegree;
	Byte videoSrcMirrorMode;

	RCString audioSrcName;
	RCString audioSrcToken;
	RCString audioSrcSourceToken;
	Byte audioSrcUseCount;

	RCString videoEncName;
	RCString videoEncToken;
	Byte videoEncUseCount;
	Byte videoEncEncodingMode;
	Word videoEncResolutionWidth;
	Word videoEncResolutionHeight;
	Byte videoEncQuality;
	Byte videoEncRateControlFrameRateLimit;
	Byte videoEncRateControlEncodingInterval;
	Word videoEncRateControlBitrateLimit;
	Byte videoEncRateControlType;
	Byte videoEncGovLength;
	Byte videoEncProfile;

	RCString audioEncName;
	RCString audioEncToken;
	Byte audioEncUseCount;
	Byte audioEncEncoding;
	Word audioEncBitrate;
	Word audioEncSampleRate;

	RCString audioOutputName;
	RCString audioOutputToken;
	RCString audioOutputOutputToken;
	Byte audioOutputUseCount;
	Byte audioOutputSendPrimacy;
	Byte audioOutputOutputLevel;

	RCString audioDecName;
	RCString audioDecToken;
	Byte audioDecUseCount;
	Word videoEncConfigRateControlTargetBitrateLimit;
	Dword audioSourceInputLevel;
}MediaProfilesParam;

typedef struct{

	Byte profilesListSize;
	MediaProfilesParam* mediaProfilesParam;
	Byte extensionFlag;
	Byte extension2Flag;
}MediaProfiles;

typedef struct{
	Dword Width;
	Dword Height;
}ResolutionsAvailableParam;

typedef struct{
	RCString name;
	RCString token;
	Byte useCount;
	Byte encoding;
	Word width;
	Word height;
	Byte quality;
	Byte frameRateLimit;
	Byte encodingInterval;
	Word bitrateLimit;
	Byte rateControlType;
	Byte govLength;
	Byte profile;
	Byte extensionFlag;
	Word targetBitrateLimit;
	Word aspectRatio;

	Byte forcePersistence;
}VideoEncConfigParam;

typedef struct{
	Byte configListSize;
	VideoEncConfigParam* configList;
	Byte extensionFlag;
}VideoEncConfig;

typedef struct{
	RCString name;
	RCString token;
	RCString srcToken;
	Byte useCount;
	Word bounds_x;
	Word bounds_y;
	Word boundsWidth;
	Word boundsHeight;
	Byte rotateMode;
	Word rotateDegree;
	Byte mirrorMode;
	Byte extensionFlag;
	Byte maxFrameRate;

	Byte forcePersistence;
}VideoSrcConfigParam;

typedef struct{
	Byte configListSize;
	VideoSrcConfigParam* configList;
	Byte extensionFlag;
}VideoSrcConfig;

typedef struct{
	RCString token;
	Byte frameRate;
	Word resolutionWidth;
	Word resolutionHeight;
}VideoSrcParam;

typedef struct{
	Byte videoSrcListSize;
	VideoSrcParam* srcList;
}VideoSrc;

typedef struct{
	RCString profileToken;
}SyncPoint;

typedef struct{
	Byte listSize;
	SyncPoint* configList;
}SyncPointList;
typedef struct{
	RCString videoSrcToken;
	Byte dateEnable;
	Byte datePosition;
	Byte dateFormat;
	Byte timeEnable;
	Byte timePosition;
	Byte timeFormat;
	Byte logoEnable;
	Byte logoPosition;
	Byte logooption;
	Byte detailInfoEnable;
	Byte detailInfoPosition;
	Byte detailInfoOption;
	Byte textEnable;
	Byte textPosition;
	RCString text;

}VideoOSDConfig;

typedef struct{
	Byte listSize;
	VideoOSDConfig* configList;
	Byte returnIndex;
}VideoOSDConfigList;

typedef struct{
	Word polygon_x;
	Word polygon_y;
}PrivateAreaPolygon;

typedef struct{

	RCString videoSrcToken;
	Byte polygonListSize;
	PrivateAreaPolygon* privateAreaPolygon;
	Byte privateAreaEnable;

}VideoPrivateArea;

typedef struct{
	Byte listSize;
	VideoPrivateArea* areaList;
	Byte returnIndex;
}VideoPrivateAreaList;

typedef struct{

	RCString videoSrcToken;
	Byte controlCommand;
}VideoSrcControl;

typedef struct{
	Byte listSize;
	VideoSrcControl* controlList;
}VideoSrcControlList;
//----------- Media Service----------
//----------- PTZ  Service----------
typedef struct{
	RCString name;
	Byte useCount;
	RCString token;
	short defaultPanSpeed;
	short defaultTiltSpeed;
	short defaultZoomSpeed;
	Dword defaultTimeout;
	Word panLimitMin;
	Word panLimitMax;
	Word tiltLimitMin;
	Word tiltLimitMax;
	Word zoomLimitMin;
	Word zoomLimitMax;
	Byte eFlipMode;
	Byte reverseMode;
	RCString videoSrcToken;
}PTZConfigParam;
typedef struct{
	Byte PTZConfigListSize;
	PTZConfigParam* PTZConfigList;
	Byte extensionFlag;
}PTZConfig;
typedef struct{
	RCString token;

	short panPosition;
	short tiltPosition;
	short zoomPosition;
	Byte panTiltMoveStatus ;
	Byte zoomMoveStatus;
	RCString error;
	Byte UTCHour;
	Byte UTCMinute;
	Byte UTCSecond;
	Word UTCYear;
	Byte UTCMonth;
	Byte UTCDay;

}PTZStatus;

typedef struct{
	Byte listSize;
	PTZStatus* configList;
	Byte returnIndex;
}PTZStatusList;

typedef struct{

	RCString presetName;
	RCString presetToken;

	short panPosition;
	short tiltPosition;
	short zoomPosition;

	short panSpeed;
	short tiltSpeed;
	short zoomSpeed;
}PTZPresetsConfig;
typedef struct{
	RCString token;
	Byte configListSize;
	PTZPresetsConfig* configList;
	
}PTZPresets;

typedef struct{
	Byte presetListSize;
	PTZPresets* presetList;
	Byte returnIndex;
}PTZPresetsList;

typedef struct{
	RCString token;
	RCString presetToken;
	short panSpeed;
	short tiltSpeed;
	short zoomSpeed;
}PTZGotoParam;

typedef struct{
	RCString token;
	RCString presetName;
	RCString presetToken;

	RCString presetToken_set;
}PTZPresetsSet;

typedef struct{
	RCString token;
	RCString presetToken;
}PTZRemoveParam;

typedef struct{
	RCString token;
	short panPosition;
	short tiltPosition;
	short zoomPosition;
	short panSpeed;
	short tiltSpeed;
	short zoomSpeed;
}PTZAbsoluteMove;

typedef struct{
	RCString token;
	short panTranslation;
	short tiltTranslation;
	short zoomTranslation;
	short panSpeed;
	short tiltSpeed;
	short zoomSpeed;
}PTZRelativeMove;

typedef struct{
	RCString token;
	short panVelocity;
	short tiltVelocity;
	short zoomVelocity;
	Dword timeout;
}PTZContinuousMove;

typedef struct{
	RCString token;

	short panPosition;
	short tiltPosition;
	short zoomPosition;

	short panSpeed;
	short tiltSpeed;
	short zoomSpeed;
}PTZHomePosition;

typedef struct{
	RCString token;
	Byte panTiltZoom;

}PTZStop;
//----------- PTZ  Service----------
//------Video Analytics Service------
typedef struct{
	Byte supportListSize;
	Byte* ruleType;
}SupportedRules;
typedef struct{
	Word polygon_x;
	Word polygon_y;
}DetectPolygon;
typedef struct{
	
	Byte motionExpression;
	Byte polygonListSize;
	DetectPolygon* detectPolygon;
	Byte metadataStreamSwitch;

}Rule_MotionDetector;

typedef struct{
	
	Word direction;
	Byte polygonListSize;
	DetectPolygon* detectPolygon;
	Byte metadataStreamSwitch;

}Rule_LineDetector;

typedef struct{

	Byte polygonListSize;
	DetectPolygon* detectPolygon;
	Byte metadataStreamSwitch;

}Rule_FieldDetector;

typedef struct{

	Dword reportTimeInterval;
	Dword resetTimeInterval;
	Word direction;
	Byte polygonListSize;
	DetectPolygon* detectPolygon;
	Byte metadataStreamSwitch;

}Rule_Counting ;

typedef struct{

	Word minCount;
	Dword alarmOnDelay;
	Dword alarmOffDelay;
	Word activeCellsSize;
	RCString activeCells;
	Byte sensitivity;
	Word layoutBounds_x;
	Word layoutBounds_y;
	Word layoutBounds_width;
	Word layoutBounds_height;
	Byte layoutColumns;
	Byte layoutRows;
	Byte metadataStreamSwitch;

}Rule_CellMotion ;
typedef struct{

	Dword timeThreshold;
	Byte isPeriodicNotify;
	Dword timeInterval;
	Byte polylineListSize;
	DetectPolygon* detectPolyLine;
	Byte metadataStreamSwitch;

}Rule_LoiteringDetector;
typedef struct{
	Byte extensionFlag;
	RCString ruleToken;
	RCString VideoSourceToken;
}DeleteRule;

typedef struct RuleInfo{
	RCString ruleToken;
	RCString ruleName;
	Byte type;
	void* rule;
	Byte extensionFlag;
	RCString videoSrcToken;
	Byte threshold;
	Byte motionSensitivity;
}TotalRule;
typedef struct{

	Byte ruleListSize;
	TotalRule* totalRuleList;
	Byte maxListSize;
	Byte extensionFlag;
}RuleList;

typedef struct{
	
	Word commandLength;
	Dword objectID;
	Byte objectTracking;
	Byte polygonListSize;
	DetectPolygon* detectPolygon;
	Byte objectClass;
	Byte Likelihood;

}AnalyticsModule_ObjectTracker;

typedef struct{
	RCString VideoSourceToken;
	RCString moduleToken;
}DeleteModule;

typedef struct{
	Byte supportListSize;
	Byte* analyticsModuleType;
}SupportedAnalyticsModules;
typedef struct ModuleInfo{
	RCString videoSrcToken;
	RCString moduleName;
	RCString moduleToken;
	Byte type;
	void* module;
}TotalModule;
typedef struct{
	Byte moduleListSize;
	TotalModule* totalModuleList;
	Byte maxModuleSize;
}ModuleList;


//------Video Analytics Service------
//-----Metadata Stream Service----
typedef struct{
	Word deviceVendorID;
	Word deviceModelID;
	Word HWVersionCode;
	Dword SWVersionCode;
	RCString textInformation;
	Byte extensionFlag;
	Dword baudRate;
}Metadata_Device;

typedef struct{
	Word videoPID;
	Byte videoEncodingType;
	Word videoResolutionWidth;
	Word videoResolutionHeight;
	Byte videoFrameRate;
	Word videoBitrate;
	Word audioPID;
	Byte audioEncodingType;
	Word audioBitrate;
	Word audioSampleRate;
	Word PCRPID;
	RCString videoSrcToken;
}Metadata_StreamParam;
typedef struct{

	Byte streamInfoListSize;
	Metadata_StreamParam*	streamInfoList;
	Byte extensionFlag;

}Metadata_Stream;
typedef struct{

	RCString ruleToken;
	Dword objectID;
	Byte UTCHour;
	Byte UTCMinute;
	Byte UTCSecond;
	Word UTCYear;
	Byte UTCMonth;
	Byte UTCDay;
	
	Byte IsInside;
	Dword count;
	Byte IsMotion;

	Byte extensionFlag;
	RCString videoSrcToken;

	Byte extensionFlag2;
	Qword TimeStamp;

	Byte SinceUTCHour;
	Byte SinceUTCMinute;
	Byte SinceUTCSecond;
	Word SinceUTCYear;
	Byte SinceUTCMonth;
	Byte SinceUTCDay;

	RCString analyticsModuleToken;
	Byte sceneTooCrowded;

}Metadata_Event;

typedef struct{

	RCString serialPortToken;
	RCString serialData;

}Metadata_SerialData;

typedef struct{

	RCString token;
	Byte logicalState;
	RCString data;

}Metadata_LogicalStateEvent;

typedef struct{
	Word version;
	Byte type;
	Metadata_Device metadata_Device;
	Metadata_Stream metadata_Stream;
	Metadata_Event metadata_Event;
	Metadata_SerialData metadata_SerialData;
	Metadata_LogicalStateEvent metadata_DigitalInputEvent;
	Metadata_LogicalStateEvent metadata_RelayOutputEvent;
}MetadataStreamInfo;

typedef struct{
	Byte deviceInfoEnable;
	Word deviceInfoPeriod;
	Byte streamInfoEnable;
	Word streamInfoPeriod;
	Byte motionDetectorEnable;
	Word motionDetectorPeriod;
	Byte extensionFlag;
	Byte serialDataEnable;
	Word serialDataPeriod;
	Byte extensionFlag2;
	Byte digitalInputEventEnable;
	Word digitalInputEventPeriod;
	Byte relayOutputEventEnable;
	Word relayOutputEventPeriod;
}MetadataSettings;
//-----Metadata Stream Service----


//---------------------General	Process------------------------
Byte Cmd_calChecksum(
	IN  Dword		payload_start_point,
	IN  Dword		payload_end_point,
	IN	const Byte*       buffer						  
) ;
Dword Cmd_splitBuffer(
	IN Byte* buf, 
	IN Dword length, 
	OUT Byte* splitBuf, 
	IN Dword splitLength,
	IN Dword* start 
);

void SWordToshort(
	IN SWord SWordNum,
	OUT short* num
);
void shortToSWord(
	IN short num,
	OUT SWord* SWordNum
);
Dword fullQueue(Byte front, Byte rear, Byte size);
Dword empty(Byte front, Byte rear);
Dword enqueue(EventQueue* eventQueue, Byte item );
Dword dequeue(EventQueue* eventQueue, Byte* item );
void initQueue(EventQueue* queue);
//--------------------------TS-------------------
Dword Cmd_addTS(
	OUT Byte* dstbuf, 
	IN Word PID, 
	IN Byte seqNum
);
Dword Cmd_sendTSCmd(
	IN Byte* buffer, 
	IN Dword bufferSize,
	IN Device* device,
	OUT  int *txLen,
	IN CmdSendConfig* cmdSendConfig
);
//--------------------------TS-------------------
//--------------------------RC-------------------
Dword Cmd_addRC(
	IN Byte* srcbuf,  
	OUT Byte* dstbuf, 
	IN Word RXDeviceID, 
	IN Word TXDeviceID, 
	IN Word seqNum, 
	IN Dword total_pktNum, 
	IN Dword pktNum, 
	IN Dword pktLength, 
	IN Byte index
);
Dword Cmd_sendRCCmd(
	IN Byte* buffer, 
	IN Dword bufferSize, 
	IN Device* device, 
	OUT int *txLen, 
	IN CmdSendConfig* cmdSendConfig
);
//--------------------------RC-------------------
void Cmd_CheckByteIndexRead(
	IN const Byte* buf , 
	IN Dword length, 
	OUT Dword* var
);
Dword Cmd_DwordRead(
	IN const Byte* buf , 
	IN Dword bufferLength,
	IN Dword* index, 
	OUT Dword* var,
	IN Dword defaultValue
);

Dword Cmd_QwordRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT Qword* var,
	IN Qword defaultValue
);

Dword Cmd_StringRead(
	IN const Byte* buf , 
	IN Dword bufferLength,
	IN Dword* index, 
	OUT RCString* str
);
Dword Cmd_BytesRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT Byte* bufDst, 
	IN Dword dstLength
);
Dword Cmd_ByteRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT Byte* var,
	IN Byte defaultValue
);
Dword Cmd_WordRead(
	IN const Byte* buf ,
	IN Dword bufferLength,
	IN Dword* index, 
	OUT Word* var,
	IN Word defaultValue
);

Dword Cmd_FloatRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT float* var, 
	IN float  defaultValue
);

Dword Cmd_ShortRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT short* var, 
	IN short defaultValue
);

Dword Cmd_CharRead(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	IN Dword* index, 
	OUT char* var, 
	IN char defaultValue
);

void Cmd_WordAssign(
	IN Byte* buf,
	IN Word var,
	IN Dword* length
);
void Cmd_DwordAssign(
	IN Byte* buf,
	IN Dword var,
	IN Dword* length
);

void Cmd_QwordAssign(
	IN Byte* buf,
	IN Qword var,
	IN Dword* length
);

void Cmd_BytesAssign(
	IN Byte* buf,
	IN Qword var,
	IN Dword BytesLength,
	IN Dword* length
);

void Cmd_StringAssign(
	IN Byte* buf,
	IN const RCString* str,
	IN Dword* length
);
void Cmd_FloatAssign(
	IN Byte* buf,
	IN float var,
	IN Dword* length
);
void Cmd_ShortAssign(
	IN Byte* buf,
	IN short var,
	IN Dword* length
);
void initCmdInfo(
	IN Cmd* cmd
);
//-------------------RC-----------------
RcCmd* RcCmdCollect(RcCmd* cmd, Byte* buffer,int bufferSize);
Dword RcHeadCheck(Byte* buf, Device* device, Cmd* cmd, DebugInfo* debugInfo, Byte offset, Byte IsHost);
void initRcCmdInfo(RcCmd* cmd);
void releaseRcCmdInfo(RcCmd* rchead);
RcCmd* popRcCmd(RcCmd* pointer);
void RcPrint(RcCmd *pointer);
RcCmd* pushRcCmd(RcCmd* pointer,  Byte* buffer, int bufferSize);
//-------------------RC-----------------
Dword TSHeadCheck(Byte* srcbuf, Device* device);
Dword cmdAssign(
	IN Cmd* cmd,
	IN Byte* buffer,
	IN Dword bufferSize,
	IN Word total_pktNum,
	IN Word pktNum
);

void cmdState(
	IN Cmd* cmd
);

Dword Cmd_StringReset(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	OUT RCString* dstStr
);
Dword Cmd_StringResetCopy(
	IN const RCString* srcStr , 
	OUT RCString* dstStr
);
Dword Cmd_StringCopy(
	IN const RCString* srcStr , 
	OUT RCString* dstStr
);
Dword Cmd_StringSet(
	IN const Byte* buf , 
	IN Dword bufferLength, 
	OUT RCString* str
);

Dword Cmd_StringClear(
	IN RCString* str
);

Byte Cmd_checkChecksum(
	IN  Dword		payload_start_point,
	IN  Dword		payload_end_point,
	IN const Byte*       buffer	
);




void RC_Init_RCHeadInfo(
	IN Device* device, 
	IN Word RXID, 
	IN Word TXID,
	IN Word Seq_recv
);

void RC_Init_TSHeadInfo(
	IN Device* device, 
	IN Word PID, 
	IN Byte Seq_recv, 
	IN Bool TSMode
);
void RC_Init_CmdConfig(
	IN CmdConfig* cmdConfig,
	IN Byte IsTimeOut, 
	IN Word expectCmd
);
void RC_Init_PortConfig(
	IN Device* device, 
	IN int RxPort, 
	IN int TxPort
);

void RC_Init_CmdSendConfig(
	IN CmdSendConfig* cmdSendConfig
);
Dword RC_Set_CmdSendConfig(
	IN AdvanceOptions* advanceOptions,
	OUT CmdSendConfig* cmdSendConfig
);

void RC_Reset_DebugInfo(
	IN DebugInfo* debugInfo
);
//---------------------General	Process------------------------

#endif
