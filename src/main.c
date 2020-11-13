
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/timer/system_timer.h>
#include <sys/util.h>
#include <sys///printk.h>
#include <inttypes.h>
#include <soc.h>

#define SLEEP_TIME_MS   80

// Switch Button and Led configured in dts at:
// LED0 = Pin PA17
// SW0 = Pin PA21

#define LED0_NODE       DT_ALIAS(led0)
#define LED0_GPIO_LABEL DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_GPIO_PIN   DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_GPIO_FLAGS (GPIO_OUTPUT | DT_GPIO_FLAGS(LED0_NODE, gpios))

#define SW0_NODE        DT_ALIAS(sw0)
#define SW0_GPIO_LABEL  DT_GPIO_LABEL(SW0_NODE, gpios)
#define SW0_GPIO_PIN    DT_GPIO_PIN(SW0_NODE, gpios)
#define SW0_GPIO_FLAGS  (GPIO_INPUT | DT_GPIO_FLAGS(SW0_NODE, gpios))

// Enable EIC clock with the Ultra Low Power 32Khz clock OSCULP32K
// for interrupt working on GPIO_INT_EDGE_RISING

void config_eic_clk() {
    //GCLK->CLKCTRL.bit.CLKEN = 0; //disable GCLK module
    while (GCLK->STATUS.bit.SYNCBUSY);
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK6
            | GCLK_CLKCTRL_ID(0x05U)); //EIC clock switched on GCLK1
    while (GCLK->STATUS.bit.SYNCBUSY);
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K
            | GCLK_GENCTRL_ID(6)); //source for GCLK6 is OSCULP32K
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
    GCLK->GENCTRL.bit.RUNSTDBY = 1; //GCLK1 run standby
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

static struct gpio_callback button_cb_data;

volatile bool pressed; 
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    printk("Button pressed\n");
    pressed=true;
}

void sleep() {
    NVMCTRL->CTRLB.bit.SLEEPPRM = NVMCTRL_CTRLB_SLEEPPRM_DISABLED_Val;
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    printk("GOING TO SLEEP...");
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    while (WDT->STATUS.bit.SYNCBUSY);
    __DSB(); // Data sync to ensure outgoing memory accesses complete
    __WFI(); // Wait for interrupt (places device in sleep mode) 
    while (WDT->STATUS.bit.SYNCBUSY);
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    printk("AWAKE!");
}

const struct device *button;
const struct device *led;

// Blink LED0 5 times
void blink() {
    bool led_is_on = false;
    for (int x=0; x<10; x++) {
        gpio_pin_set(led, LED0_GPIO_PIN, (int) led_is_on);
        led_is_on = !led_is_on;
        k_msleep(SLEEP_TIME_MS);
        
    }
}

void main() {

    config_eic_clk();

    pressed=false;
    int ret;

    button = device_get_binding(SW0_GPIO_LABEL);
    led = device_get_binding(LED0_GPIO_LABEL);

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
        printk("Error %d: failed to configure LED device %s pin %d\n", ret, 
                LED0_GPIO_LABEL, LED0_GPIO_PIN);
        return;
    }

    ret = gpio_pin_configure(button, SW0_GPIO_PIN, SW0_GPIO_FLAGS);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, 
                SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    } 

    ret = gpio_pin_interrupt_configure(button, SW0_GPIO_PIN,
            GPIO_INT_EDGE_RISING | GPIO_INT_DEBOUNCE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, 
                SW0_GPIO_LABEL, SW0_GPIO_PIN);
        return;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(SW0_GPIO_PIN));
    gpio_add_callback(button, &button_cb_data);

    EIC->WAKEUP.reg |= (1 << 5U); // Configured in EXTINT5 for PA21

    while (1) {
        if (pressed) {
            blink();
            pressed=false;
            sleep(); // Go to deep sleep
        }
        k_msleep(1);
    }

}