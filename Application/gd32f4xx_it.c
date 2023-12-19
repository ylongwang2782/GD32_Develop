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
extern uint8_t I2cReceiver[]; //存放引脚电平，[0]低8位，[1]高8位

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
                /* 引脚数据采集，采集已配置引脚的外部终端信息并存储 */
                PinDataCollect();
                /* 节点数据上传，将整个周期节点采集到的数据上传 */
                NodeDataUpload();
            }
        }
        // 周期计数器加一
        g_i16Period_counter++;
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
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
        // Store the received 1 byte data in the received_byte variable. Frequency: every one loop
        // 将接收到的数据装入单字节缓存中
        uint8_t received_byte = usart_data_receive(USART5);
        
        // Transfer the received_byte data to the receive buffer. Frequency: every one loop. Result is that buffer stores every byte during the whole byte stream.
        // 将每次串口中断接收到的单字节缓存存入数据接收缓存中
        g_u8DataReceiveBuffer[g_u8ReceiveBytePosition] = received_byte;
        
        // Check if the first three byte match the header of the data frame defined in the protocol 
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
                g_u8ReceiveByteHeaderPosition = 0; // Reset g_u8ReceiveByteHeaderPosition if a byte doesn't match
                g_u8ReceiveBytePosition = 0;
            }
        }
        else 
        {
            /* HEADER MATCHES */
            /* 帧头校验成功 */
            g_u8ReceiveBytePosition++;
            
            /* If all data is received, reset the positions, then enter the type Differentiate */
            // 当接收指针长度等于帧长度时，即判断为接收完成
            if (g_u8ReceiveBytePosition == g_u8DataReceiveBuffer[3]) 
            {
                g_u8ReceiveBytePosition = 0;
                g_u8ReceiveByteHeaderPosition = 0;
                    // Differentiate frame types by the frametype
                    // 判断帧类型
                    switch (g_u8DataReceiveBuffer[6]) 
                    {
                        case 0x00: 
                            // BroadcastFrame
                            // Reset the timer counter every time receives the broadcast in order to make time synch
                            timer_counter_value_config(TIMER1, 0);
                        
                            // Transfer the data to the stBroadcastFrame then reset the g_u8DataReceiveBuffer
                            // 将接收缓存存入广播帧结构体中
                            memcpy(&stBroadcastFrame, &g_u8DataReceiveBuffer, 43);
                            memset(&g_u8DataReceiveBuffer, 0, sizeof(BroadcastFrame));
                        
                            // Iterate through the synchSlot array to find the g_i16PeriodOrder
                            // 查询广播帧并通过ID匹配配置时隙号
                            for (i = 0; i < MAX_NODES; i++) 
                            {
                                memcpy(&stSlot, &stBroadcastFrame.synchData.synchSlot[i], sizeof(stSlot));
                                if (memcmp(stSlot.ID, stDevice.ID, sizeof(stDevice.ID)) == 0) 
                                {
                                    g_i16Period_counter = -1;
                                    g_i16PeriodOrder = i;
                                    
                                    /* 计算总阻抗脚，总通断脚 */
                                    g_i16PinNumBefore = CalculatePinSum(g_i16PeriodOrder, 0);
                                    g_i16PinNumBeforeZ = CalculatePinSum(g_i16PeriodOrder, 1);
                                    /* 引脚数赋值 */
                                    stDataUploadFrame.LineVolum = stBroadcastFrame.synchData.synchSlot[i].L_LineVolum;
                                    stDataUploadFrameZ.LineVolumZ = stBroadcastFrame.synchData.synchSlot[i].Z_LineVolum;

                                    /* 入网回复标志位 */
                                    NetReplyFlag = 1;
                                    // BatDataUpload(u16VolBat);
                                    break;
                                }
                            }// Where there g_i16PeriodOrder store the order of the stDevice in the whole period
                            break;
                        case 0x04:
                            // 指令帧
                            memcpy(&stCommandFrame, &g_u8DataReceiveBuffer, g_u8DataReceiveBuffer[3]);
                            if (stCommandFrame.commandData.InstructionType == 0x0001)
                            {
                                /* 执行开锁动作 */
                                UnlockCommand = 1;
                            }    
                            break;
                        default:
                            // Handle unknown frame types
                            break;
                    }
                
            }
        }
    }
}

