/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: peripheral_config.c
 * Brief: 配置一些外设
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
#include <stdio.h>
#include "systick.h"
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

#define ExtiPortBnum 12
#define ExtiPortCnum 16
#define ExtiPortDnum 16

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
//void I2cTask(void)
//{
//    /* i2c外设处理函数 */
//    if (I2cKeyFlag)
//    {
//        // 读取按键状态
//        eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);
//        // 获取按键状态
//        key1Closed = BitGet(i2c_buffer_read[1], 0);
//        key2Closed = BitGet(i2c_buffer_read[1], 1);

//        // 根据按键状态执行相应的动作
//        if (key1Closed && !key2Closed && !UnlockCommand) {
//            // 如果检测到 KEY1 闭合，则闭合电磁阀1，断开电磁阀2
//            eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_config_reg_cmd_H, 1);
//            eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_output_reg_cmd_H, 1);
//        } else if (!key1Closed && key2Closed && !UnlockCommand) {
//            // 如果检测到 KEY2 闭合，则闭合电磁阀2，断开电磁阀1
//            eeprom_buffer_write_interrupt(I2cFaControlSet2, XL9555_config_reg_cmd_H, 1);
//            eeprom_buffer_write_interrupt(I2cFaControlSet2, XL9555_output_reg_cmd_H, 1);
//        } else if (key1Closed && key2Closed && !UnlockCommand) {
//            // 如果检测到 KEY1 和 KEY2 都闭合，则闭合电磁阀1 和 电磁阀2
//            eeprom_buffer_write_interrupt(I2cFaControlSetBoth, XL9555_config_reg_cmd_H, 1);
//            eeprom_buffer_write_interrupt(I2cFaControlSetBoth, XL9555_output_reg_cmd_H, 1);
//        } else if (!key1Closed && !key2Closed || UnlockCommand) {
//            // 如果检测到 KEY1 和 KEY2 都断开，则断开电磁阀1 和 电磁阀2
//            eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);
//            eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);
//        }
//        // 重置按键标志位
//        I2cKeyFlag = 0;
//    }
//}

/*************************************************************************************
 * Function Name: GpioInputModeConfig
 * Description: 将所有引脚配置为Input模式
 * the configuration process.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
//void GpioInputModeConfig(void)
//{
//    int i;
//    /*配置外部中断引脚为输入模式*/
//    for (i = 0; i < ExtiPortBnum; i++)
//    {
//        gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BPINS[i]);

//    }
//    for (i = 0; i < ExtiPortCnum; i++)
//    {
//        gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, CPINS[i]);
//        gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DPINS[i]);
//    }
//    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
//    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
//    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
//    gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
//    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
//}

/*************************************************************************************
 * Function Name: ExtiReconfig
 * Description: 配置指定引脚恢复EXTI外部中断模式
 * num[in]: 指定引脚的序号
 * Retrun: none
 * Exception: none
*************************************************************************************/
void ExtiReconfig(uint8_t num)
{
    gpio_bit_reset(GPIOB, BPINS[num]);
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BPINS[num]); 
}
