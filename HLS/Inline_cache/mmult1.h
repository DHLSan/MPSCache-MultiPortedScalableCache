#ifndef _MMULT1_HH_
#define _MMULT1_HH_

#include "mmult.h"

//#define debug
#ifdef debug
#include <iostream>
using std::cout;
using std::endl;
#endif

template<typename A, typename B, typename O>
void mmult1(A &a, B &b, O &output) {
#pragma HLS INLINE
	for (unsigned int r=0;r<rank;r++){
		for (unsigned int c=0;c<rank;c++){
			TypeTest running = 0;
			for (int index=0; index< rank; index++) {
#pragma HLS pipeline
				int aIndex = r*rank + index;
				int bIndex = index*rank + c;
				running += a[aIndex] * b[bIndex];
			}
			output[r*rank + c] = running;
		}
	}
}
#endif
