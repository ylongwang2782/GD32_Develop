/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: sys_config.h
 * Brief: Config some system fuction and periphral include the timer, RCU, NVIC
 * 
 * Config the timer
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
*************************************************************************************/
#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <stdio.h>
#include "gd32f4xx.h"

#define TIMER_PERIOD 1000000 - 1

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void PrintUniqueID(void);
void ReadUniqueID(uint8_t *byte1, uint8_t *byte2, uint8_t *byte3, uint8_t *byte4);
void TimerInitial(void);
void RcuConfig(void);
void NvicConfig(void);

#endif // SYS_CONFIG_H


