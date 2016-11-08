/*
 * ledcontrol.h
 *
 *  Created on: 28 Oct 2016
 *      Author: cw15g13
 */

#ifndef SRC_LEDCONTROL_H_
#define SRC_LEDCONTROL_H_

#include "xil_types.h"

typedef enum {LED_ON, LED_OFF, LED_TOGGLE} led_command;

void ledcontrol_init();

void ledcontrol_setBitfield(u16 bitfield);

void ledcontrol_setLed(u8 ledNo, led_command ledCommand);

#endif /* SRC_LEDCONTROL_H_ */
