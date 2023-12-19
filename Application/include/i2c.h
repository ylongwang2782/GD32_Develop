/*!
    \file    i2c.h
    \brief   the header file of I2C

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

#ifndef I2C_H
#define I2C_H

#include "gd32f4xx.h"

#define I2CX_SLAVE_ADDRESS7     0x20
#define I2C_SPEED               100000
#define I2C_PAGE_SIZE           8
#define I2CX                    I2C2
#define RCU_GPIO_I2C_TX            RCU_GPIOA
#define RCU_GPIO_I2C_RX            RCU_GPIOC
#define RCU_I2C                 RCU_I2C2
#define I2C_SCL_PORT            GPIOA
#define I2C_SDA_PORT            GPIOC
#define I2C_SCL_PIN             GPIO_PIN_8
#define I2C_SDA_PIN             GPIO_PIN_9
#define I2C_GPIO_AF             GPIO_AF_4

extern volatile uint8_t I2cKeyFlag;
extern volatile uint8_t  *i2c_write;
extern volatile uint8_t  *i2c_read;
extern volatile uint16_t i2c_nbytes;
extern volatile uint16_t i2c_write_dress;
extern volatile uint16_t i2c_read_dress;
extern volatile uint8_t  i2c_process_flag;

/* function declarations */
/* configure the GPIO ports */
void i2c_gpio_config(void);
/* configure the I2CX interface */
void i2c_config(void);
/* handle I2CX event interrupt request */
void i2cx_event_irq_handler(void);
/* handle I2CX error interrupt request */
void i2cx_error_irq_handler(void);

#endif /* I2C_H */
