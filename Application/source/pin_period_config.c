/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: pin_period_config.c
 * Brief: 引脚周期主函数
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
#include <stdlib.h>
#include "string.h"
#include "systick.h"
#include "pin_period_config.h"
#include "global_config.h"
#include "peripheral_config.h"
#include "Protocol.h"
#include "adc_config.h"

/*************************************************************************************
                                    --- Local Constant & Type Defined ---
*************************************************************************************/
uint16_t UnsteadyAdcValueBuff[10][10];
uint32_t AdcChannelArray[6] = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5};
uint16_t DeviceVoltage = 33;
uint16_t Resolution = 4095;
uint16_t buffer[16];
/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/

/*************************************************************************************
 * Function Name: PinDataCollectL
 * Description: 通断数据采集函数，首先存储上一周期数据，然后采集本周期数据，保证单位时间内的数据完整
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void PinDataCollect(void)
{
    int i;
    int CurrentPinNum = g_i16Period_counter - g_i16PinNumBefore;
    int CurrentLineVolum = stBroadcastFrame.synchData.synchSlot[g_i16PeriodOrder].L_LineVolum;
    int LastPinNum = CurrentPinNum - 1;
    int LastPeriodCounter = g_i16Period_counter - 1 ;
    
    /* 数据存储分段函数自变量分段点 */
    int BufferPiecewisePoint0 = g_i16PinNumBefore;
    int BufferPiecewisePoint1 = g_i16PinNumBefore + CurrentLineVolum;
    int BufferPiecewisePoint2 = stBroadcastFrame.synchData.totalLineVolum.L_TotalLineVolum;
    /* 数据存储分段函数因变量分段点 */
    int BufferIndexBefore = CurrentLineVolum * LastPeriodCounter;
    int BufferIndexDuring = CurrentLineVolum * g_i16PinNumBefore + (CurrentLineVolum - 1) * (LastPeriodCounter - g_i16PinNumBefore);
    int BufferIndexAfter = CurrentLineVolum * g_i16PinNumBefore + 
                                    CurrentLineVolum * (CurrentLineVolum - 1) + 
                                    CurrentLineVolum * (LastPeriodCounter - (g_i16PinNumBefore + CurrentLineVolum )) ;

    // 将上一个周期的引脚恢复为外部中断输入模式
    if (CurrentLineVolum >= (g_i16Period_counter - g_i16PinNumBefore) && (g_i16Period_counter - g_i16PinNumBefore) > 0)
    {
        if(LastPinNum>= 0)
        { 
            ExtiReconfig(LastPinNum);
        }
    }
    
    // 如果处于当前时隙，删除上一周期对角线元素
    if (CurrentLineVolum > LastPinNum && LastPinNum >= 0)
    {
        RemoveElementAtIndex(g_u8SlotBinDataBuffer, LastPinNum);
    }
    
    // store the current slot data buffer into the node data buffer
    /* 将上一周期采集到的时隙二进制数据存入节点二进制数据缓存，采用分段函数存储 */
    if(LastPeriodCounter >= 0)
    {
        // 时隙前
         if (BufferPiecewisePoint0 > LastPeriodCounter && LastPeriodCounter >= 0)
        {
            memcpy(&g_u8NodeBinDataBuffer[BufferIndexBefore], &g_u8SlotBinDataBuffer, CurrentLineVolum);
        }
        // 时隙时
        if (BufferPiecewisePoint1 > LastPeriodCounter && LastPeriodCounter >= BufferPiecewisePoint0)
        { 
            memcpy(&g_u8NodeBinDataBuffer[BufferIndexDuring], &g_u8SlotBinDataBuffer, (CurrentLineVolum - 1));
        }
        // 时隙后
        if (BufferPiecewisePoint2 > LastPeriodCounter && LastPeriodCounter >= BufferPiecewisePoint1)
        {
            memcpy(&g_u8NodeBinDataBuffer[BufferIndexAfter], &g_u8SlotBinDataBuffer, CurrentLineVolum);
        }        
    }
    
    // clear the g_u8SlotBinDataBuffer for the next data collection.
    // 重新初始化时隙二进制数据缓存
    ResetArray(g_u8SlotBinDataBuffer,16);

    // 等待数据存储完毕再进行引脚中断设置（针对节点时隙交叉问题）
    for(i = 0; i <500; i++);
    
    // Check if the L_LineVolum is within a specific range and adjust GPIO pins accordingly.
    // 如果处于当前时隙，则触发引脚配置
    if (CurrentLineVolum > (g_i16Period_counter - g_i16PinNumBefore) && (g_i16Period_counter - g_i16PinNumBefore) >= 0)
    {
        PinSet(GPIOB, BPINS[g_i16Period_counter - g_i16PinNumBefore]);
    }
}