/*************************************************************************************
 * Function Name: EXTI0_IRQHandler
 * Description: 外部中断服务函数
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
uint32_t volatile IntCount[16];

void EXTI0_IRQHandler(void) 
{ 
    if (RESET != exti_interrupt_flag_get(EXTI_0)) 
    { 
        g_u8SlotBinDataBuffer[11] = 1;
        if(IntCount[11]<0xffffffff)IntCount[11]++;
        exti_interrupt_flag_clear(EXTI_0); 
    } 
}
void EXTI1_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_1)) 
    { 
        g_u8SlotBinDataBuffer[7] = 1;
        if(IntCount[7]<0xffffffff)IntCount[7]++;
        exti_interrupt_flag_clear(EXTI_1); 
    }
}
void EXTI2_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_2)) 
    { 
        g_u8SlotBinDataBuffer[3] = 1;
        if(IntCount[3]<0xffffffff)IntCount[3]++;
        exti_interrupt_flag_clear(EXTI_2); 
    } 
}
void EXTI3_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_3)) 
    { 
        g_u8SlotBinDataBuffer[10] = 1;
        if(IntCount[10]<0xffffffff)IntCount[10]++;
        exti_interrupt_flag_clear(EXTI_3); 
    }
}
void EXTI4_IRQHandler(void) {
   if (RESET != exti_interrupt_flag_get(EXTI_4)) 
   { 
        g_u8SlotBinDataBuffer[6] = 1;
        if(IntCount[6]<0xffffffff)IntCount[6]++;
        exti_interrupt_flag_clear(EXTI_4); 
   } 
}
/* EXTI5_9_IRQHandler */
void EXTI5_9_IRQHandler(void) 
{
    if (RESET != exti_interrupt_flag_get(EXTI_5)) 
    { 
        g_u8SlotBinDataBuffer[2] = 1;
        if(IntCount[2]<0xffffffff)IntCount[2]++;			
        exti_interrupt_flag_clear(EXTI_5); 
    }
	if (RESET != exti_interrupt_flag_get(EXTI_6)) 
    { 
        g_u8SlotBinDataBuffer[9] = 1;
        if(IntCount[9]<0xffffffff)IntCount[9]++;			
		exti_interrupt_flag_clear(EXTI_6); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_7)) 
    { 
        g_u8SlotBinDataBuffer[5] = 1;  
        if(IntCount[5]<0xffffffff)IntCount[5]++;
		exti_interrupt_flag_clear(EXTI_7); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_8)) 
    { 
        g_u8SlotBinDataBuffer[1] = 1;  
        if(IntCount[1]<0xffffffff)IntCount[1]++;
		exti_interrupt_flag_clear(EXTI_8); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_9)) 
    { 
        g_u8SlotBinDataBuffer[8] = 1; 
        if(IntCount[8]<0xffffffff)IntCount[8]++;			
		exti_interrupt_flag_clear(EXTI_9); 
	}
}
/* EXTI10_15_IRQHandler */
void EXTI10_15_IRQHandler(void) 
{
	if (RESET != exti_interrupt_flag_get(EXTI_10)) 
    { 
        g_u8SlotBinDataBuffer[4] = 1; 
        if(IntCount[4]<0xffffffff)IntCount[4]++;			
		exti_interrupt_flag_clear(EXTI_10); 
	}
	if (RESET != exti_interrupt_flag_get(EXTI_11)) 
    { 
        g_u8SlotBinDataBuffer[0] = 1; 
        if(IntCount[0]<0xffffffff)IntCount[0]++;
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


