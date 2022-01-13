#include  "LPC17xx.h"
#include  <stdio.h>
#include  "clock.h"
#include  "variable.h"
#include  "lcd.h"


#define PCONP           (*(volatile unsigned long *)(0x400FC0C4))
#define PINSEL1         (*(volatile unsigned long *)(0x4002C004))
#define PINSEL3			(*(volatile unsigned long *)(0x4002C00C))
#define PINSEL4         (*(volatile unsigned long *)(0x4002C010))
#define FIO1DIR			(*(volatile unsigned long *)(0x2009C020))
#define FIO2DIR         (*(volatile unsigned long *)(0x2009C040))
#define IO2IntEnF       (*(volatile unsigned long *)(0x400280B4))
#define IO2IntClr       (*(volatile unsigned long *)(0x400280AC))
#define FIO1PIN         (*(volatile unsigned long *)(0x2009C034)) 
#define FIO2PIN 		(*(volatile unsigned long *)(0x2009C054))
#define EXTINT			(*(volatile unsigned long *)(0x400FC140))
#define AD0CR           (*(volatile unsigned long *)(0x40034000))
#define AD0GDR          (*(volatile unsigned long *)(0x40034004))
#define AD0INTEN        (*(volatile unsigned long *)(0x4003400C))
#define ADSTAT	        (*(volatile unsigned long *)(0x40034030))

int position=0;
int alarm_position = 3;
int ADCdata = 0;
int DigOutData = 0;

void SysTick_Handler (void) 
{ 					/* SysTick Interrupt Handler (1ms) */
	SysTickCnt++;
}

void Delay (unsigned long tick) 
{ /* Delay Function */
	unsigned long systickcnt;
	systickcnt = SysTickCnt;
	while ((SysTickCnt - systickcnt) < tick);
}

//int getPinState (int pinNumber)
//{
//     int pinState;
//     int pinBlockState = FIO1PIN ;
//     pinState = (pinBlockState & (1 << pinNumber)) ? 1 : 0;
//     return pinState;
//}

//void adjust_hour( )
//{
//	while(getPinState(10))
//	{
//    	if (!(getPinState(23))) 
//		{
//        	hour++;
//			if (hour==24)
//			{
//				hour=0;
//			}
//        clock_display( );
//    	}
//		if (!(getPinState(26))) 
//		{
//			break;
//		}
//	}
//}

//void adjust_minute( )
//{
//	while(getPinState(10))
//	{
//    	if (!(getPinState(23))) 
//		{
//        	minute++;
//			if (minute==60)
//			{
//					minute=0;
//			}
//       		clock_display( );
//    	}
//		if (!(getPinState(26))) 
//		{
//			break;
//		}
//	}
//}

//void adjust_second( )
//{
//	while(getPinState(10))
//	{
//    	if (!(getPinState(23))) 
//		{
//        	second++;
//			if (second==60)
//			{
//				second=0;
//			}
//        clock_display( );
//    	}
//		if (!(getPinState(26))) 
//		{
//			break;
//		}
//	}
//}

void light(void)
{
	if (ADCdata>200)
      		DigOutData=(DigOutData|0x04);  //set the bit-2		
    	if (ADCdata>400)
      		DigOutData=(DigOutData|0x08);  //set the bit-3
    	if (ADCdata>600)
      		DigOutData=(DigOutData|0x10);  //set the bit-4
    	if (ADCdata>800)
      		DigOutData=(DigOutData|0x20);  //set the bit-5
    	if (ADCdata>1000)
     		DigOutData=(DigOutData|0x40);  //set the bit-6
	FIO2PIN = 0x00;
	FIO2PIN = DigOutData;        // light up LED(s)
	//AD0CR |= (1<<24);
}


void button_init( )
{
    PCONP |= (1 << 15);
	PCONP |= (1 << 12); //adc power
	PINSEL1=0x00040000;	  //channel 2  
	PINSEL3 = 0x00000000;
    PINSEL4 = 0x00500000;		  //EINT0 and EINT1
	FIO1DIR &= ~((1<<23)|(1<<26)|(1<<24)|(1<<25)|(1<<20)) ;
	FIO1DIR |= (1<<28);
	FIO1PIN &= ~(1<<28);
	//FIO1DIR |= (1<<28);
    FIO2DIR &= ~((1<<10)|(1<<11));  
	FIO2DIR |= ((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6));   
	AD0CR = 0x00000000;
	AD0CR |=      ((1 << 2)	// select channel 2  
           	     | (4 << 8)       	// Divide incoming clock by (4+1), which is 5MHz
                 | (0 << 16)      	// BURST = 0, conversions under software control  
                 | (1 << 21)      	// PDN = 1, enables ADC but conversion has not started yet
                 | (1 << 24));     	// START = 1, start A/D conversion now 
	//FIO1PIN &= ~(1<<28);
    IO2IntEnF |= (1 << 10);  // falling edge of P2.10 
	IO2IntEnF |= (1 << 11);
	AD0INTEN   |= (1 << 2);
    NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_SetPriority(ADC_IRQn, 5);
    NVIC_SetPriority(EINT0_IRQn, 2);  //set time
	NVIC_SetPriority(EINT1_IRQn, 4);  //alarm time
}

