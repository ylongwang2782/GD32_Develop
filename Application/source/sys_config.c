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
/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "sys_config.h"
#include "Protocol.h"
#include "global_config.h"

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

void StatusLed(void)
{
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1); 
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);  
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);  
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);    // Set the GPIO output options to push-pull and medium speed (50MHz).
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);    // Set the GPIO output options to push-pull and medium speed (50MHz).
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);    // Set the GPIO output options to push-pull and medium speed (50MHz).
    gpio_bit_set(GPIOC, GPIO_PIN_1); // Set the GPIO pin to a high level (master state).
    gpio_bit_set(GPIOC, GPIO_PIN_2); // Set the GPIO pin to a high level (master state).
    gpio_bit_set(GPIOC, GPIO_PIN_3); // Set the GPIO pin to a high level (master state).
}

/*************************************************************************************
 * Function Name: PrintUniqueID
 * Description: 打印芯片4位ID号
 * Param[out]: unique ID with 4 8bit variable.
 * Retrun: none
 * Exception: none
*************************************************************************************/
void PrintUniqueID(void)
{
    int i;
    // 从地址0x1FFF7A10读取设备ID
    uint32_t *device_PID = (uint32_t *)(0x1FFF7A10);
    // 将读取到的唯一ID号分别存入四个字节中
    stDevice.ID[0] = (uint8_t)(*device_PID >> 24); // Extract the most significant byte (byte1).
    stDevice.ID[1] = (uint8_t)((*device_PID >> 16) & 0xFF); // Extract the next byte (byte2).
    stDevice.ID[2] = (uint8_t)((*device_PID >> 8) & 0xFF); // Extract the next byte (byte3).
    stDevice.ID[3] = (uint8_t)(*device_PID & 0xFF); // Extract the least significant byte (byte4).
    for(i = 0; i <4 ; i++)
    {
        usart_data_transmit(USART5, stDevice.ID[i]);
        while(RESET == usart_flag_get(USART5, USART_FLAG_TBE));
    }
}

/*************************************************************************************
 * Function Name: TimerConfig
 * Description: initialize the timer
 * Param[in|out]: none
 * Retrun: none
 * Exception: none
*************************************************************************************/
void TimerInitial(void)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    /* Configure the TIMER1 to make a timer interrupt */
    timer_deinit(TIMER1);
    timer_initpara.prescaler = (1 - 1);
    timer_initpara.period = TIMER_PERIOD; // 设置定时器周期数，晶振频率为25M，定时器触发间隔=晶振频率（25M）/周期数
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_init(TIMER1, &timer_initpara);
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
}

/*************************************************************************************
  * Function Name: RcuConfig
  * Description: This function enables peripheral clocks for GPIO ports, the system configuration controller,
  * Param[in|out]: none
  * Retrun: none
  * Exception: none
*************************************************************************************/
void RcuConfig(void)
{
    /* Enable peripheral clocks for GPIO ports */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);

    /* Enable peripheral clock for the system configuration controller */
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* Enable peripheral clocks for USART modules */
    rcu_periph_clock_enable(RCU_USART0);
    rcu_periph_clock_enable(RCU_USART5);

    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
}

/*************************************************************************************
 * Function Name: NvicConfig
 * Description: config the Nvic of the peripheral which rank the priority of mutiple peripherals
 * Param[in|out]: none
 * Retrun: none
 * Exception: none
*************************************************************************************/
void NvicConfig(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    nvic_irq_enable(I2C2_EV_IRQn, 0U, 1U);
    nvic_irq_enable(I2C2_ER_IRQn, 0U, 0U);
    nvic_irq_enable(TIMER1_IRQn, 1U, 1U);
}
