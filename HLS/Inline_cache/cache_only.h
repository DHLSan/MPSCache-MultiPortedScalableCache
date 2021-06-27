#pragma once
/*
 * Cache in this implementation are designed to be used as read only or write
 * only mode. mixed use will cause errors.
 *
 */

#include "ap_int.h"
#include "dataOpera.h"

namespace CACHE_ONLY{
/*
 * Template Cache implementation with template data type, set_i bits and block bits.
 */

template<typename T, int SET_BITS, int BLOCK_BITS>
class Cache {

	static const int CACHE_SETS = 1 << SET_BITS;
	static const int LINE_SIZE = 1 <<BLOCK_BITS;
	static const int DATA_BITS = sizeof(T) * 8;
	typedef ap_uint<DATA_BITS> LocalType;
private:
	T get(const ap_int<32> addr){
#pragma HLS INLINE
		const ap_uint<32 - SET_BITS-BLOCK_BITS> tag = addr >> (SET_BITS+BLOCK_BITS);
		const ap_uint<SET_BITS> set_i = (addr >> BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;
		requests++;
		bool match = tags[set_i] == tag;

		DataType dt;
		if(valid[set_i] && match) {
			hits++;
			dt = array[set_i];
		}
		else {
			dt = ptr_mem[addr >> BLOCK_BITS];
			array[set_i] = dt;
		}
		tags[set_i] = tag;

		valid[set_i] = true;
		LocalType data = lm_data::GetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::get(dt, block);
		return *(T*)&data;
	}

	void set(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		const ap_uint<32 - SET_BITS-BLOCK_BITS> tag = addr >> (SET_BITS+BLOCK_BITS);
		const ap_uint<SET_BITS> set_i = (addr >> BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;

		requests++;

		bool match = tags[set_i] == tag;
		if(valid[set_i] && match) {
			hits++;
		}
		else {
			if(dirty[set_i]) {
				ap_uint<32> paddr = tags[set_i];
				ptr_mem[paddr << SET_BITS | set_i] = array[set_i];
			}
			array[set_i] = ptr_mem[addr >> BLOCK_BITS];
		}

		LocalType ldata = *(LocalType*)&data;

		array[set_i] = lm_data::SetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::set(array[set_i], ldata, block);
		tags[set_i] = tag;
		valid[set_i] = true;
		dirty[set_i] = true;

	}

	class inner {
	public:
		inner(Cache *cache, const ap_uint<32> addr):cache(cache),addr(addr) {
		}
		operator T() const{
#pragma HLS INLINE
			return cache->get(addr);
		}
		void operator= (T data){
#pragma HLS INLINE
			cache->set(addr, data);
		}
	private:
		Cache *cache;
		const ap_uint<32> addr;
	};

public:

	typedef ap_uint<DATA_BITS*LINE_SIZE> DataType;

	Cache(DataType * mem):ptr_mem(mem){
#pragma HLS ARRAY_PARTITION variable=tags complete dim=1
#pragma HLS ARRAY_PARTITION variable=dirty complete dim=1
#pragma HLS ARRAY_PARTITION variable=valid complete dim=1
#pragma HLS ARRAY_PARTITION variable=array complete dim=1
		for(int i=0;i<CACHE_SETS;++i) {
#pragma HLS UNROLL
			valid[i] = false;
			dirty[i] = false;
		}
		requests = 0;
		hits = 0;
	}
	double getHitRatio() {
		return hits/(requests * 1.0);
	}

	bool hit(int addr) {
		const ap_uint<32 - SET_BITS-BLOCK_BITS> tag = addr >> (SET_BITS+BLOCK_BITS);
		const ap_uint<SET_BITS> set_i = (addr >> BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;
		bool match = tags[set_i] == tag;
		return (valid[set_i] && match);
	}

	inner operator[](const ap_uint<32> addr) {
#pragma HLS INLINE
		return inner(this, addr);
	}

	T retrieve(int addr){
#pragma HLS INLINE
		const ap_uint<SET_BITS> set_i = (addr >> BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;
		LocalType data = array[set_i] >> (block * DATA_BITS);
		requests++;
		if(hit(addr))hits++;
		return *(T*)&data;
	}

	void continue_write(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		const ap_uint<32 - SET_BITS-BLOCK_BITS> tag = addr >> (SET_BITS+BLOCK_BITS);
		const ap_uint<SET_BITS> set_i = (addr >> BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;

		requests++;

		bool match = tags[set_i] == tag;
		if(valid[set_i] && match) {
			hits++;
		}
		else {
			if(dirty[set_i]) {
				ap_uint<32> paddr = tags[set_i];
				ptr_mem[paddr << SET_BITS | set_i] = array[set_i];
			}
			array[set_i] = 0;//ptr_mem[addr >> BLOCK_BITS];
		}

		LocalType ldata = *(LocalType*)&data;

		array[set_i] = lm_data::SetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::set(array[set_i], ldata, block);
		tags[set_i] = tag;
		valid[set_i] = true;
		dirty[set_i] = true;
	}
	~Cache(){
		for(int i=0;i<CACHE_SETS;i++){
#pragma HLS PIPELINE
			if(dirty[i]) {
				ap_uint<32> paddr = tags[i];
				ptr_mem[paddr << SET_BITS | i]=array[i];
			}
		}
	}
private:

	long long int requests, hits;
	DataType * const ptr_mem;
	DataType array[CACHE_SETS];
	ap_uint<32-SET_BITS-BLOCK_BITS> tags[CACHE_SETS];
	bool valid[CACHE_SETS], dirty[CACHE_SETS];
};



/*
 * Cache implementation with BLOCK_BITS
 */

template<typename T, int BLOCK_BITS>
class Cache<T, 0, BLOCK_BITS> {

	static const int LINE_SIZE = 1 <<BLOCK_BITS;
	static const int DATA_BITS = sizeof(T) * 8;

private:
	typedef ap_uint<DATA_BITS> LocalType;
	T get(const ap_int<32> addr){
#pragma HLS INLINE
		const ap_uint<32 - BLOCK_BITS> tag = addr >> (BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;
		requests++;
		bool match = tags == tag;


		DataType dt;
		if(valid && match) {
			hits++;
			dt = array;
		}
		else {
			dt = ptr_mem[tag];
			array = dt;
		}

		tags = tag;
		valid = true;
		LocalType data = lm_data::GetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::get(dt, block);
		return *(T*)&data;
	}

	void set(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		const ap_uint<32 - BLOCK_BITS> tag = addr >> (BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;

		requests++;

		bool match = tags == tag;
		if(valid && match) {
			hits++;
		}
		else {
			if(dirty) {
				ptr_mem[tags] = array;
			}
			array = ptr_mem[tag];

		}

		LocalType ldata = *(LocalType*)&data;
		array = lm_data::SetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::set(array, ldata, block);
		tags = tag;
		dirty = true;
		valid = true;
	}

	class inner {
	public:
		inner(Cache *cache, const ap_uint<32> addr):cache(cache),addr(addr) {
		}
		operator T() const{
#pragma HLS INLINE
			return cache->get(addr);
		}
		void operator= (T data){
#pragma HLS INLINE
			cache->set(addr, data);
		}
	private:
		Cache *cache;
		const ap_uint<32> addr;
	};

public:

	typedef ap_uint<DATA_BITS*LINE_SIZE> DataType;

	Cache(DataType * mem):ptr_mem(mem){

		valid = false;
		dirty = false;
		requests = 0;
		hits = 0;
	}
	double getHitRatio() {
		return hits/(requests * 1.0);
	}

	bool hit(int addr) {
		const ap_uint<32 - BLOCK_BITS> tag = addr >> (BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;
		bool match = tags == tag;
		return (valid && match);
	}

	inner operator[](const ap_uint<32> addr) {
#pragma HLS INLINE
		return inner(this, addr);
	}

	void continue_write(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		const ap_uint<32 - BLOCK_BITS> tag = addr >> (BLOCK_BITS);
		const ap_uint<BLOCK_BITS> block = addr;

		requests++;

		bool match = tags == tag;
		if(valid && match) {
			hits++;
		}
		else {
			if(dirty) {
				ptr_mem[tags] = array;
			}
			array = 0;
		}

		LocalType ldata = *(LocalType*)&data;
		array = lm_data::SetData<DATA_BITS, DATA_BITS * LINE_SIZE, BLOCK_BITS >::set(array, ldata, block);
		tags = tag;
		dirty = true;
		valid = true;
	}

	~Cache(){
		if(dirty) {
			ptr_mem[tags]=array;
		}
	}
public:

	long long int requests, hits;
	DataType * const ptr_mem;
	DataType array;
	ap_uint<32-BLOCK_BITS> tags;
	bool valid, dirty;
};

/*
 * Cache implementation with SET_BITS
 */

template<typename T, int SET_BITS>
class Cache<T, SET_BITS, 0> {

	static const int CACHE_SETS = 1 << SET_BITS;
	static const int DATA_BITS = sizeof(T) * 8;

private:
	T get(const ap_int<32> addr){
#pragma HLS INLINE
		const ap_uint<32 - SET_BITS> tag = addr >> (SET_BITS);
		const ap_uint<SET_BITS> set_i = addr;
		requests++;
		DataType data;
		bool match = tags[set_i] == tag;
		if(valid[set_i] && match) {
			hits++;
			data = array[set_i];
		}
		else {
			data = ptr_mem[addr];
			array[set_i] = data;
		}
		tags[set_i] = tag;

		valid[set_i] = true;

		return *(T*)&data;
	}

	void set(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		const ap_uint<32 - SET_BITS> tag = addr >> (SET_BITS);
		const ap_uint<SET_BITS> set_i = addr;

		requests++;

		bool match = tags[set_i] == tag;
		if(valid[set_i] && match) {
			hits++;
		}
		else {
			if(dirty[set_i]) {
				ap_uint<32> paddr = tags[set_i];
				ptr_mem[paddr << SET_BITS | set_i] = array[set_i];
			}
		}

		DataType ldata = *(DataType*)&data;
		array[set_i] = ldata;

		tags[set_i] = tag;
		dirty[set_i] = true;
		valid[set_i] = true;
	}

	class inner {
	public:
		inner(Cache *cache, const ap_uint<32> addr):cache(cache),addr(addr) {
		}
		operator T() const{
#pragma HLS INLINE
			return cache->get(addr);
		}
		void operator= (T data){
#pragma HLS INLINE
			cache->set(addr, data);
		}
	private:
		Cache *cache;
		const ap_uint<32> addr;
	};

public:

	typedef ap_uint<DATA_BITS> DataType;

	Cache(DataType * mem):ptr_mem(mem){
//#pragma HLS RESOURCE variable=tags core=RAM_2P_LUTRAM
//#pragma HLS RESOURCE variable=dirty core=RAM_1P_LUTRAM
//#pragma HLS RESOURCE variable=valid core=RAM_1P_LUTRAM

#pragma HLS ARRAY_PARTITION variable=tags complete dim=1
#pragma HLS ARRAY_PARTITION variable=dirty complete dim=1
#pragma HLS ARRAY_PARTITION variable=valid complete dim=1
#pragma HLS ARRAY_PARTITION variable=array complete dim=1
		for(int i=0;i<CACHE_SETS;++i) {
#pragma HLS UNROLL
			valid[i] = false;
			dirty[i] = false;
		}
		requests = 0;
		hits = 0;
	}
	double getHitRatio() {
		return hits/(requests * 1.0);
	}

	inner operator[](const ap_uint<32> addr) {
#pragma HLS INLINE
		return inner(this, addr);
	}
	~Cache(){
		for(int i=0;i<CACHE_SETS;i++){
#pragma HLS PIPELINE
			if(dirty[i]) {
				ap_uint<32> paddr = tags[i];
				ptr_mem[paddr << SET_BITS | i]=array[i];
			}
		}
	}
private:

	int requests, hits;
	DataType * const ptr_mem;
	DataType array[CACHE_SETS];
	ap_uint<32-SET_BITS> tags[CACHE_SETS];
	bool valid[CACHE_SETS], dirty[CACHE_SETS];
};

/*
 * Cache implementation with SET_BITS
 */

template<typename T>
class Cache<T, 0, 0> {

	static const int DATA_BITS = sizeof(T) * 8;

private:
	T get(const ap_int<32> addr){
#pragma HLS INLINE
		requests++;
		DataType data;
		bool match = tags == addr;
		if(valid && match) {
			hits++;
			data = array;
		}
		else {
			data = ptr_mem[addr];
			array = data;
		}
		tags = addr;

		valid = true;

		return *(T*)&data;
	}

	void set(const ap_int<32> addr, const T& data){
#pragma HLS INLINE
		requests++;

		bool match = tags == addr;
		if(valid && match) {
			hits++;
		}
		else {
			if(dirty) {
				ptr_mem[tags] = array;
			}
		}

		DataType ldata = *(DataType*)&data;
		array = ldata;

		tags = addr;
		dirty = true;
		valid = true;
	}

	class inner {
	public:
		inner(Cache *cache, const ap_uint<32> addr):cache(cache),addr(addr) {
		}
		operator T() const{
#pragma HLS INLINE
			return cache->get(addr);
		}
		void operator= (T data){
#pragma HLS INLINE
			cache->set(addr, data);
		}
	private:
		Cache *cache;
		const ap_uint<32> addr;
	};

public:

	typedef ap_uint<DATA_BITS> DataType;

	Cache(DataType * mem):ptr_mem(mem){
		valid = false;
		dirty = false;
		requests = 0;
		hits = 0;
	}
	double getHitRatio() {
		return hits/(requests * 1.0);
	}

	inner operator[](const ap_uint<32> addr) {
#pragma HLS INLINE
		return inner(this, addr);
	}
	~Cache(){
		if(dirty) {
			ptr_mem[tags]=array;
		}
	}
private:

	int requests, hits;
	DataType * const ptr_mem;
	DataType array;
	ap_uint<32> tags;
	bool valid, dirty;
};
}
