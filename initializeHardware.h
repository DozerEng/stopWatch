/**
 * File:   initializeHardware.h
 * Author: Michael Pillon
 *
 * Created on May 11, 2020, 12:35 PM
 */

#include <xc.h>
#include <pic18f45k22.h>
#include <stdio.h>
#include <stdlib.h>

#include "xtalFreq.h"
#include "lcd.h"

#ifndef INITIALIZEHARDWARE_H
#define	INITIALIZEHARDWARE_H

#define   SWITCH_0          PORTBbits.RB0
#define   SWITCH_1          PORTBbits.RB1

#define     BUS             PORTD
#define     BUS_0           LATDbits.LATD0
#define     BUS_1           LATDbits.LATD1
#define     BUS_2           LATDbits.LATD2
#define     BUS_3           LATDbits.LATD3
#define     BUS_4           LATDbits.LATD4
#define     BUS_5           LATDbits.LATD5
#define     BUS_6           LATDbits.LATD6
#define     BUS_7           LATDbits.LATD7

#define   LED_ON    	0
#define   LED_OFF		1

#define PRESSED     0
#define RELEASED    1


/**
 * Tristate registar constants
 */
#define PIN_INPUT 	1
#define PIN_OUTPUT 	0

#define PORT_INPUT  0xFF
#define PORT_OUTPUT 0x00

/**
 * Constants for calculating TIMER0 overflow interrupt interval
 *  - Constants must match values configured in intializeTimers()
 */
#define TIMER0_PRESCALE         256     // 1:256
#define TIMER_8BIT_MODE         256
#define TIMER_16BIT_MODE        65536
#define TIMER0_BIT_MODE_FACTOR  TIMER_8BIT_MODE
#define TIMER0_INT_INTERVAL     ( 4.0 * TIMER0_PRESCALE * TIMER0_BIT_MODE_FACTOR ) / _XTAL_FREQ;

void initializeHardware(void);
void set_osc_p18f45k22_16MHz(void);
void initializeButtonInterrupts(void);
void initializeTimers(void);

#endif	

