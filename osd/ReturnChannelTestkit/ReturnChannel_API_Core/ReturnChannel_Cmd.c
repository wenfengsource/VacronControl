#include "ReturnChannel_Cmd.h"
#include "ReturnChannel_User.h"


Dword fullQueue(Byte front, Byte rear, Byte size)
{
	return (((front+1)%size)==rear);
}
Dword emptyQueue(Byte front, Byte rear)
{
	return (rear==front);
}
Dword enqueue(EventQueue* eventQueue, Byte item )
{
	Dword	error = ModulatorError_NO_ERROR;
	Byte tmpFront = 0;
	Byte tmpRear = 0;
	Byte size = 0;
	size = sizeof(eventQueue->queue);
	tmpFront  = eventQueue->front;
	tmpRear  = eventQueue->rear;
	
	if(fullQueue(tmpFront, tmpRear, size) == 0)
	{
		eventQueue->queue[tmpFront] = item;
		tmpFront =(tmpFront+1)%size;
		eventQueue->front = tmpFront;
	}
	else
	{
		printf("Queue is full!\n");
		error = ReturnChannelError_EVENT_QUEUE_ERROR;
	}
	return error;
}
Dword dequeue(EventQueue* eventQueue, Byte* item )
{
	Dword	error = ModulatorError_NO_ERROR;
	Byte tmpFront = 0;
	Byte tmpRear = 0;
	Byte tmpitem = 0;
	Byte size = 0;
	size = sizeof(eventQueue->queue);
	tmpFront  = eventQueue->front;
	tmpRear  =  eventQueue->rear;


	if(emptyQueue(tmpFront, tmpRear) == 0)
	{
		tmpitem = eventQueue->queue[tmpRear];
		tmpRear= (tmpRear+1)%size;
		(*item) = tmpitem;
		eventQueue->rear = tmpRear;
	}else
	{
		printf("Queue is empty!\n");
		error = ReturnChannelError_EVENT_QUEUE_ERROR;
	}
	return error;
}
void initQueue(EventQueue* queue)
{
	queue->front = 0;
	queue->rear = 0;
}

void SWordToshort(SWord SWordNum,short* num)
{
	short out = 0;
	out = *((short*)&SWordNum);

	(*num) = out;
}
void shortToSWord(short num,SWord* SWordNum)
{
	SWord outSWord = 0;
	outSWord = *((SWord*)&num);

	(* SWordNum) = outSWord;

}
Dword Cmd_splitBuffer(Byte* buf, Dword length, Byte* splitBuf, Dword splitLength,Dword* start )
{
	Dword offset = (*start);
	User_memory_set(splitBuf,0, splitLength);
	if(length-offset>splitLength)
	{
		User_memory_copy(splitBuf, buf + offset, splitLength);

		offset = offset + splitLength;
		(*start) = offset;

		return (length-offset);
	}else
	{
		User_memory_copy(splitBuf, buf + offset, length - offset);

		(*start)  = length;
		return 0;	
	}

}
Dword Cmd_addTS(Byte* dstbuf, Word PID, Byte seqNum)
{
	Dword	error = ModulatorError_NO_ERROR;

	//TS header
	dstbuf[0] = 0x47;													//svnc_byte
	dstbuf[1] = 0x20 | ((Byte) (PID >> 8));					//TEI + .. +PID
	dstbuf[2] = (Byte) PID;											//PID
	dstbuf[3] = 0x10| ((Byte) (seqNum & 0x0F));		//..... + continuity counter


	return error;
}

Dword Cmd_sendTSCmd(Byte* buffer, Dword bufferSize, Device* device, int *txLen, CmdSendConfig* cmdSendConfig)
{
	Dword	error = ModulatorError_NO_ERROR;
	Byte TSPkt[188]={0xFF};
	Byte RCCmdData[RCMaxSize]={0xFF};
	Dword start = 0;
	Dword total_pktCount = 0,pktCount = 1;
	Word tmpcommand = 0;
	Dword sleepTime = 0;
	Dword dwRemainder = 0;

	if(bufferSize <= 0)
	{
		error = ReturnChannelError_CMD_WRONG_LENGTH;
		return error;
	}

	tmpcommand = buffer[4]<<8 | buffer[5];

	if( device->clientTxDeviceID == 0xFFFF)
	{
		switch(tmpcommand)
		{
		case CMD_GetTxDeviceAddressIDInput:
		case CMD_SetSystemFactoryDefaultBroadcastInput:
			break;
		default:
			error = ReturnChannelError_CMD_TXDEVICEID_ERROR;
			return error;
		}
	}
#ifdef __linux__   /* Linux */
	sleepTime = cmdSendConfig->byRepeatPacketTimeInterval * 1000;
#else
	sleepTime = cmdSendConfig->byRepeatPacketTimeInterval;
#endif
	User_mutex_lock();

	dwRemainder = bufferSize%RCMaxSize;

	if(dwRemainder == 0)
		total_pktCount = (bufferSize/RCMaxSize);
	else
		total_pktCount = (bufferSize/RCMaxSize)+1;

	while(Cmd_splitBuffer(buffer,bufferSize,RCCmdData, RCMaxSize,&start)!=0)
	{
		device->RCCmd_sequence++;
		device->TS_sequence++;

		if(device->bIsDebug)
			printf("RCCmd_sequence = %u\n",device->RCCmd_sequence);

		error = Cmd_addTS(TSPkt, device->PID, device->TS_sequence);
		if(error) goto exit;
		if(!cmdSendConfig->bIsCmdBroadcast)
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 4);
		else
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 4);
		if(error) goto exit;
		error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(TSPkt), TSPkt, txLen);
		if(error) goto exit;
		pktCount++;
	}
	device->RCCmd_sequence++;
	device->TS_sequence++;
	error = Cmd_addTS(TSPkt, device->PID, device->TS_sequence);

	if(device->bIsDebug)
		printf("RCCmd_sequence = %u\n",device->RCCmd_sequence);
	if(!cmdSendConfig->bIsCmdBroadcast)
	{
		if(dwRemainder == 0)
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 4);
		else
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, dwRemainder, 4);
	}
	else
	{
		if(dwRemainder == 0)
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 4);
		else
			error = Cmd_addRC(RCCmdData, TSPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, dwRemainder, 4);
	}
	if(error) goto exit;
	error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(TSPkt), TSPkt, txLen);
	if(error) goto exit;

