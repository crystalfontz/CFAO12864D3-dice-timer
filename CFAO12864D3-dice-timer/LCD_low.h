#ifndef __LCD_LOW_H__
#define __LCD_LOW_H__
//============================================================================

#define CLR_CS    (PORTB &= ~(0x04))
#define SET_CS    (PORTB |=  (0x04))
#define CLR_BL    (PORTB &= ~(0x10))
#define SET_BL    (PORTB |=  (0x10))
//
//
#define CLR_RESET (PORTC &= ~(0x02))
#define SET_RESET (PORTC |=  (0x02))
#define CLR_A0    (PORTC &= ~(0x08))
#define SET_A0    (PORTC |=  (0x08))


#define HRES 64
#define VRES 128

//============================================================================
void SPI_sendCommand(uint8_t command);
void SPI_sendData(uint8_t data);
void Set_Address(uint8_t column, uint8_t page);
void Set_Contrast(uint8_t contrast);
//============================================================================
#endif //  __LCD_LOW_H__
