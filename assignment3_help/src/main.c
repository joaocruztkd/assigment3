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
 *   
 *
 *
 *    Bug Button 2, primeira vez em que se carrega neste botão, não imprime nada e a seguinte vez imprime a print anterior, mesmo que já não esteja nesse estado
 *
 *
 *    
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
#define BOARDBUT1 0xb /* Pin at which BUT1 is connected. */ 
#define BOARDBUT2 0xc /* Pin at which BUT2 is connected. */ 
#define BOARDBUT3 0x18 /* Pin at which BUT3 is connected.*/
#define BOARDBUT4 0x19 /* Pin at which BUT4 is connected. */
#define BOARDBUT5 0x1c /* Pin at which BUT5 is connected. */
#define BOARDBUT6 0x1d /* Pin at which BUT6 is connected. */
#define BOARDBUT7 0x1e /* Pin at which BUT7 is connected. */
#define BOARDBUT8 0x1f /* Pin at which BUT8 is connected. */
#define START 0
#define BEER 1
#define TUNA 2
#define COFFEE 3

/* Int related declarations */
static struct gpio_callback but1_cb_data; /* Callback structure */
static struct gpio_callback but2_cb_data; /* Callback structure */
static struct gpio_callback but3_cb_data; /* Callback structure */
static struct gpio_callback but4_cb_data; /* Callback structure */
static struct gpio_callback but5_cb_data; /* Callback structure */
static struct gpio_callback but6_cb_data; /* Callback structure */
static struct gpio_callback but7_cb_data; /* Callback structure */
static struct gpio_callback but8_cb_data; /* Callback structure */

/* Callback function and variables*/
volatile int B1Flag = 0; /* Flag to signal a BUT1 press */
volatile int B2Flag = 0; /* Flag to signal a BUT2 press */
volatile int B3Flag = 0; /* Flag to signal a BUT3 press */
volatile int B4Flag = 0; /* Flag to signal a BUT4 press */
volatile int B5Flag = 0; /* Flag to signal a BUT5 press */
volatile int B6Flag = 0; /* Flag to signal a BUT6 press */
volatile int B7Flag = 0; /* Flag to signal a BUT7 press */
volatile int B8Flag = 0; /* Flag to signal a BUT8 press */

// Reset the buttons' flags:
void reset_BFlags(){
      B1Flag=0;										      
      B2Flag=0; 
      B3Flag=0; 
      B4Flag=0; 
      B5Flag=0; 
      B6Flag=0; 
      B7Flag=0; 
      B8Flag=0;    
}



