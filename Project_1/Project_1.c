/********************************************************************************
 *
 * Author:   Rosswell Tiongco & Jesus Luciano
 * Email:    rosswelltiongco@gmail.com & jlucian995@gmail.com
 * Filename: Project_1.c
 * Date:     10/16/17
 * Version:  1.0
 *
 * Description: 
 *
 * Mode 1: bouncing mode, MSB -> LSB -> MSB
						-shift 7 times then shift 7 times back
 * Mode 2: 4 bit upcounter, external interrupt P3.2 determins count up or count down
 *				 
 *				 P0.4 - P0.7 to control inital value
 * Mode 3: double bouncing, b7 & b0 -> b6 & b1 -> b5 & b2 -> b4 & b3 then repeat
 * Mode 4: stack mode b7 -> b7 &b6 -> b7 & b6 & b5 etc then repeat
 * 
 * Mode control bits: p0.0 & p0.1
 * Speed control: P0.3 & P0.2
 * 00 0.1 seconds
 * 01 0.5 seconds
 * 10 1 seconds
 * 11 2 seconds
 *
 * Mode changed only when P3.3 push button set high
 * 
 *******************************************************************************/
#include <reg51.h>

//declare control bits
sbit M0 = P0^0;
sbit M1 = P0^1;
sbit dir = P3^2;
sbit D1 = P0^3;
sbit D0 = P0^2;
bit varM1 = 0;
bit varM0 = 0;
int direction = 1;

unsigned int i, j, x, y, count, delayVal = 0;

//Timer
void timer1(void) interrupt 3{//50ms 
	TR1 = 0;//turn timer off
	delayVal = delayVal - 1;//decrease delay value
	TH1 = 0x4B;//initial values
	TL1 = 0x92;
//	TR0 = 1;//turn timer on
}
void changeMode() interrupt 2{
	varM0 = M0;
	varM1 = M1;
	
}
void changeDir() interrupt 0{
	if(direction == 1)
		direction = -1;
	else
		direction = 1;
}

void delay();

//Mode 0
void bounce();

//Mode 1
void counter();

//Mode 2	
void doubleBounce();

//Mode 3
void stack();
/********************************************************/
int main(){
	//Interrupt enable
	EA = 1;
	ET1 = 1;//IE = 0x82; == 1000_0010
	
	//Timer enable
	TMOD = 0x10;//timer 0 mode 1
	TH1 = 0x4B;//high bit value
	TL1 = 0x92;//low bit value
	TR1 = 0;//turn off timer 0
	
	//enable external interrupts
	EX1 = 1;
	EX0 = 1;
	//I/O
	//Inputs
	P0 = 0xFF;
	P3 = 0xFF;
	
	//Outputs
	P1 = 0x00;

	//super loop
	while(1){
		//call all 4 functions, each one checks seperately
		//if mode bits are correct
		bounce();
		counter();
		doubleBounce();
		stack();
	}
	
}
/********************************************************/
//time delay is 1.0825us
//<<Timing not currently accurate

void delay(){
	//Delay 0 = 0.1 sec
	if(~D1 & ~D0){
		delayVal = 2;
	}
	//Delay 1 = 0.5 sec
	else if(~D1 & D0){
		delayVal = 10;
	}
	//Delay 2 = 1 sec
	else if(D1 & ~D0){
		delayVal = 20;
	}
	//Delay 3 = 2 sec
	else{
		delayVal = 40;
	}
	//stay in loop with timer 0 on until
	//delayVal = 0
	//delayVal decreases once every 5ms
	while(delayVal > 0){
			TR1 = 1;
	}
	TR1 = 0;
}


//Mode 0
void bounce(){
	//set initial value
	P1 = 0x80;
	x = 0;
	//Continuously checks if still in mode 0 every loop
	while(~varM1 & ~varM0){
		//move left 7 times
		if(x < 7){
			P1 = P1 / 2 ;
			x = x+1;
		}
		//move right 7 times
		else if(x < 14){
			P1 = P1 * 2;
			x = x+1;
		}
		//reset counter
		else{
			x = 0;
		}
		//call delay after every change
		delay();
	}
	
}
//Mode 1
void counter(){
	//only pulls bits P0^7-P0^4
	count = P0 / 16;  //FIXME: What does this line do? Seems to work when uncommented
	//continuously checks if in mode 1
	while(~varM1 & varM0){
		P1 = count;
		//increases counter if P3^2 is high
		//if(~direction){
			count = count + direction;
		//}
		//decreases counter if P3^2 is low
		/*else{
			count = count - 1;
		}*/
		
		//checks if count is out of bounds and resets
		if(count == 16){
			count = 0; 
		}
		if(count == -1){
			count = 15;
		}
		//call delay after every iteration
		delay();
	}
}
//Mode 2
void doubleBounce(){
	//Left << 1 | Right >> 1
	//Can also be done as: (0x80>>iteration) | (0x01 << iteration)
	//set initial value
	P1 = 0x81;
	x = 0x80;
	y = 0x01;
	count = 0;
	//Continuously checks if still in mode 2 every loop
	while(varM1 & ~varM0){
		//Both sides shift 7 times
		if(count < 7){
			P1 = x | y;
			x = x / 2;
			y = y * 2;
			count++;
		}
		else if (count < 14){
			P1 = x | y;
			x = x * 2;
			y = y / 2;
			count++;
		}
		//reset counter and dummy switches
		else{
			count = 0;
			x = 0x80;
			y = 0x01;
		}
		//call delay after every change
		delay();
	}
}
//Mode 3
void stack(){
	//Can also be done as LED | LED>>1
		//set initial value
	P1 = 0x00;
	//couldn't get it to work with the same variable (step through xor process)
	x = 0x80;
	y = 0x01;
	count = 0;
	//Continuously checks if still in mode 0 every loop
	while(varM1 & varM0){
		//Stack 8 times
		if(count < 8){
			P1 = P1 ^ x;
			x = x/2;
			count++;
		}
		else if (count < 16){
			P1 = P1 ^ y;
			y = y*2;
			count++;
		}
		//reset x and y to original values, otherwise they would take in their old values
		else{
			count = 0;
			x = 0x80;
			y = 0x01;
		}
				
		//call delay after every change
		delay();
		}

}
