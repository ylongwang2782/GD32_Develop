/*************************************************************************************
 * File Name: global_config.h
 * Brief: This file contain macro defines and the global variables.
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
 * Par: 
 * Copyright(c) 2022 by XXXX. All rights reserved.
*************************************************************************************/
#ifndef global_config_H
#define global_config_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
#include "protocol.h"

/*************************************************************************************
                                   --- Constant & Macro Function ---
*************************************************************************************/
#define MaxBufferLen 256
#define SlotBufferLen 16
#define AdcChannelNum 6
#define PACKLEN 8

/*************************************************************************************
                                               --- Global Variable ---
*************************************************************************************/
extern uint8_t u8DmaSendBuffer[MaxBufferLen];
extern uint8_t u8DmaSendBufferZ[MaxBufferLen];

extern SynchSlot stSlot;
extern SynchSlot stDevice;

extern uint8_t g_u8SlotBinDataBuffer[];
extern uint8_t g_u8NodeBinDataBuffer[];
extern uint16_t g_u16SlotAdcDataBuffer[];
extern uint8_t g_u8SlotResDataBuffer[];
extern uint8_t g_u8NodeAdcDataBuffer[];

extern int16_t g_i16PinNumBefore;
extern int16_t g_i16PinNumBeforeZ;

extern uint8_t g_u8ReceiveBytePosition;
extern uint8_t g_u8ReceiveByteHeaderPosition;
extern uint8_t g_u8DataReceiveBuffer[];
extern uint8_t g_u8FrameHeader[];
extern uint8_t g_u8NodeHexDataBuffer[];

extern uint32_t APINS[];
extern uint32_t BPINS[];
extern uint32_t CPINS[];
extern uint32_t DPINS[];

extern volatile int64_t g_i16PeriodOrder;
extern volatile int64_t g_i16Period_counter;

#endif /* global_config_H */
