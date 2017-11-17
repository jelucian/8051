// This program interface 8051 with a 16x2 line LCD. It used port 2 as data bus, 
// port as control bus, and displays Hello World!on the first display line
#include <reg51.h>

#define ldata P2
#define COMMAND 0
#define LCD_DATA 1

sbit rs = P3^6;
sbit rw = P3^5;
sbit en = P3^7;
sbit busy = P2^7; // connected to DB7 on the LCD

void init_lcd();
void write_to_lcd(unsigned char value, bit mode);
void MSDelay(unsigned int itime);
void lcdready(void);


void main(void)
{
	unsigned int count;
	
	char hundred = 0x30;
	char ten= 0x30;
	char one = 0x30;
	
  unsigned char code msg[]="Hello John";
	unsigned char code ctr[]="Counter:";
  unsigned char i=0;
  init_lcd();                                                                                       

	while (ctr[i]!='\0') 
   write_to_lcd(ctr[i++],LCD_DATA);
	
	
	for (count = 0; count <= 2000; count++){		
		if (one > 0x39){
			one = 0x30;
			ten++;
			if (ten > 0x39){
				ten = 0x30;
				hundred++;
				if (hundred > 0x39){
					hundred = 0x30;
				}
			}
		}
		
		//Constantly rewriting/updating all digits.
		//Note: Write MSB first. If LSB is written first, order is reversed. 00 -> 10 -> 20 -> 30
		write_to_lcd(hundred,LCD_DATA);
		write_to_lcd(ten,LCD_DATA);
		write_to_lcd(one,LCD_DATA);
		
		one++; //Incrementing least significant bit
		
		MSDelay(10);
		write_to_lcd(0x88,COMMAND); //Sending cursor to position 8 (Hundred's location)
	}
	
  while (1);
		
}


//  JOHN's Functions //


// setup LCD for the required display 
void init_lcd()
{
  // Function set format: 001 DL N F  * *
  // Function set value: 00111000
  // DL=1, use 8-bit data bus; N=1,1/16 duty(2 lines),
  // F=0, 5x7 dot character font 
  write_to_lcd(0x38,COMMAND); 
                              
  // Display On/Off Control format: 00001 D C B
  // Display On/Off Control value: 00001110
  // D=1, display on; C=1, cursor on; B=0, cursor blink off
  write_to_lcd(0x0E,COMMAND);

  // Entry mode set format: 000001 I/D S
  // Entry mode set value: 00000110
  // I/D=1, Increment cursor position; S=0, no display shift
  write_to_lcd(0x06,COMMAND);

  // Clear display and returns cursor to the home position(address 0) 
  write_to_lcd(0x01,COMMAND);
}

// write a command or a character to LCD
void write_to_lcd(unsigned char value, bit mode)
{
  lcdready();
  ldata = value; 
  rs = mode; // set for data, reset for command
  rw = 0;
  en = 1;
  MSDelay(1);
  en = 0;
}

void MSDelay(unsigned int itime)
{
   unsigned int i, j;

   for (i=0;i<itime;i++)
     for (j=0;j<1275;j++);
}

// wait for LCD to become ready
void lcdready(void)
{
  busy = 1;
  en = 1;
  rs = 0; // It's a command
  rw = 1; // It's a read command
  while (busy) {
    en = 0;
    MSDelay(1);
    en = 1;
  }
  en=0;
  rw=0;
}
