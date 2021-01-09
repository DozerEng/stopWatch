/** 
 * File:   initializeHardware.c
 * Author: Michael Pillon
 *
 * Created on May 11, 2020, 12:09 PM
 */

#include "initializeHardware.h"

/**
 * Function Name: initializeHardware
 * Parameters: None 
 * Return: None 
 * Description: Sets initial ports and interrupts. 
 *
 */

void initializeHardware(void)
{
    set_osc_p18f45k22_16MHz();

    /** 
     * Configure PORTBbits RB0 to RB1 as switch inputs. 
     *    - RB6 and RB7 are for PICKIT3 programming interface.
     *    - RB0 and RB1 are configured for interrupts
     */
    PORTB = 0x00; //Initial Pin State
    TRISBbits.RB0 = PIN_INPUT;
    TRISBbits.RB1 = PIN_INPUT;
    ANSELB = 0;


    /**
     * Universal Interrupt settings
     */

    // Enables interrupt priority
    RCONbits.IPEN = 1;
    // Enable all high priority interrupts
    INTCONbits.GIEH = 1;
    // Enable all low priority interrupts
    INTCONbits.PEIE = 1;

    /**
     * Initialize interrupts for pushbuttons
     */
    initializeButtonInterrupts();

    /**
     * Initialize Timer0
     */
    initializeTimers();

    /**
     * Initialize LCD
     *  - Check hardware pins in lcd.h for correct pin setup
     */
    lcdInitialize(LCD_4_BIT_MODE, LCD_ROWS, LCD_5x8_CHAR);

    printf("\n\rDevice initialized successfully\n\r");
}//eo initializeHardware: ==============================================



/**
 * Function Name: set_osc_p18f45k22_16MHz
 * Parameters: None 
 * Return: None 
 * Description: Sets PIC oscillator speed to 16MHz
 * 
 */

void set_osc_p18f45k22_16MHz(void) {
    // Sleep on slp cmd, HFINT 16MHz, INT OSC Blk
	OSCCON =  0b01110011; 					
    // PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCCON2 = 0b00000100;					
	
    OSCTUNE = 0x80;							// PLL disabled, Default factory freq tuning
	while (OSCCONbits.HFIOFS != 1);         // wait for osc to become stable
}

/**
 * Function Name: initializeButtonInterrupts
 * Parameters: None 
 * Return: None 
 * Description: Setting up interrupts for pushbuttons
 * 
 */
void initializeButtonInterrupts(void) {
    /**
     * Pushbutton interrupt configuration
     *    - 2 pushbuttons set up on RB0 and RB1
     *    - RB0 is INT0
     *    - RB1 is INT1
     */

    // Ensures interrupt flags are cleared
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;

    // Enable interrupts on INT0 and INT1
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;

    // Sets interrupt priority bits. Note INT0 is always high priority. 0 = Low priority, 1 = High priority
    INTCON3bits.INT1IP = 0;

    // Sets interrupts on rising edge for 1, falling edge for 0
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;
}
/**
 * Function Name: initializeTimer
 * Parameters: None 
 * Return: None 
 * Description: Sets up TMR0
 *      - Timer interrupt frequency = _XTAL_FREQ * XTAL_TIMER_SF / ( 4 * pre-scale value * data register size )
 * Where:
 *      - pre-scale value is set in TxPS bits
 *      - data register size is 256 for 8-bit or 65536 for 16-bit
 *      - time between interrupts = 1 / timer interrupt frequency
 * 
 */

void initializeTimers(void) {
    /**
     * Timer Initialization
     */
    T0CONbits.T08BIT = 1; // 1: 8-bit, 0: 16-bit mode
    T0CONbits.T0CS = 0; // Use internal clock
    T0CONbits.PSA = 0; // Assign pre-scaler to Timer0
    T0CONbits.T0PS = 0b111; // Pre-scale set to 1:256
    T0CONbits.TMR0ON = 1; // Enable Timer0
    
    INTCONbits.T0IF = 0; // Clear Timer0 interrupt flag
    //INTCONbits.TMR0IE = 1; // Enable Timer0 overflow interrupts
    INTCONbits.TMR0IE = 0; // Disable Timer0 overflow interrupts until SWITCH_0 is pressed
    TMR0H = 0; // Clear upper 8 bits
    TMR0L = 0; // Clear lower 8 bits
    
}