exit:
	User_mutex_unlock();
	return error;
}

Dword Cmd_addRC(Byte* srcbuf,  Byte* dstbuf, Word RXDeviceID, Word TXDeviceID, Word seqNum, Dword total_pktNum, Dword pktNum, Dword pktLength, Byte index)
{
	Dword	error = ModulatorError_NO_ERROR;

	Dword payload_start_point = 0, payload_end_point = 0;
	//RC header
	dstbuf[index + 183] = 0x0d;
	dstbuf[index++] = '#';													//Leading Tag
	dstbuf[index++] = (Byte)(RXDeviceID>>8);	
	dstbuf[index++] = (Byte)RXDeviceID;	
	payload_start_point = index;
	dstbuf[index++] = (Byte)(TXDeviceID>>8);	
	dstbuf[index++] = (Byte)TXDeviceID;	
	dstbuf[index++] = (Byte)(total_pktNum>>8);				//total_pktNum
	dstbuf[index++] = (Byte)total_pktNum;						//total_pktNum
	dstbuf[index++] = (Byte)(pktNum>>8);						//pktNum
	dstbuf[index++] = (Byte)pktNum;									//pktNum
	dstbuf[index++] = (Byte)(seqNum>>8);						//seqNum
	dstbuf[index++] = (Byte)seqNum;									//seqNum
	dstbuf[index++] = (Byte)(pktLength>>8);						//seqNum
	dstbuf[index++] = (Byte)pktLength;								//pktLength 

	User_memory_copy(dstbuf + index, srcbuf, pktLength);
	index = index + (Byte)pktLength;
	payload_end_point = index - 1;

	dstbuf[index++] = Cmd_calChecksum(payload_start_point,payload_end_point,dstbuf);
	

	return error;
}

Dword Cmd_sendRCCmd(Byte* buffer, Dword bufferSize, Device* device, int *txLen, CmdSendConfig* cmdSendConfig)
{
	Dword	error = ModulatorError_NO_ERROR;
	Byte RCPkt[184]={0xFF};
	Byte RCCmdData[RCMaxSize]={0xFF};
	Dword start = 0;
	Dword total_pktCount = 0,pktCount = 1;
	Word tmpcommand = 0;
	Byte i = 0;
	Dword sleepTime = 0;
	Dword dwRemainder = 0;

	if(bufferSize <= 0)
	{
		error = ReturnChannelError_CMD_WRONG_LENGTH;
		return error;
	}

	tmpcommand = buffer[4]<<8 | buffer[5];

	if( device->clientTxDeviceID == 0xFFFF)
	{
			error = ReturnChannelError_CMD_TXDEVICEID_ERROR;
			return error;
	}
#ifdef __linux__   /* Linux */
	sleepTime = cmdSendConfig->byRepeatPacketTimeInterval * 1000;
#else
	sleepTime = cmdSendConfig->byRepeatPacketTimeInterval;
#endif
	User_mutex_lock();

	dwRemainder = bufferSize%RCMaxSize;

	if(dwRemainder == 0)
		total_pktCount = (bufferSize/RCMaxSize);
	else
		total_pktCount = (bufferSize/RCMaxSize)+1;

	while(Cmd_splitBuffer(buffer,bufferSize,RCCmdData, RCMaxSize,&start)!=0)
	{
		device->RCCmd_sequence++;

		if(device->bIsDebug) 
			printf("RCCmd_sequence = %u\n",device->RCCmd_sequence);
		if(!cmdSendConfig->bIsCmdBroadcast)
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 0);
		else
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 0);
		if(error) goto exit;

		if(cmdSendConfig->bIsRepeatPacket)
		{
			for( i = 0; i < cmdSendConfig->byRepeatPacketNumber; i ++)
			{
				error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(RCPkt), RCPkt, txLen);
				if(error) goto exit;
#ifdef __linux__   /* Linux */
				usleep(sleepTime);
#else
				Sleep(sleepTime);
#endif
			}
		}else
		{
			error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(RCPkt), RCPkt, txLen);
			if(error) goto exit;
		}
		pktCount++;
	}
	device->RCCmd_sequence++;
	if(device->bIsDebug) 
		printf("RCCmd_sequence = %u\n",device->RCCmd_sequence);
	if(!cmdSendConfig->bIsCmdBroadcast)
	{
		if(dwRemainder == 0)
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 0);
		else
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, device->clientTxDeviceID, device->RCCmd_sequence, total_pktCount, pktCount, dwRemainder, 0);
	}
	else
	{
		if(dwRemainder == 0)
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, RCMaxSize, 0);
		else
			error = Cmd_addRC(RCCmdData, RCPkt, device->hostRxDeviceID, 0xFFFF, device->RCCmd_sequence, total_pktCount, pktCount, dwRemainder, 0);
	}
	if(error) goto exit;

	if(cmdSendConfig->bIsRepeatPacket)
	{
		for( i = 0; i < cmdSendConfig->byRepeatPacketNumber; i ++)
		{
			error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(RCPkt), RCPkt, txLen);
			if(error) goto exit;
#ifdef __linux__   /* Linux */
				usleep(sleepTime);
#else
				Sleep(sleepTime);
#endif
		}
	}else
	{
			error = User_returnChannelBusTx(device->bus.BusTxComPort, sizeof(RCPkt), RCPkt, txLen);
			if(error) goto exit;
	}

