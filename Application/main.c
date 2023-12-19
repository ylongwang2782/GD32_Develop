/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: main.c
 * Brief: 
 * 线束工装嵌入式软件
 * 
 * Version: 
 * 2023-12-06 1.0.19 wang.yunlong9@byd.com 
 * Revision history
 * Date             version           Author          Description
 * 
 * 
 * Par:All code is developed by the 弗迪科技C代码规范2023
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

uint8_t I2cReceiver[2]; //存放引脚电平，[0]低8位，[1]高8位

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

int main(void) 
{
    // 系统基本配置函数
    systick_config();          
    RcuConfig();
    
    // 数据采集相关函数配置
    GpioInputModeConfig();
    ExtiConfig();
    
    // 数据传输相关函数配置
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

    /* 配置ADC */
    AdcConfig();
    AdcGpioConifg();

    /* 上电初始化即通过USART0打印设备4位ID号 */
    PrintUniqueID();
    
    /* 控制配置寄存器 */
    eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
    eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);

    /* 配置寄存器 */
    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);
    /* 输出寄存器 */
    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);

    while(1)
    {
        BatVolMonitor();
        PinDataCollectZ();
        I2cTask();
    }
}

