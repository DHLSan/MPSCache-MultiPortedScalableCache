#include <cstring>
#include "mmult.h"
#include "mmult1.h"
#include "ap_int.h"


extern "C"
void mmult(CacheTypeA::DataType *a,
		CacheTypeB::DataType *b,
		CacheTypeC::DataType *c)
{
#pragma HLS INTERFACE m_axi port=a bundle=gmem0
#pragma HLS INTERFACE m_axi port=b bundle=gmem1
#pragma HLS INTERFACE m_axi port=c bundle=gmem2

#pragma HLS INTERFACE s_axilite port=return bundle=control

	CacheTypeA A(a);
	CacheTypeB B(b);
	CacheTypeC C(c);

	mmult1(A,B,C);
	std::cout<<"ratio of Cache A is: "<<A.getHitRatio()<<std::endl;
	std::cout<<"ratio of Cache B is: "<<B.getHitRatio()<<std::endl;
	std::cout<<"ratio of Cache C is: "<<C.getHitRatio()<<std::endl;
  return;
}


