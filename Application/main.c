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
/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

int main(void) 
{
    systick_config();          
    RcuConfig();/* RCU时钟配置 */    
    NvicConfig();/* 中断优先级初始化 */
    GpioInputModeConfig();/* 通断引脚模式配置 */
    ExtiConfig();/* 外部中断配置 */    
    UsartInitUsart0();/* 串口0初始化 */
    UsartInitUsart5();/* 串口5初始化 */
    i2c_gpio_config();/* configure GPIO */
    i2c_config();/* configure I2C */
    i2c_eeprom_init();/* initialize EEPROM */
    
    eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
    eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);
//    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);/* I2C配置寄存器 */
//    eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);/* I2C输出寄存器 */
    eeprom_buffer_read_interrupt(i2c_LoRa_channel_config, XL9555_input_reg_cmd_L, 1);/* 读取拨码开关数值 */
    printf("AT+MODE=0\r\n");/* 进入配置模式 */
    delay_1ms(10);
    printf("AT+RFCH=%d\r\n",255 - i2c_LoRa_channel_config[0]);/* 配置信道 */
    delay_1ms(10);
    printf("AT+MODE=1\r\n");/* 退出配置模式 */
    delay_1ms(10);

    TimerInitial();/* 定时器初始化 */
    AdcConfig();/* 配置ADC */
    AdcGpioConifg();/* ADC引脚配置 */
    StatusLed(); /* 状态灯初始化 */
    PrintUniqueID();/* 上电初始化即通过USART0打印设备4位ID号 */

    while(1)
    {
        // BatVolMonitor();
        PinDataCollectZ();
        I2cTask();
    }
}

