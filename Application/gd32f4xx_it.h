/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: gd32f4xx_it.c
 * Brief: Config some system fuction and periphral include the timer, RCU, NVIC
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
*************************************************************************************/
#ifndef GD32F4XX_IT_H
#define GD32F4XX_IT_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
/* 定时器中断服务函数 */
void TIMER1_IRQHandler(void);
/* 串口五中断服务函数 */
void USART5_IRQHandler(void);
/* 外部中断服务函数 */
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_9_IRQHandler(void);
void EXTI10_15_IRQHandler(void) ;
/* I2C0 event handle function */
void I2C0_EV_IRQHandler(void);
/* I2C0 error handle function */
void I2C0_ER_IRQHandler(void);
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
/* this function handles SVC exception */
void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
/* this function handles PendSV exception */
void PendSV_Handler(void);
/* this function handles SysTick exception */
void SysTick_Handler(void);

#endif /* GD32F4XX_IT_H */
