/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 * 
 * File Name: receive_send_config.h
 * Brief: This file mainly configs the function about the receive and send.
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
/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "gd32f4xx.h"

#include "main.h"
#include "receive_send_config.h"
#include "sys_config.h"
#include "protocol.h"
#include "string.h"
#include "node_period_config.h"
#include "pin_period_config.h"
#include "peripheral_config.h"
#include "Protocol.h"
#include "adc_config.h"
#include "i2c.h"
#include "xl9555.h"

/*************************************************************************************
                                             --- Macro Define ---
*************************************************************************************/
#define USART5_DATA_ADDRESS ((uint32_t)&USART_DATA(USART5))
#define USART5_PORT GPIOC
#define USART5_TX_PIN GPIO_PIN_6
#define USART5_RX_PIN GPIO_PIN_7
#define USART5_GPIO_AF GPIO_AF_8
#define USART5_BAUDRATE 115200

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
void UsartInitUsart5(void)
{
    /* gpio for usart5 */
    rcu_periph_clock_enable(RCU_USART5);
    gpio_af_set(USART5_PORT, USART5_GPIO_AF, USART5_TX_PIN);
    gpio_af_set(USART5_PORT, USART5_GPIO_AF, USART5_RX_PIN);
    gpio_mode_set(USART5_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART5_TX_PIN);
    gpio_mode_set(USART5_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART5_RX_PIN);
    gpio_output_options_set(USART5_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART5_TX_PIN);
    gpio_output_options_set(USART5_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART5_RX_PIN);

    usart_deinit(USART5);
    usart_baudrate_set(USART5, USART5_BAUDRATE);          // Set baud rate to 115200
    usart_parity_config(USART5, USART_PM_NONE);           // No parity
    usart_word_length_set(USART5, USART_WL_8BIT);         // 8 data bits
    usart_stop_bit_set(USART5, USART_STB_1BIT);           // 1 stop bit
    usart_transmit_config(USART5, USART_TRANSMIT_ENABLE); // Enable transmit
    usart_receive_config(USART5, USART_RECEIVE_ENABLE);   // Enable receive
    usart_enable(USART5);                                 // Enable USART5

    usart_interrupt_enable(USART5, USART_INT_RBNE); // Enable the USART interrupt
    nvic_irq_enable(USART5_IRQn, 2U, 0U);           // Set for the usart intterupt priority
}
