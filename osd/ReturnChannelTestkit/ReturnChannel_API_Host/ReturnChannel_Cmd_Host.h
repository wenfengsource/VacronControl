#ifndef RETURN_CHANNEL_CMD_HOST_H
#define RETURN_CHANNEL_CMD_HOST_H


struct RCCallBack{
	void (*ReplyCallBack)(void*, Word);
	void (*MetaDataCallBack)(void*, Word);
	void (*EventCallBack)(void*, Word);
};

typedef struct RCHostHandler{
	//--------------General-------------------
	CmdConfig											cmdConfig;
	Device													device;
	Security												security;
	ReturnValue											returnValue;
	CmdSendConfig									cmdSendConfig;
	//--------------General-------------------
	//-----------ccHDtv Service--------------
	NewTxDevice										newTxDevice;
	HwRegisterInfo										hwRegisterInfo;
	AdvanceOptions									advanceOptions;
	TPSInfo												tpsInfo;
	EncryptConfigOption								encryptConfigOption;
	EncryptConfig									encryptConfig;
	TransmissionParameterCapabilities		transmissionParameterCapabilities;
	TransmissionParameter							transmissionParameter;
	PSITable												psiTable;
	NITLoacation										nitLoacation;
	ServiceConfig										serviceConfig;
	CalibrationTable									calibrationTable;
	EITInfo													eitInfo;
	//-----------ccHDtv Service--------------
	//-------Device Management Service-----
	DeviceCapability									deviceCapability;
	ManufactureInfo									manufactureInfo;
	HostInfo												hostInfo;
	SystemTime											systemTime;
	SystemLog											systemLog;
	OSDInfo												osdInfo;
	SystemDefault										systemDefault;
	SystemReboot										systemReboot;
	SystemFirmware									systemFirmware;
	MultipleSetCommand									multipleSetCommandApply;
	MultipleSetCommand									multipleSetCommandStart;
	//-------Device Management Service-----
	//----------Device_IO Service-----------
	DigitalInputsInfo									digitalInputsInfo;
	RelayOutputs										relayOutputs;
	RelayOutputsParam								relayOutputsSetParam;
	SerialPorts											serialPorts;
	SerialPortConfig									serialPortConfig;
	SerialPortConfigList								serialPortConfigList;
	SerialPortConfigOptions						serialPortConfigOptions;
	RelayOutputState									relayOutputState;
	SerialCommand									serialCommand;
	//----------Device_IO Service-----------
	//------------------Imaging Service---------------------
	ImageConfig											imageConfig;
	FocusStatusInfo									focusStatusInfo;
	FocusMoveInfo									focusMoveInfo;
	FocusStopInfo										focusStopInfo;
	ImageConfigOption								imageConfigOption;
	UserDefinedSettings								userDefinedSettings;
	//------------------Imaging Service---------------------
	//------------------Media Service------------------
	AudioEncConfig									audioEncConfig;
	AudioSrcConfig									audioSrcConfig;
	AudioSources										audioSources;
	GuaranteedEncs									guaranteedEncs;
	MediaProfiles										mediaProfiles;
	VideoEncConfig									videoEncConfig;
	VideoSrcConfig									videoSrcConfig;
	VideoSrc												videoSrc;
	VideoSrcConfigOptions						videoSrcConfigOptions;
	VideoEncConfigOptions						videoEncConfigOptions;
	AudioSrcConfigOptions						audioSrcConfigOptions;
	AudioEncConfigOptions						audioEncConfigOptions;

	AudioEncConfigParam							audioEncConfigSetParam;
	AudioSrcConfigParam							audioSrcConfigSetParam;
	VideoEncConfigParam							videoEncConfigSetParam;
	VideoSrcConfigParam							videoSrcConfigSetParam;
	SyncPoint												syncPoint;
	VideoOSDConfig									videoOSDConfig;
	VideoPrivateArea									videoPrivateArea;
	VideoSrcControl									videoSrcControl;
	//------------------Media Service------------------
	//-------------------PTZ Service-------------------
	PTZConfig											ptzConfig;
	PTZStatus											ptzStatus;
	PTZPresets											ptzPresetsGet;
	PTZGotoParam									ptzGotoParam;
	PTZPresetsSet										ptzPresetsSet;
	PTZRemoveParam								ptzRemoveParam;
	PTZAbsoluteMove								ptzAbsoluteMove;
	PTZRelativeMove									ptzRelativeMove;
	PTZContinuousMove							ptzContinuousMove;
	PTZHomePosition								ptzHomePosition;
	PTZStop												ptzStop;
	//-------------------PTZ Service-------------------
	SupportedRules									supportedRules;
	TotalRule												totalRule;
	DeleteRule											deleteRule;
	RuleList												ruleList;

	SupportedAnalyticsModules						supportedModules;
	TotalModule										totalModule;
	DeleteModule									deleteModule;
	ModuleList										moduleList;

	//------------Metadata Stream Service------------
	MetadataStreamInfo								metadataStreamInfo;
	MetadataSettings									metadataSettings;
	//------------Metadata Stream Service------------
	//------------CallBackFunctionPointer------------
	struct RCCallBack											callBackSet;
	//------------CallBackFunctionPointer------------
}RCHostInfo;


