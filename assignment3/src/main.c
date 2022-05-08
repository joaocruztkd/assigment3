/**
 * Ana Luísa Coelho, 93371
 * Vending Machine Emulation, Assignment 3
 * 
 * Button 1 - Up
 * Button 2 - Select
 * Button 3 - Down
 * Button 4 - Return
 * Button 5 - 0.1€
 * Button 6 - 0.2€
 * Button 7 - 0.5€
 * Button 8 - 1€
 * The products, money inserted and the change it's gona be printed in the terminal!
 *
 *    ****** NOTE: must add "CONFIG_PWM=y" to prj.conf ****
 *
 *
 *      HW info
 *      https://infocenter.nordicsemi.com/topic/struct_nrf52/struct/nrf52840.html
 *      Section: nRF52840 Product Specification -> Peripherals -> GPIO / GPIOTE and PWM
 * 
 *      Board specific HW info can be found in the nRF52840_DK_User_Guide_20201203. I/O pins available at pg 27
 *
 *
 *      Peripheral Libs: 
 *              https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/peripherals/index.html
 *              GPIO and PWM
 * 
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <stdio.h>


/* Refer to dts file */
#define GPIO0_NID DT_NODELABEL(gpio0) 
#define BOARDBUT1 0xb /* Pin at which BUT1 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT2 0xc /* Pin at which BUT2 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT3 0x18 /* Pin at which BUT3 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT4 0x19 /* Pin at which BUT4 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT5 0x1c /* Pin at which BUT5 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT6 0x1d /* Pin at which BUT6 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT7 0x1e /* Pin at which BUT7 is connected. Addressing is direct (i.e., pin number) */
#define BOARDBUT8 0x1f /* Pin at which BUT8 is connected. Addressing is direct (i.e., pin number) */


/* Int related declarations */
static struct gpio_callback but1_cb_data; /* Callback structure */
static struct gpio_callback but2_cb_data; /* Callback structure */
static struct gpio_callback but3_cb_data; /* Callback structure */
static struct gpio_callback but4_cb_data; /* Callback structure */
/*...(external buttons)...*/

/* Callback function and variables*/
volatile int B1Flag = 0; /* Flag to signal a BUT1 press */
volatile int B2Flag = 0; /* Flag to signal a BUT2 press */
volatile int B3Flag = 0; /* Flag to signal a BUT3 press */
volatile int B4Flag = 0; /* Flag to signal a BUT4 press */
volatile int B5Flag = 0; /* Flag to signal a BUT5 press */
volatile int B6Flag = 0; /* Flag to signal a BUT6 press */
volatile int B7Flag = 0; /* Flag to signal a BUT7 press */
volatile int B8Flag = 0; /* Flag to signal a BUT8 press */


void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /* Inform that button was hit*/
    printk("But1 pressed!\n\r");
    
    /* Update Flag*/
    B1Flag = 1;
}

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /* Inform that button was hit*/
    printk("But2 pressed!\n\r");
    
    /* Update Flag*/
    B2Flag = 1;
}

void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /* Inform that button was hit*/
    printk("But3 pressed!\n\r");
    
    /* Update Flag*/
    B3Flag = 1;
}

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /* Inform that button was hit*/
    printk("But4 pressed!\n\r");
    
    /* Update Flag*/
    B4Flag = 1;
}

void coin_detector(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    /* Inform that button was hit*/
    printk("Coin detected!\n\r");
    
    /* Update Flag*/
    cd = 1;
}


/* Main function */
void main(void) {

    /* Local vars */
    const struct device *gpio0_dev;         /* Pointer to GPIO device structure */
    int ret=0,ret2=0,ret3=0,ret4=0;         /* Generic return value variable */
    
        
    /* Task init code */
   
    /* Bind to GPIO 0 and PWM0 */
    gpio0_dev = device_get_binding(DT_LABEL(GPIO0_NID));
    if (gpio0_dev == NULL) {
        printk("Error: Failed to bind to GPIO0\n\r");        
	return;
    }
    else {
        printk("Bind to GPIO0 successfull \n\r");        
    }
    
        
    /* Configure PINS */    
    
    /* Note that PCB does not include pull-up resistors */
    /* See nRF52840v1.0.0 DK Users Guide V 1.0.0, pg 29 */
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT1, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 1 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT1 pin \n\r", ret);
	return;
    }

    ret2 = gpio_pin_configure(gpio0_dev, BOARDBUT2, GPIO_INPUT | GPIO_PULL_UP);
    if (ret2 < 0) {
        printk("Error %d: Failed to configure BUT 2 \n\r", ret2);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret2 = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT2, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret2 != 0) {
	printk("Error %d: failed to configure interrupt on BUT2 pin \n\r", ret2);
	return;
    }

    ret3 = gpio_pin_configure(gpio0_dev, BOARDBUT3, GPIO_INPUT | GPIO_PULL_UP);
    if (ret3 < 0) {
        printk("Error %d: Failed to configure BUT 3 \n\r", ret3);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret3 = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT3, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret3 != 0) {
	printk("Error %d: failed to configure interrupt on BUT3 pin \n\r", ret3);
	return;
    }

    ret4 = gpio_pin_configure(gpio0_dev, BOARDBUT4, GPIO_INPUT | GPIO_PULL_UP);
    if (ret4 < 0) {
        printk("Error %d: Failed to configure BUT 4 \n\r", ret4);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret4 = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT4, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret4 != 0) {
	printk("Error %d: failed to configure interrupt on BUT4 pin \n\r", ret4);
	return;
    }
    
    /* Set callback */
    gpio_init_callback(&but1_cb_data, but1press_cbfunction, BIT(BOARDBUT1));
    gpio_add_callback(gpio0_dev, &but1_cb_data);

    gpio_init_callback(&but2_cb_data, but2press_cbfunction, BIT(BOARDBUT2));
    gpio_add_callback(gpio0_dev, &but2_cb_data);

    gpio_init_callback(&but3_cb_data, but3press_cbfunction, BIT(BOARDBUT3));
    gpio_add_callback(gpio0_dev, &but3_cb_data);

    gpio_init_callback(&but4_cb_data, but4press_cbfunction, BIT(BOARDBUT4));
    gpio_add_callback(gpio0_dev, &but4_cb_data);
    
    /* More Local Variables*/
    int cd=0, credit=0; 
    /* main loop */
    while(1) {        
        
        if(B5Flag!=0 || B6Flag!=0 || B7Flag!=0 || B8Flag!=0)
                  
        if(dcToggleFlag) {
            dcIndex++;
            if(dcIndex == 4) 
                dcIndex = 0;
            dcToggleFlag = 0;
            printk("PWM DC value set to %u %%\n\r",dcValue[dcIndex]);

            ret = pwm_pin_set_usec(pwm0_dev, BOARDLED1,
		      pwmPeriod_us,(unsigned int)((pwmPeriod_us*dcValue[dcIndex])/100), PWM_POLARITY_NORMAL);
            if (ret) {
                printk("Error %d: failed to set pulse width\n", ret);
		return;
            }
        }            
           
    }

    return;
}

