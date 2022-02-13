
// Program to Display LCD on MBED Application Board
#include  "LPC17xx.h"                      
#include  <stdio.h>
#include  "spi.h"
#include  "lcd.h"
#include  "variable.h"
#include  "clock.h"
#include  "adjust.h"
       
int main() 
{    
    SystemInit( );
    LPC_SC->PCONP |= ( 1 << 15 ); // power up GPIO
    spi_init( );    // Initialise SPI
    lcd_init( );    // Initialise LCD
    lcd_clear( );   // Clear LCD Screen
    clock_init( );  // enable interrupt
	button_init( );
	while(1)
	{
    	clock_display( );
		if((alarm_set == 1) && (alarm_done == 0))
		alarm_check();
	}
}    