exit:
	User_mutex_unlock();
	return error;
}

Dword Cmd_StringReset(const Byte* buf , Dword bufferLength, RCString* dstStr)	
{
	Dword	error = ModulatorError_NO_ERROR;
	error = Cmd_StringClear(dstStr);
	if(error) return error;
	Cmd_StringSet(buf, bufferLength, dstStr);
	return error;
}
Dword Cmd_StringResetCopy(const RCString* srcStr , RCString* dstStr)		//clear str and read data form str and assign to str (String structure)
{
	Dword	error = ModulatorError_NO_ERROR;
	error = Cmd_StringClear(dstStr);
	if(error) return error;
	error = Cmd_StringCopy(srcStr, dstStr);
	return error;
}
Dword Cmd_StringCopy(const RCString* srcStr , RCString* dstStr)		//read data form str and assign to str (String structure)
{
	Dword	error = ModulatorError_NO_ERROR;
	if(srcStr->stringLength < 0)
	{
		error = ReturnChannelError_STRING_WRONG_LENGTH;
		return error;
	}
	dstStr->stringLength = srcStr->stringLength;

	dstStr->stringData = (Byte*)User_memory_allocate( dstStr->stringLength* sizeof(Byte));
	
	User_memory_copy(dstStr->stringData, srcStr->stringData, dstStr->stringLength);

	return error;
}
Dword Cmd_StringSet(const Byte* buf , Dword bufferLength, RCString* str)		//read data form buf and assign to str (String structure)
{
	Dword	error = ModulatorError_NO_ERROR;

	str->stringLength = bufferLength;
	if(bufferLength >= 0)
	{
		str->stringData = (Byte*)User_memory_allocate( bufferLength* sizeof(Byte));

		User_memory_copy( str->stringData, buf, bufferLength);

	}else
	{
		str->stringData  = NULL;
		error = ReturnChannelError_STRING_WRONG_LENGTH;
		if(Debug_check_error) printf("StringSet fail error = %lx\n",ReturnChannelError_STRING_WRONG_LENGTH);
	}
	return error;
}