void button_press(int input) 
{		
	lcd_clear();
	clock_display();
	sel[0] = '*';
	sel[1] = '*';
	switch (input) 
    {
       	case 0:	lcd_disp(2,9,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1)) 	 //up_joystick
	     			second += 1;
	     		if(second > 60)
					second = 0;
       	    	break;

        case 1:	lcd_disp(2,7,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1))
				{
	     			minute += 1;
	     			if(minute > 60)
						minute = 0;
				}
       	    	break;

        case 2:	lcd_disp(2,5,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1))
	     			hour += 1;
	     		if(hour > 24)
					hour = 0;
       	    	break;
		case 3:	lcd_disp(3,9,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1))
	     			alarm_second += 1;
	     		if(alarm_second > 60)
					alarm_second = 0;
       	    	break;
		case 4:	lcd_disp(3,7,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1))
	     			alarm_minute += 1;
	     		if(alarm_minute > 60)
					alarm_minute = 0;
       	    	break;
		case 5:	lcd_disp(3,5,sel);
				if(!((FIO1PIN >> 23) & 0x01 == 1))
	     			alarm_hour += 1;
	     		if(alarm_hour > 24)
					alarm_hour = 0;
       	    	break;
	}
    //clock_display( );
}

//void EINT0_IRQHandler(void)
//{
//	FIO1PIN |= (1<<28);
//	flag_interrupt++;
//    while((getPinState(10)) && (flag_interrupt%2 == 1))
//	{
//  		if (!(getPinState(26)))       //left P1.16
//   	 	{	
//			counter++;
//       	 	if (counter>3)
//			{
//         		counter=1;
//          	}
//		}
//        switch(counter)
//		{
//           	case 1: adjust_hour( );
//           			break;
//           	case 2: adjust_minute( ); 
//                   	break;  
//            case 3: adjust_second( );
//                   	break;
//        } 
//    }
// 	IO2IntClr |= 1 << 10;
//	NVIC_ClearPendingIRQ(TIMER0_IRQn);
//	clock_init( );
//}

void EINT0_IRQHandler(void)
{
	//flag_interrupt++;
	//((FIO2PIN >>24) & 0x01 ==1) && (flag_interrupt%2 == 1)
	while(((FIO1PIN >>24) & 0x01 == 1))//right_joystick
	{
		if(!((FIO1PIN >> 26) & 0x01 == 1)) //left joystick
		{
			position++; 
			if(position>2)
				position = 0;
		}
		
		button_press(position) ;
	}
	//IO2IntClr |= (1<<10);
	EXTINT = 0x00000001;
	//IO2IntEnF &= ~(1<<10);
	NVIC_ClearPendingIRQ(TIMER0_IRQn); //clear timer0 pending interrupt
	clock_init( ); //reset timer0
	lcd_clear();
}

void EINT1_IRQHandler(void)
{
	a_flag = 1;
	clock_display();
	//flag_interrupt++;
	//((FIO2PIN >>24) & 0x01 ==1) && (flag_interrupt%2 == 1)
	while(((FIO1PIN >>25) & 0x01 == 1))//down_joystick
	{
		if(!((FIO1PIN >> 20) & 0x01 == 1)) //select_joystick
		{
			alarm_position++; 
			if(alarm_position>5)
				alarm_position = 3;
		}
		
		button_press(alarm_position) ;
	}
	//IO2IntClr |= (1<<11);
	EXTINT = 0x00000002;
	//NVIC_ClearPendingIRQ(TIMER0_IRQn); //clear timer0 pending interrupt
	//clock_init( ); //reset timer0
	a_flag = 0;
	alarm_set = 1;
	alarm_done = 0;
	lcd_clear();
}

void ADC_IRQHandler(void)
{
	ADCdata = AD0GDR;
	//AD0CR &= 0xF8FFFFFF;
	ADCdata=(ADCdata>>6)&0x03FF;
	light();
	//NVIC_DisableIRQ(ADC_IRQn);
	//ADINT;
	ADSTAT = 0x00000000;
}


void alarm_check(void)
{
	if((hour==alarm_hour) && (minute == alarm_minute) )
	{
		FIO1PIN |= (1<<28);
		alarm_done = 1;
		Delay(5000);
		FIO1PIN &= ~(1<<28);
	}
}
