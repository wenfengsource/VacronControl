#define RETURNCHANNEL_PID 0x201
#define TRANSPARENT_UART_PID 0x1FED
#define GETDATATIMEOUT 3000

#include "it930x-core.h"
#include <linux/stddef.h>

//For Dynamic setting URB buffer
static int URB_BUFSIZE = 153408; //188 * 816

module_param(URB_BUFSIZE, int, S_IRUGO);
MODULE_PARM_DESC(URB_BUFSIZE, "URB_BUFSIZE = 188*n");


Endeavour *g_it9300;
static struct usb_driver IT930x_Driver;
static DEFINE_MUTEX(WRregist_flag);
Dword g_it913x_rxid_register;

struct usb_device_id IT930x_usb_id_table[] = {
	{USB_DEVICE(0x048D, 0x9306)},	//it930x
	{USB_DEVICE(0x048D, 0x9307)},	//it930x
	{USB_DEVICE(0x07ca, 0x0871)},	//A871
	{USB_DEVICE(0x07ca, 0x0891)},	//TD511B
	{0},/* Terminating entry */
};

static DEFINE_MUTEX(staticmutex);

MODULE_DEVICE_TABLE(usb, IT930x_usb_id_table);


Dword IsLock(Device_Context *DC, Bool* locked, int br_idx, int ts_idx)
{
       Dword error = Error_NO_ERROR;

        Word emptyLoop = 0;
        Word tpsLoop = 0;
        Word mpeg2Loop = 0;
        Byte channels;
        Byte emptyChannel = 1;
        Byte tpsLocked = 0;
//     FILE* fp;    
    
        DefaultDemodulator* demod;
        Bool retry = False;
        demod = (DefaultDemodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev;
        
        *locked = False;
  
        demod->statistic.signalPresented = False;
        demod->statistic.signalLocked = False;
        demod->statistic.signalQuality = 0;
        demod->statistic.signalStrength = 0;



    channels = 2;
        while (emptyLoop < 30) 
        {
				mutex_lock(&DC->dev_mutex);	//mutex lock
				
                error = Standard_readRegister ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, Processor_OFDM, empty_channel_status, &channels);
                
                mutex_unlock(&DC->dev_mutex); //mutex unlock
                
                if (error) goto exit;

                if (channels == 1) 
                {
                        emptyChannel = 0;
                        break;
                }

                User_delay ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, 50);
                emptyLoop++;
        }

        if (channels == 2) 
        {
/*
                printf("emptyLoop = %d!\n", emptyLoop);
                printf("EmptyChannel = 2!\n");
                fopen_s(&fp,"dumpMonitor.txt", "a");
                fprintf(fp, "EmptyChannel!\n");
                fclose(fp);
*/
                goto exit;
        }

        while (tpsLoop < 30) 
        {
                /** Empty channel */
                
                mutex_lock(&DC->dev_mutex);	//mutex lock
                error = Standard_isTpsLocked ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, &demod->statistic.signalPresented);
                mutex_unlock(&DC->dev_mutex); //mutex unlock
                
                if (error) goto exit;
                if (demod->statistic.signalPresented == True) {
                        tpsLocked = 1;
                        break;
                }

                if (tpsLocked == 1) break;

                User_delay ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, 60);
                tpsLoop++;
        }

        if (tpsLocked == 0) 
        {
/*
                printf("TPS Unlocked!\n");
                fopen_s(&fp,"dumpMonitor.txt", "a");
                fprintf(fp, "TPS Unlocked!\n");
                fclose(fp);
*/
                goto exit;
        }

        mpeg2Loop = 0;
        while (mpeg2Loop < 70) 
        {
				mutex_lock(&DC->dev_mutex);	//mutex lock
                error = Standard_isMpeg2Locked ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, &demod->statistic.signalLocked);
                mutex_unlock(&DC->dev_mutex); //mutex unlock
                
                if (error) goto exit;
                if (demod->statistic.signalLocked == True) {
                        demod->statistic.signalQuality = 80;
                        demod->statistic.signalStrength = 80;
                        *locked = True;
                        break;
                }
                User_delay ((Demodulator*) DC->it9300.tsSource[br_idx][ts_idx].htsDev, 100);
                mpeg2Loop++;
        }

        demod->statistic.signalQuality = 0;
        demod->statistic.signalStrength = 20;

        if(*locked == False){
/*
                printf("Mpeg Unlocked!\n");
                fopen_s(&fp,"dumpMonitor.txt", "a");
                fprintf(fp, "Mpeg Unlocked!\n");
                fclose(fp);
*/
        }
  
exit:

        return (error);
}

static void Rx_urbs_kill(Chip *br_chip)
{
	int urb_index = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
		if (br_chip->urbstatus[urb_index] == 1) {
			deb_data("\t kill busy urb[%d]\n", urb_index);
			usb_kill_urb(br_chip->urbs[urb_index]);
			//br_chip->urbstatus[urb_index] == 0;
		}
	}
}

static void Rx_urbs_unlink(Chip *br_chip)
{
	int urb_index = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
		if (br_chip->urbstatus[urb_index] == 1) {
			deb_data("\t Unlink busy urb[%d]\n", urb_index);
			usb_unlink_urb(br_chip->urbs[urb_index]);
		}
	}

	return;
}

static int Rx_urbs_stop_transfer(IT930x_Device *dev, Chip *br_chip)
{
	deb_data("- Enter %s Function (%d)\n", __func__, br_chip->acq_transfer_count);
	
	mutex_lock(&dev->DC->dev_mutex);

	if (br_chip->acq_transfer_count == 1) {
		if (!br_chip->urb_streaming) {
			//deb_data("\t %s warning: urb already stop!\n", __func__);
			return 0;
		}

		DRV_ResetRxPort(dev->DC, 0);

		br_chip->urb_streaming = false;

		msleep(1000); //Waiting urb complete.

		Rx_urbs_kill(br_chip); //Kill urb if not call back.
	}

	if (br_chip->acq_transfer_count > 0)
		br_chip->acq_transfer_count--;

	mutex_unlock(&dev->DC->dev_mutex);

	deb_data("%s done, acq_transfer_count = %d\n", __func__, br_chip->acq_transfer_count);

	return 0;
}

