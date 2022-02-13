#include  "LPC17xx.h"                    
#include  "stdio.h"
#include  "variable.h"
#include  "spi.h"
#include  "lcd.h"


void clock_display( )
{
    hr1=((hour/10)+'0');
    hr2=((hour%10)+'0');
    mi1=((minute/10)+'0');
    mi2=((minute%10)+'0');
    sc1=((second/10)+'0');
    sc2=((second%10)+'0');
	time[0]=hr1;
	time[1]=hr2;
	time[2]=':';
	time[3]=mi1;
	time[4]=mi2;
	time[5]=':';
	time[6]=sc1;
	time[7]=sc2;
	time[8]=' ';
	if(a_flag == 1)
	{
		a_hr1=((alarm_hour/10)+'0');
    	a_hr2=((alarm_hour%10)+'0');
    	a_mi1=((alarm_minute/10)+'0');
    	a_mi2=((alarm_minute%10)+'0');
    	a_sc1=((alarm_second/10)+'0');
    	a_sc2=((alarm_second%10)+'0');
		a_time[0]=a_hr1;
		a_time[1]=a_hr2;
		a_time[2]=':';
		a_time[3]=a_mi1;
		a_time[4]=a_mi2;
		a_time[5]=':';
		a_time[6]=a_sc1;
		a_time[7]=a_sc2;
		a_time[8]=' ';
		lcd_disp(2,5,a_time);
	}	
	lcd_disp(1,5,time);
}

void TIMER0_IRQHandler (void)
{    
	if((LPC_TIM0->IR & 0x01)==0x01)
	{
		LPC_TIM0->IR |= 1 << 0; // Reset the counter
		second++;
		if (second==60)
		{
        		minute++;
        		second=0;
    		}
   		if (minute==60)
		{
        		hour++;
        		minute=0;
    		}
    		if (hour==24)
		{
        		hour=0;
    		}
	}
}  

void clock_init( )
{
	LPC_SC->PCONP |= 1 << 1; //Power up Timer0
	LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK
	LPC_TIM0->MR0 = 100000000; // 12MHz,generate 1 second
	LPC_TIM0->MCR |= 1 << 0; // Interrupt on Match0 compare
	LPC_TIM0->MCR |= 1 << 1; //reset Match0
	LPC_TIM0->TCR |= 1 << 1; // Reset Timer0
	LPC_TIM0->TCR &= ~(1 << 1); //The counters remain reset until TCR[1] is returned to zero,pg494
	NVIC_SetPriority(TIMER0_IRQn, 3);
	NVIC_EnableIRQ(TIMER0_IRQn); // Enable timer interrupt
	LPC_TIM0->TCR |= 1 << 0; // Start timer
}	
	

                
