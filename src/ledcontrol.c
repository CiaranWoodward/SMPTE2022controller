/*
 * ledcontrol.c
 *
 *  Created on: 28 Oct 2016
 *      Author: cw15g13
 */

#include "platform.h"
#include "ledcontrol.h"
#include "xgpio.h"

static XGpio gpioStruct = {0};
static u16 curLEDState = 0;

void ledcontrol_init(){
	gpioStruct.BaseAddress = XPAR_GPIO_0_BASEADDR;
	gpioStruct.InterruptPresent = XPAR_GPIO_0_INTERRUPT_PRESENT;
	gpioStruct.IsDual = XPAR_GPIO_0_IS_DUAL;
	gpioStruct.IsReady = 0;

	XGpio_Initialize(&gpioStruct, XPAR_GPIO_0_DEVICE_ID);
}

void ledcontrol_setBitfield(u16 bitfield){
	curLEDState = bitfield;
	XGpio_DiscreteWrite(&gpioStruct, 1, curLEDState);
}

void ledcontrol_setLed(u8 ledNo, led_command ledCommand){

	switch(ledCommand){
	case LED_ON:
		curLEDState |= (1<<ledNo);
		break;
	case LED_OFF:
		curLEDState &= ~(1<<ledNo);
		break;
	case LED_TOGGLE:
		curLEDState = curLEDState ^ (1<<ledNo);
		break;
	}

	XGpio_DiscreteWrite(&gpioStruct, 1, curLEDState);

}
