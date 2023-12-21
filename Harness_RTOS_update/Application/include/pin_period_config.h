/*************************************************************************************
 * File Name: pin_period_config.h
 * Brief: This file mainly contains the function about the pin period.
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
#ifndef PIN_PERIOD_CONIFG_H
#define PIN_PERIOD_CONIFG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
#include <stdio.h>

extern uint32_t BPINS[];

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void RemoveElementAtIndex(uint8_t arr[], uint8_t index);
void PinDataCollect(void);
void PinDataCollectZ(void);
void PinSet(uint32_t gpio_periph, uint32_t pin);
void PinReset(uint32_t gpio_periph, uint32_t pin);
void ResetArray(uint8_t arr[], int size);
uint32_t CalculatePinSum(uint8_t u8SlotNum, uint8_t type);
#endif // PIN_PERIOD_CONIFG_H
