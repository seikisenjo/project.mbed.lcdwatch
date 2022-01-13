//		   #include "lpc17xx.h"
		   void delay(void);

		   #define PINSEL1 (*(volatile unsigned long *)(0x4002C004))
		   #define PCONP (*(volatile unsigned long *)(0x400FC0C4))
		   #define AD0CR (*(volatile unsigned long *)(0x40034000))
		   #define AD0GDR (*(volatile unsigned long *)(0x40034004))
		   #define FIO2DIR0 (*(volatile unsigned char *)(0x2009C040))
		   #define FIO2PIN0 (*(volatile unsigned char *)(0x2009C054))
		  
		   int main() 
		   {
//		   		unsinged long ADCdata;
				unsigned long value;
				FIO2DIR0 |= 0xFF;
				PINSEL1 &= ~(3<<18);
				PINSEL1 |= (1<<18);
				PCONP |= (1<<12);
				PCONP |= (1<<15);
				AD0CR = (1<<2)|(4<<8)|(1<<21);
				AD0CR |= (1<<24);
//				while (AD0GDR & 0X80000000 == 0);
//				value = (AD0GDR>>4) & 0xFFF;

				while(1)
				{
				AD0CR = AD0CR | 0x01000000;
				while ((AD0GDR & 0x80000000)==0)
				{
				value = AD0GDR;
				AD0CR &= 0xF8FFFFFF;
				value=(value>>6)&0x03FF;
				if(value>200)
				{
				FIO2PIN0 &= 0x00;
				FIO2PIN0 |= 0x04;
				}
				if(value>400)
				{
				FIO2PIN0 &= 0x00;
				FIO2PIN0 |= 0x0C;
				}
				if(value>600)
				{
				FIO2PIN0 &= 0x00;
				FIO2PIN0 |= 0x1C;
				}
				if(value>800)
				{
				FIO2PIN0 &= 0x00;
				FIO2PIN0 |= 0x2C;
				}
				if(value>1000)
				{
				FIO2PIN0 &= 0x00;
				FIO2PIN0 |= 0x7C;
				}
				}
				}
			}


				


		   