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

uint8_t I2cReceiver[2]; //������ŵ�ƽ��[0]��8λ��[1]��8λ

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

int main(void) 
{
    // ϵͳ�������ú���
    systick_config();          
    RcuConfig();
    
    // ���ݲɼ���غ�������
    GpioInputModeConfig();
    ExtiConfig();
    
    // ���ݴ�����غ�������
    UsartInitUsart0();
    UsartInitUsart5();
//    Usart0DmaConfig();
//    Usart5DmaConfig();

    /* configure GPIO */
    i2c_gpio_config();
    /* configure I2C */
    i2c_config();
    /* initialize EEPROM */
    i2c_eeprom_init();

    TimerInitial();
    NvicConfig();

    /* ����ADC */
    AdcConfig();
    AdcGpioConifg();

    /* �ϵ��ʼ����ͨ��USART0��ӡ�豸4λID�� */
    PrintUniqueID();
    
    /* �������üĴ��� */
    eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
    eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);

    /* ���üĴ��� */
    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);
    /* ����Ĵ��� */
    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);

    while(1)
    {
        BatVolMonitor();
        PinDataCollectZ();
        I2cTask();
    }
}

