/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: global_config.h
 * Brief: 定义全局变量
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

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
#include "global_config.h"
#include "Protocol.h"

/*************************************************************************************
                                    --- Local Constant & Type Defined ---
*************************************************************************************/
uint8_t u8DmaSendBuffer[MaxBufferLen];
uint8_t u8DmaSendBufferZ[MaxBufferLen];
// Array for the detecting pins
uint32_t APINS[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5};
uint32_t BPINS[] = {GPIO_PIN_11, GPIO_PIN_8, GPIO_PIN_5, GPIO_PIN_2, GPIO_PIN_10, GPIO_PIN_7, GPIO_PIN_4 ,GPIO_PIN_1, 
                            GPIO_PIN_9, GPIO_PIN_6, GPIO_PIN_3 ,GPIO_PIN_0};
uint32_t CPINS[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6 ,GPIO_PIN_7, 
                            GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 ,GPIO_PIN_11,GPIO_PIN_12, GPIO_PIN_13,GPIO_PIN_14 ,GPIO_PIN_15};
uint32_t DPINS[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6 ,GPIO_PIN_7, 
                            GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 ,GPIO_PIN_11,GPIO_PIN_12, GPIO_PIN_13,GPIO_PIN_14 ,GPIO_PIN_15};
                            
uint8_t g_u8NodeHexDataBuffer[MaxBufferLen];/* Hex data buffer during the node time */
int16_t g_i16PinNumBefore; /* The sum of the pins that before this slot. */
uint8_t g_u8SlotBinDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8NodeBinDataBuffer[MaxBufferLen];    /* The binary data buffer during the node time */

int16_t g_i16PinNumBeforeZ; /* The sum of the pins that before this slot. */
uint16_t g_u16SlotAdcDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8SlotResDataBuffer[SlotBufferLen]; /* The binary data buffer during the stSlot time */
uint8_t g_u8NodeAdcDataBuffer[MaxBufferLen];    /* The binary data buffer during the node time */

uint8_t g_u8ReceiveBytePosition = 0u; /* The pointer g_u8ReceiveBytePosition for the receiving byte array check */
uint8_t g_u8ReceiveByteHeaderPosition = 0u;  /* The pointer of the header g_u8ReceiveBytePosition for the header frame check */
uint8_t g_u8DataReceiveBuffer[sizeof(BroadcastFrame)];  /* The data receive buffer for the data received from the usart */
uint8_t g_u8FrameHeader[] = {0xA5, 0xFF, 0xCC}; /* Array of the frame header for header check */

volatile int64_t g_i16Period_counter = -1;  /* A time pointer for the all period time, counted by the timer interrupt */
volatile int64_t g_i16PeriodOrder = -1; /* The order of the stSlot, config by the broadcast frame, deafault is -1 */

/* 默认设备信息 */
SynchSlot stDevice = { {0x00, 0x00, 0x00, 0x01}, 0 , 0 }; 
SynchSlot stSlot;   /* A SynchSlot stSlot used to compare the headerfile */
