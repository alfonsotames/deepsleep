
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/timer/system_timer.h>
#include <sys/util.h>
#include <sys///printk.h>
#include <inttypes.h>
#include <power/power.h>

#include "buttons.h"

#define SLEEP_TIME_MS   80


/**
  * @brief  Processor uses deep sleep as its low power mode
  * @rmtoll SCB_SCR      SLEEPDEEP     LL_LPM_EnableDeepSleep
  * @retval None
  */
__STATIC_INLINE void LL_LPM_EnableDeepSleep(void)
{
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Disable SRAM3 content retention in Stop mode
  * @rmtoll CR1          RRSTP           LL_PWR_DisableSRAM3Retention
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableSRAM3Retention(void)
{
  CLEAR_BIT(PWR->CR1, PWR_CR3_RRS);
}



// Switch Button and Led configured in dts at:
// LED0 = Pin PA17
// SW0 = Pin PA21





#define SLEEP_TIME_MS   80


volatile bool pressed; 



static struct k_timer timer;


void peep(int n) {
        gpio_pin_set(buz, BUZ0_GPIO_PIN, true);
        k_msleep(n);
        gpio_pin_set(buz, BUZ0_GPIO_PIN, false);
        //k_msleep(40);
}


// Blink LED0 5 times
void blink(int n, int delay) {

    bool led_is_on = false;
    for (int x=0; x<n; x++) {

        gpio_pin_set(led, LED0_GPIO_PIN, (int) led_is_on);
        led_is_on = !led_is_on;
        k_msleep(delay);  
    }
}


void hbeat() {


}


void button0_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    //peep();
    //printk("Button 0 pressed\n");
    pressed=true;

}


void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    //peep();
    //printk("Button 1 pressed\n");
    pressed=true;
}

void button2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    //peep();
    //printk("Button 2 pressed\n");
    pressed=true;
}









void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinPolarity)
{
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinPolarity));

  /* Specifies the Wake-Up pin polarity for the event detection
    (rising or falling edge) */
  MODIFY_REG(PWR->CR4, (PWR_CR3_EWUP & WakeUpPinPolarity), (WakeUpPinPolarity >> PWR_WUP_POLARITY_SHIFT));

  /* Enable wake-up pin */
  SET_BIT(PWR->CR3, (PWR_CR3_EWUP & WakeUpPinPolarity));


}

/**
  * @brief Disable the WakeUp PINx functionality.
  * @param WakeUpPinx: Specifies the Power Wake-Up pin to disable.
  *         This parameter can be one of the following values:
  *           @arg @ref PWR_WAKEUP_PIN1, PWR_WAKEUP_PIN2, PWR_WAKEUP_PIN3, PWR_WAKEUP_PIN4, PWR_WAKEUP_PIN5
  * @retval None
  */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx)
{
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));

  CLEAR_BIT(PWR->CR3, (PWR_CR3_EWUP & WakeUpPinx));
}







void button3_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    //peep();
    //printk("Button 3 pressed\n");
    printk("GOING TO SLEEP....zzzzzzzzzz\n\n");
    pressed = true;
    
      
    LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN1);
    
    LL_PWR_ClearFlag_WU1();
    if (LL_PWR_IsActiveFlag_WU1()) {
        LL_PWR_ClearFlag_WU1();
    }
    
    LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN1);
    
    //for checking if it comes from a reset
    WRITE_REG( RTC->BKP31R, 0x1 );
  
    
    LL_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);
    //LL_PWR_SetPowerMode(LL_PWR_MODE_STOP0);


    LL_LPM_EnableDeepSleep();
    // Request Wait For Interrupt
    __WFI();
   
    
    /*

  MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_SHUTDOWN);
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

#if defined ( __CC_ARM)
  __force_stores();
#endif
  __WFI();
  */

    

}

bool timer_expired;
extern void timer_expire(struct k_timer *timer_id) {
    timer_expired=true;
}



void main() {
    timer_expired = true;
    printk("\n\n********** Deep Sleep with Cortex M4 *********\n\n");
        init_buttons();

      if (READ_REG(RTC->BKP31R) == 1)
  {
     WRITE_REG( RTC->BKP31R, 0x0 );  /* reset back-up register */
    /* Blink LED3 to indicate that the system was resumed from Standby mode */

     peep(50);
  }

    pressed=false;
    


    blink(3,100);
     
k_timer_init(&timer, timer_expire, NULL);
    



        int n=0;
    while (1) {
        if (pressed) {

            blink(3,50);
            peep(50);
            pressed = false;
            printk("Pressed %d...\n",n);
            n++;
        }
        
        if (timer_expired) {
            blink(3,50);
            //printk("Timer Expired!!!\n");
            k_timer_start(&timer, K_MSEC(500), K_MSEC(0));
            timer_expired=false;
        }

        //peep();
        //printk("Hola %d\n",n);
        //blink(3);        
        //k_msleep(1);
        

    }

}
