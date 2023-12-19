/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: node_period_config.h
 * Brief: Some config function for node period
 * 
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
#include "gd32f4xx.h"     
#include <stdlib.h>
#include <string.h>
#include "node_period_config.h"
#include "receive_send_config.h"
#include "global_config.h"
#include "pin_period_config.h"
#include "Protocol.h"
#include "systick.h"
#include "xl9555.h"

#define CheckSumLength 1

/*************************************************************************************
                                    --- Local Constant & Type Defined ---
*************************************************************************************/


/*************************************************************************************
                                             --- Local Function ---
*************************************************************************************/
/*************************************************************************************
 * Function Name: NodeDataUpload
 * Description: This function calculates a g_i16PinNumBefore, converts data into a buffer, 
 * and sends the data when g_i16Period_counter - g_i16PinNumBefore equals 0.
 * Param[in|out]: none
 * Retrun: none
 * Exception: 
*************************************************************************************/
void NodeDataUpload(void)
{
    int i;
    int slotLLineVolum = stBroadcastFrame.synchData.synchSlot[g_i16PeriodOrder].L_LineVolum;
    int slotZLineVolum = stBroadcastFrame.synchData.synchSlot[g_i16PeriodOrder].Z_LineVolum;
    /* ����ڵ�ʮ���������ݵ���Ч���ȣ����㹫ʽΪ�ڵ�����������*��ϵͳ�������� - 1�� */
    uint8_t ValidHexDataLength = RoundDivideOf8(slotLLineVolum * (stBroadcastFrame.synchData.totalLineVolum.L_TotalLineVolum - 1));
    
    uint8_t ValidDataFrameLength  =  ValidHexDataLength + sizeof(DataUploadFrame) + CheckSumLength - 1;
    uint8_t ValidDataFrameLengthZ  =  (slotZLineVolum * (stBroadcastFrame.synchData.totalLineVolum.Z_TotalLineVolum - 1)) + sizeof(DataUploadFrame) + CheckSumLength - 1;
    

    /* ����Ƿ����㷢������ */
    if (g_i16Period_counter - g_i16PinNumBefore == 0)
    {
        /* �����յ��㲥֡�󣬷��������ظ� */
        if (NetReplyFlag == 1)
        {
            WirelessDataPacket((uint8_t const *)&stDevice.ID, 4, NetReplyBuff, 1, 0);
            for(i = 0; i < 4 + PACKLEN ; i++)
            {
                usart_data_transmit(USART5, NetReplyBuff[i]);
                while(RESET == usart_flag_get(USART5, USART_FLAG_TBE));
            }
            NetReplyFlag = 0;
        }

        /* ��ȡһ������״̬ */
        DeviceStatusRead();

        /* ������ȷ�����鳤�ȣ������������ݻ���ת��Ϊʮ���������ݻ��� */
        MultipleBinaryArrayToHex(g_u8NodeBinDataBuffer, MaxBufferLen, stDataUploadFrame.Data);
        /* ����ͨ������֡ */
        WirelessDataPacket((uint8_t const *)&stDataUploadFrame, ValidDataFrameLength , u8DmaSendBuffer, 2, g_i16PeriodOrder + 1);
        for(i = 0; i <(sizeof(DataFrameHead) + ValidDataFrameLength); i++)
        {
            usart_data_transmit(USART5, u8DmaSendBuffer[i]);
            while(RESET == usart_flag_get(USART5, USART_FLAG_TBE));
        }
        if (slotZLineVolum > 0)
        {
            /* �����迹����֡ */
            WirelessDataPacket((uint8_t const *)&stDataUploadFrameZ, ValidDataFrameLengthZ, u8DmaSendBufferZ, 3, g_i16PeriodOrder + 1);
            for(i = 0; i <(sizeof(DataFrameHead) + ValidDataFrameLengthZ); i++)
            {
                usart_data_transmit(USART5, u8DmaSendBufferZ[i]);
                while(RESET == usart_flag_get(USART5, USART_FLAG_TBE));
            }
        }
    }

    if (UnlockCommand == 1)
    {
        
    }
    
}

