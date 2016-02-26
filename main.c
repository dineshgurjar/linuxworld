/* VECTOR Final Project */
/* LAN Based Data Acquisition System */

#include"header.h"

void lcd_display(float a,float b);
void lcd_display_time(const char *tbuff);
void uart_display_time(const char *tbuff);
void uart_display(float a,float b);

float a,b;

main()
{
	unsigned char temp1=0,tbuff[16];
	float a,b;
	
	lcdinit();	
	uartinit();
	rtcinit();
	upstring("Data in UART\n\r");

  while(1)
  {
		 
			strcpy(tbuff,rtcread());
    
  		a=read_adc(1);
     	b=read_adc(0);
		
		  lcd_display(a,b);
		  lcd_display_time(tbuff);
		  
		  uart_display_time(tbuff);
  		uart_display(a,b);
			// uart_tx('\r');
  }


//	  rtcread();
/*
		if(temp1==0)//ch0
		{
			rtcread();
			
			lcdcmd(0x80);
			lcddata('1');
			lcddata(' ');
			b=read_adc(0);
			lcd_float(b);
			lcddata('v');

			uart_tx('1');
			uart_tx(' ');
			uart_float(b);
			uart_tx('v');
			upstring("  ");
			ms_delay(100);

//			lcdcmd(0x86);
//			lcddata(' ');
		}
		else if(temp1==1)
		{
			lcdcmd(0x89);
			lcddata('2');
			lcddata(' ');
			a=read_adc(1);
			lcd_integer(a);
			uart_tx('2');
			uart_tx(' ');
			uart_int(a);
			upstring("\r");
			ms_delay(100);
		}
		temp1++;
		if(temp1>1) temp1=0;
		
	} */
}
