//============================================================================
// Display is Crystalfontz CFAO12864D3/CFAG12864Q1
//   https://www.crystalfontz.com/product/cfao12864d3tmi
//   https://www.crystalfontz.com/product/cfag12864q1tfh
//
// The controller is a Sitronix ST7567
//   http://www.crystalfontz.com/controllers/Sitronix/ST7567/
//
// Breakout board is Crystalfontz PCB 10072
//   https://www.crystalfontz.com/product/cfag12864q1tfhe1
//
//============================================================================

#include <SPI.h>
#include "LCD_low.h"
#include "LCD_draw.h"

//============================================================================
//
// LCD SPI & backlight control lines
//   ARD   | Port | LCD
// #10/D10 |  PB2 | LCD_CS_NOT
// #11/D11 |  PB3 | LCD_MOSI (hardware SPI)
// #12/D12 |  PB4 | LCD_BL
// #13/D13 |  PB6 | LCD_SCK (hardware SPI)
#define CLR_CS    (PORTB &= ~(0x04))
#define SET_CS    (PORTB |=  (0x04))
#define CLR_BL    (PORTB &= ~(0x10))
#define SET_BL    (PORTB |=  (0x10))
//
// LCD control lines
//   ARD  | Port | LCD
// #15/A1 |  PC1 | LCD_RES_NOT
// #17/A3 |  PC3 | LCD_A0
//
#define CLR_RESET (PORTC &= ~(0x02))
#define SET_RESET (PORTC |=  (0x02))
#define CLR_A0    (PORTC &= ~(0x08))
#define SET_A0    (PORTC |=  (0x08))

//----------------------------------------------------------------------------

void lcd_init()
  {
  //For this demonstration, the display can just be selected forever.
  //If your hardware shares the data bus (nothing wrong with that) then
  //you will want to manage the CS line.
  CLR_CS;
  //For this simple demonstration code, we never read the LCD, so we
  //will just park the read line high. If you want to read the LCD, you
  //would want to control the RD line.
  //
  // This initialization code is based on the ST7567 Ver1.4b datasheet
  // Page 29 of:
  //   http://www.crystalfontz.com/controllers/Sitronix/ST7567/
  //Hardware reset pulse
  CLR_RESET;
  //"Wait for the power stabilized"
  _delay_ms(50);
  SET_RESET;
  //Wait for reset circuit finished (<1ms)
  _delay_ms(1);
  //The display is now initialized with its power-on defaults.
  // Enter Power Save (Compound Command)
  //  1: Display Off 0xAE
  //  2: Display All Points ON 0xA5
  // Now we are in sleep mode
  SPI_sendCommand(0xAE);
  SPI_sendCommand(0xA5);
  // LCD bias setting
  //  0xA2 = 1/9 bias @ 1/65 duty
  //  0xA3 = 1/7 bias @ 1/65 duty
  SPI_sendCommand(0xA2);
  // ADC Select (Segment Driver Direction Select)
  // (flips display R<>L)
  //  0xA0 = Normal (for our LCD glass layout)
  //  0xA1 = Reverse (for our LCD glass layout)
  SPI_sendCommand(0xA0);
  // Common Output Mode Select
  // (flips display T<>B)
  //  0xC0 = Normal (for our LCD glass layout)
  //  0xC8 = Reverse (for our LCD glass layout)
  SPI_sendCommand(0xC0);
  // V0 Voltage Regulator Internal Resistor Ratio Set
  // (essentially the gain of the internal op-amp, you can think
  //  of it as setting the range over which the contrast can be
  //  adjusted.)
  //  0x20 = Lower voltage, lighter contrast
  //   . . .
  //  0x27 = Higher voltage, darker contrast
  //
  // A setting of 0x26 seems about right for this LCD
  SPI_sendCommand(0x26);
  // The Electronic Volume (Double Byte Command)
  // This is the command to set the contrast. For our LCD
  // and a resistor ratio of 6, a setting of 31 is just about
  // perfect.
  // The range that this command can control is determined
  // by the resistor ratio command immediately above.
  SPI_sendCommand(0x81);  //The Electronic Volume Mode Set
  SPI_sendCommand(31);    //Electronic Volume Register Set
  // The Booster Ratio (Double Byte Command)
  // This ratio much match the hardware capacitor configuration.
  // Our demo board uses 4x configuration.
  SPI_sendCommand(0xF8);  //Booster Ratio Select Mode Set
  SPI_sendCommand(0x00);  //Booster Ratio Register Set: 00 = 2x,3x,4x
  //Exit Power Save (Compound Command)
  // 1: Display On 0xAF
  // 2: Display All Points OFF 0xA4
  // Now we are not in sleep mode
  SPI_sendCommand(0xAF);
  SPI_sendCommand(0xA4);
  // Power Controller Set
  // This command must match the hardware. Our demo board
  // depends on Boost, regulator and follower all enabled.  
  SPI_sendCommand(0x2F);
  }



//============================================================================
void setup()
  {
  //Set the port directions
  // LCD SPI & backlight control lines
  //   ARD   | Port | LCD
  // #10/D10 |  PB2 | LCD_CS_NOT
  // #11/D11 |  PB3 | LCD_MOSI (hardware SPI)
  // #12/D12 |  PB4 | LCD_BL
  // #13/D13 |  PB6 | LCD_SCK (hardware SPI)
  DDRB |= 0x5C;
  
  //
  // LCD control lines
  //   ARD  | Port | LCD
  // #15/A1 |  PC1 | LCD_RES_NOT
  // #17/A3 |  PC3 | LCD_A0
  //
  DDRC |= 0x0A;
  
  //Drive the ports to a reasonable starting state.
  CLR_CS;
  SET_RESET;
  CLR_A0;

  
  
  // initialize SPI. By default the clock is 4MHz. The chip is good to 10 MHz
  SPI.begin();  
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
   //Initialize the LCD controller
  lcd_init();
  randomSeed(analogRead(0));
  }
//============================================================================

void loop()
  {
  uint8_t
    i;
  uint8_t
    j;
  const char *alphabet = "ABCDEFGHIJKLMNOPRSTW";
  const char *time = "9876543210";
  //Turn the backlight on
  SET_BL;

    
    //Clear
    Fill_Framebuffer(0x00, 0x00);    
    delay(1000); 
    //Filled
    Fill_Framebuffer(0xFF,0xFF);    
    delay(1000); 

    Fill_Framebuffer(0x00, 0x00);
    //====================================================
    //6 sided die chooser
    // for(i=0;i<random(20);i++)
    //   {
    //   put_string(60,
    //             24,
    //             1,
    //             time+3+random(6));
    //   delay(200);
    //   }
    // delay(500);

    // //====================================================
    //Scattegories Letter chooser
    for(i=0;i<random(20);i++)
      {
      put_string(60,
                24,
                1,
                alphabet+random(19));
      delay(200);
      }
    delay(500);
    //========================================================
    // 99 second count down timer
    for(i=0;i<10;i++)
        {
         put_string(56,
             0,
             1,
             time+i); 
          for(j=0; j<10;j++)
          {

           put_string(64,
             0,
             1,
             time+j); 
            delay(1000);
          }

        }

    //Filled
    Fill_Framebuffer(0xFF,0xFF);    
    
        put_string(20,
           24,
           10,
           "time's up!"); 
    
    CLR_BL;
    delay(200);
    SET_BL;
    delay(200);
    CLR_BL;
    delay(200);
    SET_BL;
    delay(200);
    CLR_BL;
    delay(200);
    SET_BL;
    _delay_ms(5000);
    
 
  } // void loop()
//============================================================================

