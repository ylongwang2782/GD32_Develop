/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: adc_config.c
 * Brief: adc相关配置函数
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
#include "adc_config.h"

uint16_t AdcBat;
uint8_t VolBat;
uint16_t u16VolBat;

void BatVolMonitor(void)
{
    /* 计数器为-1时读取一次电池电压 */
    if (g_i16Period_counter == -1)
    {
        AdcBat = AdcChannelSample(ADC_CHANNEL_14) * 2;
        u16VolBat = AdcBat * 3300 / 4095;

        /* 低电量报警判断 */
        if (3700 > u16VolBat)
        {
            stDataUploadFrame.deviceStatus.statusBattaryL = stDataUploadFrameZ.deviceStatus.statusBattaryL = 1;
            gpio_bit_set(GPIOC, GPIO_PIN_3);
        }
        else if (u16VolBat >= 3700)
        {
            stDataUploadFrame.deviceStatus.statusBattaryL = stDataUploadFrameZ.deviceStatus.statusBattaryL = 0;
            gpio_bit_reset(GPIOC, GPIO_PIN_3);
        } 
    }
}

/*************************************************************************************
 * Function Name: AdcToResistance
 * Description: ADC值转电阻值，因变量类型限值，故电阻值转换使用分段函数转换
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
uint8_t AdcToResistance(uint16_t AdcValue)
{
    //0R = B80
    //120R = BB0
    uint8_t R;
    if (100 > AdcValue)
    {
        R = 0x00;
    }
    else if (1300> AdcValue && AdcValue >= 100)
    {
        R = 0x78;
    }
    else if (AdcValue >= 1300)
    {
        R = 0xFF;
    }
    return R;
}

/*************************************************************************************
 * Function Name: movingAverageFilter
 * Description: 均值滤波函数
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
uint16_t movingAverageFilter(uint16_t *buffer, int length) 
{
    int i;
    uint16_t sum = 0;
    for (i = 0; i < length; ++i) {
        sum += buffer[i];
    }
    return sum / length;
}

/*************************************************************************************
 * Function Name: AdcGpioConifg
 * Description: 配置ADC引脚模式
 * the configuration process.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void AdcGpioConifg(void)
{
    /* 阻抗检测引脚 */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5);
    /* 电池检测引脚 */
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);
}

/*************************************************************************************
 * Function Name: GpioInputModeConfig
 * Description: 将所有引脚配置为Input模式
 * the configuration process.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void AdcReconfig(uint8_t num)
{
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, APINS[num]);
}

/*************************************************************************************
 * Function Name: AdcConfig
 * Description: 
 * This function configures the ADC by setting various parameters such as mode,
 * data alignment, trigger source, and enables the ADC interface.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void AdcConfig(void)
{
    /* reset ADC */
    adc_deinit();
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 2U);

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_T0_CH0); 
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* ADC Vbat channel enable */
    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH,ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}

/*************************************************************************************
 * Function Name: AdcChannelSample
 * Description: 
 * This function configures and triggers the ADC to sample a specified channel
 * and returns the sampled value.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
uint16_t AdcChannelSample(uint8_t channel)
{
    /* ADC routine channel config */
    adc_routine_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_routine_data_read(ADC0));
}