/*************************************************************************************
 * Function Name: PinDataCollectZ
 * Description: 阻抗数据采集函数，采用轮询检测，检测本周期数据，然后将本周期数据存储
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void PinDataCollectZ(void)
{
    /* 当处于激活状态时才触发 */
    if (stBroadcastFrame.synchData.synchSlot[g_i16PeriodOrder].Z_LineVolum > g_i16PeriodOrder >= 0)
    {
        int i, j;
        uint8_t FilterNum = 3;
        uint8_t TimeStamp = g_i16Period_counter;
        int CurrentPinNumZ = TimeStamp - g_i16PinNumBeforeZ;
        int SlotLineVolumZ = stBroadcastFrame.synchData.synchSlot[g_i16PeriodOrder].Z_LineVolum;
        
        /* 数据存储分段函数自变量分段点 */
        int BufferPiecewisePoint0 = g_i16PinNumBeforeZ;
        int BufferPiecewisePoint1 = g_i16PinNumBeforeZ + SlotLineVolumZ;
        int BufferPiecewisePoint2 = stBroadcastFrame.synchData.totalLineVolum.Z_TotalLineVolum;
        /* 数据存储分段函数因变量分段点 */
        int BufferIndexBefore = SlotLineVolumZ * TimeStamp;
        int BufferIndexDuring = SlotLineVolumZ * g_i16PinNumBeforeZ + (SlotLineVolumZ - 1) * CurrentPinNumZ;
        int BufferIndexAfter = SlotLineVolumZ * g_i16PinNumBeforeZ + 
                                        SlotLineVolumZ * (SlotLineVolumZ - 1) + 
                                        (CurrentPinNumZ - SlotLineVolumZ ) * SlotLineVolumZ;
        // 重新初始化时隙二进制数据缓存
        ResetArray(g_u8SlotResDataBuffer,16);
        
        // 检测是否处于当前时隙
        if (SlotLineVolumZ > CurrentPinNumZ && CurrentPinNumZ >= 0)
        {
            // 如果处于当前时隙，则拉低引脚
            PinReset(GPIOA, APINS[5 - CurrentPinNumZ]);
        }
        
        /* 在引脚周期内对每个引脚的ADC值采集1次（ij是为滤波需求预留） */
        for (j = 0; j < FilterNum; j++)
        {
            for ( i = 0; i < SlotLineVolumZ; i++)
            {
                UnsteadyAdcValueBuff[i][j] = AdcChannelSample(AdcChannelArray[5 - i]);
            }
        }

        /* 滤波并存储本引脚周期数据，滤波次数由FilterNum确定 */
        for ( i = 0; i < SlotLineVolumZ; i++)
        {
            g_u16SlotAdcDataBuffer[i] = movingAverageFilter(UnsteadyAdcValueBuff[i],FilterNum); // 对不稳定的ADC数据进行滤波处理
            g_u8SlotResDataBuffer[i] = AdcToResistance(g_u16SlotAdcDataBuffer[i]);
        }
        
        if (SlotLineVolumZ > CurrentPinNumZ && CurrentPinNumZ >= 0)
        {
            RemoveElementAtIndex(g_u8SlotResDataBuffer, CurrentPinNumZ);
        }

        /* 数据采集 */
        // 时隙前
        if (BufferPiecewisePoint0 > TimeStamp)
        {
            memcpy(&stDataUploadFrameZ.Data[BufferIndexBefore], &g_u8SlotResDataBuffer, SlotLineVolumZ);
        }
        // 时隙时
        if (BufferPiecewisePoint1 > TimeStamp && TimeStamp >= BufferPiecewisePoint0)
        { 
            memcpy(&stDataUploadFrameZ.Data[BufferIndexDuring], &g_u8SlotResDataBuffer, SlotLineVolumZ - 1);
        }
        // 时隙后
        if (BufferPiecewisePoint2 > TimeStamp && TimeStamp >= BufferPiecewisePoint1)
        {
            memcpy(&stDataUploadFrameZ.Data[BufferIndexAfter], &g_u8SlotResDataBuffer, SlotLineVolumZ);
        }        

        // 将本周期的引脚恢复为模拟输入模式
        if (SlotLineVolumZ > CurrentPinNumZ && CurrentPinNumZ >= 0)
        {
            AdcReconfig(5 - CurrentPinNumZ);
        }
    }
}

