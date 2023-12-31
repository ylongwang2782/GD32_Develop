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
#include "peripheral_config.h"
#include "global_config.h"
#include "Protocol.h"
#include "xl9555.h"
#include "i2c.h"
/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
/*************************************************************************************
 * Function Name: I2cTask
 * Description: I2C任务，包括读取按键状态，根据按键状态控制电磁阀
 * the configuration process.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void I2cTask(void)
{
    /* i2c外设处理函数 */
    if (I2cKeyFlag)
    {
        // 写入控制字
        eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);
        // 获取按键状态
        key1Closed = BitGet(i2c_buffer_read[1], 0);
        key2Closed = BitGet(i2c_buffer_read[1], 1);

        // 根据按键状态执行相应的动作
        if (key1Closed && !key2Closed && !UnlockCommand) {
            // 如果检测到 KEY1 闭合，则闭合电磁阀1，断开电磁阀2
            eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_config_reg_cmd_H, 1);
            eeprom_buffer_write_interrupt(I2cFaControlSet1, XL9555_output_reg_cmd_H, 1);
        } else if (!key1Closed && key2Closed && !UnlockCommand) {
            // 如果检测到 KEY2 闭合，则闭合电磁阀2，断开电磁阀1
            eeprom_buffer_write_interrupt(I2cFaControlSet2, XL9555_config_reg_cmd_H, 1);
            eeprom_buffer_write_interrupt(I2cFaControlSet2, XL9555_output_reg_cmd_H, 1);
        } else if (key1Closed && key2Closed && !UnlockCommand) {
            // 如果检测到 KEY1 和 KEY2 都闭合，则闭合电磁阀1 和 电磁阀2
            eeprom_buffer_write_interrupt(I2cFaControlSetBoth, XL9555_config_reg_cmd_H, 1);
            eeprom_buffer_write_interrupt(I2cFaControlSetBoth, XL9555_output_reg_cmd_H, 1);
        } else if (!key1Closed && !key2Closed || UnlockCommand) {
            // 如果检测到 KEY1 和 KEY2 都断开，则断开电磁阀1 和 电磁阀2
            eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_config_reg_cmd_H, 1);
            eeprom_buffer_write_interrupt(I2cFaControlResetBoth, XL9555_output_reg_cmd_H, 1);
        }
        // 重置按键标志位
        I2cKeyFlag = 0;
    }
}

/*************************************************************************************
 * Function Name: GpioInputModeConfig
 * Description: 将所有通断检测引脚配置为Input模式
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void GpioInputModeConfig(void)
{
    int i;
    /*配置外部中断引脚为输入模式*/
    for (i = 0; i < ExtiPortBnum; i++)
    {
        gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BPINS[i]);
    }
    for (i = 0; i < ExtiPortCnum; i++)
    {
        gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, CPINS[i]);
        gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DPINS[i]);
    }
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
}

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

/*************************************************************************************
 * Function Name: ExtiConfig
 * Description: 
 * This function initializes EXTI lines by configuring the GPIO pins, enabling NVIC interrupts, setting
 * EXTI sources, and configuring the trigger conditions for each line. It uses predefined macros to simplify
 * the configuration process.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void ExtiConfig(void)
{
	/* CONFIG_EXTIE0_4_LINE */
    CONFIG_EXTI_B_LINE(0);
	CONFIG_EXTI_B_LINE(1);
	CONFIG_EXTI_B_LINE(2);
	CONFIG_EXTI_B_LINE(3);
	CONFIG_EXTI_B_LINE(4);
	
	/* CONFIG_EXTIE5_9_LINE */
	CONFIG_EXTI_B_5_9_LINE(5);
	CONFIG_EXTI_B_5_9_LINE(6);
	CONFIG_EXTI_B_5_9_LINE(7);
	CONFIG_EXTI_B_5_9_LINE(8);
	CONFIG_EXTI_B_5_9_LINE(9);

	/* CONFIG_EXTIE10_15_LINE */
	CONFIG_EXTI_B_10_15_LINE(10);
	CONFIG_EXTI_B_10_15_LINE(11);
    CONFIG_EXTI_B_10_15_LINE(12);
}
