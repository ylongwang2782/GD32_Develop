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
 * Description: ��ʱ��1�жϷ�������ÿ�δ�����ʱ���ͽ���ú�����ִ�ж�ʱ����������
 * param[in]: 
 * param[in]: 
 * Retrun: 
 * Exception: none
*************************************************************************************/
void TIMER1_IRQHandler(void)    
{ 
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)) 
    {
        /* ֻ�е��豸�����ڴ����������ִ�ж��� */
        if(g_i16PeriodOrder >= 0 && g_i16Period_counter >= 0)
        {
            /* ֻ�е����ڼ�����С����������ʱ��ִ�� */
            if(stBroadcastFrame.synchData.totalLineVolum.L_TotalLineVolum >=  g_i16Period_counter)
            {
                /* �������ݲɼ����ɼ�ͨ�����ŵ����� */
                PinDataCollect();
                /* �����ڹ涨��ʱ϶���ϴ����ڵ������ */
                NodeDataUpload();
            }
        }
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
        /* ������б����� */
        if (Running_holder != 0)
        {
            /* ���ڱ��ֽ׶Σ���λ������ */
            if (g_i16Period_counter == 80)
            {
                g_i16Period_counter = -1;
                Running_holder--;
            }                
        }
        /* ���ӳ�ʱ����λ�����ظ���������ͨѶ���ϵ� */
        if (g_i16Period_counter >= 200)
        {
            NetReplyFlag = 1;
        }
        g_i16Period_counter++;/* ���ڼ�������һ */
    }
}

/*************************************************************************************
 * Function Name: USART5_IRQHandler
 * Description: ����5�жϺ�����ִ�д��ڽ�����������в���
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
        // �����յ�������װ�뵥�ֽڻ�����
        uint8_t received_byte = usart_data_receive(USART5);
        // ��ÿ�δ����жϽ��յ��ĵ��ֽڻ���������ݽ��ջ�����
        g_u8DataReceiveBuffer[g_u8ReceiveBytePosition] = received_byte;
        // ֡ͷУ��
        if (g_u8ReceiveByteHeaderPosition < 3) 
        {
            if (received_byte == g_u8FrameHeader[g_u8ReceiveByteHeaderPosition]) 
            {
                g_u8ReceiveByteHeaderPosition++;
                g_u8ReceiveBytePosition++;
            }
            else 
            {
                // ֡ͷУ��ʧ�ܲ���λ֡ͷУ��ָ��
                g_u8ReceiveByteHeaderPosition = 0;
                g_u8ReceiveBytePosition = 0;
            }
        }
        else 
        {
            /* ֡ͷУ��ɹ� */
            g_u8ReceiveBytePosition++;
            // ������ָ�볤�ȵ���֡����ʱ�����ж�Ϊ�������
            if (g_u8ReceiveBytePosition == g_u8DataReceiveBuffer[3]) 
            {
                g_u8ReceiveBytePosition = 0;
                g_u8ReceiveByteHeaderPosition = 0;
                    // �ж�֡����
                    switch (g_u8DataReceiveBuffer[6]) 
                    {
                        case 0x00: 
                            /* �㲥֡��ִ�й㲥֡���� */
                            timer_counter_value_config(TIMER1, 0);/* ��λ��ʱ�������� */
                            memcpy(&stBroadcastFrame, &g_u8DataReceiveBuffer, 43);/* �����ջ������㲥֡�ṹ���� */
                            memset(&g_u8DataReceiveBuffer, 0, sizeof(BroadcastFrame));/* ��λ���ݽ��ջ��� */
                            // ��ѯ�㲥֡��ͨ��IDƥ��ʱ϶��
                            for (i = 0; i < MAX_NODES; i++) 
                            {
                                memcpy(&stSlot, &stBroadcastFrame.synchData.synchSlot[i], sizeof(stSlot));
                                /* ƥ��ɹ� */
                                if (memcmp(stSlot.ID, stDevice.ID, sizeof(stDevice.ID)) == 0) 
                                {
                                    /* �ϴ�������������λ */
                                    Running_holder = UPLOADCOUNT;

                                    g_i16Period_counter = -1;/* ���ڼ�������λ */
                                    g_i16PeriodOrder = i;/* ����ʱ϶ */
                                    g_i16PinNumBefore = CalculatePinSum(g_i16PeriodOrder, 0);/* ������ͨ�ϼ������ */
                                    g_i16PinNumBeforeZ = CalculatePinSum(g_i16PeriodOrder, 1);/*�������迹�������  */
                                    stDataUploadFrame.LineVolum = stBroadcastFrame.synchData.synchSlot[i].L_LineVolum;/* ��ȡʱ϶ͨ�ϼ���������� */
                                    stDataUploadFrameZ.LineVolumZ = stBroadcastFrame.synchData.synchSlot[i].Z_LineVolum;/* ��ȡʱ϶�迹����������� */
                                    // BatDataUpload(u16VolBat);
                                    break;
                                }
                            }
                            break;
                        case 0x04:
                            /* ָ��֡��ִ��ָ��֡�������� */
                            memcpy(&stCommandFrame, &g_u8DataReceiveBuffer, g_u8DataReceiveBuffer[3]);
                            if (stCommandFrame.commandData.InstructionType == 0x0001)
                            {
                                /* ִ�п������� */
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
 * Description: ���е��ⲿ�жϷ���������Ҫ��ͨ�ϼ��ʹ��
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
 * Description: i2c �¼��жϺ���
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
 * Description: i2c �����жϺ���
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


