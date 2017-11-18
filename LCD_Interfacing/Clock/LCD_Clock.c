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
	
	//Initializing every decimal place to 0
	char msbHour = 0x30;
	char lsbHour = 0x30;
	char msbMinute = 0x30;
	char lsbMinute = 0x30;
	bit meridian = 1;
	
	unsigned char code time[]="Time ";
  unsigned char i=0;
  init_lcd();                                                                                       

	while (time[i]!='\0') 
   write_to_lcd(time[i++],LCD_DATA);
	
	
	for (count = 0; count <= 99999999; count++){		
		//If lsb minute exceeds 9, reset it to 0 and increment msb minute
		if (lsbMinute > 0x39){
			lsbMinute = 0x30;
			msbMinute++;
			
			//If msb minute exceeds 5, reset it to 0 and increment lsb hour
			if (msbMinute > 0x35){
				msbMinute = 0x30;
				lsbHour++;
				
				//if lsb hour exceeds 9, reset it to 0 and increment msb hour
				if (lsbHour > 0x39){
					lsbHour = 0x30;
					msbHour++;
				}
				//if msb hour exceeds 1 while msb hour is also 1, reset both msb and lsb hours to 0
				else if (lsbHour > 0x31 & msbHour == 0x31){
					msbHour = 0x30;
					lsbHour = 0x30;
					meridian = ~meridian;
				}
			}
		}
		
		//Writing hours
		write_to_lcd(msbHour,LCD_DATA);
		write_to_lcd(lsbHour,LCD_DATA);
		//Writing colon
		write_to_lcd(0x3A,LCD_DATA);
		//Writing minutes
		write_to_lcd(msbMinute,LCD_DATA);
		write_to_lcd(lsbMinute,LCD_DATA);
		//Writing meridians
		if (meridian){
				write_to_lcd(0x20,LCD_DATA); //space
				write_to_lcd(0x41,LCD_DATA); //"A"
				write_to_lcd(0x4D,LCD_DATA); //"M"
		}
		else{
				write_to_lcd(0x20,LCD_DATA); //space
				write_to_lcd(0x50,LCD_DATA); //"P"
				write_to_lcd(0x4D,LCD_DATA); //"M"
		}
			
		lsbMinute++; //Incrementing least significant second
		
		MSDelay(5);
		
		//Repositioning cursor to start of time
		write_to_lcd(0x85,COMMAND); 
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
		//for (j=0;j<1;j++); //Cannot catch up to such a quick speed
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
