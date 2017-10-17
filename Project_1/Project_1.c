/********************************************************************************
 *
 * Author:   Rosswell Tiongco & Jesus Luciano
 * Email:    rosswelltiongco@gmail.com & jlucian995@gmail.com
 * Filename: Project_1.c
 * Date:     
 * Version:  1.0
 *
 * Description: 
 *
 * Mode 1: bouncing mode, MSB -> LSB -> MSB
 * Mode 2: 4 bit upcounter, external interrupt P3.2
 *				 P0.4 - P0.7 to control inital value
 * Mode 3: double bouncing, b7 & b0 -> b6 & b1 -> b5 & b2 -> b4 & b3 then repeat
 * Mode 4: stack mode b7 -> b7 &b6 -> b7 & b6 & b5 etc then repeat
 * 
 *
 *******************************************************************************/
#include<reg51.h>
#define LED

void delay(int x);

int main(){
	
	
	
	
}

void delay(int x){
	for(int i = 0; i < x; i++)
		for(int j = 0; j < 200,000; j++)
}
