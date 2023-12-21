/*************************************************************************************
 * Copyright(c) 2022 by BYD FinDreams Technology. All Rights Reserved.
 *
 * File Name: sys_config.h
 * Brief: Config some system fuction and periphral include the timer, RCU, NVIC
 * 
 * Config the timer
 * config the RCU
 * config the NVIC
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

#include "xl9555.h"
#include "i2c.h"
#include <stdio.h>
#include <stdbool.h>

uint16_t eeprom_address;
uint8_t i2c_buffer_write[BUFFER_SIZE];
uint8_t i2c_buffer_read[BUFFER_SIZE];
volatile uint8_t  *i2c_write;
volatile uint8_t  *i2c_read;
volatile uint16_t i2c_nbytes;
volatile uint16_t i2c_write_dress;
volatile uint16_t i2c_read_dress;
volatile uint8_t  i2c_process_flag = 0;

uint8_t I2cConfRegisterL[1] = {0xFF};
uint8_t I2cConfRegisterH[1] = {0x0F};

uint8_t I2cKeyBuffer[1];
uint8_t I2cFaControlSet1[1] = {0x2F};
uint8_t I2cFaControlSet2[1] = {0x1F};
uint8_t I2cFaControlSetBoth[1] = {0x3F};
uint8_t I2cFaControlResetBoth[1] = {0x0F};

bool DeviceStatusReadFlag;
bool key1Closed;
bool key2Closed;

/*!
    \brief      initialize peripherals used by the I2C EEPROM driver
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_eeprom_init(void)
{
    eeprom_address = EEPROM_BLOCK0_ADDRESS;
}

/*!
    \brief      write buffer of data to the I2C EEPROM by interrupt
    \param[in]  p_buffer: pointer to the buffer  containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_buffer_write_interrupt(uint8_t *p_buffer, uint8_t write_address, uint16_t number_of_byte)
{
    uint8_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;

    address = write_address % I2C_PAGE_SIZE;
    count = I2C_PAGE_SIZE - address;
    number_of_page = number_of_byte / I2C_PAGE_SIZE;
    number_of_single = number_of_byte % I2C_PAGE_SIZE;

    i2c_process_flag = 0;
    /* if write_address is I2C_PAGE_SIZE aligned  */
    if(0 == address) 
    {
        while(number_of_page--) 
        {
            i2c_nbytes = I2C_PAGE_SIZE;
            i2c_write_dress = write_address;
            i2c_write = p_buffer;
            /* write data by interrupt */
            eeprom_page_write_interrupt();
            eeprom_wait_standby_state();

            write_address +=  I2C_PAGE_SIZE;
            p_buffer += I2C_PAGE_SIZE;
        }
        if(0 != number_of_single) 
        {
            i2c_nbytes = number_of_single;
            i2c_write_dress = write_address;
            i2c_write = p_buffer;
            /* write data by interrupt */
            eeprom_page_write_interrupt();
            eeprom_wait_standby_state();
        }
    } 
    else 
    {
        /* if write_address is not I2C_PAGE_SIZE aligned */
        if(number_of_byte < count) 
        {
            i2c_nbytes = number_of_byte;
            i2c_write_dress = write_address;
            i2c_write = p_buffer;
            /* write data by interrupt */
            eeprom_page_write_interrupt();
            eeprom_wait_standby_state();
        } 
        else 
        {
            number_of_byte -= count;
            number_of_page = number_of_byte / I2C_PAGE_SIZE;
            number_of_single = number_of_byte % I2C_PAGE_SIZE;
            
            if(0 != count) 
            {
                i2c_nbytes = count;
                i2c_write_dress = write_address;
                i2c_write = p_buffer;
                /* write data by interrupt */
                eeprom_page_write_interrupt();
                eeprom_wait_standby_state();

                write_address += count;
                p_buffer += count;
            }
            /* write page */
            while(number_of_page--) 
            {
                i2c_nbytes = number_of_page;
                i2c_write_dress = write_address;
                i2c_write = p_buffer;
                /* write data by interrupt */
                eeprom_page_write_interrupt();
                eeprom_wait_standby_state();

                write_address +=  I2C_PAGE_SIZE;
                p_buffer += I2C_PAGE_SIZE;;
            }
            /* write single */
            if(0 != number_of_single) 
            {
                i2c_nbytes = number_of_page;
                i2c_write_dress = write_address;
                i2c_write = p_buffer;
                /* write data by interrupt */
                eeprom_page_write_interrupt();
                eeprom_wait_standby_state();
            }
        }
    }
    i2c_process_flag = SET;
}

/*!
    \brief      write more than one byte to the EEPROM by interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eeprom_page_write_interrupt(void)
{
    i2c_process_flag = 0;
    /* enable the I2CX interrupt */
    i2c_interrupt_enable(I2CX, I2C_INT_ERR);
    i2c_interrupt_enable(I2CX, I2C_INT_EV);
    i2c_interrupt_enable(I2CX, I2C_INT_BUF);
    /* the master waits until the I2C bus is idle */
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));
    /* the master sends a start condition to I2C bus */
    i2c_start_on_bus(I2CX);
    while((i2c_nbytes > 0)) {
    }
}

/*!
    \brief      read data from the EEPROM by interrupt
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the EEPROM
    \param[in]  read_address: EEPROM's internal address to start reading from
    \param[in]  number_of_byte: number of bytes to reads from the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_buffer_read_interrupt(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    i2c_read = p_buffer;
    i2c_read_dress = read_address;
    i2c_nbytes = number_of_byte;

    i2c_process_flag = SET;
    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
    /* enable the I2CX interrupt */
    i2c_interrupt_enable(I2CX, I2C_INT_ERR);
    i2c_interrupt_enable(I2CX, I2C_INT_EV);
    i2c_interrupt_enable(I2CX, I2C_INT_BUF);
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));
    if(2 == number_of_byte) {
        i2c_ackpos_config(I2CX, I2C_ACKPOS_NEXT);
    }
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2CX);
    while((i2c_nbytes > 0)) {
    }
}

/*!
    \brief      wait for EEPROM standby state
    \param[in]  none
    \param[out] none
    \retval     none
*/
void eeprom_wait_standby_state(void)
{
    __IO uint32_t val = 0;

    while(1) {
        /* wait until I2C bus is idle */
        while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

        /* send a start condition to I2C bus */
        i2c_start_on_bus(I2CX);

        /* wait until SBSEND bit is set */
        while(!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

        /* send slave address to I2C bus */
        i2c_master_addressing(I2CX, eeprom_address<<1, I2C_TRANSMITTER);

        /* keep looping till the Address is acknowledged or the AE flag is set (address not acknowledged at time) */
        do {
            /* get the current value of the I2C_STAT0 register */
            val = I2C_STAT0(I2CX);
        } while(0 == (val & (I2C_STAT0_ADDSEND | I2C_STAT0_AERR)));

        /* check if the ADDSEND flag has been set */
        if(val & I2C_STAT0_ADDSEND) {
            /* clear ADDSEND flag */
            i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* exit the function */
            return ;
        } else {
            /* clear the bit of AERR */
            i2c_flag_clear(I2CX, I2C_FLAG_AERR);
        }
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2CX);
        /* wait until the stop condition is finished */
        while(I2C_CTL0(I2CX) & I2C_CTL0_STOP);
    }
}
