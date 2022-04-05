
// 2017 - 06 - 15 Brent A. Crosby
// 2022- 04 - 04 Kelsey Zaches
// Crystalfontz America, Inc. http://www.crystalfontz.com
//============================================================================
//  This is free and unencumbered software released into the public domain.
//
//  Anyone is free to copy, modify, publish, use, compile, sell, or
//  distribute this software, either in source code form or as a compiled
//  binary, for any purpose, commercial or non-commercial, and by any
//  means.
//
//  In jurisdictions that recognize copyright laws, the author or authors
//  of this software dedicate any and all copyright interest in the
//  software to the public domain. We make this dedication for the benefit
//  of the public at large and to the detriment of our heirs and
//  successors. We intend this dedication to be an overt act of
//  relinquishment in perpetuity of all present and future rights to this
//  software under copyright law.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.
//
//  For more information, please refer to <http://unlicense.org/>
//===========================================================================
#include <Arduino.h>
#include <SPI.h>
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.cpp
// C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\SPI\src\SPI.h
#include "LCD_low.h"


//===========================================================================
void SPI_sendCommand(uint8_t command)
  {
  // Select the LCD's command register
  CLR_A0;
  // Select the LCD controller
  CLR_CS;
  //Send the command via SPI:
  SPI.transfer(command);
  // Deselect the LCD controller
  SET_CS;
  }
//============================================================================
void SPI_sendData(uint8_t data)
  {
  // Select the LCD's data register
  SET_A0;
  // Select the LCD controller
  CLR_CS;
  //Send the data via SPI:
  SPI.transfer(data);
  // Deselect the LCD controller
  SET_CS;
  }
//============================================================================
void Set_Address(uint8_t column, uint8_t page)
  {

  //Set column-lower nibble
  SPI_sendCommand(0b00010000 | (column >> 4));
  //Set column-upper nibble
  SPI_sendCommand(0b00000000 | (column & 0b1111));
  //Set page address, limiting from 0 to 7
  SPI_sendCommand(0b10110000 | (page & 0b1111));
  }
//============================================================================
void Set_Contrast(uint8_t contrast)
  {
  // Sets the contrast, 0 = light, 63 = dark, 31 = about right.
  // The Electronic Volume (Double Byte Command)
  // This is the command to set the contrast.
  // The range that this command can control is determined
  // by the resistor ratio command immediately above.
  SPI_sendCommand(0x81);  //The Electronic Volume Mode Set
  SPI_sendCommand(contrast & 0x3F);    //Electronic Volume Register Set
  }
//============================================================================

