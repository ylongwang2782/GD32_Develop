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
#ifndef NODE_PERIOD_CONFIG_H
#define NODE_PERIOD_CONFIG_H

/*************************************************************************************
                                             --- Header Files ---
*************************************************************************************/
#include "gd32f4xx.h" 

/*************************************************************************************
                                                --- Public Function ---
*************************************************************************************/
void DeviceStatusRead(void);
void NodeDataUpload(void);
uint8_t BitGet(uint8_t source, int sourceBitPosition) ;
uint8_t SingleBinaryArrayToHex(const uint8_t binaryArray[], uint8_t arrayLength) ;
uint8_t MultipleBinaryArrayToHex(const uint8_t binaryArray[], uint16_t arrayLength, uint8_t hexArray[]);
int RoundDivideOf8(int dividend);

#endif
