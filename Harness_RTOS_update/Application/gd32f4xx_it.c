/*!
    \file  gd32f4xx_it.c
    \brief interrupt service routines
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include "gd32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "protocol.h"

extern SemaphoreHandle_t binary_semaphore;
extern SemaphoreHandle_t timer_semaphore;

void TIMER1_IRQHandler(void)
{ 
    BaseType_t xHigherPriorityTaskWoken;
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)) 
    {
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
        if(NULL != timer_semaphore)
        {
            /* release binary semaphores. */
            xSemaphoreGiveFromISR(timer_semaphore, &xHigherPriorityTaskWoken);
            /* make a task switch if necessary. */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

void USART5_IRQHandler(void) 
{
    BaseType_t xHigherPriorityTaskWoken;
    if (usart_flag_get(USART5, USART_FLAG_RBNE)) 
    {
        usart_flag_clear(USART5, USART_FLAG_RBNE);
        if(NULL != binary_semaphore)
        {
            /* release binary semaphores. */
            xSemaphoreGiveFromISR(binary_semaphore, &xHigherPriorityTaskWoken);
            /* make a task switch if necessary. */
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

void EXTI0_IRQHandler(void) 
{ 
    if (RESET != exti_interrupt_flag_get(EXTI_0)) 
    { 
        g_u8SlotBinDataBuffer[11] = 1;
        exti_interrupt_flag_clear(EXTI_0); 
    } 
}
void EXTI1_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_1)) 
    { 
        g_u8SlotBinDataBuffer[7] = 1;
        exti_interrupt_flag_clear(EXTI_1); 
    }
}
void EXTI2_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_2)) 
    { 
        g_u8SlotBinDataBuffer[3] = 1;
        exti_interrupt_flag_clear(EXTI_2); 
    } 
}
void EXTI3_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_3)) 
    { 
        g_u8SlotBinDataBuffer[10] = 1;
        exti_interrupt_flag_clear(EXTI_3); 
    }
}
void EXTI4_IRQHandler(void) {
   if (RESET != exti_interrupt_flag_get(EXTI_4)) 
   { 
        g_u8SlotBinDataBuffer[6] = 1;
        exti_interrupt_flag_clear(EXTI_4); 
   } 
}
/* EXTI5_9_IRQHandler */
void EXTI5_9_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_5)) 
    { 
        g_u8SlotBinDataBuffer[2] = 1;
        exti_interrupt_flag_clear(EXTI_5); 
    }
	if (RESET != exti_interrupt_flag_get(EXTI_6)) 
    { 
        g_u8SlotBinDataBuffer[9] = 1;
		exti_interrupt_flag_clear(EXTI_6); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_7)) 
    { 
        g_u8SlotBinDataBuffer[5] = 1;  
		exti_interrupt_flag_clear(EXTI_7); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_8)) 
    { 
        g_u8SlotBinDataBuffer[1] = 1;  
		exti_interrupt_flag_clear(EXTI_8); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_9)) 
    { 
        g_u8SlotBinDataBuffer[8] = 1; 
		exti_interrupt_flag_clear(EXTI_9); 
	}
}
/* EXTI10_15_IRQHandler */
void EXTI10_15_IRQHandler(void) 
{
	if (RESET != exti_interrupt_flag_get(EXTI_10)) 
    { 
        g_u8SlotBinDataBuffer[4] = 1; 
		exti_interrupt_flag_clear(EXTI_10); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_11)) 
    { 
        g_u8SlotBinDataBuffer[0] = 1; 
		exti_interrupt_flag_clear(EXTI_11); 
	}
   
}

