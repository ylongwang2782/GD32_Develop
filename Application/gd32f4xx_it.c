/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: gd32f4xx_it.c
 * Brief: Config some system fuction and periphral include the timer, RCU, NVIC
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
#include "gd32f4xx_it.h"
#include "systick.h"
#include "string.h"
#include "receive_send_config.h" 
#include "global_config.h"
#include "peripheral_config.h"
#include "pin_period_config.h"
#include "node_period_config.h"
#include "i2c.h"
#include "xl9555.h"
#include "adc_config.h"

/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
/*************************************************************************************
 * Function Name: TIMER1_IRQHandler
 * Description: 定时器1中断服务函数，每次触发定时器就进入该函数，执行定时器周期任务
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void TIMER1_IRQHandler(void)    
{ 
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)) 
    {
        /* 只有当设备的周期次序大于零后才执行动作 */
        if(g_i16PeriodOrder >= 0 && g_i16Period_counter >= 0)
        {
            /* 只有当周期计数器小于总引脚数时才执行 */
            if(stBroadcastFrame.synchData.totalLineVolum.L_TotalLineVolum >=  g_i16Period_counter)
            {
                /* 引脚数据采集，采集通断引脚的数据 */
                PinDataCollect();
                /* 在周期规定的时隙内上传本节点的数据 */
                NodeDataUpload();
            }
        }
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
        /* 检查运行保持器 */
        if (Running_holder != 0)
        {
            /* 处在保持阶段，则复位计数器 */
            if (g_i16Period_counter == 80)
            {
                g_i16Period_counter = -1;
                Running_holder--;
            }                
        }
        /* 连接超时，复位入网回复，并点亮通讯故障灯 */
        if (g_i16Period_counter >= 200)
        {
            NetReplyFlag = 1;
        }
        g_i16Period_counter++;/* 周期计数器加一 */
    }
}

/*************************************************************************************
 * Function Name: USART5_IRQHandler
 * Description: 串口5中断函数，执行串口解析等相关所有操作
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void USART5_IRQHandler(void) 
{
    int i;
    if (usart_flag_get(USART5, USART_FLAG_RBNE)) 
    {
        // 将接收到的数据装入单字节缓存中
        uint8_t received_byte = usart_data_receive(USART5);
        // 将每次串口中断接收到的单字节缓存存入数据接收缓存中
        g_u8DataReceiveBuffer[g_u8ReceiveBytePosition] = received_byte;
        // 帧头校验
        if (g_u8ReceiveByteHeaderPosition < 3) 
        {
            if (received_byte == g_u8FrameHeader[g_u8ReceiveByteHeaderPosition]) 
            {
                g_u8ReceiveByteHeaderPosition++;
                g_u8ReceiveBytePosition++;
            }
            else 
            {
                // 帧头校验失败并复位帧头校验指针
                g_u8ReceiveByteHeaderPosition = 0;
                g_u8ReceiveBytePosition = 0;
            }
        }
        else 
        {
            /* 帧头校验成功 */
            g_u8ReceiveBytePosition++;
            // 当接收指针长度等于帧长度时，即判断为接收完成
            if (g_u8ReceiveBytePosition == g_u8DataReceiveBuffer[3]) 
            {
                g_u8ReceiveBytePosition = 0;
                g_u8ReceiveByteHeaderPosition = 0;
                    // 判断帧类型
                    switch (g_u8DataReceiveBuffer[6]) 
                    {
                        case 0x00: 
                            /* 广播帧，执行广播帧程序 */
                            timer_counter_value_config(TIMER1, 0);/* 复位定时器计数器 */
                            memcpy(&stBroadcastFrame, &g_u8DataReceiveBuffer, 43);/* 将接收缓存存入广播帧结构体中 */
                            memset(&g_u8DataReceiveBuffer, 0, sizeof(BroadcastFrame));/* 复位数据接收缓存 */
                            // 查询广播帧并通过ID匹配时隙号
                            for (i = 0; i < MAX_NODES; i++) 
                            {
                                memcpy(&stSlot, &stBroadcastFrame.synchData.synchSlot[i], sizeof(stSlot));
                                /* 匹配成功 */
                                if (memcmp(stSlot.ID, stDevice.ID, sizeof(stDevice.ID)) == 0) 
                                {
                                    /* 上传次数保持器复位 */
                                    Running_holder = UPLOADCOUNT;

                                    g_i16Period_counter = -1;/* 周期计数器复位 */
                                    g_i16PeriodOrder = i;/* 配置时隙 */
                                    g_i16PinNumBefore = CalculatePinSum(g_i16PeriodOrder, 0);/* 计算总通断检测引脚 */
                                    g_i16PinNumBeforeZ = CalculatePinSum(g_i16PeriodOrder, 1);/*计算总阻抗检测引脚  */
                                    stDataUploadFrame.LineVolum = stBroadcastFrame.synchData.synchSlot[i].L_LineVolum;/* 读取时隙通断检测引脚数量 */
                                    stDataUploadFrameZ.LineVolumZ = stBroadcastFrame.synchData.synchSlot[i].Z_LineVolum;/* 读取时隙阻抗检测引脚数量 */
                                    // BatDataUpload(u16VolBat);
                                    break;
                                }
                            }
                            break;
                        case 0x04:
                            /* 指令帧，执行指令帧解析程序 */
                            memcpy(&stCommandFrame, &g_u8DataReceiveBuffer, g_u8DataReceiveBuffer[3]);
                            if (stCommandFrame.commandData.InstructionType == 0x0001)
                            {
                                /* 执行开锁动作 */
                                UnlockCommand = 1;
                            }    
                            break;
                        default:
                            break;
                    }
                
            }
        }
    }
}