Dword RingBuffer_wirte(
	Chip* chip,
    Byte* pBuffer,		//want write from buffer, len = 188
    Dword* pBufferLength
)
{	
	Dword dwCpBuffLen = *pBufferLength;
	//Dword dwReadBuffAddr;
	//Dword dwWriteBuffAddr;
	Dword dwBuffLen = 0;
	unsigned long flags;

	//deb_data("- Enter %s Function -\n", __func__);

	if (chip == NULL || pBuffer == NULL)
		return Error_NO_ERROR;

	//dwReadBuffAddr = chip->ReadBuffPointAddr;
	//dwWriteBuffAddr = chip->TxWriteBuffPointAddr;

	pBuffer[0] = 0x47;

	if (chip->if_chip_start_capture) {
		if (chip->dwTxRemaingBufferSize < dwCpBuffLen) {
			*pBufferLength = 0;
			//deb_data("Overflow !! Overflow !! Overflow !! Overflow !! Overflow !!\n");
			return Error_NO_ERROR;
		}
	} else {
		return Error_NO_ERROR;
	}

    /* check read and write position */
    if (chip->TxWriteBuffPointAddr >= chip->ReadBuffPointAddr) {
		dwBuffLen = chip->dwTolBufferSize - chip->TxWriteBuffPointAddr;	/* remaining size (from current to end) */

		/* if remaining size is enough */
		if (dwBuffLen >= dwCpBuffLen) {
			memcpy((chip->pRingBuffer + chip->TxWriteBuffPointAddr), pBuffer, dwCpBuffLen);
		} else {
			memcpy((chip->pRingBuffer + chip->TxWriteBuffPointAddr), pBuffer, dwBuffLen);
			memcpy(chip->pRingBuffer, (pBuffer + dwBuffLen), dwCpBuffLen - dwBuffLen);
		}
	} else {
		memcpy(chip->pRingBuffer + chip->TxWriteBuffPointAddr, pBuffer, dwCpBuffLen);
	}

	spin_lock_irqsave(&chip->read_buf_lock, flags);
	
	//chip->TxWriteBuffPointAddr = (chip->TxWriteBuffPointAddr + dwCpBuffLen) % (chip->dwTolBufferSize);
	if ((chip->TxWriteBuffPointAddr + dwCpBuffLen) >= chip->dwTolBufferSize)
		chip->TxWriteBuffPointAddr = (chip->TxWriteBuffPointAddr + dwCpBuffLen) - chip->dwTolBufferSize;
	else
		chip->TxWriteBuffPointAddr = chip->TxWriteBuffPointAddr + dwCpBuffLen;

	chip->dwTxRemaingBufferSize -= dwCpBuffLen;
	chip->dwReadRemaingBufferSize += dwCpBuffLen;
	
	spin_unlock_irqrestore(&chip->read_buf_lock, flags);

	if (chip->g_if_wait_read) {
		if (chip->dwReadRemaingBufferSize >= chip->read_wait_len) {
			chip->g_if_wait_read = false;
			wake_up(&chip->read_wait_queue);
		}
	}

	return Error_NO_ERROR;
}

Dword RingBuffer_read(
	Byte *pBuffer,
	Dword *pBufferLength,
	Chip* chip)
{
	Dword dwCpBuffLen = *pBufferLength;
	Dword dwReadBuffAddr = chip->ReadBuffPointAddr;
	Dword dwBuffLen = 0;
	unsigned long flags;
	int tempval;

    if (*pBufferLength == 0) {
		*pBufferLength = 0;
        return Error_BUFFER_INSUFFICIENT;
    }

	while (chip->dwReadRemaingBufferSize < dwCpBuffLen) {
		chip->g_if_wait_read = true;
		chip->read_wait_len = dwCpBuffLen;
		wait_event_timeout(chip->read_wait_queue, !chip->g_if_wait_read, GETDATATIMEOUT);

		if(chip->dwReadRemaingBufferSize < dwCpBuffLen) {
			*pBufferLength = 0;
			chip->g_if_wait_read = false;
			//deb_data("RingBuffer_read : No input, time out!!!!");
			return Error_NO_ERROR;
		}

		if (!chip->if_chip_start_capture)
			return Error_NO_ERROR;

		break;
	}

	/*
	 * dwReadBuffAddr : read position
	 * dwTolBufferSize : total ringbuffer size
	 */

	/* check read and write position */
	if (dwReadBuffAddr >= chip->TxWriteBuffPointAddr) {

		dwBuffLen = chip->dwTolBufferSize - dwReadBuffAddr; //remaining size (from current to end)

		/* if remaining size is enough */
		if (dwBuffLen >= dwCpBuffLen) {
			//memcpy(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwCpBuffLen);
			tempval = copy_to_user(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwCpBuffLen);
		} else {
			//memcpy(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwBuffLen);
			//memcpy((pBuffer + dwBuffLen), chip->pRingBuffer, (dwCpBuffLen - dwBuffLen));
			tempval = copy_to_user(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwBuffLen);
			tempval = copy_to_user((pBuffer + dwBuffLen), chip->pRingBuffer, (dwCpBuffLen - dwBuffLen));
		}
	} else {
		//memcpy(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwCpBuffLen);
		tempval = copy_to_user(pBuffer, (chip->pRingBuffer + dwReadBuffAddr), dwCpBuffLen);
	}

	spin_lock_irqsave(&chip->read_buf_lock, flags);
	
	//chip->ReadBuffPointAddr = (chip->ReadBuffPointAddr + dwCpBuffLen) % (chip->dwTolBufferSize);
	if ((chip->ReadBuffPointAddr + dwCpBuffLen) >= chip->dwTolBufferSize)
		chip->ReadBuffPointAddr = (chip->ReadBuffPointAddr + dwCpBuffLen) - (chip->dwTolBufferSize);
	else
		chip->ReadBuffPointAddr = (chip->ReadBuffPointAddr + dwCpBuffLen);

	chip->dwReadRemaingBufferSize -= dwCpBuffLen;
	chip->dwTxRemaingBufferSize += dwCpBuffLen;
	
	spin_unlock_irqrestore(&chip->read_buf_lock, flags);
	
	return Error_NO_ERROR;
}

static void Rx_urbs_free(Chip *br_chip)
{
	int urb_index = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
		//deb_data("\t Free urb[%d]\n", urb_index);
		usb_free_urb(br_chip->urbs[urb_index]);
		br_chip->urbs[urb_index] = NULL;
	}

	return;
}

int Get_device_from_fops(
	File_Information *file_info,
	IT930x_Device **dev,
	Chip **chip,
	Chip **br_chip)
{
	if (!file_info) {
		deb_data("\t File_info not exist\n");
		return -1;
	}

	*dev = file_info->dev;

	if (!*dev) {
		deb_data("\t Device not exist\n");
		return -1;
	}

	*chip = (*dev)->chip[file_info->chip_index];

	if (!*chip) {
		deb_data("\t Chip not exist\n");
		return -1;
	}

	*br_chip = (*dev)->br_chip;

	if (!*br_chip) {
		deb_data("\t br_chip not exist\n");
		return -1;
	}

	return 0;
}

extern int SaveReturnChannelPackets(Byte *data_buffer, int buffer_length);
extern int SaveTransparentUARTPackets(Byte *data_buffer, int buffer_length);

