/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "vplat.h"
#include "ledcontrol.h"
#include "smpte2022_control.h"

int main()
{
    init_platform();	//Init caches and UART

    print("Begin.\n\r");

    ledcontrol_init();

    ledcontrol_setBitfield(0);
    vplat_delayHuman();
    ledcontrol_setBitfield(0xFF);
    vplat_delayHuman();

    print("Init SMPTE2022.\n\r");

	smpte2022_init(XPAR_V_SMPTE2022_56_RX_0_BASEADDR);
	smpte2022_channel_getSemaphore(0, 0);
	smpte2022_channel_setDDR3Params(XPAR_MIG7SERIES_0_BASEADDR, 300);
	smpte2022_channel_setEnabled(1);
	smpte2022_channel_matchDestIp(1, 0xEF00277A); //239.0.39.122
	smpte2022_channel_releaseSemaphore();

	print("Done.\n\r");

	while(1){
		for(int j = 0; j < 8; j++){
			ledcontrol_setLed(j, LED_TOGGLE);
			vplat_delayHuman();
		}
	}

    cleanup_platform();
    return 0;
}
