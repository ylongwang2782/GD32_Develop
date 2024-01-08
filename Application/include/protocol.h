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
	uint16_t	L_TotalLineVolum:10;		//Ҫ���ͨ�ϵ��ߵ�����
	uint16_t	Z_TotalLineVolum:6;		//��Ҫ������ֵ��������
}TotalLineIndex;

typedef struct  
{
	uint8_t	ID[4];		//�豸ΨһID�ţ����ذ��˲�������ʱ϶��
	uint8_t	L_LineVolum;	//���豸�����Ƶ�ͨ�ϼ����������
	uint8_t Z_LineVolum;	//���豸�����Ƶ��迹�����������
}SynchSlot;

typedef struct
{
	uint8_t 		SlotVolum;						//��ʱ϶����Ҳ�Ǳ������ܵĽڵ���
	TotalLineIndex 	totalLineVolum;		//�����������������ͨ���໹���迹�࣬����ֵ�ϵ������нڵ������Ӧ��������֮��
	SynchSlot 		synchSlot[MAX_NODES] ;				//�ڵ���������������ÿ���ڵ��ID�ź������Ƶ�������
}SynchData;

typedef struct 
{
	uint8_t frameHEADER[3];
	uint8_t frameLength;
	uint8_t frameSeqNum;
	uint8_t slotNumber;
	uint8_t frameType;  
} DataFrameHeader;  // ֡ͷ�ṹ��

/* BROADCAST FRAME STRUCTURE */
/* �㲥֡�ṹ�� */
typedef struct 
{
	DataFrameHeader header;
	SynchData synchData;
	uint8_t checksum;
} BroadcastFrame;

/* ���ݷ��ͽṹ�� */
typedef struct 
{
	DataFrameHeader header;
	uint8_t Data_Send[1];
	uint8_t checksum;
} DataSendFrame;

/* DATA FRAME STRUCTURE */
/* ͨ�����ݽṹ�� */
typedef struct 
{
	uint8_t Head[3];
	uint8_t Length;
	uint8_t SeqNum;
	uint8_t SlotNumber;
	uint8_t Type;
	uint8_t Data[1];
}DataFrameHead;

/* �豸״̬�ṹ�� */
typedef struct
{
    uint16_t 	statusColor	:1;				//��ɫ������ƥ��״̬��	0����ɫ��ƥ����޴�������1����ɫƥ��
    uint16_t 	statusProbe1	:1;				//ͨ��״̬1��			0��̽��Ͽ���1��̽�뵼ͨ
    uint16_t 	statusProbe2	:1;				//ͨ��״̬2��			0��̽��Ͽ���1��̽�뵼ͨ
    uint16_t	statusBattaryL	:1;				//��ص͵�����			0�����������1����ص͵���
    uint16_t	statusPress	:1;				//��������״̬��			0�������ԶϿ���1�������Ե�ͨ
    uint16_t	valuePress	:11;				//��ѹ��ֵ��0~2047����Ӧ0~2047kpa
}DeviceStatus; 

/* 0x02ͨ�������ϱ�֡�ṹ�� */
typedef struct 
{
    uint8_t LineVolum;
    DeviceStatus deviceStatus;
	uint8_t Data[256];
}DataUploadFrame;

/* 0x03 �迹�����ϱ�֡�ṹ�� */
typedef struct 
{
    uint8_t LineVolumZ;
    DeviceStatus deviceStatus;
	uint8_t Data[256];
}DataUploadFrameZ;

/* 0x04 ָ��֡ */
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