void GetReturnChannelPackets_toVirtualCom(
	IT930x_Device *dev,
	Byte *From_buf,
	size_t count
)
{
	Dword len = count;
	int i = 0, j = 0;
	Word RC_PID = RETURNCHANNEL_PID;
	Word Receive_PID = 0;
	int SyncByte_pos = -1;
	Dword BufferLen;
	int offset_forRxDeviceID = 5;

	//deb_data("=================== it930x_read_toVirtualCom =================== \n");

	if (len < (188 * 5))
		return;

	/* check buffer shift and search syncbyte */
	if (dev->br_chip->if_search_syncbyte) {
		for (i = 0; i < (URB_BUFSIZE - 940); i++) {
			if ((From_buf[i] & 0xB8) == 0 &&
				(From_buf[i + 188] & 0xB8) == 0 &&
				(From_buf[i + 376] & 0xB8) == 0 &&
				(From_buf[i + 564] & 0xB8) == 0 &&
				(From_buf[i + 752] & 0xB8) == 0) {
				SyncByte_pos = i;
				deb_data("--------------------- SyncByte position = %d !!!!!\n", SyncByte_pos);
				break;
			}
		}

		if (SyncByte_pos == -1) {
			deb_data("it9135_read_toVirtualCom: CAN'T fined SyncByte !!!!!\n");
			return;
		}
	} else {
		SyncByte_pos = 0;
	}

	for (i = SyncByte_pos; i < len; i += 188) {
		if( i + 188 > len)
			continue;			
		/*if lost syncbyte*/
		if ((From_buf[i] & 0xBF) >= dev->DC->rx_number) {
			for (j = i; j < (URB_BUFSIZE - 940); j++) {
				if ((From_buf[j] & 0xB8) == 0 &&
					(From_buf[j + 188] & 0xB8) == 0 &&
					(From_buf[j + 376] & 0xB8) == 0 &&
					(From_buf[j + 564] & 0xB8) == 0 &&
					(From_buf[j + 752] & 0xB8) == 0) {
					i = j;
					deb_data("-------------------Re-search SyncByte position = %d !!!!!\n", j);
					break;
				} else {
					//deb_data("it9135_read_toVirtualCom: Can't find SyncByte !!!!!\n");
					return;
				}
			}
		}

		/* get return channel packets to virtual com */
		if (dev->DealWith_ReturnChannelPacket == 1) {
			Receive_PID = ((From_buf[i + 1] & 0x1F) << 8) + (From_buf[i + 2] & 0xFF);

			switch (Receive_PID) {

				case RETURNCHANNEL_PID:
					/* add rx_id */
					From_buf[i + offset_forRxDeviceID] = (dev->DC->rx_device_id[From_buf[i] & 0xBF] >> 8) & 0xFF;
					From_buf[i + offset_forRxDeviceID + 1] = dev->DC->rx_device_id[From_buf[i] & 0xBF] & 0xFF;

					SaveReturnChannelPackets((From_buf + i), 188);
					break;
				case TRANSPARENT_UART_PID:
					SaveTransparentUARTPackets((From_buf + i), 188);
					break;
				default:
					break;
			}
		}

		BufferLen = 188;
		
		/* write in match buffer */
		RingBuffer_wirte(dev->chip[(From_buf[i] & 0xBF)], From_buf+i, &BufferLen);

		//printk("chip num = 0x%x, Rx Num = %d\n",(From_buf[i] & 0xBF), dev->DC->rx_number);
	}
}

static void Rx_urbs_completion(struct urb *purb)
{
	Rx_Urb_Context *context = NULL;
	IT930x_Device *dev = NULL;
	Chip *br_chip = NULL;
	int ret = 0, ptype = 0;
	unsigned long flags;
	int urb_index = 0;
	Dword error = BR_ERR_NO_ERROR;
	int i;
	Byte val_1;

	context = purb->context;
	br_chip = context->br_chip;
	dev = (IT930x_Device *)br_chip->dev;
	br_chip->if_search_syncbyte = false;

	if (purb->actual_length != URB_BUFSIZE) {
		deb_data("\t Warning: Rx_urb[%d].length = %d\n", context->index, purb->actual_length);
		br_chip->if_search_syncbyte = true;
	}

	ptype = usb_pipetype(purb->pipe);

	switch(purb->status) {
	case 0:				//success
		break;

	case -ETIMEDOUT:	//NAK
		deb_data("\t Warning: Rx_urb[%d] ETIMEDOUT.\n", context->index);
		return;

	case -ECONNRESET:	//unlink
		deb_data("\t Warning: Rx_urb[%d] ECONNRESET.\n", context->index);
		return;

	case -ENOENT:		//kill
		deb_data("\t Warning: Rx_urb[%d] ENOENT.\n", context->index);
		br_chip->urbstatus[context->index] = 0;
		return;

	case -ESHUTDOWN:
		deb_data("\t Warning: Rx_urb[%d] ESHUTDOWN.\n", context->index);
		return;
	case -ETIME:
		deb_data("\t Warning: Rx_urb[%d] ETIME.\n", context->index);
		return;
	case -EOVERFLOW:
		deb_data("\t Warning: Rx_urb[%d] EOVERFLOW.\n", context->index);
		return;		
	default:			//error
		deb_data("\t Warning: Rx_urb[%d] Unknown Error = %d.\n", context->index, purb->status);
		break;
	}

	if (ptype != PIPE_BULK) {
		deb_data("\t Error: RX unsupported URB type %d\n", ptype);
		return ;
	}
	
	//deb_data("\t Warning: Rx_urb[%d] \n", context->index);

	br_chip->urbstatus[context->index] = 0;

	/* wait all urb submit when start */
	if (br_chip->urb_streaming) {
		if (purb->actual_length == URB_BUFSIZE)
			GetReturnChannelPackets_toVirtualCom(dev, br_chip->urbs[context->index]->transfer_buffer, URB_BUFSIZE);

		if (br_chip->urb_streaming) {		/* second check after parse function! */
			//if(!br_chip->urbstatus[context->index])
			{
				ret = usb_submit_urb(br_chip->urbs[context->index], GFP_ATOMIC);

				if (ret != 0) {
					deb_data("\t Error: urb[%d] resubmit fail, err = %d\n" , context->index, ret);
					//Rx_urbs_stop_transfer(br_chip);
					return ;
				} else {
					//deb_data("\tcomplete urb[%d] resubmit success\n", context->index);
				}
				br_chip->urbstatus[context->index] = 1;
			}
		}
	}
}

static void Rx_urbs_start_transfer(IT930x_Device *dev, Chip *br_chip, Chip *chip)
{
	int urb_index = 0, ret = -ENOMEM;

	deb_data("- Enter %s Function (%d, %d)\n", __func__, chip->chip_index, br_chip->acq_transfer_count);

	mutex_lock(&dev->DC->dev_mutex);

	br_chip->acq_transfer_count++;
	
///==========================for it913x ringbuffer============================
	/* Init ringbuffer parameter*/
	chip->ReadBuffPointAddr = 0;
	chip->dwReadRemaingBufferSize = 0;
	chip->TxWriteBuffPointAddr = 0;
	chip->dwTxRemaingBufferSize = chip->dwTolBufferSize;

///==========================for it930x URB============================

	if (br_chip->acq_transfer_count == 1) {
		DRV_ResetRxPort(dev->DC, 1);

		for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
			//if(!br_chip->urbstatus[urb_index])
			{
				ret = usb_submit_urb(br_chip->urbs[urb_index], GFP_ATOMIC);

				if (ret != 0) {
					deb_data("\t Error: urb[%d] start tran submit fail(%d), err = %d\n", urb_index, br_chip->acq_transfer_count, ret);
					
					mutex_unlock(&dev->DC->dev_mutex);
					Rx_urbs_stop_transfer(dev, br_chip);
					return;
				}
				else {
					//deb_data("\t start urb[%d] submit success\n", urb_index);
				}

				br_chip->urbstatus[urb_index] = 1;
			}
		}
		
		br_chip->urb_streaming = true;
	}
	
	mutex_unlock(&dev->DC->dev_mutex);

	deb_data("%s done (%d), Start transferring\n", __func__, br_chip->acq_transfer_count);
}

int Rx_fops_open(
	struct inode *inode,
	struct file *file)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	struct usb_interface *interface = NULL;
	int minor[2];
	int ret = 0;
	Byte chip_index = 0;

	deb_data("- Enter %s Function -\n", __func__);

	minor[0] = iminor(inode);
	minor[1] = minor[0] - 1;

	deb_data("OPEN - Input minor: %d\n", minor[0]);

