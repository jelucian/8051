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
#include<reg51.h>

#define M0 P0^0
#define M1 P0^1
#define time0 P0^2
#define time1 P0^3

#define LED P1

void delay();

//Mode 1
void bounce();

//Mode 2
void count();

//Mode 3	
void doubleBounce();

//Mode 4
void stack();

int main(){
	
	
	
	
}
//time delay is 1.0825us

//
void delay(){
	int delayVal = 0;
	
	
	/*
	
	
	
	*/

}

//Mode 1
void bounce(){
		P0 = 0x80;
		int i = 0;
	while(~M0 & ~M1){
		if(i < 7){
			i++;
			P0 >>= 1;
		}
		else if(i < 14){
			i++;
			P0 <<= 1;
		}
		else{
			i = 0;
		}
		
	}
	/*
	for(int i = 0; i < 7; ++i){
		P0 >>= 1;
	}
	for(int i = 0; i < 7; ++i){
		P0 <<= 1;
	}
	*/
	
}
//Mode 2
void count(){
	if 
}
//Mode 3	
void double(){
	
}
//Mode 4
void stack(){
	
}




//commment test