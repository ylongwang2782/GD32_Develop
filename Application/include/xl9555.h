/*!
    \file    at24cxx.h
    \brief   the header file of AT24Cxx

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

#ifndef XL9555_H
#define XL9555_H

#include "gd32f4xx.h"
#include <stdbool.h>

typedef enum {
    I2C_SEND_ADDRESS_FIRST = 0,
    I2C_CLEAR_ADDRESS_FLAG_FIRST,
    I2C_TRANSMIT_WRITE_READ_ADD,
    I2C_SEND_ADDRESS_SECOND,
    I2C_CLEAR_ADDRESS_FLAG_SECOND,
    I2C_TRANSMIT_DATA,
    I2C_STOP
} i2c_process_enum;

#define EEPROM_BLOCK0_ADDRESS    0x20
#define BUFFER_SIZE              16

#define XL9555_input_reg_cmd_L 0x00
#define XL9555_input_reg_cmd_H 0x01
#define XL9555_output_reg_cmd_L 0x02
#define XL9555_output_reg_cmd_H 0x03
#define XL9555_polar_inv_reg_cmd_L 0x04
#define XL9555_polar_inv_reg_cmd_H 0x05
#define XL9555_config_reg_cmd_L 0x06
#define XL9555_config_reg_cmd_H 0x07

#define EEP_FIRST_PAGE 0x00
#define I2C_OK         0
#define I2C_FAIL       1
#define I2CX           I2C2

extern uint8_t i2c_buffer_write[];
extern uint8_t i2c_buffer_read[];
extern uint8_t i2c_LoRa_channel_config[];

extern uint8_t I2cConfRegisterL[];
extern uint8_t I2cConfRegisterH[];
extern uint8_t I2cConfRegisterL[];
extern uint8_t I2cConfRegisterH[];

extern uint8_t I2cKeyBuffer[];
extern uint8_t I2cFaControlSet1[];
extern uint8_t I2cFaControlSet2[];
extern uint8_t I2cFaControlSetBoth[1];
extern uint8_t I2cFaControlResetBoth[];

extern bool DeviceStatusReadFlag;
extern bool key1Closed;
extern bool key2Closed;

/* function declarations */
/* I2C read and write functions */
uint8_t i2c_xl9555_read(void);
/* initialize peripherals used by the I2C EEPROM driver */
void i2c_eeprom_init(void);
/* write buffer of data to the I2C EEPROM by interrupt */
void eeprom_buffer_write_interrupt(uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte);
/* write more than one byte to the EEPROM by interrupt */
void eeprom_page_write_interrupt(void);
/* read data from the EEPROM by interrupt */
void eeprom_buffer_read_interrupt(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte);
/* wait for EEPROM standby state */
void eeprom_wait_standby_state(void);

#endif /* XL9555_H */
