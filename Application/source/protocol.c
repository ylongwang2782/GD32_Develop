/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: protocol.c
 * Brief: ͨѶЭ��
 * Version: 
 * 
 * Revision history
 * Date             version           Author          Description
 * 
 * 
 * Par:
 * Warning: 
*************************************************************************************/

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <string.h>
#include <stdint.h>
#include "protocol.h"
#include "global_config.h"

/*************************************************************************************
                                    --- Local Constant & Type Defined ---
*************************************************************************************/
BroadcastFrame stBroadcastFrame;	/* Define the broadcast frame using the BroadcastFrame structure */
DataFrameHead stDataFrameHead;	/* Define the stDataSendFrame using the DataSendFrame structure */
DataUploadFrame stDataUploadFrame;
DataUploadFrameZ stDataUploadFrameZ;
CommandFrame stCommandFrame;
static uint8_t s_u8Ser=0u;															//��̬ȫ�ֱ���,ֻ�ڱ��ļ�����Ч
const uint8_t s_byDataHead[]={0xA5,0xFF,0xCC};
uint8_t UnlockCommand;
uint8_t NetReplyFlag = 0;
uint8_t NetReplyBuff[64];

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
/*************************************************************************************
 * Function Name: CheckOut
 * Description: ��������ĵ�ַ�ͳ����������У����
 * *addr[in]: ҪУ������ݵ�ַ
 * num[in]: У�����ݵĳ���
 * Retrun: ���У��ֵ
 * Exception: none
*************************************************************************************/
uint8_t CheckOut(uint8_t const *addr,uint8_t num)
{
	uint16_t i;
	uint8_t chk=0u;
	for(i=0;i<num;i++)
	{
		chk^=addr[i];
	}
	return chk;
}

/*************************************************************************************
 * Function Name: WirelessDataPacket
 * Description: Pack the structure into the a bity array.
 * *addr[in]: The address of the structure
 * num[in]: the length of the structure
 * *output[out]: output byte array 
 * type[in]: typeframe of the output byte array
 * slot[in]: slotnum of the output byte array
 * Retrun: 
 * Exception: none
*************************************************************************************/
uint8_t WirelessDataPacket(uint8_t const *addr,uint8_t num,uint8_t *output,uint8_t type,uint8_t slot)
{
	DataFrameHead *pDataPack;
	pDataPack=(DataFrameHead *)output;
	if(num > MAX_FRAME)
	{
		return 0;																																			//���Ҫ��������ݳ����޶����ȣ������������ı��βε�ַ����
	}
	else
	{
		memcpy(pDataPack->Head,s_byDataHead,sizeof(s_byDataHead));										//֡ͷ����
		pDataPack->SeqNum=s_u8Ser++;																									//֡��ˮ�����������
		pDataPack->SlotNumber=slot;																										//���ʱ϶��
		pDataPack->Type=type;																													//���֡����
		pDataPack->Length=num+sizeof(DataFrameHead);																//ͳ�Ʋ�����У������ݰ�����
		memcpy(pDataPack->Data,addr,num);																							//���ݸ���
		pDataPack->Head[pDataPack->Length - 1]=CheckOut(pDataPack->Head,pDataPack->Length);				//����У��,֡ͷ�����������
//		pDataPack->Length;																												 	//��У���ֽں��㵽�ܳ�����ȥ
		return 1;
	}
}

/*************************************************************************************
 * Function Name: BatDataUpload
 * Description: ��ص�ѹ����ϴ�����
 * Retrun: 
 * Exception: none
*************************************************************************************/
void BatDataUpload(uint16_t data)
{
    // ��ֳɸ��ֽں͵��ֽ�
    uint8_t i;
    uint8_t ByteArray[2];
    uint8_t BatBuff[20];
    ByteArray[0] = (uint8_t)(data >> 8);
    ByteArray[1] = (uint8_t)data;

    WirelessDataPacket((uint8_t const *)&ByteArray, 2, BatBuff, 9, g_i16PeriodOrder + 1);

    for ( i = 0; i < 2 + PACKLEN; i++)
    {
        usart_data_transmit(USART5, BatBuff[i]);
        while (RESET == usart_flag_get(USART5, USART_FLAG_TBE));
    }
}

/*************************************************************************************
 * Function Name: BitGet
 * Description: ���ָ��λ����
 * Param[in]:
 * Param[in]: 
 * Retrun: 
 * Exception: 
*************************************************************************************/
uint8_t BitGet(uint8_t source, int sourceBitPosition)
{
    uint8_t bitValue = (source >> sourceBitPosition) & 0x01;
    return bitValue;
}