//*******************Parser************************
Dword Cmd_HostParser(
	IN	RCHostInfo* deviceInfo
);

Dword parseReadyCmd_Host(
	IN RCHostInfo* deviceInfo
);
//*******************Parser************************
//*******************Get*************************
//----------------------General--------------------------------
Dword Cmd_SetCmdConfigTimeOut (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command,
	IN	void (*callback)(void*, Word)
) ;

Dword Cmd_Send (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_generalGet (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getWithByte(
	IN  RCHostInfo*			    deviceInfo,
	IN	Byte				byteData,
	IN	Word				command
) ;
Dword Cmd_getWithString(
	IN  RCHostInfo*			    deviceInfo,
	IN	const RCString*				string,
	IN	Word				command
);

Dword Cmd_getWithStrings(
	IN  RCHostInfo*			    deviceInfo,
	IN	const RCString*				stringArray,
	IN	Dword				stringSize,
	IN	Word				command
) ;

//----------------------General--------------------------------
//---------------------ccHDtv Service--------------------------
Dword Cmd_getHwRegisterValues (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setTxDeviceAddressID (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setCalibrationTable (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setHwRegisterValues(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setAdvanceOptions (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setTPSInfo (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setTransmissionParameters(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setPSITable (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setNitLocation (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setSdtServiceConfiguration (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setEITInformation  (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_setEncryptionConfigurations  (
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

//---------------------ccHDtv Service--------------------------
//------------------Device Management Service----------------


Dword Cmd_setSystemDateAndTime(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setOSDInformation(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_upgradeSystemFirmware(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;
//------------------Device Management Service----------------
//--------------------Device_IO Service-----------------------
Dword Cmd_setSerialPortConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_sendReceiveSerialCommand(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setRelayOutputSettings(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setRelayOutputState(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_getSerialPortConfigOptions(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
//--------------------Device_IO Service-----------------------
//----------------------Imaging Service------------------------
Dword Cmd_move(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;

Dword Cmd_setImagingSettings(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setUserDefinedSettings(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;
//----------------------Imaging Service------------------------
//----------------------Media Service--------------------------
Dword Cmd_setVideoSrcControl(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setVideoPrivateArea(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_setVideoOSDConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setAudioEncConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setAudioSrcConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setVideoEncConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_setVideoSrcConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getVideoSrcConfigOptions(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getVideoEncConfigOptions(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getAudioSrcConfigOptions(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getAudioEncConfigOptions(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getVideoOSDConfig(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_getVideoPrivateArea(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
//----------------------Media Service--------------------------
//------------------------PTZ Service--------------------------
Dword Cmd_gotoPreset(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_gotoPreset(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_setPreset(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_absoluteMove(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
) ;
Dword Cmd_relativeMove(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_continuousMove(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_gotoHomePosition(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_PTZStop(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_removePreset(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
//------------------------PTZ Service--------------------------
//------------------Video Analytics Service---------------------
Dword Cmd_createRule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_modifyRule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_deleteRule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);

Dword Cmd_createAnalyticsModule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_modifyAnalyticsModule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
Dword Cmd_deleteAnalyticsModule(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
//------------------Video Analytics Service---------------------
//------------------Metadata Stream Service---------------------
Dword Cmd_getMetadataSettings(
    IN  RCHostInfo*			    deviceInfo,
	IN Word							command
) ;
Dword Cmd_setMetadataSettings(
    IN  RCHostInfo*			    deviceInfo,
	IN	Word				command
);
//------------------Metadata Stream Service---------------------
//*******************Get*************************
#endif

