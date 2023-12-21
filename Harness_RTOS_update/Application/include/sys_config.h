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

#define CONFIG_EXTI_B_LINE(line) \
	nvic_irq_enable(EXTI##line##_IRQn, 2U, 1U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);
#define CONFIG_EXTI_B_5_9_LINE(line) \
	nvic_irq_enable(EXTI5_9_IRQn, 2U, 1U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);
#define CONFIG_EXTI_B_10_15_LINE(line) \
	nvic_irq_enable(EXTI10_15_IRQn, 2U, 1U); \
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN##line); \
	exti_init(EXTI_##line, EXTI_INTERRUPT, EXTI_TRIG_RISING); \
	exti_interrupt_flag_clear(EXTI_##line);

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <stdio.h>
#include "gd32f4xx.h"

#define TIMER_PERIOD 1000000 - 1

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void TimerInitial(void);
void PrintUniqueID(void);
void ExtiConfig(void);
void RcuConfig(void);
void NvicConfig(void);
void keyConfig(void);
void ledConfig(void);


#endif // SYS_CONFIG_H


