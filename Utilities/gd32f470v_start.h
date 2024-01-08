/*!
    \file    gd32f470v_start.h
    \brief   definitions for GD32F470_START's leds and keys hardware resources

    \version 2022-04-26, V2.0.0, demo for GD32F4xx
*/

#ifndef GD32F470V_START_H
#define GD32F470V_START_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
     
/* exported types */
typedef enum 
{
    LED2
} led_typedef_enum;

typedef enum 
{
    KEY_USER
} key_typedef_enum;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* eval board low layer led */
#define LEDn                             1U

#define LED2_PIN                         GPIO_PIN_6
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCU_GPIOC

#define KEYn                             1U

/* user push-button */
#define USER_KEY_PIN                     GPIO_PIN_1
#define USER_KEY_GPIO_PORT               GPIOA
#define USER_KEY_GPIO_CLK                RCU_GPIOA
#define USER_KEY_EXTI_LINE               EXTI_1
#define USER_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define USER_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN1
#define USER_KEY_EXTI_IRQn               EXTI1_IRQn

/* function declarations */
/* configure led GPIO */
void gd_eval_led_init(led_typedef_enum led_num);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum led_num);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum led_num);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum led_num);
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
/* return the selected key state */
uint8_t gd_eval_key_state_get(key_typedef_enum key);

#ifdef cplusplus
}
#endif

#endif /* GD32F470V_START_H */
