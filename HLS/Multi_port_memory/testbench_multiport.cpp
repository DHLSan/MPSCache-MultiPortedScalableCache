#include <stdio.h>
#include "ap_int.h"

void multiport(ap_uint<512> *mem, ap_uint<512> *mem_out);

static int memory[32], memory_out[32];

int main(){

	printf("\n********************* TEST *********************\n");

	for(int j=0; j<32;j++){
		memory[j]=rand()%25;
	}
	printf("first array: ");
	for (int i = 0; i < 32; i++)
	        printf("%d ", memory[i]);

	printf("\n");
	multiport((ap_uint<512> *)memory, (ap_uint<512> *)memory_out);


	printf("sorted array: ");
	for (int i = 0; i < 32; i++)
	        printf("%d ", memory_out[i]);



	printf("\n********************* END **********************\n");

	return 0;
}
