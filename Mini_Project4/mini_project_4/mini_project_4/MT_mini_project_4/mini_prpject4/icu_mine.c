/*
 * icu_mine.c
 *
 *  Created on: Feb 27, 2024
 *      Author: dell
 */
#include "icu_mine.h"
#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */


/**definition of the pointer used to call back function**/
static volatile void(*g_ptr_callBack)(void) = NULL_PTR;

ISR(TIMER1_CAPT_vect)
{
	if(g_ptr_callBack != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_ptr_callBack)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


void ICU_init(const ICU_ConfigType * Config_Ptr){
	GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_INPUT);
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->clock);
	//f8=1111 1000
	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge)<<6);
	//BF=1011 1111
	/* Initial Value for Timer1 */
		TCNT1 = 0;
	/* Initial Value for the input capture register */
		ICR1 = 0;
	/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
		SET_BIT(TIMSK,TICIE1);
}



void ICU_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_ptr_callBack = a_ptr;
}

void ICU_setEdgeDetectionType(const ICU_EdgeType edgeType){
	TCCR1B =( (TCCR1B & 0xBF) | ((edgeType)<<6));
}
uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void)
{
	/* Clear All Timer1/ICU Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	TIMSK &= ~(1<<TICIE1);

	/* Reset the global pointer value */
	g_ptr_callBack = NULL_PTR;
}
