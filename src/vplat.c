/*
 * vplat.c
 *
 *  Created on: 9 Nov 2016
 *      Author: cw15g13
 */

#include "vplat.h"
#include "ledcontrol.h"

void vplat_assert(uint8_t condition){
	if(!condition){
		while(1){
			delay_human();
			ledcontrol_setBitfield(0x00);
			delay_human();
			ledcontrol_setBitfield(0xFF);
		}
	}
}

void delay_human(){	//Simple convenience function to delay an effect (such as LED) so it is noticeable by the human eye
#ifndef __SIM
	for(int i = 0; i < DELAY_HUMAN; i++) i = i;
#endif
}
