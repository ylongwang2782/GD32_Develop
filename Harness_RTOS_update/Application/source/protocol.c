/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: protocol.c
 * Brief: 通讯协议
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

#include "main.h"
#include "receive_send_config.h"
#include "sys_config.h"
#include "protocol.h"
#include "string.h"
#include "node_period_config.h"
#include "pin_period_config.h"
#include "peripheral_config.h"
#include "Protocol.h"
#include "adc_config.h"
#include "i2c.h"
#include "xl9555.h"
/*************************************************************************************
                                    --- Local Constant & Type Defined ---
*************************************************************************************/
uint8_t Running_holder = 0;
uint8_t u8DmaSendBuffer[MaxBufferLen];
uint8_t u8DmaSendBufferZ[MaxBufferLen];
int16_t g_i16PinNumBefore; /* The sum of the pins that before this slot. */
uint8_t g_u8NodeHexDataBuffer[MaxBufferLen];/* Hex data buffer during the node time */
uint8_t g_u8SlotBinDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8NodeBinDataBuffer[MaxBufferLen];    /* The binary data buffer during the node time */

int16_t g_i16PinNumBeforeZ; /* The sum of the pins that before this slot. */
uint16_t g_u16SlotAdcDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8SlotResDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8NodeAdcDataBuffer[MaxBufferLen];    /* The binary data buffer during the node time */

BroadcastFrame stBroadcastFrame;	/* Define the broadcast frame using the BroadcastFrame structure */
DataFrameHead stDataFrameHead;	/* Define the stDataSendFrame using the DataSendFrame structure */
DataUploadFrame stDataUploadFrame;
DataUploadFrameZ stDataUploadFrameZ;
CommandFrame stCommandFrame;

uint8_t g_u8SlotBinDataBuffer[16]; /* The binary data buffer during the stSlot time */
uint8_t g_u8ReceiveBytePosition = 0u; /* The pointer g_u8ReceiveBytePosition for the receiving byte array check */
uint8_t g_u8ReceiveByteHeaderPosition = 0u;  /* The pointer of the header g_u8ReceiveBytePosition for the header frame check */
uint8_t g_u8DataReceiveBuffer[sizeof(BroadcastFrame)];  /* The data receive buffer for the data received from the usart */
uint8_t g_u8FrameHeader[] = {0xA5, 0xFF, 0xCC}; /* Array of the frame header for header check */

uint8_t g_u8ReceivedByte;

volatile int64_t g_i16Period_counter = -1;  /* A time pointer for the all period time, counted by the timer interrupt */
volatile int64_t g_i16PeriodOrder = -1; /* The order of the stSlot, config by the broadcast frame, deafault is -1 */
/* 默认设备信息 */
SynchSlot stDevice = { {0x00, 0x00, 0x00, 0x01}, 0 , 0 }; 
SynchSlot stSlot;   /* A SynchSlot stSlot used to compare the headerfile */

static uint8_t s_u8Ser=0u;															//静态全局变量,只在本文件中生效
const uint8_t s_byDataHead[]={0xA5,0xFF,0xCC};
uint8_t UnlockCommand;
uint8_t NetReplyFlag = 0;
uint8_t NetReplyBuff[64];
/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
/*************************************************************************************
 * Function Name: CheckOut
 * Description: 根据输入的地址和长度生成亦或校验码
 * *addr[in]: 要校验的数据地址
 * num[in]: 校验数据的长度
 * Retrun: 亦或校验值
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
		return 0;																																			//如果要打包的数据超过限定长度，则不予打包，不改变形参地址内容
	}
	else
	{
		memcpy(pDataPack->Head,s_byDataHead,sizeof(s_byDataHead));										//帧头复制
		pDataPack->SeqNum=s_u8Ser++;																									//帧流水号组包后自增
		pDataPack->SlotNumber=slot;																										//打包时隙号
		pDataPack->Type=type;																													//打包帧类型
		pDataPack->Length=num+sizeof(DataFrameHead);																//统计不包括校验的数据包长度
		memcpy(pDataPack->Data,addr,num);																							//数据复制
		pDataPack->Head[pDataPack->Length - 1]=CheckOut(pDataPack->Head,pDataPack->Length);				//计算校验,帧头至数据域结束
//		pDataPack->Length;																												 	//将校验字节核算到总长度中去
		return 1;
	}
}

/*************************************************************************************
 * Function Name: BatDataUpload
 * Description: 电池电压打包上传函数
 * Retrun: 
 * Exception: none
*************************************************************************************/
void BatDataUpload(uint16_t data)
{
    // 拆分成高字节和低字节
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
 * Description: 获得指定位数据
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