get_interface:
	while (!interface) {
		minor[1]++;

		if (minor[1] > minor[0] + 16) {
			deb_data("\t Error: can't find interface from minor %d\n", minor[1]);
			ret = -ENODEV;
			goto exit;
		}

		interface = usb_find_interface(&IT930x_Driver, minor[1]);
	}

	dev = usb_get_intfdata(interface);

	if (!dev) {
		deb_data("\t Error: device not exist\n");
		ret = -ENODEV;
		goto exit;
	}

	if ((minor[0] != dev->minor[0]) & (minor[0] != dev->minor[1]) &
		(minor[0] != dev->minor[2]) & (minor[0] != dev->minor[3]) &
		(minor[0] != dev->minor[4]) & (minor[0] != dev->minor[5]) &
		(minor[0] != dev->minor[6]) & (minor[0] != dev->minor[7])) {

		interface = NULL;
		goto get_interface;
	}

	/* 4ch device */
	if (dev->DC->it9300.receiver_chip_number[0] == 4 && dev->minor[0] == minor[0])
		chip_index = 0;
	if (dev->DC->it9300.receiver_chip_number[0] == 4 && dev->minor[1] == minor[0])
		chip_index = 1;
	if (dev->DC->it9300.receiver_chip_number[0] == 4 && dev->minor[2] == minor[0])
		chip_index = 2;
	if (dev->DC->it9300.receiver_chip_number[0] == 4 && dev->minor[3] == minor[0])
		chip_index = 3;

	/* 8ch device */
	if (dev->DC->it9300.receiver_chip_number[1] == 4 && dev->minor[4] == minor[0])
		chip_index = 4;
	if (dev->DC->it9300.receiver_chip_number[1] == 4 && dev->minor[5] == minor[0])
		chip_index = 5;
	if (dev->DC->it9300.receiver_chip_number[1] == 4 && dev->minor[6] == minor[0])
		chip_index = 6;
	if (dev->DC->it9300.receiver_chip_number[1] == 4 && dev->minor[7] == minor[0])
		chip_index = 7;

	/* 2ch device */
	if (dev->DC->it9300.receiver_chip_number[0] == 2 && dev->minor[0] == minor[0])
		chip_index = 0;
	if (dev->DC->it9300.receiver_chip_number[0] == 2 && dev->minor[1] == minor[0])
		chip_index = 1;

	deb_data("OPEN - chip_index: %d\n", chip_index);

	chip = dev->chip[chip_index];

	if (!chip) {
		deb_data("\tError: chip not exist\n");
		ret = -ENODEV;
		goto exit;
	}

	chip->chip_index = chip_index;

	atomic_add(1, &dev->DC->filter_count);
	deb_data("OPEN - Filter Count: %d\n", atomic_read(&dev->DC->filter_count));
	chip->chip_open_count++;

refile_info:
	file_info = kzalloc(sizeof(File_Information), GFP_KERNEL);

	if (!file_info) {
		deb_data("\t Error: file_info out of memory\n");
		ret = -ENODEV;
		goto refile_info;
	}

	/* Save our object in the file's private structure */
	file->private_data = file_info;
	file_info->dev = dev;
	file_info->chip_index = chip->chip_index;
	file_info->if_file_start_capture = false;
	
	deb_data("chip->chip_open_count = %d\n", chip->chip_open_count);

	if (chip->chip_open_count == 1) {
		dev->file[chip->chip_index] = file;
		dev->chip[chip->chip_index] = chip;
		/* Init ringbuffer parameter */
		spin_lock_init(&chip->read_buf_lock);
	}

	deb_data("- %s success -\n",  __func__);
	return ret;

exit:
	deb_data("- %s fail -\n",  __func__);
    return ret;
}

static int Rx_fops_release(
	struct inode *inode,
	struct file *file)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	//Byte chip_index = 0;
	int urb_index = 0, br_idx = 0, ts_idx = 0;
	//int i;
	//Dword	error = Error_NO_ERROR;

	deb_data("- Enter %s Function -\n", __func__);

	file_info = (File_Information *)file->private_data;

	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("\t Warning: fops already release\n");
		return -ENODEV;
	}

	///Illegal close the ap.
	if (chip->if_chip_start_capture && file_info->if_file_start_capture) {
		if (!dev->disconnect) {
			Rx_urbs_stop_transfer(dev, br_chip);
			if (br_chip->acq_transfer_count == 0)
				Rx_urbs_unlink(br_chip);
		}

		chip->if_chip_start_capture = false;
		file_info->if_file_start_capture = false;
	}

	//chip_index = file_info->chip_index;
	chip->chip_open_count--;

	atomic_sub(1, &dev->DC->filter_count);
	deb_data("Release - Filter Count: %d\n", atomic_read(&dev->DC->filter_count));	

	if (dev->disconnect == 1) {
	
		deb_data("Release - it9300 Acquire transfer count: %d \n", br_chip->acq_transfer_count);
		deb_data("Release - >>>>>>>>>>>>release chip %d\n", file_info->chip_index);

		if (atomic_read(&dev->DC->filter_count) == 0) {

			/* Release chip */
			for (br_idx = 0; br_idx < dev->DC->it9300_Number; br_idx++) {
				for (ts_idx = 0; ts_idx < dev->DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {
					file_info->chip_index = ((4 * br_idx) + ts_idx);
					

					if (dev->chip[file_info->chip_index]->pRingBuffer) {
						deb_data("Release - %d, %d chip release!!", br_idx, ts_idx);	
						free_pages((long unsigned int)dev->chip[file_info->chip_index]->pRingBuffer, get_order(dev->chip[file_info->chip_index]->dwTolBufferSize));

						kfree(dev->chip[file_info->chip_index]);
						dev->chip[file_info->chip_index] = NULL;
						dev->file[file_info->chip_index] = NULL;
					}
				}
			}

			/* Release urb buffer */
			Rx_urbs_free(br_chip);

			for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
				if (br_chip->br_pRingBuffer[urb_index]) {
					free_pages((long unsigned int)br_chip->br_pRingBuffer[urb_index], get_order(URB_BUFSIZE));
					br_chip->br_pRingBuffer[urb_index] = NULL;
				}
			}

			kfree(br_chip);
			deb_data("Release - br_chip release!!");

			if (dev->DC->temp_read_buffer)
				kfree(dev->DC->temp_read_buffer);

			if (dev->DC->temp_write_buffer)
				kfree(dev->DC->temp_write_buffer);

			kfree(dev->DC);
			kfree(dev);
			
			//g_it913x_rxid_register = 0;
		}
		
		msleep(50);
	}
	
	kfree(file_info);
	file->private_data = NULL;

	return 0;
}

