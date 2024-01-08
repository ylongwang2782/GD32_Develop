/*************************************************************************************
 * File Name: peripheral_config.h
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
 * Par: 
 * Copyright(c) 2022 by XXXX. All rights reserved.
*************************************************************************************/
#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void AdcConfig(void);
void AdcReconfig(uint8_t num);
void AdcGpioConifg(void);
uint16_t movingAverageFilter(uint16_t *buffer, int length);
uint16_t AdcChannelSample(uint8_t channel);
uint8_t AdcToResistance(uint16_t AdcValue);
void BatVolMonitor(void);

#endif /* ADC_CONFIG_H */
