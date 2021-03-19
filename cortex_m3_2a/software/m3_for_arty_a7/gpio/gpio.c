/*
 * Copyright:
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2014, 2018 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     gpio.c
 * Release Information : Cortex-M3 DesignStart-r0p1-00rel0
 * ----------------------------------------------------------------
 *
 */

/*
 * --------Included Headers--------
 */

#include "gpio.h"
#include "xparameters.h"        // Project memory and device map
#include "xgpio.h"              // Xilinx GPIO routines
#include "peripherallink.h"     // IRQ definitions

/************************** Variable Definitions **************************/
 
static XGpio Gpio_Led_PB;   /* The driver instance for GPIO Device 0 */
int button;

/*****************************************************************************/

// Initialise the GPIO and zero the outputs
int InitialiseGPIO( void )
{
    // Define local variables
    int status;

    /*
     * Initialize the GPIO driver so that it's ready to use,
     * specify the device ID that is generated in xparameters.h
    */
    status = XGpio_Initialize(&Gpio_Led_PB, XPAR_AXI_GPIO_0_DEVICE_ID);
    if (status != XST_SUCCESS)  {
        return XST_FAILURE;
    }
		
    // GPIO0
    // Port0 drives led_4bits.
    XGpio_SetDataDirection(&Gpio_Led_PB, ARTY_A7_LED_CHANNEL, 0xFFFFFFF0);

    // Port 1 inputs the push buttons_4bits.
    XGpio_SetDataDirection(&Gpio_Led_PB, ARTY_A7_PB_CHANNEL, 0xFFFFFFFF);
    
    // Write 0x0 to LEDs
    XGpio_DiscreteWrite(&Gpio_Led_PB, ARTY_A7_LED_CHANNEL, 0x0);

    return XST_SUCCESS;
    
}

// Set GPIO interrupts
void EnableGPIOInterrupts( void )
{
    // Push buttons are on Channel 2
   
    XGpio_InterruptEnable(&Gpio_Led_PB, XGPIO_IR_CH2_MASK);

    // Having enabled the M1 to handle the interrupts, now enable the GPIO to send the interrupts

    XGpio_InterruptGlobalEnable(&Gpio_Led_PB);
}


// Define the GPIO interrupt handlers
void GPIO0_Handler ( void )
{
		// Trigger an event on a rising edge
		if (XGpio_DiscreteRead(&Gpio_Led_PB, ARTY_A7_PB_CHANNEL) == 1)
		{
			button = 1;
		}
		
    // Clear interrupt from GPIO
    XGpio_InterruptClear(&Gpio_Led_PB, XGPIO_IR_MASK);
    // Clear interrupt in NVIC
    NVIC_ClearPendingIRQ(GPIO0_IRQn);
}

// Send the state of the button
int button_pressed( void )
{
	return button;
}

// Resetting the state of the button
void reset_button( void )
{
	button = 0;
}

// Setting the led to a given value
void SetLedValue( volatile uint32_t value )
{
    XGpio_DiscreteWrite(&Gpio_Led_PB, ARTY_A7_LED_CHANNEL, (value));   // Set LEDs
    
}