int Rx_fops_ioctl(
	struct inode *inode,
	struct file *file,
	unsigned int cmd,
	unsigned long parg)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	
	//Byte br_idx = 0, ts_idx = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	file_info = (File_Information *)file->private_data;

	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("%s error: file operate not correct\n", __func__);
		return -ENODEV;
	}
	
	//printk("----------chip index = %d \n\n", chip->chip_index);
	//printk("----------chip index2 = %d \n\n", file_info->dev->chip[file_info->chip_index]->chip_index);

	if (dev->disconnect) {
		deb_data("\t Error: device already disconnect\n");
		return -1;
	}

	//br_idx = chip->chip_index / 4;
	//ts_idx = chip->chip_index % 4;

	switch (cmd)
	{
		case IOCTL_ITE_DEMOD_STARTCAPTURE:
		{
			if (!chip->if_chip_start_capture) {
				chip->if_chip_start_capture = true;
				file_info->if_file_start_capture = true;
				Rx_urbs_start_transfer(dev, br_chip, chip);
				return 0;
			} else {
				return -1;
			}
		}

		case IOCTL_ITE_DEMOD_STOPCAPTURE:
		{
			if (file_info->if_file_start_capture) {
				Rx_urbs_stop_transfer(dev, br_chip);
				file_info->if_file_start_capture = false;
				chip->if_chip_start_capture = false;

				if (chip->g_if_wait_read) {
					chip->g_if_wait_read = false;
					wake_up(&chip->read_wait_queue);
				}
				return 0;
			} else {
				return -1;
			}
		}

		case IOCTL_ITE_DEMOD_GETRXDEVICEID:
		{
		
			Word *rx_id = NULL;
			
			rx_id = (Word *)parg;
			*rx_id = (dev->DC->rx_device_id[chip->chip_index] & 0xFFF);
			//deb_data("-----------RXID = %d", (dev->DC->rx_device_id[chip->chip_index] & 0xFFF));

			return 0;
		}
	}

	//return DL_DemodIOCTLFun((void*)dev, (Dword)cmd, parg, chip->chip_index);
	return DemodIOCTLFun((void*)dev, (Dword)cmd, parg, chip->chip_index);
	//return DemodIOCTLFun((void*)file_info->dev, (Dword)cmd, parg, file_info->dev->chip[file_info->chip_index]->chip_index);
}

long Rx_fops_unlocked_ioctl(
	struct file *file,
	unsigned int cmd,
	unsigned long parg)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	
	//Byte br_idx = 0, ts_idx = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	file_info = (File_Information *)file->private_data;
	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("%s error: file operate not correct\n", __func__);
		return -ENODEV;
	}

	if (dev->disconnect) {
		deb_data("\t Error: device already disconnect\n");
		return -1;
	}

	//br_idx = chip->chip_index / 4;
	//ts_idx = chip->chip_index % 4;

	switch (cmd) {
	case IOCTL_ITE_DEMOD_STARTCAPTURE:
	{		
		if (!chip->if_chip_start_capture) {
			chip->if_chip_start_capture = true;
			file_info->if_file_start_capture = true;
			Rx_urbs_start_transfer(dev, br_chip, chip);
			return 0;
		} else {
			return -1;
		}
	}

	case IOCTL_ITE_DEMOD_STOPCAPTURE:
	{		
		if (file_info->if_file_start_capture) {
			Rx_urbs_stop_transfer(dev, br_chip);
			file_info->if_file_start_capture = false;
			chip->if_chip_start_capture = false;

			if (chip->g_if_wait_read) {
				chip->g_if_wait_read = false;
				wake_up(&chip->read_wait_queue);
			}
			return 0;
		} else {
			return -1;
		}
	}

	case IOCTL_ITE_DEMOD_GETRXDEVICEID:
	{
		Word rx_id = 0xFFFF;
		
		//rx_id = (Word *)parg;
		//*rx_id = ((dev->DC->it9133[br_idx][ts_idx].returnchannel_rx_dev_id) & 0xFFFF);
		rx_id = (dev->DC->rx_device_id[chip->chip_index] & 0xFFFF);
		copy_to_user((void *)parg, (void *)&rx_id, sizeof(Word));
		return 0;
	}

	case IOCTL_ITE_DEMOD_ACQUIRECHANNEL:
	{
		AcquireChannelRequest Request;

		if (chip->if_chip_start_capture) {
			Request.error = 2;
			copy_to_user((void *)parg, (void *)&Request, sizeof(AcquireChannelRequest));
			return 0;
		}
	}

	}

	return DemodIOCTLFun((void*)dev, (Dword)cmd, parg, chip->chip_index);
}

#ifdef CONFIG_COMPAT
static long Rx_fops_compact_ioctl(
	struct file *file,
	unsigned int cmd,
	unsigned long parg)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	Word *rx_id = NULL;
	unsigned long compat_parg;
	Byte br_idx = 0, ts_idx = 0;

	file_info = (File_Information *)file->private_data;
	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("%s error: file operate not correct\n", __func__);
		return -ENODEV;
	}

	br_idx = chip->chip_index / 4;
	ts_idx = chip->chip_index % 4;

	//deb_data("- Enter %s Function -\n", __func__);

	compat_parg = (unsigned long)compat_ptr(parg);

	return Rx_fops_unlocked_ioctl(file, cmd, compat_parg);
}
#endif

static ssize_t Rx_fops_read(
	struct file *file,
	char __user *buf,
	size_t count,
	loff_t *ppos)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	Dword len = 0;

	file_info = (File_Information *)file->private_data;
	
	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("%s error: file operate not correct\n", __func__);
		return -1;
	}

	if (dev->disconnect) {
		deb_data("\t Error: device already disconnect\n");
		return -1;
	}

	if (!file_info->if_file_start_capture) {
		return -1;
	}

	len = count;
	RingBuffer_read(buf, &len, chip);

	return len;
}

static ssize_t Rx_fops_write(
	struct file *file,
	const char __user *user_buffer,
	size_t count,
	loff_t *ppos)
{
	File_Information *file_info = NULL;
	IT930x_Device *dev = NULL;
	Chip *chip = NULL;
	Chip *br_chip = NULL;
	struct usb_device *usb_dev = NULL;
	Byte *b_buf = kzalloc(sizeof(user_buffer), GFP_KERNEL);
	Dword result = Error_NO_ERROR;
	int act_len = 0;

	//deb_data("- Enter %s Function -\n", __func__);

	file_info = (File_Information *)file->private_data;
	if (Get_device_from_fops(file_info, &dev, &chip, &br_chip) < 0) {
		deb_data("%s error: file operate not correct\n", __func__);
		return -ENODEV;
	}

	if (dev->disconnect) {
		deb_data("\t Error: device already disconnect\n");
		kfree(b_buf);
		return -1;
	}

	usb_dev = dev->DC->usb_dev;

	//copy_from_user(b_buf, user_buffer, count);

	result = usb_bulk_msg(usb_get_dev(usb_dev),
			   usb_sndbulkpipe(usb_get_dev(usb_dev), 0x06),
			   b_buf, count, &act_len, 100000);

	if (result)
		deb_data("%s error: Usb2_writeControlBus fail [0x%08lx]\n", __func__, result);

	kfree(b_buf);

	return 0;
}

static struct file_operations Rx_File_Operations = {
	.owner = THIS_MODULE,
	.open = Rx_fops_open,
	.read = Rx_fops_read,
	.write = Rx_fops_write, //AirHD
	.release = Rx_fops_release,

#ifdef CONFIG_COMPAT
	.compat_ioctl = Rx_fops_compact_ioctl,
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 35)
	.unlocked_ioctl = Rx_fops_unlocked_ioctl,
#else
	.ioctl = Rx_fops_ioctl,
#endif
};

static struct usb_class_driver IT930x_Class_Driver = {
	.name = "usb-it930x%d",
	.fops = &Rx_File_Operations,
	.minor_base = USB_it930X_MINOR_BASE
};

static struct usb_class_driver IT930x_Class_Driver_2 = {
	.name = "usb-it930x%d",
	.fops = &Rx_File_Operations,
	.minor_base = USB_it930X_MINOR_BASE
};

