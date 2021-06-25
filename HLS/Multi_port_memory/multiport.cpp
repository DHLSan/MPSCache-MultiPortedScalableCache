#include <stdio.h>
#include <stdlib.h>
#include "ap_int.h"


#define ARRAY_SIZE 32

int* getLocalLine(volatile ap_uint<512> *mem, int lineNumber){
#pragma HLS inline
	static int local_int_mem[16];
	ap_uint<512> local_line = mem[lineNumber];

	for(int i=0; i<16;i++){
#pragma HLS UNROLL COMPLETE
		unsigned int itemp = local_line((i+1)*32-1,i*32);
		int ftemp = *(int*)(&itemp);
		local_int_mem[i] = ftemp;
	}

	return local_int_mem;
}

void writeLocalLine(volatile ap_uint<512> *mem, int local_int_mem[16], int lineNumber){
#pragma HLS inline
	ap_uint<512> local_line;
	for(int i=0;i<16;i++){
#pragma HLS UNROLL COMPLETE
		int ftemp = local_int_mem[i];
		unsigned int itemp = *(unsigned int*)(&ftemp);
		local_line((i+1)*32-1,i*32) = itemp;
	}

	mem[lineNumber] = local_line;

}
int* xor_based(int wren1,int wline_number1, int wrdata1,int wren2,int wline_number2, int wrdata2, int ren,int rline_number1, int rline_number2, int bram_w0w1[],int bram_w0r0[], int bram_w0r1[],int bram_w1w0[], int bram_w1r0[],int bram_w1r1[]){
#pragma HLS inline

	int value[2];
    static int read_data[2];
    int mem_data;

	//Write port 0
	if(wren1==1){
		value[0]=wrdata1 ^ bram_w1w0[wline_number1];
		bram_w0w1[wline_number1]= value[0];
	    bram_w0r0[wline_number1]= value[0];
	    bram_w0r1[wline_number1]= value[0];
	}

	//Write port 1
	if(wren2==1){
		value[1]=wrdata2 ^ bram_w0w1[wline_number2];
		bram_w1w0[wline_number2]= value[1];
		bram_w1r0[wline_number2]= value[1];
		bram_w1r1[wline_number2]= value[1];
	}

	if(ren==1){
		// Read port
		read_data[0]= bram_w0r0[rline_number1] ^ bram_w1r0[rline_number1];
		read_data[1]= bram_w0r1[rline_number2] ^ bram_w1r1[rline_number2];
		//printf("%d, %d\n", read_data[0],read_data[1]);
		return read_data;
	}


	return 0;
}

void selection_sort(int size,int bram_w0w1[],int bram_w0r0[], int bram_w0r1[],int bram_w1w0[], int bram_w1r0[],int bram_w1r1[])
{

   int   i, j, temp, min;
   int* reads,writes;

   for (i = 0; i < size - 1; i++) {
      min = i;
      for (j = i + 1; j < size; j++){
    	 reads=xor_based(0,0,0,0,0,0,1,min,j,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);
         if (reads[0] > reads[1]){
        	 //printf("Read_Selection Sort: %d, %d\n", reads[0],reads[1]);
             min = j;
         }
      }
      reads=xor_based(0,0,0,0,0,0,1,min,i,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);
      xor_based(1,min,reads[1],1,i,reads[0],0,0,0,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);
   }
}

void multiport(ap_uint<512> *mem, ap_uint<512> *mem_out ){

#pragma HLS INTERFACE m_axi port=mem bundle=BUS_MEM num_read_outstanding=16 num_write_outstanding=16 max_read_burst_length=16 max_write_burst_length=16
#pragma HLS INTERFACE m_axi port=mem_out bundle=BUS_MEM num_read_outstanding=16 num_write_outstanding=16 max_read_burst_length=16 max_write_burst_length=16
//#pragma HLS INTERFACE m_axi port=mem bundle=BUS_MEM num_read_outstanding=16 num_write_outstanding=16 max_read_burst_length=16 max_write_burst_length=16
//#pragma HLS INTERFACE s_axilite port=mem bundle=BUS_SLAVE
#pragma HLS INTERFACE s_axilite port=return bundle=BUS_SLAVE

	int bram_w0w1[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w0w1 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w0w1 complete

	int bram_w0r0[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w0r0 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w0w1 complete

	int bram_w0r1[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w0r1 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w0r1 complete

	int bram_w1w0[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w1w0 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w1w0 complete

	int bram_w1r0[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w1r0 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w1r0 complete

	int bram_w1r1[ARRAY_SIZE];
	#pragma HLS resource variable=bram_w1r1 core=RAM_2P_BRAM
	#pragma HLS ARRAY_PARTITION variable=bram_w1r1 complete

	int *local_mem;



 loop_x:   for(int m=0; m <ARRAY_SIZE; m++){
#pragma HLS pipeline II=1
    	bram_w0w1[m]= 0;
    	bram_w0r0[m]=0;
    	bram_w0r1[m]=0;
    	bram_w1w0[m]= 0;
    	bram_w1r0[m]=0;
    	bram_w1r1[m]=0;
    }

    loop_d:for(int x=0;x < ARRAY_SIZE/16; x++){
    	local_mem = getLocalLine(mem,x);
        for(int y=0; y < 16; y=y+2){
#pragma HLS UNROLL COMPLETE
        	xor_based(1, x*16+y, local_mem[y], 1, x*16+y+1, local_mem[y+1], 0,0,0,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);
        }
    }

    selection_sort(ARRAY_SIZE,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);

    int* read_values;
    int local_write_mem[ARRAY_SIZE];
#pragma HLS ARRAY_PARTITION variable=local_write_mem complete

 loop_y:   for(int k=0; k<ARRAY_SIZE; k+=2){
#pragma HLS pipeline II=1
    	read_values=xor_based(0,0,0,0,0,0,1,k,k+1,bram_w0w1,bram_w0r0,bram_w0r1,bram_w1w0,bram_w1r0,bram_w1r1);
    	local_write_mem[k]=read_values[0];
    	local_write_mem[k+1]=read_values[1];
    }

    loop_z:for(int z=0; z < ARRAY_SIZE/2; z++){
#pragma HLS pipeline II = 1
    	writeLocalLine(mem_out,&local_write_mem[z*16], z);
    }

    return;
}
