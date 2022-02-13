/* Library for the mbed Application Board's  C12832 LCD
 * 
 * by Ahmad Zaid bin Noh
 * Elvira Systems Sdn Bhd
 *
 * The Software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 * 15.11.2014  initial design
 */
 
#include  "LPC17xx.h"						// Include this as part of CMSIS
#include  "stdio.h"
#include  "small_font.h"

// Global Variables
	unsigned char current_lcd_line;	// LCD line or page

// Setting up the LCD Interfaces
void lcd_interface( ){
	LPC_GPIO0->FIODIR |= (1<<18); 			// Set Output chip select (CS) (P0.18)
	LPC_GPIO0->FIODIR |= (1<<8);				// Set Output Reset (P0.8)
	LPC_GPIO0->FIODIR |= (1<<6);				// Set Output A0 (P0.6)
	
	LPC_PINCON->PINSEL0 |=(2<<14);			// Set Output SCLK1, Alternate Function 2 (P0.7)
	LPC_PINCON->PINSEL0 |= (2<<18);			// Set Output MOSI1, Alternate Function 2 (P0.9)	
																				
	LPC_GPIO0->FIOPIN |= ((1<<6)|(1<<8)|(1<<18));				// Initialise CS, Reset and A0 to high 
}

// Transfer data to LCD - MOSI and SCK operation
void	spi_lcd_tfr(char mosi_data) {	
	/* Add codes 10 here */
	LPC_GPIO0->FIOCLR = 1<<18;
	LPC_SSP1 ->DR= mosi_data;
	while((LPC_SSP1->SR & (1<<4))!=0);
	LPC_GPIO0->FIOSET = 1<<18;
}	 

// Write Command to LCD controller
void	wr_cmd(char cmd) {
	/* Add codes 11 here */
	LPC_GPIO0->FIOCLR = 1<<6;
	spi_lcd_tfr(cmd);
	LPC_GPIO0->FIOSET = 1<<6;
} 

// Write Data to LCD
void wr_dat(char dat) {
	spi_lcd_tfr(dat);								// Transfer 8 bit data through SPI/SSP1	
}

void 	delay_us(int dd ){
	int i;
	dd= 3*dd;
	for (i=0; i< dd; i++);
}

void 	delay_ms(int dd ){
	int i;
	for (i=0; i< dd; i++) delay_us(1000);
}

// Reset LCD 
void lcd_reset( ){
	/* Add code 12 here */			 	// Reset set to LOW
	LPC_GPIO0->FIOCLR = 1<<8; 
	delay_us(50);
	LPC_GPIO0->FIOSET = 1<<8; 	// release Reset
	delay_ms(5);
}

// Initialize LCD
void 	lcd_init( ){
	lcd_interface( ); // Set up LCD Interface		
	lcd_reset( );  	// Reset LCD 
	// Initialize LCD Controller sequence
	wr_cmd(0xAE); 	// display off
	wr_cmd(0xA2); 	// bias voltage
	wr_cmd(0xA0);	  // sets the display RAM address SEG output normal
	wr_cmd(0xC8); 	// column normal
	wr_cmd(0x22); 	// voltage resistor ratio
	wr_cmd(0x2F); 	// power on
	wr_cmd(0x40); 	// start line = 0
	wr_cmd(0xAF); 	// display ON
	wr_cmd(0x81); 	// set contrast
	wr_cmd(0x17); 	// set contrast
	wr_cmd(0xA6); 	// display normal 
} 

// Locate the LCD's cursor
char lcd_locate(char line, char col){
	if (line > 3) line = 0; 			// out of range errors
	if (col > 127) col = 0;
	wr_cmd(0x00 + (col & 0x0F));	// set column low nibble 0
	wr_cmd(0x10 + ((col & 0x70)>>4)); 	// set column hi nibble 0
	wr_cmd(0xB0 + line); 				 	// set page address 0
	current_lcd_line = line;
	return current_lcd_line;
}

// Clear LCD Screen
void	lcd_clear( ){
	int i,j;
	for (i=0;i<4;i++){
		lcd_locate(i,0);
		for (j=0; j<128; j++) wr_dat(0x00);	//clear all pixels
	}
	lcd_locate(0,0);	
} 

// Write character to LCD
int 	write_char(unsigned char ch){
	int i, j, k;	
	//j = (ch - (' ')) * 8;			// Adjust Starting Char 'Space' = 0x20
	j = (ch - (0x20)) * 8;		// or use this 'Space' = 0x20	
	
	if (ch == 0x20){	 				// If Char is 'space'
		for (k=0; k<4; k++) 
			wr_dat(0x00);					// Set space bar = 4 columns
	}
	
	else {
		for (i=0; i<8; i++) {
			if (Small_font[j] == 0x00)	
				j++;									// Don't write to column if data is 0x00
			else { 
				wr_dat(Small_font[j]);// Write data to column
				j++; } }	
		wr_dat(0x00);							// Set 1 column space between Char
	}	
	return ch;
}

// Write string to LCD
void	lcd_disp(int line, int col, char * s) {
	lcd_locate(line,8*col);					// Locate the LCD cursor
	while (*s)
	write_char(*s++);								// Write character until end of string
}

/******************************************************************************/
/* 'Retarget' layer for target-dependent low level functions      						*/
/******************************************************************************/
/* Edited by Ahmad Zaid Noh from file Retarget.c															*/
/* Retarget.c template available at C:\Keil\ARM\Startup												*/
/* Modified due to printf() calls fputc()																			*/
/* NOTE: You must define fputc() so it calls function that will output  			*/
/*       a single character to the LCD, ie: write_char( )											*/
/******************************************************************************/

struct __FILE { int handle;};
FILE __stdout;
FILE __stdin;

int fputc(int value, FILE *f){
	unsigned int next_line;
	
  if (value == '\n'){     // new line
		if (current_lcd_line == 3)
			lcd_locate(0,0);
		else if (current_lcd_line < 3){
			next_line = current_lcd_line + 1;
			lcd_locate(next_line, 0);
		}
	}
	else {
    write_char(value);		// retarget to function write_char( )
  }		
  return value;
}
