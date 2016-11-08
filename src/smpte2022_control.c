/*
 * smpte2022_control.c
 *
 *  Created on: 8 Nov 2016
 *      Author: cw15g13
 */

#include "smpte2022_control.h"
#include "smpte2022_regdef.h"

static uint32_t smpte2022_baseAddr;

void smpte2022_init(uint32_t baseAddr){
	smpte2022_baseAddr = baseAddr;
}

void smpte2022_reset(void){

}
