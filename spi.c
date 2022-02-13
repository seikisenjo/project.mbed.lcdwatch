/* Library - SPI initialization to provide serial communication 
 * to control mbed Application Board's C12832 LCD
 *
 * by Ahmad Zaid bin Noh
 * Elvira Systems Sdn Bhd
 *
 * The Software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement.
 * 15.11.2014  initial design
 */

#include  "LPC17xx.h"                       // Include this as part of CMSIS

// Initialize SPI
void    spi_init( ) {
    LPC_SC->PCONP |= 1 <<10;            // Enable POWER to SSP1 
    LPC_SC->PCLKSEL0 |= 1<<20;      // Set Peripheral Clock Selection for SSP1 pclk = 12MHz
    /* Add code 6 here */                   // Set Clock Prescale to divide SSP1 clock by 6 = 2MHz
    /* Add code 7 here */               // Set clock phase (CPOL=1 and CPHA=1)
    /* Add code 8 here */               // Set SSP1 as 8 bits data transfer (DSS)
    /* Add code 9 here */                 // Enable SSP1
    LPC_SSP1->CPSR |=6;
    LPC_SSP1->CR0 |=3<<6;
    LPC_SSP1->CR0 |=7<<0;
    LPC_SSP1->CR1 |=1<<1;
}
