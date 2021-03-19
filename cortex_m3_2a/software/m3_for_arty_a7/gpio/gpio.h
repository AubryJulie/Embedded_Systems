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
 * File:     gpio.h
 * Release Information : Cortex-M3 DesignStart-r0p1-00rel0
 * ----------------------------------------------------------------
 *
 */
#include "xil_types.h"

int InitialiseGPIO( void );
void EnableGPIOInterrupts( void );
int button_pressed( void );
void reset_button( void );
void SetLedValue( volatile uint32_t value );

/* GPIO channel identifiers */
#define ARTY_A7_LED_CHANNEL 1
#define ARTY_A7_PB_CHANNEL  2
