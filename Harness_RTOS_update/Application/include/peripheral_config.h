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
#ifndef PERIPHERAL_CONFIG_H
#define PERIPHERAL_CONFIG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"

extern uint32_t APINS[];
/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void I2cTask(void);
void GpioInputModeConfig(void);
void ExtiReconfig(uint8_t num);
void ExtiConfig(void);

#endif /* PERIPHERAL_CONFIG_H */
