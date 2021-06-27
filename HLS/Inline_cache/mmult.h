#ifndef _MMULT_HH_
#define _MMULT_HH_
#include "ap_int.h"
#include "cache_only.h"

#define rank 32
#define Bit 4

typedef int TypeTest;
typedef CACHE_ONLY::Cache<TypeTest, 0, Bit> CacheTypeA;
typedef CACHE_ONLY::Cache<TypeTest, 8-Bit, Bit> CacheTypeB;
typedef CACHE_ONLY::Cache<TypeTest, 0, Bit> CacheTypeC;
extern "C"
void mmult(CacheTypeA::DataType *a,
		CacheTypeB::DataType *b,
	 CacheTypeC::DataType *c);

#endif
