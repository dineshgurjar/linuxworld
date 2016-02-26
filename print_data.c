#include"header.h"

void lcd_display(float a,float b)
{
          lcdcmd(0x80);
	  pstring("1 ");
          lcd_float(b);
	  pstring("v 2 ");
          lcd_integer(a);
	  lcdcmd(0xc0);
}

void lcd_display_time(const char *tbuff)
{
	lcdcmd(0xc0);
	pstring("Time ");
	pstring(tbuff);	
}

void uart_display(float a,float b)
{
          upstring("\r1 ");
          uart_float(b);
          upstring("v 2 ");
          uart_int(a);
         // upstring("\r\n");
          ms_delay(100);
}

void uart_display_time(const char *tbuff)
{
					upstring(" Time ");
					upstring(tbuff);
}