void IT930x_print_info(void)
{
	deb_data("======================================================\n");
	#ifdef DRIVER_RELEASE_VERSION
		deb_data("DRIVER_RELEASE_VERSION: 		%s\n", DRIVER_RELEASE_VERSION);
	#else
		deb_data("DRIVER_RELEASE_VERSION: 		v0.0.0.0\n");
	#endif

	#ifdef ENDEAVOUR_FW_RELEASE_VERSION
		deb_data("ENDEAVOUR_FW_RELEASE_VERSION: 		%s\n", ENDEAVOUR_FW_RELEASE_VERSION);
	#else
		deb_data("ENDEAVOUR_FW_RELEASE_VERSION:	v0_0_0_0\n");
	#endif

	#ifdef DVB_LL_VERSION1
		deb_data("ENDEAVOUR_FW_RELEASE_LINK_VERSION:	%d.%d.%d.%d\n", DVB_LL_VERSION1, DVB_LL_VERSION2, DVB_LL_VERSION3, DVB_LL_VERSION4);
		deb_data("ENDEAVOUR_FW_RELEASE_OFDM_VERSION:	%d.%d.%d.%d\n", DVB_OFDM_VERSION1, DVB_OFDM_VERSION2, DVB_OFDM_VERSION3, DVB_OFDM_VERSION4);
	#else
		deb_data("ENDEAVOUR_FW_RELEASE_LINK_VERSION:	v0.0.0.0\n");
		deb_data("ENDEAVOUR_FW_RELEASE_OFDM_VERSION:	v0.0.0.0\n");
	#endif

	#ifdef LL_VERSION1
		deb_data("FW_RELEASE_LINK_VERSION:		%d.%d.%d.%d\n", LL_VERSION1, LL_VERSION2, LL_VERSION3, LL_VERSION4);
		deb_data("FW_RELEASE_OFDM_VERSION:		%d.%d.%d.%d\n", OFDM_VERSION1, OFDM_VERSION2, OFDM_VERSION3, OFDM_VERSION4);
	#else
		deb_data("FW_RELEASE_LINK_VERSION:		v0.0.0.0\n");
		deb_data("FW_RELEASE_OFDM_VERSION:		v0.0.0.0\n");
	#endif

	#ifdef Version_NUMBER
		deb_data("API_RX_RELEASE_VERSION:		%X.%X.%X\n", Version_NUMBER, Version_DATE, Version_BUILD);
	#else
		deb_data("API_RX_RELEASE_VERSION:		000.00000000.0\n");
	#endif

	deb_data("Company:ITEtech\n");
	deb_data("======================================================\n");
}

static int IT913x_reboot_notifier(struct notifier_block *n, unsigned long state, void *cmd)
{
	struct _IT930x_DEVICE *dev;

	deb_data("- Enter %s Function -\n", __func__);

	dev = container_of(n, struct _IT930x_DEVICE, reboot_notifier);
	//mtd->_reboot(mtd);
	
	if (dev && !dev->disconnect) {
		DRV_ResetRxPort(dev->DC, 0);
		msleep(1000); //Waiting urb complete.
	}
	
	return NOTIFY_DONE;
}

