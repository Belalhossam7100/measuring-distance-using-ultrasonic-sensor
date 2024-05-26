/*
 * display_distance.c
 *
 *  Created on: Mar 4, 2024
 *      Author: dell
 */


#include "ultrasonic.h"
#include "lcd.h"

int main(void){
uint16 measered_distance=0;
LCD_init();
Ultrasonic_init();
LCD_displayString("Distance= ");


 while(1)
{
	 LCD_moveCursor(0,11);
	 measered_distance = Ultrasonic_readDistance();
	 if(measered_distance >= 100)
	 				{
	 					LCD_intgerToString(measered_distance);
	 				}
	 				else
	 				{
	 					LCD_intgerToString(measered_distance);
	 					/* In case the digital value is two or one digits print space in the next digit place */
	 					LCD_displayCharacter(' ');
	 				}

}
}