/*************************************************************************************
 * Function Name: DeviceStatusRead
 * Description: ͨ��I2C��ȡ�豸״̬
 * Param[in]:
 * Param[in]: 
 * Retrun: 
 * Exception: 
*************************************************************************************/
void DeviceStatusRead(void)
{
    /* �������мĴ���Ϊ����ģʽ */
    eeprom_buffer_write_interrupt(I2cConfRegisterL, XL9555_config_reg_cmd_L, 1);
    eeprom_buffer_write_interrupt(I2cConfRegisterH, XL9555_config_reg_cmd_H, 1);
    
    eeprom_buffer_read_interrupt(i2c_buffer_read, XL9555_input_reg_cmd_L, 2);
    stDataUploadFrame.deviceStatus.statusProbe1 = stDataUploadFrameZ.deviceStatus.statusProbe1= BitGet(i2c_buffer_read[1], 0);
    stDataUploadFrame.deviceStatus.statusProbe2 = stDataUploadFrameZ.deviceStatus.statusProbe2 = BitGet(i2c_buffer_read[1], 1);
    stDataUploadFrame.deviceStatus.statusPress = stDataUploadFrameZ.deviceStatus.statusPress= BitGet(i2c_buffer_read[1], 2);
    stDataUploadFrame.deviceStatus.statusColor = stDataUploadFrameZ.deviceStatus.statusColor = BitGet(i2c_buffer_read[1], 3);
}


/*************************************************************************************
 * Function Name: SingleBinaryArrayToHex
 * Description: This function takes a binary array and its length, and converts it into a 16-bit hexadecimal value.
 * Param[in]: *binaryArray: input binary array
 * Param[in]: arrayLength: output hex array
 * Retrun: decimalValue: the decimal format present the hex data which is transformed by the input binary data
 * Exception: 
*************************************************************************************/
uint8_t SingleBinaryArrayToHex(const uint8_t binaryArray[], uint8_t arrayLength) 
{
    uint16_t decimalValue = 0;
    int i;
    // Convert the binary integer array to a 16-bit unsigned integer
    for (i = 0; i < arrayLength; i++) 
    {
        decimalValue = (decimalValue << 1) | binaryArray[i];
    }

    // Return the resulting hexadecimal value
    return decimalValue;
}

/*************************************************************************************
 * Function Name: MultipleBinaryArrayToHex
 * Description: ������������ת��Ϊʮ����������
 * binaryArray[in]: ����Ķ���������
 * arrayLength[in]: ���������鳤��
 * hexArray[out]: �����ʮ����������
 * Retrun: decimalValue: the decimal format present the hex data which is transformed by the input binary data
 * Exception: 
*************************************************************************************/
uint8_t MultipleBinaryArrayToHex(const uint8_t binaryArray[], uint16_t arrayLength, uint8_t hexArray[]) 
{
    int i;
    int j;
    if (arrayLength % 8 != 0) {
        // ���������鳤�Ȳ���8�ı������޷����ȷָ�
        return 0;
    }

    for (i = 0; i < arrayLength / 8; i++) {
        uint8_t binaryChunk[8];
        for (j = 0; j < 8; j++) {
            binaryChunk[j] = binaryArray[i * 8 + j];
        }

        hexArray[i] = SingleBinaryArrayToHex(binaryChunk, 8);
    }
    return arrayLength / 8;
}

/*************************************************************************************
 * Function Name: RoundDivideOf8
 * Description: ����8������ȡ��
 * dividend[in]: ������
 * Retrun: ����ȡ������
 * Exception: 
*************************************************************************************/
int RoundDivideOf8(int dividend){
    int divisor = 8; 
    int result = dividend / divisor;
    // ������������ֶ���һ���Դ�ʵ������ȡ��
    if (dividend % divisor != 0) {
        result += 1;
    }
    return result;
}
