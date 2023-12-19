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

#define CONFIG_EXTI_B_LINE(line) \
	nvic_irq_enable(EXTI##line##_IRQn, 0U, 3U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);
#define CONFIG_EXTI_B_5_9_LINE(line) \
	nvic_irq_enable(EXTI5_9_IRQn, 0U, 3U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);
#define CONFIG_EXTI_B_10_15_LINE(line) \
	nvic_irq_enable(EXTI10_15_IRQn, 0U, 3U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void I2cTask(void);
void GpioInputModeConfig(void);
void ExtiReconfig(uint8_t num);
void ExtiConfig(void);

#endif /* PERIPHERAL_CONFIG_H */
