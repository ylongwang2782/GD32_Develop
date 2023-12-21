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

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void UsartInitUsart5(void);
void Exti0Config(void);

#endif // USART_H
