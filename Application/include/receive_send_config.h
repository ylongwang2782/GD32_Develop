/*************************************************************************************
 * File Name: receive_send_config.h
 * Brief: This file mainly configs the function about the receive and send.
 * 
 * 
 * config the RCU
 * config the NVIC
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
#ifndef RECEIVE_SEND_CONFIG_H
#define RECEIVE_SEND_CONFIG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <stdio.h> // 包含标准I/O库，用于FILE类型的定义
#include "gd32f4xx.h"
#include "global_config.h"

#define USART0_DATA_ADDRESS ((uint32_t)&USART_DATA(USART0))
#define USART2_DATA_ADDRESS ((uint32_t)&USART_DATA(USART2))
#define USART5_DATA_ADDRESS ((uint32_t)&USART_DATA(USART5))

#define USART0_PORT GPIOA
#define USART0_TX_PIN GPIO_PIN_9
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_GPIO_AF GPIO_AF_7
#define USART0_BAUDRATE 115200

#define USART5_PORT GPIOC
#define USART5_TX_PIN GPIO_PIN_6
#define USART5_RX_PIN GPIO_PIN_7
#define USART5_GPIO_AF GPIO_AF_8
#define USART5_BAUDRATE 115200


/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/



void Usart0DmaConfig(void);
void Usart0DmaSend(uint8_t *u8DmaBuffer,uint16_t u16DmaDataLen);
void Usart5DmaConfig(void);
void Usart5DmaSend(uint8_t *u8DmaBuffer,uint16_t u16DmaDataLen);

void UsartInitUsart0(void);
void UsartInitUsart5(void);

#endif // USART_H
