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
#include "xil_cache.h"
#include <xtime_l.h>

int * mmult = (int *)(0x43C00000);

int * matrix_a = (int *)(0x01000000);
int * matrix_b = (int *)(0x02000000);
int * matrix_c = (int *)(0x03000000);

void run_mm(){

    int rank = 32;
	for (unsigned int r=0;r<rank;r++){
		for (unsigned int c=0;c<rank;c++){
			int running = 0;
			for (int index=0; index< rank; index++) {
				int aIndex = r*rank + index;
				int bIndex = index*rank + c;
				running += matrix_a[aIndex] * matrix_b[bIndex];
			}
			matrix_c[r*rank + c] = running;
		}
	}
}

void matrix_initialize(){
	 for(int i=0;i<32*32;i++){
	        	matrix_a[i]=i;
	        	matrix_b[i]=i;
	    }
	    for(int i=0;i<32*32 ;i++){
	    	matrix_c[i]=0;

	    }
}
int main()
{

    init_platform();
    int sec, usec;
    double  ElapsedTime;
    XTime tStart, tEnd;
    Xil_DCacheDisable();

    print("Fatma\n\r");

    matrix_initialize();
	//start block
    mmult[0] |= 0x1;
    XTime_GetTime(&tStart);
    while((mmult[0]&0x2) != 0x2);

    XTime_GetTime(&tEnd);

    printf("In HW, Output took %llu clock cycles.\n\r", (int)2*(tEnd - tStart));
    ElapsedTime = 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND);

    sec = (int)ElapsedTime;
    usec = (int)((ElapsedTime - sec) * 1000000);
    printf("In HW, Output took %d.%06d\n\r",sec,usec);

    for(int i=0;i<256;i++){
    		printf("matrix_C[%d]=%X\n",i,matrix_c[i]);
    }

    matrix_initialize();
    XTime_GetTime(&tStart);
    run_mm();
    XTime_GetTime(&tEnd);

    printf("In SW, Output took %llu clock cycles.\n\r", (int)2*(tEnd - tStart));
    ElapsedTime = 1.0 * (tEnd - tStart) / (COUNTS_PER_SECOND);
    sec = (int)ElapsedTime;
    usec = (int)((ElapsedTime - sec) * 1000000);
    printf("In SW, Output took %d.%06d\n\r",sec,usec);

    for(int i=0;i<256;i++){
        	printf("matrix_C[%d]=%X\n",i,matrix_c[i]);
    }

    cleanup_platform();
    return 0;
}
