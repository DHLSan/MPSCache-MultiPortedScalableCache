#pragma once

#include "ap_int.h"

namespace lm_data{


	template<int BITS> inline int LOG2(){
#pragma HLS INLINE
		return LOG2<(BITS >> 1)>() + 1;
	}

	template<>inline int LOG2<1>(){
#pragma HLS INLINE
		return 0;
	}

	template<int DATAWIDTH, int BITS, int CTB> class GetData{
	public:
		static ap_uint<DATAWIDTH>
		get(ap_uint<BITS> word, ap_uint<CTB> ctl){
#pragma HLS INLINE
			const int ctb = LOG2<(BITS)>() - LOG2<DATAWIDTH>();
			if(CTB != ctb)
				return 0;
			//bool up = (ctl & (1 << (ctb - 1)))!=0;
			bool up = ctl[CTB - 1];
			if(up)
				return GetData<DATAWIDTH, (BITS >> 1), CTB - 1>::get(word(BITS - 1, BITS >> 1), ctl);
			else
				return GetData<DATAWIDTH, (BITS >> 1), CTB - 1>::get(word, ctl);
		}
	};

	template<int DATAWIDTH>
	class GetData<DATAWIDTH,DATAWIDTH,0> {
	public:
		static ap_uint<DATAWIDTH>
		get(ap_uint<DATAWIDTH> word, ap_uint<1> ctl){
#pragma HLS INLINE
			return word;
		}
	};


	template<int DATAWIDTH, int BITS, int CTB> class SetData{
	public:
		static ap_uint<BITS>
		set(ap_uint<BITS> word, ap_uint<DATAWIDTH> data, ap_uint<CTB> ctl){
#pragma HLS INLINE
			const int ctb = LOG2<(BITS)>() - LOG2<DATAWIDTH>();
			if(CTB != ctb)
				return 0;
			//bool up = (ctl & (1 << (ctb - 1)))!=0;
			bool up = ctl[CTB - 1];
			if(up)
				word(BITS - 1, BITS >>1)
				= SetData<DATAWIDTH,(BITS>>1), CTB - 1>::set(word(BITS - 1, BITS >>1), data, ctl);
			else
				word((BITS >> 1) - 1, 0)
				= SetData<DATAWIDTH,(BITS>>1), CTB - 1>::set(word, data, ctl);
			return word;
		}
	};

	template<int DATAWIDTH>
	class SetData<DATAWIDTH, DATAWIDTH, 0> {
	public:
		static ap_uint<DATAWIDTH>
		set(ap_uint<DATAWIDTH> word, ap_uint<DATAWIDTH> data, ap_uint<1> ctl){
#pragma HLS INLINE
			return data;
		}
	};

}