/*************************************************************************************
 * Function Name: EXTI_IRQHandler
 * Description: 所有的外部中断服务函数，主要是通断检测使用
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void EXTI0_IRQHandler(void) 
{ 
    if (RESET != exti_interrupt_flag_get(EXTI_0)) 
    { 
        g_u8SlotBinDataBuffer[11] = 1;
        exti_interrupt_flag_clear(EXTI_0); 
    } 
}
void EXTI1_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_1)) 
    { 
        g_u8SlotBinDataBuffer[7] = 1;
        exti_interrupt_flag_clear(EXTI_1); 
    }
}
void EXTI2_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_2)) 
    { 
        g_u8SlotBinDataBuffer[3] = 1;
        exti_interrupt_flag_clear(EXTI_2); 
    } 
}
void EXTI3_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_3)) 
    { 
        g_u8SlotBinDataBuffer[10] = 1;
        exti_interrupt_flag_clear(EXTI_3); 
    }
}
void EXTI4_IRQHandler(void) {
   if (RESET != exti_interrupt_flag_get(EXTI_4)) 
   { 
        g_u8SlotBinDataBuffer[6] = 1;
        exti_interrupt_flag_clear(EXTI_4); 
   } 
}
/* EXTI5_9_IRQHandler */
void EXTI5_9_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_5)) 
    { 
        g_u8SlotBinDataBuffer[2] = 1;
        exti_interrupt_flag_clear(EXTI_5); 
    }
	if (RESET != exti_interrupt_flag_get(EXTI_6)) 
    { 
        g_u8SlotBinDataBuffer[9] = 1;
		exti_interrupt_flag_clear(EXTI_6); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_7)) 
    { 
        g_u8SlotBinDataBuffer[5] = 1;  
		exti_interrupt_flag_clear(EXTI_7); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_8)) 
    { 
        g_u8SlotBinDataBuffer[1] = 1;  
		exti_interrupt_flag_clear(EXTI_8); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_9)) 
    { 
        g_u8SlotBinDataBuffer[8] = 1; 
		exti_interrupt_flag_clear(EXTI_9); 
	}
}
/* EXTI10_15_IRQHandler */
void EXTI10_15_IRQHandler(void) 
{
	if (RESET != exti_interrupt_flag_get(EXTI_10)) 
    { 
        g_u8SlotBinDataBuffer[4] = 1; 
		exti_interrupt_flag_clear(EXTI_10); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_11)) 
    { 
        g_u8SlotBinDataBuffer[0] = 1; 
		exti_interrupt_flag_clear(EXTI_11); 
	}
    
	if (RESET != exti_interrupt_flag_get(EXTI_12)) 
    { 
        I2cKeyFlag = 1;
		exti_interrupt_flag_clear(EXTI_12); 
	}
}

/*************************************************************************************
 * Function Name: I2C2_EV_IRQHandler
 * Description: i2c 事件中断函数
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void I2C2_EV_IRQHandler(void)
{
    i2cx_event_irq_handler();
}

/*************************************************************************************
 * Function Name: I2C2_ER_IRQHandler
 * Description: i2c 错误中断函数
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void I2C2_ER_IRQHandler(void)
{
    i2cx_error_irq_handler();
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}


