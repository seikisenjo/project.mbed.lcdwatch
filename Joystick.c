#include "lpc17xx.h"


//Define Input Pins */


#define left_Joystick (1 << 26) /*pin p1.26 */
#define right_Joystick (1 << 24) /*pin p1.24 */
#define up_Joystick (1 << 23) /*pin P1.23 */
#define down_Joystick (1 << 25) /*pin P1.25 */
#define select_Joystick (1 << 20) /*pin P1.20 */


/*Define Output Pins*/


#define LED1 (1 << 28) /*pin p1.28 */
#define LED2 (1 << 29) /*pin p1.29 */
#define LED3 (1 << 31) /*pin P1.31 */
#define LED4 (1 << 2) /*pin P2.2 */
#define LED5 (1 << 3) /*pin P2.3 */
#define LED6 (1 << 4) /*pin P2.4 */
#define LED7 (1 << 5) /*pin P2.5 */
#define LED8 (1 << 6) /*pin P2.6 */


// put function prototypes here if needed


int main (void)
{

SystemInit(); 


SC -> PCONP |= (1<<15);

PINCON -> PINSEL3 &= ((0<<20)|(0<<16)|(0<<14)|(0<<18)|(0<<8));

GPIO1 -> FIODIR &= ~(left_Joystick|right_Joystick|up_Joystick|down_Joystick|select_Joystick);

PINCON -> PINSEL3 &= ((0<<30)|(0<<29)|(0<<24));

//PINCON -> PINSEL4 &= ((0<<4)|(0<<6)|(0<<8)|(0<<10)|(0<<12));

GPIO1 ->FIODIR |= (LED1|LED2|LED3);

GPIO2 ->FIODIR |= (LED4|LED5|LED6|LED7|LED8);


GPIO1 -> FIOPIN &= ~(LED1|LED2|LED3);

GPIO2 -> FIOPIN &= ~(LED4|LED5|LED6|LED7|LED8);


while(1)
{

	if ((GPIO1->FIOPIN >> 26)&0x00000001 == 1) {GPIO1 -> FIOPIN &= ~(LED1|LED2);}	
	else {GPIO1 -> FIOPIN |= (LED1|LED2);};
	
	if ((GPIO1->FIOPIN >> 24)&0x00000001 == 1) {GPIO2 -> FIOPIN &= ~(LED7|LED8);}
	else {GPIO2 -> FIOPIN |= (LED7|LED8);};
																				
	if ((GPIO1->FIOPIN >> 23)&0x00000001 == 1) {GPIO2 -> FIOPIN &= ~(LED4|LED5);}
	else {GPIO2 -> FIOPIN |= (LED4|LED5);};

	if ((GPIO1->FIOPIN >> 25)&0x00000001 == 1) { GPIO1 -> FIOPIN &= ~(LED2|LED3); GPIO2 -> FIOPIN &= ~(LED6|LED7);}
	else {GPIO1 -> FIOPIN |= (LED2|LED3); GPIO2 -> FIOPIN |= (LED6|LED7);};

	if ((GPIO1->FIOPIN >> 20)&0x00000001 == 1) {GPIO1 -> FIOPIN &= ~(LED1|LED2|LED3); GPIO2 -> FIOPIN &= ~(LED4|LED5|LED6|LED7);}
	else {GPIO1 -> FIOPIN |= (LED1|LED2|LED3); GPIO2 -> FIOPIN |= (LED4|LED5|LED6|LED7);};

/*

uint32_t joystick_val = 0;

joystick_val = (GPIO->FIOPIN>>20) & 0x79;

if(joystick_val%10==1) {GPIO1 -> FIOPIN |= (LED1|LED2|LED3|); GPIO2 -> FIOPIN |=(LED4|LED5|LED6|LED7)}
else {GPIO1 -> FIOPIN &= ~(LED1|LED2|LED3|); GPIO2 -> FIOPIN &= ~(LED4|LED5|LED6|LED7)}



if ((GPIO1->FIOPIN >> 26)&0x00000001 == 0) {GPIO1 -> FIOPIN |= (LED1|LED2);}	
	else {GPIO1 -> FIOPIN &= ~(LED1|LED2);};
	
	if ((GPIO1->FIOPIN >> 24)&0x00000001 == 0) {GPIO2 -> FIOPIN |= (LED7|LED8);}
	else {GPIO2 -> FIOPIN &= ~(LED7|LED8);};
																				
	if ((GPIO1->FIOPIN >> 23)&0x00000001 == 0) {GPIO2 -> FIOPIN |= (LED4|LED5);}
	else {GPIO2 -> FIOPIN &= ~(LED4|LED5);};

	if ((GPIO1->FIOPIN >> 25)&0x00000001 == 0) 
			{ GPIO1 -> FIOPIN |= (LED2|LED3); GPIO2 -> FIOPIN |=(LED6|LED7);}
	else {GPIO1 -> FIOPIN |= ~(LED2|LED3); GPIO2 -> FIOPIN &= ~(LED6|LED7);};

	if ((GPIO1->FIOPIN >> 20)&0x00000001 == 0) {GPIO1 -> FIOPIN |= (LED1|LED2|LED3); GPIO2 -> FIOPIN |=(LED4|LED5|LED6|LED7);}
	else {GPIO1 -> FIOPIN &= ~(LED1|LED2|LED3); GPIO2 -> FIOPIN &= ~(LED4|LED5|LED6|LED7);};

*/
	



}}