void StateMachine() {
    int state = START; /* Or whatever is the initial state.*/
    int beer_price=150;
    int tuna_price=100;
    int coffee_price=50;
    int credit = 0;
    
    while (1) {
      switch (state) {
        case START:
          /*printk("Insert coins or choose a product pressing Up and Down.\nTo get your money back press Return.\n");*/
            if(B5Flag!=0){
                  credit = credit+10;
                  printk("Current credit: %d.%d\n", credit/100, credit%100);
                  reset_BFlags();
                  state = START;
            }
            if(B6Flag!=0){                  
                  credit = credit+20;
                  printk("Current credit: %d.%d\n", credit/100, credit%100);
                  reset_BFlags();
                  state = START;
            }
            if(B7Flag!=0){                  
                  credit = credit+50;
                  printk("Current credit: %d.%d\n", credit/100, credit%100);
                  reset_BFlags();
                  state = START;
            }
            if(B8Flag!=0){                  
                  credit = credit+100;
                  printk("Current credit: %d.%d\n", credit/100, credit%100);
                  reset_BFlags();
                  state = START;
            }
            if(B1Flag!=0){  
                  reset_BFlags();               
                  state = BEER;
            }
            if(B3Flag!=0){
                  reset_BFlags();
                  state = COFFEE;
            }
            if(B4Flag!=0){
                  printk("%d.%d EUR returned!\n", credit/100, credit%100);
                  credit = 0;
                  reset_BFlags();
                  state = START;
            }            
        break;

        case BEER:
            /*printk("   Coffee: 0.5€\n-> Beer: 1.5€\n   Tuna Salad: 1€   Credit is %d.%d\n", credit/100, credit%100);*/
            if(B2Flag!=0){
                  if(credit < beer_price){
                        printk("Not enough credit, Product Beer costs 1.5€, credit is %d.%d\nPress Return to insert the correct amount!\n", credit/100, credit%100);
                  }
                  if(credit >= beer_price){
                        credit = credit - beer_price;
                        printk("Product Beer dispensed, remaining credit: %d.%d\n", credit/100, credit%100);
                  }
                  reset_BFlags();
                  state = BEER;
             }
             if(B1Flag!=0){
                  printk("   Coffee: 0.5€\n-> Beer: 1.5€, credit is %d.%d\n",  credit/100, credit%100);
                  reset_BFlags();
                  state = COFFEE;
             }
             if(B3Flag!=0){
                  printk("-> Beer: 1.5€, credit is %d.%d\n   Tuna Salad: 1€\n",  credit/100, credit%100);
                  reset_BFlags();
                  state = TUNA;
             }
             if(B4Flag!=0){
                  printk("%d.%d EUR returned!\n",  credit/100, credit%100);
                  credit = 0;
                  reset_BFlags();
                  state = START;
             }
             if(B5Flag!=0 || B6Flag!=0 || B7Flag!=0 || B8Flag!=0){
                  printk("To insert a coin press Return and only then, insert the money!\n");
                  reset_BFlags();
                  state = BEER;
             } 
        break;

        case TUNA: 
            /*printk("   Beer: 1.5€\n-> Tuna Salad: 1€\n   Coffee: 0.5€   Credit is %d.%d\n", credit/100, credit%100);*/     
            if(B2Flag!=0){
                  if(credit < tuna_price){
                        printk("Not enough credit, Product Tuna Salad costs 1€, credit is %d.%d\nPress Return to insert the correct amount!\n", credit/100, credit%100);
                  }
                  if(credit >= tuna_price){
                        credit = credit - tuna_price;
                        printk("Product Tuna Salad dispensed, remaining credit: %d.%d\n", credit/100, credit%100);
                  }
                  reset_BFlags();
                  state = TUNA;
             }
             if(B1Flag!=0){
                  printk("   Beer: 1.5€\n-> Tuna Salad: 1€, credit is %d.%d\n", credit/100, credit%100);
                  reset_BFlags();
                  state = BEER;
             }
             if(B3Flag!=0){
                  printk("-> Tuna Salad: 1€, credit is %d.%d\n   Coffee: 0.5€\n", credit/100, credit%100);
                  reset_BFlags();
                  state = COFFEE;
             }
             if(B4Flag!=0){
                  printk("%d.%d EUR returned!\n",  credit/100, credit%100);
                  credit = 0;
                  reset_BFlags();
                  state = START;
             }  
             if(B5Flag!=0 || B6Flag!=0 || B7Flag!=0 || B8Flag!=0){
                  printk("To insert a coin press Return and only then, insert the money!\n");
                  reset_BFlags();
                  state = TUNA;
             } 
        break;
                             
        case COFFEE:      
            /*printk("   Tuna Salad: 1€\n-> Coffee: 0.5€\n   Beer: 1.5€   Credit is %d.%d\n", credit/100, credit%100);*/
            if(B2Flag!=0){
                  if(credit < coffee_price){
                        printk("Not enough credit, Product Coffee costs 0.5€, credit is %d.%d\nPress Return to insert the correct amount!\n", credit/100, credit%100);
                  }
                  if(credit >= coffee_price){
                        credit = credit - coffee_price;
                        printk("Product Coffee dispensed, remaining credit: %d.%d\n",  credit/100, credit%100);
                  }
                  reset_BFlags();
                  state = COFFEE;
             }
             if(B1Flag!=0){
                  printk("   Tuna Salad: 1€\n-> Coffee: 0.5€, credit is %d.%d\n",  credit/100, credit%100);
                  reset_BFlags();
                  state = TUNA;
             }
             if(B3Flag!=0){
                  printk("-> Coffee: 0.5€, credit is %d.%d\n   Beer: 1.5€\n",  credit/100, credit%100);
                  reset_BFlags();
                  state = BEER;
             }
             if(B4Flag!=0){
                  printk("%d.%d EUR returned!\n", credit/100, credit%100);
                  credit = 0;
                  reset_BFlags();
                  state = START;
             }
             if(B5Flag!=0 || B6Flag!=0 || B7Flag!=0 || B8Flag!=0){
                  printk("To insert a coin press Return and only then, insert the money!\n");
                  reset_BFlags();
                  state = COFFEE;
             } 
        break;
        }          
   }                        
}

