#include "gd32f4xx.h"
#include "gd32f450z_eval.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

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

#define INIT_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 2 )
#define DEBUG_TASK_PRIO    ( tskIDLE_PRIORITY + 3 )

uint8_t *Test_Ptr = NULL;
static TaskHandle_t TIMER_Task_Handle = NULL;/* LED任务句柄 */
 
void LED1_task(void * pvParameters); 
void Usart2_task(void * pvParameters);
void Debug_task(void * pvParameters);
void Timer_task(void * pvParameters);

void init_task(void * pvParameters);

/* Binary semaphore handle definition. */
SemaphoreHandle_t binary_semaphore;
SemaphoreHandle_t timer_semaphore;

int main(void)
{
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    /* init task */
    xTaskCreate(init_task, "INIT", configMINIMAL_STACK_SIZE, NULL, INIT_TASK_PRIO, NULL);
    
    /* start scheduler */
    vTaskStartScheduler();

    while (1)
    {
    }
}

void init_task(void * pvParameters)
{
    rcu_periph_clock_enable(RCU_SYSCFG);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

    ExtiConfig();
    UsartInitUsart5();
    TimerInitial();
    
    /* create a binary semaphore. */
    binary_semaphore = xSemaphoreCreateBinary();
    timer_semaphore = xSemaphoreCreateBinary();

    PrintUniqueID();
    /* start toogle LED task every 250ms */
    xTaskCreate(Usart2_task, "Usart2", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
    xTaskCreate(Timer_task, "Timer", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, &TIMER_Task_Handle);
    for( ;; )
    {
        vTaskDelete(NULL);
    }
}

void Usart2_task(void * pvParameters)
{
    BaseType_t err = pdFALSE;
    uint8_t i;
    for( ;; )
    {
        if(NULL != binary_semaphore)
        {
            err = xSemaphoreTake(binary_semaphore, portMAX_DELAY);
            if(pdPASS == err)
            {
                // 将接收到的数据装入单字节缓存中
                g_u8ReceivedByte = usart_data_receive(USART5);
                // 将每次串口中断接收到的单字节缓存存入数据接收缓存中
                g_u8DataReceiveBuffer[g_u8ReceiveBytePosition] = g_u8ReceivedByte;
                if (g_u8ReceiveByteHeaderPosition < 3)
                {
                    if (g_u8ReceivedByte == g_u8FrameHeader[g_u8ReceiveByteHeaderPosition]) 
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
                                // 将接收缓存存入广播帧结构体中
                                memcpy(&stBroadcastFrame, &g_u8DataReceiveBuffer, 43);
                                memset(&g_u8DataReceiveBuffer, 0, sizeof(BroadcastFrame));
                                // 查询广播帧并通过ID匹配配置时隙号
                                for (i = 0; i < MAX_NODES; i++) 
                                {
                                    memcpy(&stSlot, &stBroadcastFrame.synchData.synchSlot[i], sizeof(stSlot));
                                    if (memcmp(stSlot.ID, stDevice.ID, sizeof(stDevice.ID)) == 0) 
                                    {
                                        /* 上传次数保持器复位 */
                                        Running_holder = UPLOADCOUNT;
                                        g_i16Period_counter = -1;
                                        g_i16PeriodOrder = i;
                                        /* 计算总阻抗脚，总通断脚 */
                                        g_i16PinNumBefore = CalculatePinSum(g_i16PeriodOrder, 0);
                                        g_i16PinNumBeforeZ = CalculatePinSum(g_i16PeriodOrder, 1);
                                        /* 引脚数赋值 */
                                        stDataUploadFrame.LineVolum = stBroadcastFrame.synchData.synchSlot[i].L_LineVolum;
                                        stDataUploadFrameZ.LineVolumZ = stBroadcastFrame.synchData.synchSlot[i].Z_LineVolum;
                                        break;
                                    }
                                }// Where there g_i16PeriodOrder store the order of the stDevice in the whole period
                                break;
                            case 0x04:
                                NetReplyFlag = 1;
                                break;
                            default:
                                // Handle unknown frame types
                                break;
                        }
                    }
                }
            }
        }
        else if(pdFAIL == err)
        {
            vTaskDelay(10 / portTICK_RATE_MS);      
        }
    }
}

void Timer_task(void * pvParameters)
{  
    BaseType_t err = pdFALSE;
    for( ;; )
    {
        if(NULL != timer_semaphore)
        {
            err = xSemaphoreTake(timer_semaphore, portMAX_DELAY);
            if(pdPASS == err)
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
                /* 检查运行保持器 */
                if (Running_holder != 0)
                {
                    /* 处在保持阶段，则复位计数器 */
                    if (g_i16Period_counter == 50)
                    {
                        g_i16Period_counter = -1;
                    }                
                }
                
                // 周期计数器加一
                g_i16Period_counter++;
            }
        }
        else if(pdFAIL == err)
        {
            vTaskDelay(10 / portTICK_RATE_MS);      
        }
    }
}
