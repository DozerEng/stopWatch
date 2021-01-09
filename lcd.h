/* 
 * File    lcd.h
 * Author  Michael Pillon
 *
 * Created on January 4, 2021, 5:51 PM
 * Last Modified on
 * 
 * LCD control
 * 
 * Notes:
 *  - To configure lcd, see "Hardware Defines and Application specific defines"
 *  - 16 x 1 displays often use an 8 x 2 configuration 
 * 
 * ToDo: 
 *  - Configure data bits D4, D5, D6, and D7 so they can be used as inputs to read register values
 *      - Once completed, create functions to toggle specific bits instead of full registers
 *      - Remove delay timings and instead monitor BF (Busy Flag) on pin D7
 * 
 * References:
 * https://deepbluembedded.com/interfacing-16x2-lcd-with-pic-microcontrollers-mplab-xc8/
 * 
 */

#include <xc.h>
#include <stdint.h>
#include <string.h>

#include "xtalFreq.h"

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Hardware Defines and Application specific defines
 *  - Customize based on pins used for LCD
 *  - Customize other defines based on LCD being used
 */
#define LCD_CTRL_PORT       LATE
#define LCD_CTRL_TRIS       TRISE
#define LCD_CTRL_MASK       0x03    // Mask for control bits RS, RW, and EN
#define LCD_RS              LATEbits.LATE0
#define LCD_RW              LATEbits.LATE1
#define LCD_EN              LATEbits.LATE2
    
#define LCD_DATA_PORT       LATA
#define LCD_DATA_TRIS       TRISA
#define LCD_DATA_MASK       0x0F // Mask for data bits D4, D5, D6, and D7
#define LCD_D4              LATAbits.LATA0
#define LCD_D5              LATAbits.LATA1
#define LCD_D6              LATAbits.LATA2
#define LCD_D7              LATAbits.LATA3
    
#define LCD_ROWS         LCD_2_LINES // Number of rows and Columns
#define LCD_COLS         8

/**
 * LCD Defines
 *  - Customize based on specifications of LCD being used
 */
    
#define LCD_READ    1
#define LCD_WRITE   0

#define LCD_IR      0   //Internal Register
#define LCD_DR      1   //Data Register
    
#define LCD_ENABLE      1
#define LCD_DISABLE     0
    
#define LCD_PULSE_DELAY     5  // "Enable" pulse width in us
#define LCD_EXECUTION_DELAY 50  // Minimum execution delay in us

/**
 * Internal Register Configuration Bits
 */

// Function set constants
#define LCD_FUNCTION_SET    0b00100000
#define LCD_8_BIT_MODE      0b00010000
#define LCD_4_BIT_MODE      0b00000000
#define LCD_2_LINES         0b00001000
#define LCD_1_LINE          0b00000000
#define LCD_5x10_CHAR       0b00000100
#define LCD_5x8_CHAR        0b00000000
#define LCD_5x7_CHAR        0b00000000  
    
// Set cursor defines
#define LCD_ROW_1           0b10000000
#define LCD_ROW_2           0b11000000

// Clear display defines
#define LCD_CLEAR           0b00000001
#define LCD_CLEAR_DELAY     5   // Execution delay to clear display in ms

// Entry mode defines
#define LCD_ENTRY_MODE      0b00000100
#define LCD_MOVE_INCREMENT  0b00000010
#define LCD_MOVE_DECREMENT  0b00000000
#define LCD_ACCOMPANY_DS    0b00000001 // Cursor movement accompanies display shift
#define LCD_MOVE_ONLY_CURSOR 0b00000000
    
// Display control defines
#define LCD_DISPLAY_CONTROL 0b00001000
#define LCD_DISPLAY_ENABLE  0b00000100
#define LCD_DISPLAY_DISABLE 0b00000000
#define LCD_CURSOR_ENABLE   0b00000010
#define LCD_CURSOR_DISABLE  0b00000000
#define LCD_BLINK_ENABLE    0b00000001
#define LCD_BLINK_DISABLE   0b00000000
    
// Cursor and display shift defines
#define LCD_CURSOR_AND_DS   0b00010000 //Cursor and display shift 
#define LCD_DISPLAY_SHIFT   0b00001000
#define LCD_CURSOR_SHIFT    0b00000000
#define LCD_SHIFT_RIGHT     0b00000100
#define LCD_SHIFT_LEFT      0b00000000

/**
 * Function Declarations
 */

void lcdInitialize(uint8_t dataBitMode, uint8_t numLines, uint8_t charFont);

void lcdWriteNibble(uint8_t regType, char nibble);
void lcdWriteByte(uint8_t regType, char byte);

void lcdWriteChar(char data);
void lcdWriteString(char *data);

void lcdFunctionSet(uint8_t dataBitMode, uint8_t displayLines, uint8_t charFont);
void lcdClearDisplay(void);
void lcdSetCursor(uint8_t row, uint8_t col);

void lcdEntryMode(uint8_t moveDir, uint8_t cursorMovement);
void lcdDisplayControl(uint8_t displayOn, uint8_t cursorEn, uint8_t blinkEn);
void lcdCursorDisplayShift(uint8_t cursor, uint8_t displayShift);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