void but1press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But1 pressed!\n\r");*/
    reset_BFlags();
    B1Flag = 1;   /* Update Flag*/
}

void but2press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But2 pressed!\n\r");*/
    reset_BFlags();
    B2Flag = 1;   /* Update Flag*/
}

void but3press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But3 pressed!\n\r");*/
    reset_BFlags();
    B3Flag = 1;   /* Update Flag*/
}

void but4press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But4 pressed!\n\r");*/
    reset_BFlags();
    B4Flag = 1;   /* Update Flag*/
}

void but5press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But5 pressed!\n\r");*/
    reset_BFlags();
    B5Flag = 1;   /* Update Flag*/
}

void but6press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But6 pressed!\n\r");*/
    reset_BFlags();
    B6Flag = 1;   /* Update Flag*/
}

void but7press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But7 pressed!\n\r");*/
    reset_BFlags();
    B7Flag = 1;   /* Update Flag*/
}

void but8press_cbfunction(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    
    /*printk("But8 pressed!\n\r");*/
    reset_BFlags();
    B8Flag = 1;   /* Update Flag*/
}


/* Main function */
void main(void) {

    /* Local vars */
    const struct device *gpio0_dev;         /* Pointer to GPIO device structure */
    int ret=0;         /* Generic return value variable */
    
        
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

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT2, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 2 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT2, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT2 pin \n\r", ret);
	return;
    }

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT3, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 3 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT3, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT3 pin \n\r", ret);
    }

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT4, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 4 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT4, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT4 pin \n\r", ret);
	return;
    }
    
    ret = gpio_pin_configure(gpio0_dev, BOARDBUT5, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 5 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT5, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT5 pin \n\r", ret);
	return;
    }

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT6, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 6 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT6, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT6 pin \n\r", ret);
	return;
    }

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT7, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 7 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT7, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT7 pin \n\r", ret);
	return;
    }

    ret = gpio_pin_configure(gpio0_dev, BOARDBUT8, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Error %d: Failed to configure BUT 8 \n\r", ret);
	return;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure(gpio0_dev, BOARDBUT8, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	printk("Error %d: failed to configure interrupt on BUT8 pin \n\r", ret);
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

    gpio_init_callback(&but5_cb_data, but5press_cbfunction, BIT(BOARDBUT5));
    gpio_add_callback(gpio0_dev, &but5_cb_data);

    gpio_init_callback(&but6_cb_data, but6press_cbfunction, BIT(BOARDBUT6));
    gpio_add_callback(gpio0_dev, &but6_cb_data);

    gpio_init_callback(&but7_cb_data, but7press_cbfunction, BIT(BOARDBUT7));
    gpio_add_callback(gpio0_dev, &but7_cb_data);
    
    gpio_init_callback(&but8_cb_data, but8press_cbfunction, BIT(BOARDBUT8));
    gpio_add_callback(gpio0_dev, &but8_cb_data);
         
    /* State Machine */
    StateMachine();

    return;
}