/* Register a USB device node end */
static int IT930x_probe(
	struct usb_interface *intf,
	const struct usb_device_id *id)
{
	IT930x_Device *dev = NULL;
	Device_Context *DC = NULL;
	Chip *br_chip = NULL;
	int ret = 0, urb_index = 0, act_len = 0, i = 0, j = 0;
	Bool is_enough = false;
	Byte chip_index = 0, br_idx = 0, ts_idx = 0, valtemp = 0;
	Byte reg_value = 0;	
	char path[40] = {0};
	Word temp_devid;

	IT930x_print_info();

	///========= default set ============
	deb_data("===== it930x usb device pluged in =====\n");
	deb_data("- Enter %s Function -\n", __func__);


	/* Allocate memory for our device state and initialize */
	i = 0;
all_dev:
	dev = kzalloc(sizeof(IT930x_Device), GFP_KERNEL);
	if (!dev) {
		deb_data("\t Error: IT930x_Device out of memory\n");
		ret = -ENOMEM;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_dev;
	}

	
	i = 0;
all_DC:
	DC = kzalloc(sizeof(Device_Context), GFP_KERNEL);
	if (!DC)	{
		deb_data("\t Error: Device_Context out of memory\n");
		ret = -ENOMEM;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_DC;
	}
	
	i = 0;
all_temp_rebuf:
	DC->temp_read_buffer = kzalloc(sizeof(int) * 1024, GFP_KERNEL);
	if (!DC->temp_read_buffer) {
		deb_data("\t Error: temp_read_buffer out of memory\n");
		ret = -ENOMEM;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_temp_rebuf;
	}
	
	i = 0;
all_temp_wrbuf:
	DC->temp_write_buffer = kzalloc(sizeof(int) * 1024, GFP_KERNEL);
	if (DC->temp_write_buffer == NULL) {
		deb_data("\t Error: temp_write_buffer out of memory\n");
		ret = -ENOMEM;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_temp_wrbuf;
	}
	
	i = 0;
all_temp_urbbuf:
	DC->temp_urb_buffer = kzalloc(URB_BUFSIZE, GFP_KERNEL);
	if (DC->temp_urb_buffer == NULL) {
		deb_data("\t Error: temp_urb_buffer out of memory\n");
		ret = -ENOMEM;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_temp_urbbuf;
	}

	for (i = 0; i < 16; i++) {
		dev->minor[i] = 0;
	}

	dev->DC = DC;
	dev->DealWith_ReturnChannelPacket = 0;
	mutex_init(&dev->DC->dev_mutex);

	DC->usb_dev = interface_to_usbdev(intf);
	DC->it9300.userData = DC;
	atomic_set(&dev->DC->filter_count, 0);

	DC->it9300.ctrlBus = BUS_USB;	///for read eeprom
	DC->it9300.maxBusTxSize = 63;	///for read eeprom

	g_it9300 = &DC->it9300;

	DC->it9300.usbbus_timeout = 500;
	DC->rx_number = 0;
	DC->map_enalbe = false;
	DC->hihg_byte = 0;
	DC->low_byte = 0;
	DC->if_degug = 0;

	///======================= Device Communication Test =======================
	for(i = 0; i < 5; i++) {
		DL_device_communication_test(DC);
	}
	///=======================================================================
	DC->it9300.usbbus_timeout = 3000;//10000;

	///========================== Get EEPROM Config ===========================
	ret = DL_GetEEPROMConfig(DC);
	if (ret) {
		deb_data("\t[PROBE] - Error: Get EEPROM fail!. \n");
		goto exit;
	}

	///=========================== it930 default set ===========================	
	i = 0;
all_brchip:
	dev->br_chip = kzalloc(sizeof(Chip), GFP_KERNEL);
	if (!dev->br_chip) {
		deb_data("\t Error: br_chip out of memory\n");
		ret = -ENODEV;
		if(i > 10) goto exit;
		i++;
		msleep(500);
		goto all_brchip;
	}

	dev->br_chip->acq_transfer_count = 0;

	///=========================== it913 default set ===========================	
	for (br_idx = 0; br_idx < DC->it9300_Number; br_idx++) {
		DC->rx_number += DC->it9300.receiver_chip_number[br_idx];

		for (ts_idx = 0; ts_idx < DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {
			chip_index = ((4 * br_idx) + ts_idx);
			i = 0;
all_chip:
			dev->chip[chip_index] = kzalloc(sizeof(Chip), GFP_KERNEL);	///Chip
			if (!dev->chip[chip_index]) {
				deb_data("\t Error: chip[%d] out of memory\n", chip_index);
				ret = -ENODEV;
				if(i > 10) goto exit;
				i++;
				msleep(500);
				goto all_chip;
			}

			dev->chip[chip_index]->dev = (void *)dev;
			dev->chip[chip_index]->dwTolBufferSize = URB_BUFSIZE * URB_COUNT;

			init_waitqueue_head(&dev->chip[chip_index]->read_wait_queue);
			dev->chip[chip_index]->g_if_wait_read = false;
			dev->chip[chip_index]->read_wait_len = 0;
			
			i = 0;
all_ringbuf:
			dev->chip[chip_index]->pRingBuffer = (Byte*)__get_free_pages(GFP_KERNEL, get_order(dev->chip[chip_index]->dwTolBufferSize));
			if (!dev->chip[chip_index]->pRingBuffer) {
				deb_data("\t Error: Chip RingBuffer[%d] out of memory\n", chip_index);
				ret = -ENODEV;
				if(i > 10) goto exit;
				i++;
				msleep(500);
				goto all_ringbuf;
			}

			dev->chip[chip_index]->chip_open_count = 0;	/* Chip open count */
			dev->chip[chip_index]->if_chip_start_capture = false; 	/* chip startcapture flag */
		}
	}

	//Dynamic urb buffer size
	if(URB_BUFSIZE <= 0) 
		URB_BUFSIZE = 153408;
	else if(((URB_BUFSIZE % 188)!= 0))	
		URB_BUFSIZE=((URB_BUFSIZE/188)+1)*188;	
	
	DC->it9300.it9300user_usb20_frame_size_dw = URB_BUFSIZE / 4;
	deb_data("--- URB_BUFSIZE:%u --- \n", URB_BUFSIZE);

	///========================= Endeavour init ========================
	ret = DL_IT930x_device_init(DC);
	if (ret) {
		deb_data("\t[PROBE] - Error: device init fail\n");
		goto exit;
	}

	///====================== it930 urb default set =====================	
	br_chip = dev->br_chip;
	if (!br_chip) {
		deb_data("\t[PROBE] - Error: chip not exist\n");
		ret = -ENODEV;
		goto exit;
	}

	//dev->br_chip = br_chip;
	br_chip->urb_streaming = false;

	//atomic_set(&br_chip->urb_count, BR_URB_COUNT);
	dev->br_chip->dwTolBufferSize = (URB_BUFSIZE * BR_URB_COUNT);

	/*Allocate RX urb*/
	for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
		
		i = 0;
all_br_ringbuf:
		dev->br_chip->br_pRingBuffer[urb_index] = (Byte*)__get_free_pages(GFP_KERNEL, get_order(URB_BUFSIZE));
		br_chip->urbs[urb_index] = usb_alloc_urb(0, GFP_KERNEL);
		if (!br_chip->urbs[urb_index]) {
			deb_data("\t Error: brChip RingBuffer[%d] out of memory\n", chip_index);
			ret = -ENOMEM;
			if(i > 10) goto exit;
			i++;
			msleep(500);
			goto all_br_ringbuf;
		}

		br_chip->urbs[urb_index]->transfer_buffer = br_chip->br_pRingBuffer[urb_index];
		if (!br_chip->urbs[urb_index]->transfer_buffer) {
			usb_free_urb(br_chip->urbs[urb_index]);
			br_chip->urbs[urb_index] = NULL;
			ret = -ENOMEM;
			goto exit;
		}

		br_chip->urb_context[urb_index].index = urb_index;
		br_chip->dev = (void *)dev;
		br_chip->urbstatus[urb_index] = 0;
		br_chip->urb_context[urb_index].br_chip =(void *)dev->br_chip;
		br_chip->urbs[urb_index]->status = -EINPROGRESS;

		usb_fill_bulk_urb(
			br_chip->urbs[urb_index],
			dev->DC->usb_dev,
			usb_rcvbulkpipe(dev->DC->usb_dev, 0x84),
			br_chip->urbs[urb_index]->transfer_buffer,
			URB_BUFSIZE,
			Rx_urbs_completion,
			&br_chip->urb_context[urb_index]);
	}

	///============================= register =============================	
	//if(dev->DC->map_enalbe) {
		/* search enough & continuous handle name space for device*/

		if((dev->DC->rx_device_id[0] < 0) || (dev->DC->rx_device_id[0] >= 32)) 
			temp_devid = 0;
		else 
			temp_devid = dev->DC->rx_device_id[0];

		DC->auto_regist = False;
		
		for(i = temp_devid; i < 32; i++) {
			if(g_it913x_rxid_register & (1 << i)) {
				j = 0;
			} else {
				j += 1;

				if(j == (dev->DC->rx_number)) {
					j = i - (dev->DC->rx_number) + 1;
					is_enough = True;
					DC->auto_regist = True;
					break;
				}
			}
		}

		if(!is_enough) {
			j = 0;
			for(i = 0; i <= temp_devid; i++) {
				if(g_it913x_rxid_register & (1 << i)) {
					j = 0;
				} else {
					j += 1;

					if(j == (dev->DC->rx_number)) {
						j = i - (dev->DC->rx_number) + 1;
						is_enough = True;
						DC->auto_regist = True;
						break;
					}
				}
			}
		}
	//}

	///======================== Clean PSB Buffer =========================
	/* read and throw the shift data from PSB buffer. (data len = 512) */
	usb_bulk_msg(usb_get_dev(dev->DC->usb_dev),
					usb_rcvbulkpipe(usb_get_dev(dev->DC->usb_dev), 0x84),
					DC->temp_urb_buffer,
					URB_BUFSIZE,
					&act_len,
					1000);

	deb_data("Throw shift data, urb length = %d\n", act_len);

	kfree(DC->temp_urb_buffer);

	///========================== Enable TsPort ==========================
	if (DC->board_id != 48) {
		for (br_idx = 0; br_idx < dev->DC->it9300_Number; br_idx++) {
			/* enable tsport_0 */
			ret = IT9300_enableTsPort(&dev->DC->it9300, br_idx, 4);
			if (ret) {
				deb_data("PROBE - Enable %d it9300 TsPort_0 fail\n", br_idx);
			}

			/* ignore_sync_byte */
			ret = IT9300_writeRegister(&dev->DC->it9300, br_idx, 0xda1a, 0x1);
			if (ret) {
				deb_data("PROBE - Set %d it9300 ignore_sync_byte fail\n", br_idx);
			}
		}
	}

	/* enable tsport_1~4 */
	for (br_idx = 0; br_idx < dev->DC->it9300_Number; br_idx++) {
		for (ts_idx = 0; ts_idx < dev->DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {

			ret = IT9300_enableTsPort(&dev->DC->it9300, br_idx, ts_idx);
			if (ret) {
				deb_data("PROBE - Enable %d it9300 Tsport_%d fail\n", br_idx, ts_idx+1);
			}

			reg_value = 1;
			//FDI : omega ofdm 0xa5 close.
			//Demodulator_writeRegisters((Demodulator *) dev->DC->it9300.tsSource[br_idx][ts_idx].htsDev, 8, 0xa5, 1, &reg_value);
			//DL_Demodulator_writeRegisters((Demodulator *) dev->DC->it9300.tsSource[br_idx][ts_idx].htsDev, 8, 0xa5, 1, &reg_value);
			DL_Demodulator_writeRegisters(dev->DC, 8, 0xa5, 1, &reg_value, br_idx, ts_idx);
		}

		IT9300_setIgnoreFail(g_it9300, br_idx, true);	//open error packet
	}
	
	usb_set_intfdata(intf, dev);
	
	/* Register the device, get minor */
	for(br_idx = 0; br_idx < DC->it9300_Number; br_idx++) {
		for(ts_idx = 0; ts_idx < DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {
			chip_index = ((4 * br_idx) + ts_idx);
			intf->minor = -1;

			//if(dev->DC->map_enalbe) {
				if(DC->auto_regist) {
					dev->DC->rx_device_id[chip_index] = (j + chip_index);	//change rx_ID
					printk("REGIST - %d, %d, change RxDevice_ID = %d \n", br_idx, ts_idx, dev->DC->rx_device_id[chip_index]);
					sprintf(DC->filter_name[chip_index], "usb-it930x%d", (j + chip_index)); /* Change handle number */
					g_it913x_rxid_register |= 1 << (j + chip_index); // set bit = 1
			
					if(DC->is_rx_init[br_idx][ts_idx]) {
						IT930x_Class_Driver.name = DC->filter_name[chip_index];
						deb_data("handle name -->> %s\n", IT930x_Class_Driver.name);
						ret = usb_register_dev(intf, &IT930x_Class_Driver);
					}else{
						deb_data("handle name -->> %s Fail\n", IT930x_Class_Driver.name);
					}
				} else {
					ret = usb_register_dev(intf, &IT930x_Class_Driver_2);
				}
				
				
			//} 
			//else {
			//	ret = usb_register_dev(intf, &IT930x_Class_Driver_2);
			//}
			
			if (ret) {
				deb_data("\t Error: not able to get a minor for chip[%d].\n", chip_index);				
				usb_set_intfdata(intf, NULL);
				ret = -ENOTTY;
				goto exit;
			}

			dev->minor[chip_index] = intf->minor;
			//deb_data("INIT ----------- minor %d = %d\n", chip_index, intf->minor);
		}
	}
	
	dev->reboot_notifier.notifier_call = IT913x_reboot_notifier;
	register_reboot_notifier(&dev->reboot_notifier);

	deb_data("- %s success -\n",  __func__);
	return ret;

exit:
	deb_data("- %s fail -\n",  __func__);
    return ret;
}

static int IT930x_suspend(
	struct usb_interface *intf,
	pm_message_t state)
{
	return 0;
}

static int IT930x_resume(struct usb_interface *intf)
{
	return 0;
}

static void IT930x_disconnect(struct usb_interface *intf)
{
	IT930x_Device *dev = NULL;
	int chip_index = 0, br_idx = 0, ts_idx = 0, urb_index = 0;
	//Dword	error = Error_NO_ERROR;

	deb_data("- Enter %s Function -\n", __func__);

	dev = usb_get_intfdata(intf);
	if (!dev) {
		deb_data("DISCONNECT - Error: device not exist\n");
		return;
	}

	dev->disconnect = true;

	Rx_urbs_stop_transfer(dev, dev->br_chip);
	//msleep(1000);
	//Rx_urbs_kill(dev->br_chip);
	Rx_urbs_unlink(dev->br_chip);
	Rx_urbs_free(dev->br_chip);
	
	DL_IT930x_device_deinit(dev->DC);

	/* Give back minor */
	for (br_idx = 0; br_idx < dev->DC->it9300_Number; br_idx++) {
		for (ts_idx = 0; ts_idx < dev->DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {
			chip_index = ((4 * br_idx) + ts_idx);

			if(dev->DC->auto_regist) {
				g_it913x_rxid_register &= ~(1 << dev->DC->rx_device_id[chip_index]);	//set 0
				IT930x_Class_Driver.name = dev->DC->filter_name[chip_index];
				usb_deregister_dev(intf, &IT930x_Class_Driver);
			} else {
				usb_deregister_dev(intf, &IT930x_Class_Driver_2);
			}			
			
			intf->minor = dev->minor[chip_index];			
#ifdef DISCONNECT_DEBUG_MESSAGE
			deb_data("DISCONNECT - USB IT930x (USBSkel-%d) now disconnected\n", dev->minor[chip_index]);
#endif
		}
	}

	if (dev->reboot_notifier.notifier_call)
		unregister_reboot_notifier(&dev->reboot_notifier);

	usb_set_intfdata(intf, NULL);

	if (atomic_read(&dev->DC->filter_count) == 0) {
		for (br_idx = 0; br_idx < dev->DC->it9300_Number; br_idx++) {
			for (ts_idx = 0; ts_idx < dev->DC->it9300.receiver_chip_number[br_idx]; ts_idx++) {
				chip_index = ((4 * br_idx) + ts_idx);

				if (dev->chip[chip_index]->pRingBuffer)
					free_pages((long unsigned int)dev->chip[chip_index]->pRingBuffer, get_order(dev->chip[chip_index]->dwTolBufferSize));

				kfree(dev->chip[chip_index]);
				dev->chip[chip_index] = NULL;
#ifdef DISCONNECT_DEBUG_MESSAGE
				deb_data("DISCONNECT - %d, %d chip close!!", br_idx, ts_idx);
#endif
			}
		}

		for (urb_index = 0; urb_index < BR_URB_COUNT; urb_index++) {
			if (dev->br_chip->br_pRingBuffer[urb_index]) {
				free_pages((long unsigned int)dev->br_chip->br_pRingBuffer[urb_index], get_order(URB_BUFSIZE));
			}
		}

		kfree(dev->br_chip);
		dev->br_chip = NULL;

		if (dev->DC->temp_read_buffer)
			kfree(dev->DC->temp_read_buffer);

		if (dev->DC->temp_write_buffer)
			kfree(dev->DC->temp_write_buffer);
		
		kfree(dev->DC);
		kfree(dev);
			
		//g_it913x_rxid_register = 0;
	}
	else {
		deb_data("DISCONNECT - Ap is open!!");
	}

	return;
}

static struct usb_driver IT930x_Driver = {
#if LINUX_VERSION_CODE <=  KERNEL_VERSION(2, 6, 15)
	.owner = THIS_MODULE,
#endif
	.name = "usb-it930x",
	.probe = IT930x_probe,
	.disconnect = IT930x_disconnect,
	.id_table = IT930x_usb_id_table,
	.suspend = IT930x_suspend,
	.resume = IT930x_resume,
};

static int __init IT930x_module_init(void)
{
	int result = 0;

	deb_data("- Enter %s Function -\n", __func__);
	//request_module("tty_Virtual");
	if ((result = usb_register(&IT930x_Driver))) {
		deb_data("\t Error: usb_register fail [%d]\n", result);
		return result;
	}

	return 0;
}

static void __exit IT930x_module_exit(void)
{
	deb_data("- Enter %s Function -\n", __func__);
	usb_deregister(&IT930x_Driver);
}

module_init(IT930x_module_init);
module_exit(IT930x_module_exit);

MODULE_AUTHOR("Jason Dong <jason.dong@ite.com.tw>");
MODULE_DESCRIPTION("DTV Native Driver for Device Based on ITEtech it930x");
MODULE_VERSION(DRIVER_RELEASE_VERSION);
MODULE_LICENSE("GPL");

