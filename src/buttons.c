/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   buttons.c
 * Author: mufufu
 *
 * Created on November 6, 2020, 2:12 PM
 */

#include "buttons.h"

/*
 * 
 */


 const struct device *button0;
 const struct device *button1;
 const struct device *button2;
 const struct device *button3;
 const struct device *led;
 const struct device *backlight;
 const struct device *buz;

 struct gpio_callback button0_cb_data;
 struct gpio_callback button1_cb_data;
 struct gpio_callback button2_cb_data;
 struct gpio_callback button3_cb_data;


void init_buttons() {

    
    led = device_get_binding(LED0_GPIO_LABEL);
    backlight = device_get_binding(BACKLIGHT0_GPIO_LABEL);
    buz = device_get_binding(BUZ0_GPIO_LABEL);    
    button0 = device_get_binding(SW0_GPIO_LABEL);
    button1 = device_get_binding(SW1_GPIO_LABEL);
    button2 = device_get_binding(SW2_GPIO_LABEL);
    button3 = device_get_binding(SW3_GPIO_LABEL);
    
    


    int ret;
    

    if (button0 == NULL) {
        printk("Error: didn't find %s device\n", SW0_GPIO_LABEL);
        return;
    }
    if (button1 == NULL) {
        printk("Error: didn't find %s device\n", SW1_GPIO_LABEL);
        return;
    }
    if (button2 == NULL) {
        printk("Error: didn't find %s device\n", SW2_GPIO_LABEL);
        return;
    }
    if (button3 == NULL) {
        printk("Error: didn't find %s device\n", SW3_GPIO_LABEL);
        return;
    }    
    if (led == NULL) {
        printk("Error: didn't find %s device\n", LED0_GPIO_LABEL);
        return;
    }
    if (backlight == NULL) {
        printk("Error: didn't find %s device\n", BACKLIGHT0_GPIO_LABEL);
        return;
    }
    if (buz == NULL) {
        printk("Error: didn't find %s device\n", BUZ0_GPIO_LABEL);
        return;
    }
    

    /************ Led 0 ***********/
    ret = gpio_pin_configure(led, LED0_GPIO_PIN, LED0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure LED device %s pin %d\n", ret, 
                LED0_GPIO_LABEL, LED0_GPIO_PIN);
        return;
    }

    /************ Backlight ***********/
    ret = gpio_pin_configure(led, BACKLIGHT0_GPIO_PIN, BACKLIGHT0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure LED device %s pin %d\n", ret, 
                BACKLIGHT0_GPIO_LABEL, BACKLIGHT0_GPIO_PIN);
        return;
    }
    
     /************ Buzzer ***********/
    ret = gpio_pin_configure(buz, BUZ0_GPIO_PIN, BUZ0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure BUZZER device %s pin %d\n", ret, 
                BUZ0_GPIO_LABEL, BUZ0_GPIO_PIN);
        return;
    }  
    
    /************ Button 0 ***********/
    ret = gpio_pin_configure(button0, SW0_GPIO_PIN, SW0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, 
                SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    } 

    ret = gpio_pin_interrupt_configure(button0, SW0_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, 
                SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    }
    gpio_init_callback(&button0_cb_data, button0_pressed, BIT(SW0_GPIO_PIN));
    gpio_add_callback(button0, &button0_cb_data);
    
    /************ Button 1 ***********/
    
    ret = gpio_pin_configure(button1, SW1_GPIO_PIN, SW1_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, 
                SW1_GPIO_LABEL, SW1_GPIO_PIN);
        return;
    } 

    ret = gpio_pin_interrupt_configure(button1, SW1_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, 
                SW1_GPIO_LABEL, SW1_GPIO_PIN);
        return;
    }
    gpio_init_callback(&button1_cb_data, button1_pressed, BIT(SW1_GPIO_PIN));
    gpio_add_callback(button1, &button1_cb_data);
    
    /************ Button 2 ***********/
    ret = gpio_pin_configure(button2, SW2_GPIO_PIN, SW2_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, 
                SW2_GPIO_LABEL, SW2_GPIO_PIN);
        return;
    } 

    ret = gpio_pin_interrupt_configure(button2, SW2_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, 
                SW2_GPIO_LABEL, SW2_GPIO_PIN);
        return;
    }
    gpio_init_callback(&button2_cb_data, button2_pressed, BIT(SW2_GPIO_PIN));
    gpio_add_callback(button2, &button2_cb_data);
    
    /************ Button 3 ***********/
    ret = gpio_pin_configure(button3, SW3_GPIO_PIN, SW3_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, 
                SW3_GPIO_LABEL, SW3_GPIO_PIN);
        return;
    } 

    ret = gpio_pin_interrupt_configure(button3, SW3_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, 
                SW3_GPIO_LABEL, SW3_GPIO_PIN);
        return;
    }
    gpio_init_callback(&button3_cb_data, button3_pressed, BIT(SW3_GPIO_PIN));
    gpio_add_callback(button3, &button3_cb_data);
    
    
}