/*************************************************************************************
 * Function Name: RemoveElementAtIndex
 * Description: 
 * 消除数组中指定元素，并将制定元素后面元素向前移动，本函数不改变数组大小
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void RemoveElementAtIndex(uint8_t arr[], uint8_t index) {
    int i;
    if (index >= 16) 
    {
        return;
    }
    for (i = index; i < 15; i++) 
    {
        arr[i] = arr[i + 1];
    }
}

/*************************************************************************************
 * Function Name: ResetArray
 * Description: 将指定的数组中的元素全部归零
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void ResetArray(uint8_t arr[], int size) 
{
    int i;
    for (i = 0; i < size; ++i) 
    {
        arr[i] = 0;
    }
}

/*************************************************************************************
 * Function Name: PinSet
 * Description: 配置引脚为主模式，高电平输出模式，用于通断检测
 * Param[in]: GPIOx(x = A,B,C,D,E,F,G,H,I)
 * Param[in]: x(x=0..15)
 * Retrun: none
 * Exception: 
*************************************************************************************/
void PinSet(uint32_t gpio_periph, uint32_t pin)
{  
    gpio_mode_set(gpio_periph, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);  // Configure the GPIO pin as an output, no pull-up or pull-down.  
    gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);    // Set the GPIO output options to push-pull and medium speed (50MHz).
    gpio_bit_set(gpio_periph, pin); // Set the GPIO pin to a high level (master state).
}

/*************************************************************************************
 * Function Name: PinReset
 * Description: 配置引脚为低电平输出模式，作为ADC拉低引脚
 * Param[in]: GPIOx(x = A,B,C,D,E,F,G,H,I)
 * Param[in]: x(x=0..15)
 * Retrun: none
 * Exception: 
*************************************************************************************/
void PinReset(uint32_t gpio_periph, uint32_t pin)
{  
    gpio_mode_set(gpio_periph, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);  // Configure the GPIO pin as an output, no pull-up or pull-down.  
    gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);    // Set the GPIO output options to push-pull and medium speed (50MHz).
    gpio_bit_reset(gpio_periph, pin); // Set the GPIO pin to low level (master state).
}

/*************************************************************************************
 * Function Name: CalculatePinSum
 * Description: 计算时隙前通断引脚总数
 * Param[in]: SlotNum, which is gonna to calculate
 * Retrun: the sum of the pins before the input slotnum
 * Exception: 
*************************************************************************************/
uint32_t CalculatePinSum(uint8_t u8SlotNum, uint8_t type)
{
    uint32_t u32SlotSum = 0;
    uint8_t j;

    if (type == 0)
    {
        for (j = 0; j < u8SlotNum; j++) 
        {
            u32SlotSum += stBroadcastFrame.synchData.synchSlot[j].L_LineVolum;
        }
    }
    else if (type == 1)
    {
        for (j = 0; j < u8SlotNum; j++)
        {
            u32SlotSum += stBroadcastFrame.synchData.synchSlot[j].Z_LineVolum;
        }
    }
    // Return the calculated sum of PIN values.
    return u32SlotSum;
}
