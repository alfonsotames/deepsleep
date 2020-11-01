
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/timer/system_timer.h>
#include <sys/util.h>
#include <sys///printk.h>
#include <inttypes.h>
#include <soc.h>

#define SLEEP_TIME_MS   80


#define LED0_NODE       DT_ALIAS(led0)
#define LED0_GPIO_LABEL DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_GPIO_PIN   DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_GPIO_FLAGS (GPIO_OUTPUT | DT_GPIO_FLAGS(LED0_NODE, gpios))

#define SW0_NODE        DT_ALIAS(sw0)
#define SW0_GPIO_LABEL  DT_GPIO_LABEL(SW0_NODE, gpios)
#define SW0_GPIO_PIN    DT_GPIO_PIN(SW0_NODE, gpios)
#define SW0_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW0_NODE, gpios))

void configGCLK6() {

    // enable EIC clock
    GCLK->CLKCTRL.bit.CLKEN = 0; //disable GCLK module
    while (GCLK->STATUS.bit.SYNCBUSY);

    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK6 | GCLK_CLKCTRL_ID(0x05U)); //EIC clock switched on GCLK6
    while (GCLK->STATUS.bit.SYNCBUSY);
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(6)); //source for GCLK6 is OSCULP32K
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    GCLK->GENCTRL.bit.RUNSTDBY = 1; //GCLK6 run standby
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

}



static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb,
        uint32_t pins) {

    //NVIC_SystemReset(); 
    //timerStart(); //se atora
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());

}

void main() {

    configGCLK6();
    EIC->WAKEUP.reg |= (1 << 5);
    int ret;

    const struct device *button = device_get_binding(SW0_GPIO_LABEL);
    const struct device *led = device_get_binding(LED0_GPIO_LABEL);

    if (button == NULL) {
        printk("Error: didn't find %s device\n", SW0_GPIO_LABEL);
        return;
    }
    if (led == NULL) {
        printk("Error: didn't find %s device\n", LED0_GPIO_LABEL);
        return;
    }

    ret = gpio_pin_configure(led, LED0_GPIO_PIN, LED0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure LED device %s pin %d\n", ret, LED0_GPIO_LABEL, LED0_GPIO_PIN);
        return;
    }

    ret = gpio_pin_configure(button, SW0_GPIO_PIN, SW0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    } else {
        printk("Set up button at %s pin %d\n", SW0_GPIO_LABEL, SW0_GPIO_PIN);
    }

    ret = gpio_pin_interrupt_configure(button,
            SW0_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    } else {
        printk("Set up interrupt at %s pin %d\n", SW0_GPIO_LABEL, SW0_GPIO_PIN);
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(SW0_GPIO_PIN));
    gpio_add_callback(button, &button_cb_data);



    bool led_is_on = true;
    int c = 0;
    while (1) {
        gpio_pin_set(led, LED0_GPIO_PIN, (int) led_is_on);
        led_is_on = !led_is_on;

        if (c > 15) {
            c = 0;


            NVMCTRL->CTRLB.bit.SLEEPPRM = NVMCTRL_CTRLB_SLEEPPRM_DISABLED_Val;

            SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
            printk("SLEEPING...");
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

            while (WDT->STATUS.bit.SYNCBUSY);

            __DSB(); // Data sync to ensure outgoing memory accesses complete

            __WFI(); // Wait for interrupt (places device in sleep mode) 

            while (WDT->STATUS.bit.SYNCBUSY);
            
            SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
            printk("AWAKE");


        }
        c++;
        k_msleep(SLEEP_TIME_MS);
    }

}