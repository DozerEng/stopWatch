/*
 * File:   main.c
 * Author: Michael Pillon
 *
 * Created on January 4, 2021, 10:59 AM
 */


#include <xc.h>
#include <pic18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pragmas.h"
#include "xtalFreq.h"
#include "initializeHardware.h"
#include "lcd.h"

// Global variable to act as counter for timer
float timeElapsed = 0.0; 

/**
 * Function Declarations
 */
void __interrupt(high_priority) highPriorityISR (void);
void __interrupt(low_priority) lowPriorityISR (void);

void main(void) {
    initializeHardware();
    
    // This function is not pointless, you are pointless, don't question me!
    for(int i = 0; i < 3; i++) {
        lcdWriteString("Loading");
        __delay_ms(250);
        lcdWriteString("Loading.");
        __delay_ms(250);
        lcdWriteString("Loading..");
        __delay_ms(250);
        lcdWriteString("Loading...");
        __delay_ms(250);
    }

    // Initialize LCD
    lcdWriteString("Timer: 0.00s");
    
    while (1) {
        
        // Nothing to do but wait!
        
        __delay_ms(1000);
	} //eo infinite loop
    
    return;
} //eo main


/**
 * Function Name: highPriorityISR
 * Parameters: None 
 * Return: None 
 * Description: Interrupt subroutine for high priority interrupts
 *      - Handing SWITCH_0 interrupts
 *          - SWITCH_0 starts and stops timer
 *      - Handing Timer0 overflow interrupts
 *          - Every time Timer0 overflows, timeElapsed is incremented by TIMER0_INT_INTERVAL
 * 
 */

void __interrupt(high_priority) highPriorityISR (void)           // High priority interrupt
{
    /**
     * INT0 - Push button RB0
     *  - Toggles Timer0 on or off
     */
    if (INTCONbits.INT0IF == 1 && INTCONbits.INT0IE) { 
        // Toggle start/stop state of Timer0
        if(INTCONbits.TMR0IE == 0) {
            INTCONbits.TMR0IF = 0; // Clear interrupt flag before enabling timer
            INTCONbits.TMR0IE = 1;
        } else {
            INTCONbits.TMR0IE = 0;
        }
        // Wait for button to be released
        while(SWITCH_0 == PRESSED); 
        // Clear INT0 interrupt flag
        INTCONbits.INT0IF = 0;
    }
    
    /**
     *  Timer0 interrupt
     */
    if (INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1) { 
        // Clear interrupt flag
        INTCONbits.TMR0IF = 0;
        // Increment timeElapsed based on timer overflow interval (See initializeHardware.c)
        timeElapsed += TIMER0_INT_INTERVAL;
        char buffer[17];
        sprintf(buffer, "Timer: %0.2fs", timeElapsed);
        lcdWriteString(buffer);
    }
}

/**
 * Function Name: lowPriorityISR
 * Parameters: None 
 * Return: None 
 * Description: Interrupt subroutine for low priority interrupts
 *      - Handing SWITCH_1 interrupts
 *          - SWITCH_1 resets timeElapsed value to 0.0 seconds
 *          - Also resets values in Timer0 data registers 
 * 
 */

void __interrupt(low_priority) lowPriorityISR (void) {
    /**
     * INT1 - Push button RB1
     *  - Resets timeElapsed which is controlled by Timer0
     */
    if (INTCON3bits.INT1IF == 1 && INTCON3bits.INT1IE == 1) {
        // Clear INT1 interrupt flag
        INTCON3bits.INT1IF = 0;
        
        /**
         * Reset time elapsed and timer
         */
        timeElapsed = 0.0;
        lcdWriteString("Timer: 0.00s");
        
        /**
         * Reset timer 16-bit data registers
         */
        TMR0H = 0; // Upper 8 bits
        TMR0L = 0; // Lower 8 bits
        // Wait for button to be released
        while(SWITCH_1 == PRESSED);
    }
}