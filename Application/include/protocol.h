/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: protocol.h
 * Brief: This file mainly contains some funcition to config the peripherals.
 * 
 * 
 * 
 * Version: 
 * 
 * Revision history
 * Date             version           Author          Description
 * 
 * 
 * Par:
 * Warning: 
*************************************************************************************/
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <stdint.h>

/*************************************************************************************
                                   --- Constant & Macro Function ---
*************************************************************************************/
#define MAX_NODES 5
#define MAX_FRAME 255U
#define UPLOADCOUNT 5

extern uint8_t Running_holder;

/*************************************************************************************
                                           --- Data Structure ---
*************************************************************************************/
#pragma pack(1)

typedef struct
{
	uint16_t	L_TotalLineVolum:10;		//要检测通断的线的数量
	uint16_t	Z_TotalLineVolum:6;		//需要检测电阻值的线数量
}TotalLineIndex;

typedef struct  
{
	uint8_t	ID[4];		//设备唯一ID号，网关按此参数分配时隙号
	uint8_t	L_LineVolum;	//该设备所控制的通断检测线针数量
	uint8_t Z_LineVolum;	//该设备所控制的阻抗检测线针数量
}SynchSlot;

typedef struct
{
	uint8_t 		SlotVolum;						//总时隙数，也是表述了总的节点数
	TotalLineIndex 	totalLineVolum;		//总线针的数量，无论通断类还是阻抗类，在数值上等于所有节点的所对应的线针数之和
	SynchSlot 		synchSlot[MAX_NODES] ;				//节点描述符，描述了每个节点的ID号和所控制的线针数
}SynchData;

typedef struct 
{
	uint8_t frameHEADER[3];
	uint8_t frameLength;
	uint8_t frameSeqNum;
	uint8_t slotNumber;
	uint8_t frameType;  
} DataFrameHeader;  // 帧头结构体

/* BROADCAST FRAME STRUCTURE */
/* 广播帧结构体 */
typedef struct 
{
	DataFrameHeader header;
	SynchData synchData;
	uint8_t checksum;
} BroadcastFrame;

/* 数据发送结构体 */
typedef struct 
{
	DataFrameHeader header;
	uint8_t Data_Send[1];
	uint8_t checksum;
} DataSendFrame;

/* DATA FRAME STRUCTURE */
/* 通用数据结构体 */
typedef struct 
{
	uint8_t Head[3];
	uint8_t Length;
	uint8_t SeqNum;
	uint8_t SlotNumber;
	uint8_t Type;
	uint8_t Data[1];
}DataFrameHead;

/* 设备状态结构体 */
typedef struct
{
    uint16_t 	statusColor	:1;				//颜色传感器匹配状态。	0，颜色不匹配或无传感器；1，颜色匹配
    uint16_t 	statusProbe1	:1;				//通断状态1。			0，探针断开；1，探针导通
    uint16_t 	statusProbe2	:1;				//通断状态2。			0，探针断开；1，探针导通
    uint16_t	statusBattaryL	:1;				//电池低电量。			0，电池正常；1，电池低电量
    uint16_t	statusPress	:1;				//气密性线状态。			0，气密性断开；1，气密性导通
    uint16_t	valuePress	:11;				//气压数值。0~2047，对应0~2047kpa
}DeviceStatus; 

/* 0x02通断数据上报帧结构体 */
typedef struct 
{
    uint8_t LineVolum;
    DeviceStatus deviceStatus;
	uint8_t Data[256];
}DataUploadFrame;

/* 0x03 阻抗数据上报帧结构体 */
typedef struct 
{
    uint8_t LineVolumZ;
    DeviceStatus deviceStatus;
	uint8_t Data[256];
}DataUploadFrameZ;

/* 0x04 指令帧 */
typedef struct 
{
	uint16_t InstructionType;
	uint64_t TimeSlotIndex;
	uint8_t InstructionComment[32];
} CommandData;

typedef struct 
{
	DataFrameHeader header;
	CommandData commandData;
	uint8_t checksum;
} CommandFrame;

/*************************************************************************************
                                               --- Global Variable ---
*************************************************************************************/
extern BroadcastFrame stBroadcastFrame;
extern DataFrameHead stDataFrameHead;
extern DataUploadFrame stDataUploadFrame;
extern DataUploadFrameZ stDataUploadFrameZ;
extern CommandFrame stCommandFrame;

extern uint8_t UnlockCommand;
extern uint8_t NetReplyFlag;
extern uint8_t NetReplyBuff[];

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
uint8_t WirelessDataPacket(uint8_t const *addr,uint8_t num,uint8_t *output,uint8_t type,uint8_t slot);
uint8_t CheckOut(uint8_t const *addr,uint8_t num);
uint8_t BitGet(uint8_t source, int sourceBitPosition);
void BatDataUpload(uint16_t data);
#pragma pack()
#endif