Dword Cmd_StringClear(RCString* str)				//clear str (String structure)
{
	Dword error = ModulatorError_NO_ERROR;
	if(str->stringLength >=0)
	{
		if(str->stringData!= NULL)
			User_memory_free(str->stringData) ;
		str->stringLength = 0;
		str->stringData = NULL;
	}else
	{
		error = ReturnChannelError_STRING_WRONG_LENGTH;
		if(Debug_check_error) printf("StringClear fail error = %lx\n",ReturnChannelError_STRING_WRONG_LENGTH);
	}
	return error;
}
Dword Cmd_BytesRead(const Byte* buf , Dword bufferLength, Dword* index, Byte* bufDst, Dword dstLength)
{
	Dword error = ModulatorError_NO_ERROR;
	Dword tempIndex = 0;

	tempIndex = (* index);
	if(bufferLength< tempIndex + dstLength)
	{
		error = ReturnChannelError_CMD_READ_FAIL;

		User_memory_set( bufDst, 0xFF, dstLength);

		if(Debug_check_error) printf("BytesRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		User_memory_copy( bufDst, buf + tempIndex, dstLength);

		(* index) = (* index) +dstLength;
	}
	return error;
}

Dword Cmd_ByteRead(const Byte* buf , Dword bufferLength, Dword* index, Byte* var, Byte defaultValue)
{
	Dword error = ModulatorError_NO_ERROR;
	Dword tempIndex = 0;

	tempIndex = (* index);
	if(bufferLength< tempIndex +1)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("ByteRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		(* var) = buf[tempIndex];
		(* index) = (* index) +1;
	}
	return error;
}

Dword Cmd_WordRead(const Byte* buf , Dword bufferLength, Dword* index, Word* var, Word defaultValue)
{
	Dword error = ModulatorError_NO_ERROR;
	Dword tempIndex = 0;

	tempIndex = (* index);
	if(bufferLength< tempIndex +2)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("WordRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		(* var) = (buf[tempIndex]<<8) | buf[tempIndex+1] ;
		(* index) = (* index) + 2;
	}
	return error;
}

Dword Cmd_DwordRead(const Byte* buf , Dword bufferLength, Dword* index, Dword* var, Dword  defaultValue)
{
	Dword tempIndex = 0;
	Dword error = ModulatorError_NO_ERROR;

	tempIndex = (* index);
	if(bufferLength< tempIndex +4)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("DwordRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		(* var) = (buf[tempIndex]<<24) | (buf[tempIndex+1]<<16) | (buf[tempIndex+2]<<8) | buf[tempIndex+3] ;
		(* index) = (* index) + 4;
	}
	return error;
}

Dword Cmd_FloatRead(const Byte* buf , Dword bufferLength, Dword* index, float* var, float  defaultValue)
{
	Dword tempIndex = 0;
	Float tempFloat = 0;
	Dword error = ModulatorError_NO_ERROR;
	Byte *unsignedint_ptr = (Byte *)&tempFloat;
	Byte *float_ptr = (Byte *)var;

	tempIndex = (* index);
	if(bufferLength< tempIndex +4)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("FloatRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{


		tempFloat = (buf[tempIndex]<<24) | (buf[tempIndex+1]<<16) | (buf[tempIndex+2]<<8) | buf[tempIndex+3] ;

		float_ptr[0] = unsignedint_ptr[0];
		float_ptr[1] = unsignedint_ptr[1];
		float_ptr[2] = unsignedint_ptr[2];
		float_ptr[3] = unsignedint_ptr[3];

		(* index) = (* index) + 4;
	}
	return error;
}

Dword Cmd_ShortRead(const Byte* buf , Dword bufferLength, Dword* index, short* var, short defaultValue)
{
	Dword error = ModulatorError_NO_ERROR;
	Dword tempIndex = 0;
	SWord tempSWord = 0;

	tempIndex = (* index);
	if(bufferLength< tempIndex +2)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("ShortRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{

		tempSWord = (buf[tempIndex]<<8) | buf[tempIndex+1] ;
		SWordToshort( tempSWord , var);
		(* index) = (* index) + 2;
	}
	return error;
}

Dword Cmd_CharRead(const Byte* buf , Dword bufferLength, Dword* index, char* var, char defaultValue)
{
	Dword error = ModulatorError_NO_ERROR;
	Dword tempIndex = 0;

	tempIndex = (* index);
	if(bufferLength< tempIndex +1)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("CharRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		(* var) = buf[tempIndex];
		(* index) = (* index) +1;
	}
	return error;
}

void Cmd_CheckByteIndexRead(const Byte* buf , Dword length, Dword* var)
{
	(* var) = (buf[length]<<24) | (buf[length+1]<<16) | (buf[length+2]<<8) | buf[length+3] ;
}

Dword Cmd_QwordRead(const Byte* buf , Dword bufferLength, Dword* index, Qword* var, Qword defaultValue)
{
	Dword tempIndex = 0;
	Byte i = 0;
	Dword error = ModulatorError_NO_ERROR;

	tempIndex = (* index);
	if(bufferLength< tempIndex +8)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		(* var) = defaultValue;
		if(Debug_check_error) printf("QwordRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		for( i = 0; i < 8; i ++)
		{
			(* var) =  (* var)  << 8;
			(* var)  =  (* var) | buf[tempIndex+i];
		}
		(* index) = (* index) + 8;
	}
	return error;
}
Dword Cmd_StringRead(const Byte* buf , Dword bufferLength, Dword* index, RCString* str)
{
	Dword tempIndex = 0;
	Dword error = ModulatorError_NO_ERROR;
	Dword strLength = 0;
	tempIndex = (* index);

	if(bufferLength< tempIndex +4)
	{
		error = ReturnChannelError_CMD_READ_FAIL;
		str->stringLength = 0;
		str->stringData = NULL;
		if(Debug_check_error) printf("StringRead fail error = %lx\n",ReturnChannelError_CMD_READ_FAIL);
	}else
	{
		strLength = (buf[tempIndex]<<24) | (buf[tempIndex+1]<<16) | (buf[tempIndex+2]<<8) | buf[tempIndex+3] ;
		tempIndex = tempIndex + 4;

		if(bufferLength < tempIndex + strLength)
		{
			error = ReturnChannelError_CMD_READ_FAIL;
			str->stringLength = 0;
			str->stringData = NULL;
		}else
		{
			str->stringLength = strLength;
			str->stringData = (Byte*) User_memory_allocate(strLength * sizeof(Byte));

			User_memory_copy( str->stringData, buf + tempIndex, strLength);

			tempIndex = tempIndex + str->stringLength;
			(* index) = tempIndex;
		}
	}
	return error;
}
void Cmd_WordAssign(Byte* buf,Word var,Dword* length)
{
	Dword tempLength =0;
	tempLength = (* length);
	buf[tempLength+0] = (Byte)(var>>8);
	buf[tempLength+1] = (Byte)(var);

	(*length) = (*length) + 2;
}

void Cmd_DwordAssign(Byte* buf,Dword var,Dword* length)
{
	Dword tempLength =0;
	tempLength = (* length);
	buf[tempLength+0] = (Byte)(var>>24);
	buf[tempLength+1] = (Byte)(var>>16);
	buf[tempLength+2] = (Byte)(var>>8);
	buf[tempLength+3] = (Byte)(var);

	(*length) = (*length) + 4;
}
void Cmd_QwordAssign(Byte* buf,Qword var,Dword* length)
{
	Dword tempLength =0;
	tempLength = (* length);
	buf[tempLength+0] = (Byte)(var>>56);
	buf[tempLength+1] = (Byte)(var>>48);
	buf[tempLength+2] = (Byte)(var>>40);
	buf[tempLength+3] = (Byte)(var>>32);
	buf[tempLength+4] = (Byte)(var>>24);
	buf[tempLength+5] = (Byte)(var>>16);
	buf[tempLength+6] = (Byte)(var>>8);
	buf[tempLength+7] = (Byte)(var);
	(*length) = (*length) + 8;
}

void Cmd_BytesAssign(Byte* buf,Qword var,Dword BytesLength,Dword* length)
{
	if(BytesLength < 8)
	{
		Byte i = 0;
		Dword tempLength =0;
		tempLength = (* length);

		for(i=0;i<BytesLength;i++)
		{
			buf[tempLength+i] = (Byte)(var>>((BytesLength-1-i)*8));
		}

		(*length) = (*length) + BytesLength;
	}
	
}
void Cmd_StringAssign(Byte* buf,const RCString* str,Dword* length)
{
	Dword tempLength = (* length);
	Dword strLength = str->stringLength;
	buf[tempLength + 0] = (Byte)(strLength>>24);
	buf[tempLength + 1]  = (Byte)(strLength>>16);
	buf[tempLength + 2] = (Byte)(strLength>>8);
	buf[tempLength + 3]  = (Byte)strLength;

	tempLength = tempLength + 4;

	User_memory_copy( buf + tempLength, str->stringData, strLength);

	tempLength = tempLength + strLength;
	(*length) = tempLength;
}

void Cmd_FloatAssign(Byte* buf,float var,Dword* length)
{
	Float tempFloat = 0;
	Byte *unsignedint_ptr = (Byte *)&tempFloat;
	Byte *float_ptr = (Byte *)&var;
	Dword tempLength =0;
	
	tempLength = (* length);
	
	unsignedint_ptr[0] = float_ptr[0];
	unsignedint_ptr[1] = float_ptr[1];
	unsignedint_ptr[2] = float_ptr[2];
	unsignedint_ptr[3] = float_ptr[3];
	
	
	//Big Endian
	buf[tempLength+0] = (Byte)(tempFloat>>24);
	buf[tempLength+1] = (Byte)(tempFloat>>16);
	buf[tempLength+2] = (Byte)(tempFloat>>8);
	buf[tempLength+3] = (Byte)(tempFloat);
	

	(*length) = (*length) + 4;
}

void Cmd_ShortAssign(Byte* buf,short var,Dword* length)
{
	Dword tempLength =0;
	SWord tempSWord;

	shortToSWord( var, &tempSWord);

	tempLength = (* length);
	buf[tempLength+0] = (Byte)(tempSWord>>8);
	buf[tempLength+1] = (Byte)(tempSWord);

	(*length) = (*length) + 2;
}

Byte Cmd_calChecksum(
	IN  Dword		payload_start_point,
	IN  Dword		payload_end_point,
	IN	const Byte*       buffer						  
) {

	Dword i;
	Word Sum = 0;
	Byte Checksum = 0;

	for(i = payload_start_point;i <= payload_end_point;i++){
		Sum = Sum + buffer[i];
	}
	Checksum = (Byte)(Sum%256);
	return Checksum;
}

Byte Cmd_checkChecksum(
	IN  Dword		payload_start_point,
	IN  Dword		payload_end_point,
	IN const Byte*       buffer	
){

	Dword i;
	Byte ret;
	Word Sum = 0;
	Byte Checksum = 0;

	for(i = payload_start_point;i <= payload_end_point;i++){
		Sum = Sum + buffer[i];
	}
	Checksum = (Byte)(Sum%256);

	if(Checksum == buffer[payload_end_point + 1])
		ret = 0;
	else
		ret = 0xFE;

	return ret;
}
//-------------------RC process-------------------------
void initRcCmdInfo(RcCmd* cmd)
{
	cmd->cmdSize = 0;
	cmd->cmdState = RC_CMDINFO_STATE_INIT;
	cmd->next = NULL;
}
void releaseRcCmdInfo(RcCmd* rchead)
{
	RcCmd* rctmp;
	while(rchead->next != NULL)
	{
		rctmp = rchead;
		rchead = rchead->next;
		User_memory_free(rctmp);
	}
	User_memory_free(rchead);

}
RcCmd* popRcCmd(RcCmd* pointer)
{
	RcCmd* rctemp;
	if( pointer->cmdState == 3)	// it has more than 2 commands
	{
		rctemp = pointer;
		pointer = pointer -> next;
		User_memory_free(rctemp);  
	}else										// it has only one command
	{
		initRcCmdInfo(pointer);
	}
	return pointer;
}
RcCmd* pushRcCmd(RcCmd* pointer,  Byte* buffer, int bufferSize)
{

	//pointer = pointer -> next;

	pointer->next = (RcCmd *)User_memory_allocate(sizeof(RcCmd));

	pointer = pointer->next;
	initRcCmdInfo(pointer);
	pointer = RcCmdCollect(pointer, buffer, bufferSize);

	return pointer;
}
RcCmd* RcCmdCollect(RcCmd* cmd, Byte* buffer,int bufferSize)
{
	Dword remain = 0;
	Byte* remainBuffer;

	if(bufferSize<=0)
		return cmd;
	if(cmd->cmdState == RC_CMD_STATE_INIT)
	{
		if(buffer[0] == '#' )
		{
			cmd->cmdState = RC_CMD_STATE_WAIT;				//wait for collecting

			if(bufferSize > RcCmdMaxSize)	
			{
				cmd->cmdState = RC_CMD_STATE_OVERFLOW;			//overflow
				remain =  bufferSize - RcCmdMaxSize;

				User_memory_copy( cmd->buffer_Cmd, buffer , RcCmdMaxSize);

				cmd->cmdSize = RcCmdMaxSize;
				remainBuffer = (Byte*)User_memory_allocate(remain*sizeof(Byte));

				User_memory_copy( remainBuffer, buffer + bufferSize-remain, remain);

				cmd = pushRcCmd(cmd,  remainBuffer, remain);
				User_memory_free(remainBuffer);
			}
			else
			{
				User_memory_copy( cmd->buffer_Cmd, buffer , bufferSize);

				cmd->cmdSize = cmd->cmdSize + bufferSize;
				if(cmd->cmdSize == RcCmdMaxSize)
					cmd->cmdState = RC_CMD_STATE_FINISH;				//full
			}
		}else
		{
			printf("<Leading Tag Not Found!>\n");
		}
	}else if(cmd->cmdState == RC_CMD_STATE_WAIT)
	{
		if(cmd->cmdSize + bufferSize<=RcCmdMaxSize)
		{
			User_memory_copy( cmd->buffer_Cmd + cmd->cmdSize, buffer , bufferSize);

			cmd->cmdSize = cmd->cmdSize + bufferSize;
			if(cmd->cmdSize == RcCmdMaxSize)
				cmd->cmdState = RC_CMD_STATE_FINISH;
		}else
		{
			cmd->cmdState = RC_CMD_STATE_OVERFLOW;
			remain = cmd->cmdSize + bufferSize - RcCmdMaxSize;

			User_memory_copy( cmd->buffer_Cmd + cmd->cmdSize, buffer , bufferSize-remain);

			cmd->cmdSize = RcCmdMaxSize;
			remainBuffer = (Byte*)User_memory_allocate(remain*sizeof(Byte));

			User_memory_copy( remainBuffer, buffer + bufferSize - remain, remain);

			cmd = pushRcCmd(cmd,  remainBuffer, remain);
			User_memory_free(remainBuffer);
		}
	}
	return cmd;

}

void RcCmdState(RcCmd* cmd)
{
	Dword i = 0;
	printf("\n----------------------RC----------------------\n");
	printf("\nRcCmdState = %u\n",cmd->cmdState);
	printf("RcCmdSize = %lu\n",cmd->cmdSize);
	for( i = 0; i < cmd->cmdSize; i ++)
	{
		printf("%03lu : %03x   ", i, cmd->buffer_Cmd[i]);
		if(i % 8 ==7 )
			printf("\n");
	}
	printf("\n----------------------RC----------------------\n");
}

void RcPrint(RcCmd *pointer)
{
        if(pointer==NULL)
        {
                return;
        }
        RcCmdState(pointer);
        RcPrint(pointer->next);
}

Dword RcHeadCheck(Byte* buf, Device* device, Cmd* cmd, DebugInfo* debugInfo, Byte offset, Byte IsHost)
{
	Dword error = ModulatorError_NO_ERROR;

	
	Word total_pktNum = 0, pktNum = 0,pktLength = 0;
	Word RXDeviceID = 0, TXDeviceID = 0, seqNum = 0;
	Byte checkSum = 0;

	if((buf[offset + 0]!= '#') || (buf[offset + 183]!= 0x0d))
	{
		error = ReturnChannelError_CMD_SYNTEX_ERROR;
		if(buf[offset + 0]!= '#')
			printf("<RC syntex Error for No Leading Tag!>\n");
		else
			printf("<RC syntex Error for No End Tag!>\n");

#if _DEBUG_RC 
		//Dword i = 0;
		//for( i = 0;i< 184;i++)
		//{
		//	printf("0x%02X ",buf[offset + i]);
		//	if((i%10) == 9)
		//		printf("\n");
		//}
		//printf("\n");
#endif


		debugInfo->endTagErrorCount ++;
		return error;			//syntex error
	}

	RXDeviceID = ((buf[offset + 1])<<8) | buf[offset + 2];
	TXDeviceID = (buf[offset + 3]<<8) | buf[offset + 4];
	total_pktNum = (buf[offset + 5]<<8) | buf[offset + 6];
	pktNum = (buf[offset + 7]<<8) | buf[offset + 8];
	seqNum = (buf[offset + 9]<<8) | buf[offset + 10];
	pktLength = (buf[offset + 11]<<8) | buf[offset + 12];
	checkSum = buf[offset + 13 + pktLength];

	if(	(device->hostRxDeviceID != RXDeviceID) && (IsHost != 0))
	{
#if _DEBUG_RC 
		printf("RXDeviceID Error\n");
#endif
		error = ReturnChannelError_CMD_RXDEVICEID_ERROR;
		return error;
	}
	if(	(device->clientTxDeviceID != TXDeviceID) && (TXDeviceID != 0xFFFF) )
	{
#if _DEBUG_RC 
		printf("TXDeviceID Error\n");
#endif
		error = ReturnChannelError_CMD_TXDEVICEID_ERROR;
		return error;
	}
	if(TXDeviceID == 0xFFFF)
	{
		cmd->IsBroadCast = 1;
	}

	if(Cmd_checkChecksum(offset + 3,offset + 12 + pktLength,buf)!=0)
	{
		printf("<checkSum Error!>\n");
		error = ReturnChannelError_CMD_CHECKSUM_ERROR;
		debugInfo->checkSumErrorCount ++;
		return error;
	}
	if(total_pktNum == 0)
	{
		error = ReturnChannelError_CMD_PKTCHECK_ERROR;
		return error;			
	}
	if(pktNum == 0)
	{
		error = ReturnChannelError_CMD_PKTCHECK_ERROR;
		return error;			
	}
	if(total_pktNum < pktNum)
	{
		error = ReturnChannelError_CMD_PKTCHECK_ERROR;
		return error;			
	}

	if(device->bIsDebug)
		printf("RCCmd_sequence_recv = %u\n",	device->RCCmd_sequence_recv);

	if(	device->RCCmd_sequence_recv != (seqNum-1))	//Not Seq
	{
		if((device->RCCmd_sequence_recv == seqNum)	&& (IsHost == 1))// Same Seq
		{
			if(device->bIsDebug)
			{
				printf("<Same Pkt!> ");
				printf(" PKTNum / TotalPKTNum = %u/%u ", pktNum,total_pktNum );
			}
			return error;
		}else{
			printf("<Lost Pkt!>\n");
			device->RCCmd_sequence_recv = seqNum;
			if(cmd->cmdState != RC_CMDINFO_STATE_INIT)
			{
				error = ReturnChannelError_CMD_SEQUENCE_ERROR;
				printf("Clear command buffer for collecting\n");
				User_memory_free(cmd->buffer_Cmd);
				initCmdInfo(cmd);
				return error;
			}
		}

	}
	else
	{
		device->RCCmd_sequence_recv = seqNum;
	}

	buf = buf + offset + 13;
	error = cmdAssign(cmd,buf, pktLength, total_pktNum, pktNum);				//Rebuild fragments of command

	return error;
}
//-------------------RC process-------------------------
//-------------------TS process-------------------------

Dword TSHeadCheck(Byte* srcbuf, Device* device)
{
	Word PID = 0;
	Byte tsSeq = 0; 
	Dword error = ModulatorError_NO_ERROR;


	if(srcbuf[0]!= 0x47)
	{
		printf("<TS syntex Error!>\n");
		error = ReturnChannelError_CMD_TS_SYNTEX_ERROR;
		return error;			//syntex error
	}

	PID = ((srcbuf[1]&0x1F)<<8) | srcbuf[2];
	tsSeq = srcbuf[3]&0x0F;

	if(device->PID!=PID)
	{
		error = ReturnChannelError_CMD_TS_SYNTEX_ERROR;
		return error;			//Not deivce
	}

	if(	device->TS_sequence != (tsSeq-1))
	{
		device->TS_sequence = tsSeq;
		printf("<TS Lost Pkt!>\n");
	}
	else
		device->TS_sequence = tsSeq;


	return 0;
}
//-------------------TS process-------------------------
//-------------------Cmd process-----------------------
void initCmdInfo(Cmd* cmd)
{
	cmd->buffer_Cmd =NULL;   
	cmd->cmdExpectSize = 0;
	cmd->cmdRealSize = 0;
	cmd->cmdState = RC_CMDINFO_STATE_INIT;
	cmd->IsBroadCast = 0;
	cmd->next = NULL;
}

void print(Cmd *pointer)
{
        if(pointer==NULL)
        {
                return;
        }
        cmdState(pointer);
        print(pointer->next);
}

void cmdState(Cmd* cmd)
{

	printf("\ncmdState = %u\n",cmd->cmdState);
	printf("cmdExpectSize= %lu\n",cmd->cmdExpectSize);
	printf("cmdRealSize = %lu\n",cmd->cmdRealSize);

	if((cmd->cmdState ==RC_CMDINFO_STATE_FINISH) )
	{

		/*for( i = 0; i < cmd->cmdRealSize; i ++)
		{
			printf("%03d : %03x   ", i, cmd->buffer_Cmd[i]);
			if(i % 8 ==7 )
				printf("\n");
		}*/
	}
}

Dword cmdAssign(Cmd* cmd, Byte* buffer,Dword bufferSize, Word total_pktNum, Word pktNum)
{
	Dword error = ModulatorError_NO_ERROR;

	if(bufferSize<=0)
	{
		error = ReturnChannelError_CMD_WRONG_LENGTH;
		return error;
	}

	if((cmd->cmdState == RC_CMDINFO_STATE_INIT) && (pktNum == 1 ))
	{
		cmd->cmdState = RC_CMDINFO_STATE_WAIT;					//Not full

		User_memory_copy( cmd->cmdLength, buffer , CmdHeadSize);

		cmd->cmdExpectSize = ((cmd->cmdLength[0]<<24) | (cmd->cmdLength[1]<<16) | (cmd->cmdLength[2]<<8) | cmd->cmdLength[3] ) + 1;
		cmd->buffer_Cmd = (Byte*)User_memory_allocate((cmd->cmdExpectSize) * sizeof(Byte));

		if(bufferSize > cmd->cmdExpectSize)				//cmdExpectSize error
		{
			User_memory_free(cmd->buffer_Cmd);
			initCmdInfo(cmd);
			error = ReturnChannelError_CMD_WRONG_LENGTH;
			return error;
		}

		User_memory_copy( cmd->buffer_Cmd, buffer , bufferSize);

		cmd->cmdRealSize = bufferSize;

		if((cmd->cmdRealSize == cmd->cmdExpectSize) )
		{
			if(total_pktNum == pktNum )
				cmd->cmdState = RC_CMDINFO_STATE_FINISH;				//Full & Right
			else																								//Full but fail
			{
				User_memory_free(cmd->buffer_Cmd);
				initCmdInfo(cmd);
				error = ReturnChannelError_CMD_PKTCHECK_ERROR;
				return error;
			}
		}

	}else if ((cmd->cmdState == RC_CMDINFO_STATE_WAIT) && (pktNum != 1 ) && (total_pktNum > 1  ))
	{
		if(cmd->cmdRealSize + bufferSize > cmd->cmdExpectSize)				//cmdExpectSize error
		{
			User_memory_free(cmd->buffer_Cmd);
			initCmdInfo(cmd);
			error = ReturnChannelError_CMD_WRONG_LENGTH;
			return error;
		}

		User_memory_copy( cmd->buffer_Cmd + cmd->cmdRealSize, buffer , bufferSize);

		cmd->cmdRealSize = cmd->cmdRealSize + bufferSize;
		if(cmd->cmdRealSize == cmd->cmdExpectSize)
		{
			if(total_pktNum == pktNum )
				cmd->cmdState = RC_CMDINFO_STATE_FINISH;				//Full & Right
			else																								//Full but fail
			{
				User_memory_free(cmd->buffer_Cmd);
				initCmdInfo(cmd);
				error = ReturnChannelError_CMD_PKTCHECK_ERROR;
				return error;
			}
		}

	}

	return error;

}


//-------------------Cmd process-----------------------

void RC_Init_RCHeadInfo(Device* device, Word RXID, Word TXID,Word Seq_recv)
{
	device->clientTxDeviceID = TXID;	
	device->hostRxDeviceID = RXID;	
	device->RCCmd_sequence = 0;
	device->RCCmd_sequence_recv = Seq_recv;
}

void RC_Init_TSHeadInfo(Device* device, Word PID, Byte Seq_recv, Bool TSMode)
{
	device->PID = PID;

	device->TS_sequence = 0;
	device->TS_sequence_recv = Seq_recv;
	device->TSMode = TSMode;
}

void RC_Init_CmdConfig(CmdConfig* cmdConfig,Byte IsTimeOut, Word expectCmd)
{
	cmdConfig->ExpectCmd = expectCmd;
	cmdConfig->IsParseFinish = 0;
	cmdConfig->IsValid = 0;
	cmdConfig->IsTimeOut = IsTimeOut;

	cmdConfig->cmd = (Cmd *)User_memory_allocate(sizeof(Cmd));	
	initCmdInfo(cmdConfig->cmd);
}

void RC_Init_PortConfig(Device* device, int RxPort, int TxPort)
{
	device->bus.BusRxComPort = RxPort;
	device->bus.BusTxComPort = TxPort;
}

void RC_Init_CmdSendConfig(CmdSendConfig* cmdSendConfig)
{
	cmdSendConfig->bIsRepeatPacket = False;
	cmdSendConfig->byRepeatPacketNumber = 1;
	cmdSendConfig->byRepeatPacketTimeInterval = 0;
	cmdSendConfig->bIsCmdBroadcast = False;
}


Dword RC_Set_CmdSendConfig(AdvanceOptions* advanceOptions, CmdSendConfig* cmdSendConfig)
{
	Dword error = ModulatorError_NO_ERROR;
	if(advanceOptions->repeatPacketMode == 0)
		cmdSendConfig->bIsRepeatPacket = False;
	else if(advanceOptions->repeatPacketMode == 1)
		cmdSendConfig->bIsRepeatPacket = True;
	else
	{
		cmdSendConfig->bIsRepeatPacket = False;
		printf("UnSupported Setting RepeatMode : %u!\n", advanceOptions->repeatPacketMode);
		printf("System Disable Command RepeatMode!\n");
		error = ReturnChannelError_CMD_REPEAT_ERROR;
		return error;
	}

	if(advanceOptions->repeatPacketNum > 0)
		cmdSendConfig->byRepeatPacketNumber = advanceOptions->repeatPacketNum;
	else
	{
		error = ReturnChannelError_CMD_REPEAT_ERROR;
		return error;
	}

	cmdSendConfig->byRepeatPacketTimeInterval = advanceOptions->repeatPacketTimeInterval;
	return error;

}

void RC_Reset_DebugInfo(DebugInfo* debugInfo)
{
	debugInfo->totalGetBufferCount = 0;
	debugInfo->totalPKTCount = 0;
	debugInfo->leadingTagErrorCount = 0;
	debugInfo->endTagErrorCount = 0;
	debugInfo->checkSumErrorCount = 0;

}
