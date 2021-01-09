/* 
 * File:   lcd.c
 * Author: Michael Pillon
 *
 * Created on January 4, 2021, 5:51 PM
 */

#include "lcd.h"

void lcdInitialize(uint8_t dataBitMode, uint8_t numLines, uint8_t charFont) {
    // Configure LCD Pins as digital outputs
    LCD_DATA_PORT &= ~LCD_CTRL_MASK; //Initialize output Pins to zero
    LCD_CTRL_PORT &= ~LCD_DATA_MASK;
    LCD_DATA_TRIS = 0x00; //Set pins to output
    LCD_CTRL_TRIS = 0x00; 
    ANSELA = 0;
    ANSELE = 0;
    
    /*
     * Initialize LCD
     *  - As per data sheet specifications
     */
    __delay_ms(50); // LCD requires time to load after power is applied
    lcdWriteNibble(LCD_IR, 0x03);
    __delay_us(4500);
    lcdWriteNibble(LCD_IR, 0x03);
    __delay_us(150);
    lcdWriteNibble(LCD_IR, 0x03);
    __delay_us(LCD_EXECUTION_DELAY);
    // Set bit mode. If 4 bit mode, only send a nibble.
    if(dataBitMode == LCD_4_BIT_MODE) {
        lcdWriteNibble(LCD_IR, LCD_4_BIT_MODE >> 4); 
    } else {
        lcdWriteByte(LCD_IR, LCD_8_BIT_MODE);
    }
    __delay_us(LCD_EXECUTION_DELAY);
    
    /* 
     * LCD Setup
     */
    lcdFunctionSet(dataBitMode, numLines, charFont);
    lcdClearDisplay();
    lcdDisplayControl(LCD_DISPLAY_ENABLE, LCD_CURSOR_DISABLE, LCD_BLINK_DISABLE);
    lcdEntryMode(LCD_MOVE_INCREMENT, LCD_MOVE_ONLY_CURSOR);
}

void lcdWriteNibble(uint8_t regType, char nibble) {
    LCD_EN = 0; // Ensure Enable pin is low
    LCD_RS = regType; // Internal or data register
    LCD_RW = LCD_WRITE;
    // Write Command to Internal Registers
    LCD_D4 = (nibble & 0x01);
    LCD_D5 = (nibble & 0x02) >> 1;
    LCD_D6 = (nibble & 0x04) >> 2;
    LCD_D7 = (nibble & 0x08) >> 3;
    // Send Command
    LCD_EN = 1;
    __delay_us(LCD_PULSE_DELAY);
    LCD_EN = 0;
    __delay_us(LCD_PULSE_DELAY);
}

void lcdWriteByte(uint8_t regType, char byte) {
    char upperNibble = (byte & 0xF0) >> 4;
    char lowerNibble = byte & 0x0F;
    lcdWriteNibble(regType, upperNibble);
    lcdWriteNibble(regType, lowerNibble);
}

void lcdWriteChar(char data) {
    lcdWriteByte(LCD_DR, data);
}

void lcdWriteString(char *data) {
    lcdClearDisplay();
    
    int stringLength = strlen(data);
    
    for(int i = 0; i < stringLength; i++ ) {
        if(i >= LCD_COLS) {
            break;
        }
        lcdWriteChar(data[i]);
    }
    lcdSetCursor(1, 0);
    for( int i = LCD_COLS; i < stringLength; i++) {
        lcdWriteChar(data[i]);
    }
}

void lcdSetCursor(uint8_t row, uint8_t col) {
    if(row == 0) {
        lcdWriteByte(LCD_IR, LCD_ROW_1 + col);
    } else if(row == 1) {
        lcdWriteByte(LCD_IR, LCD_ROW_2 + col);
    } 
    __delay_us(LCD_EXECUTION_DELAY);
}

void lcdFunctionSet(uint8_t dataBitMode, uint8_t displayLines, uint8_t charFont) {
    uint8_t cmd = LCD_FUNCTION_SET | dataBitMode | displayLines | charFont;
    lcdWriteByte(LCD_IR, cmd);
    __delay_ms(LCD_EXECUTION_DELAY);
}

void lcdClearDisplay(void) {
    lcdWriteByte(LCD_IR, LCD_CLEAR); 
    __delay_ms(LCD_CLEAR_DELAY);
}

void lcdEntryMode(uint8_t moveDir, uint8_t cursorMovement) {
    uint8_t cmd = LCD_ENTRY_MODE | moveDir | cursorMovement;
    lcdWriteByte(LCD_IR, cmd); 
    __delay_us(LCD_EXECUTION_DELAY);
}
void lcdDisplayControl(uint8_t display, uint8_t cursor, uint8_t blink) {
    uint8_t cmd = LCD_DISPLAY_CONTROL | display | cursor | blink;
    lcdWriteByte(LCD_IR, cmd); 
    __delay_us(LCD_EXECUTION_DELAY);
}
void lcdCursorDisplayShift(uint8_t cursor, uint8_t displayShift) {
    uint8_t cmd = LCD_CURSOR_AND_DS | cursor | displayShift;
    lcdWriteByte(LCD_IR, cmd); 
    __delay_us(LCD_EXECUTION_DELAY);
}

