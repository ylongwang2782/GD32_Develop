/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: main.c
 * Brief: 
 * ������װǶ��ʽ���
 * 
 * Version: 
 * 2023-12-06 1.0.19 wang.yunlong9@byd.com 
 * Revision history
 * Date             version           Author          Description
 * 
 * 
 * Par:All code is developed by the ���ϿƼ�C����淶2023
 * Warning: 
*************************************************************************************/
/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <stdbool.h>
#include "node_period_config.h"
#include "receive_send_config.h"
#include "sys_config.h"
#include "peripheral_config.h"
#include "pin_period_config.h"
#include "i2c.h"
#include "xl9555.h"
#include "adc_config.h"
/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

int main(void) 
{
    systick_config();          
    RcuConfig();/* RCUʱ������ */    
    NvicConfig();/* �ж����ȼ���ʼ�� */
    GpioInputModeConfig();/* ͨ������ģʽ���� */
    ExtiConfig();/* �ⲿ�ж����� */    
    UsartInitUsart0();/* ����0��ʼ�� */
    UsartInitUsart5();/* ����5��ʼ�� */
    i2c_gpio_config();/* configure GPIO */
    i2c_config();/* configure I2C */
    i2c_eeprom_init();/* initialize EEPROM */
    
    eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
    eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);
//    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);/* I2C���üĴ��� */
//    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);/* I2C����Ĵ��� */
    eeprom_buffer_read_interrupt(i2c_LoRa_channel_config, XL9555_input_reg_cmd_L, 1);/* ��ȡ���뿪����ֵ */
    printf("AT+MODE=0\r\n");/* ��������ģʽ */
    delay_1ms(10);
    printf("AT+RFCH=%d\r\n",255 - i2c_LoRa_channel_config[0]);/* �����ŵ� */
    delay_1ms(10);
    printf("AT+MODE=1\r\n");/* �˳�����ģʽ */
    delay_1ms(10);

    TimerInitial();/* ��ʱ����ʼ�� */
    AdcConfig();/* ����ADC */
    AdcGpioConifg();/* ADC�������� */
    StatusLed(); /* ״̬�Ƴ�ʼ�� */
    PrintUniqueID();/* �ϵ��ʼ����ͨ��USART0��ӡ�豸4λID�� */

    while(1)
    {
        // BatVolMonitor();
        PinDataCollectZ();
        I2cTask();
    }
}

