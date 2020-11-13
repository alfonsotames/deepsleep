/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   buttons.h
 * Author: mufufu
 *
 * Created on November 6, 2020, 2:09 PM
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif
    
    
#include <device.h>
#include <drivers/gpio.h>
    
    
#define LED0_NODE       DT_ALIAS(led0)
#define LED0_GPIO_LABEL DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_GPIO_PIN   DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_GPIO_FLAGS (GPIO_OUTPUT | DT_GPIO_FLAGS(LED0_NODE, gpios))
    
#define BACKLIGHT0_NODE       DT_ALIAS(backlight0)
#define BACKLIGHT0_GPIO_LABEL DT_GPIO_LABEL(BACKLIGHT0_NODE, gpios)
#define BACKLIGHT0_GPIO_PIN   DT_GPIO_PIN(BACKLIGHT0_NODE, gpios)
#define BACKLIGHT0_GPIO_FLAGS (GPIO_OUTPUT | DT_GPIO_FLAGS(BACKLIGHT0_NODE, gpios))    

#define BUZ0_NODE       DT_ALIAS(buz0)
#define BUZ0_GPIO_LABEL DT_GPIO_LABEL(BUZ0_NODE, gpios)
#define BUZ0_GPIO_PIN   DT_GPIO_PIN(BUZ0_NODE, gpios)
#define BUZ0_GPIO_FLAGS (GPIO_OUTPUT | DT_GPIO_FLAGS(BUZ0_NODE, gpios))

#define SW0_NODE        DT_ALIAS(sw0)
#define SW0_GPIO_LABEL  DT_GPIO_LABEL(SW0_NODE, gpios)
#define SW0_GPIO_PIN    DT_GPIO_PIN(SW0_NODE, gpios)
#define SW0_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW0_NODE, gpios))

#define SW1_NODE        DT_ALIAS(sw1)
#define SW1_GPIO_LABEL  DT_GPIO_LABEL(SW1_NODE, gpios)
#define SW1_GPIO_PIN    DT_GPIO_PIN(SW1_NODE, gpios)
#define SW1_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW1_NODE, gpios))

#define SW2_NODE        DT_ALIAS(sw2)
#define SW2_GPIO_LABEL  DT_GPIO_LABEL(SW2_NODE, gpios)
#define SW2_GPIO_PIN    DT_GPIO_PIN(SW2_NODE, gpios)
#define SW2_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW2_NODE, gpios))

#define SW3_NODE        DT_ALIAS(sw3)
#define SW3_GPIO_LABEL  DT_GPIO_LABEL(SW3_NODE, gpios)
#define SW3_GPIO_PIN    DT_GPIO_PIN(SW3_NODE, gpios)
#define SW3_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW3_NODE, gpios))
    
    
extern const struct device *button0;
extern const struct device *button1;
extern const struct device *button2;
extern const struct device *button3;
extern const struct device *led;
extern const struct device *backlight;
extern const struct device *buz;

extern struct gpio_callback button0_cb_data;
extern struct gpio_callback button1_cb_data;
extern struct gpio_callback button2_cb_data;
extern struct gpio_callback button3_cb_data;

void button0_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void button2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
void button3_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);


void init_buttons();


#ifdef __cplusplus
}
#endif

#endif /* BUTTONS_H */

