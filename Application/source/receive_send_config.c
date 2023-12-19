/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: receive_send_config.h
 * Brief: 收发相关函数
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
#include "receive_send_config.h"
#include "global_config.h"
#include "Protocol.h"

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

/*************************************************************************************
 * Function Name: Usart0DmaConfig
 * Description: Usart0 TX = DMA1 Channel 7
 * Param[in|out]: none
 * Retrun: none
 * Exception: none
 *************************************************************************************/
void Usart0DmaConfig(void)
{
    dma_single_data_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH7);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);
}

/*************************************************************************************
 * Function Name: Usart0DmaSend
 * Description: Usart0 TX = DMA1 Channel 7
 * Param[in]: u8DmaBuffer, input the buffer to send by DMA
 * Param[in]: u16DmaDataLen, the length of the buffer
 * Retrun: none
 * Exception: none
 *************************************************************************************/
void Usart0DmaSend(uint8_t *u8DmaBuffer, uint16_t u16DmaDataLen)
{
    dma_channel_disable(DMA1, DMA_CH7);
    dma_memory_address_config(DMA1, DMA_CH7, DMA_MEMORY_0, (uint32_t)u8DmaBuffer);
    dma_transfer_number_config(DMA1, DMA_CH7, u16DmaDataLen);
    dma_channel_enable(DMA1, DMA_CH7);
    usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
}

/*************************************************************************************
 * Function Name: Usart5DmaConfig
 * Description: 通过DMA将数据发送至USART5 Usart5 TX = DMA1 Channel 6
 * Param[in|out]: none
 * Retrun: none
 * Exception: none
 *************************************************************************************/
void Usart5DmaConfig(void)
{
    dma_single_data_parameter_struct dma_init_struct;
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.periph_addr = USART5_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH7);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);
}

/*************************************************************************************
 * Function Name: Usart5DmaSend
 * Description: 触发DMA发送
 * Param[in]: u8DmaBuffer, input the buffer to send by DMA
 * Param[in]: u16DmaDataLen, the length of the buffer
 * Retrun: none
 * Exception: none
 *************************************************************************************/
void Usart5DmaSend(uint8_t *u8DmaBuffer, uint16_t u16DmaDataLen)
{
    dma_channel_disable(DMA1, DMA_CH7);
    dma_memory_address_config(DMA1, DMA_CH7, DMA_MEMORY_0, (uint32_t)u8DmaBuffer);
    dma_transfer_number_config(DMA1, DMA_CH7, u16DmaDataLen);
    dma_channel_enable(DMA1, DMA_CH7);
    usart_dma_transmit_config(USART5, USART_TRANSMIT_DMA_ENABLE);
}

/*************************************************************************************
 * Function Name: UsartInitUsart0
 * Description: Initialize USART0 for communication.
 * Param[in|out]:
 * Retrun:
 * Exception:
 *************************************************************************************/
void UsartInitUsart0(void)
{
    /* gpio for usart0 */
    gpio_af_set(USART0_PORT, USART0_GPIO_AF, USART0_TX_PIN);
    gpio_af_set(USART0_PORT, USART0_GPIO_AF, USART0_RX_PIN);
    gpio_mode_set(USART0_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_TX_PIN);
    gpio_mode_set(USART0_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART0_RX_PIN);
    gpio_output_options_set(USART0_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_TX_PIN);
    gpio_output_options_set(USART0_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_RX_PIN);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, USART0_BAUDRATE);          // Set baud rate to 115200
    usart_parity_config(USART0, USART_PM_NONE);           // No parity
    usart_word_length_set(USART0, USART_WL_8BIT);         // 8 data bits
    usart_stop_bit_set(USART0, USART_STB_1BIT);           // 1 stop bit
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); // Enable transmit
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);   // Enable receive
    usart_enable(USART0);                                 // Enable USART0
}

/*************************************************************************************
 * Function Name: UsartInitUsart5
 * Description: Initialize USART5 for communication. PC6 TX PC7 RX
 * Param[in|out]:
 * Retrun:
 * Exception:
 *************************************************************************************/
void UsartInitUsart5(void)
{
    /* gpio for usart5 */
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
    nvic_irq_enable(USART5_IRQn, 0U, 4U);           // Set for the usart intterupt priority
}
