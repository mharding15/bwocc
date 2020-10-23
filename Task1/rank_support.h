#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "bit_vector.h"
#include "math.h"

using namespace std;

class rank_support
{
	private:
		int superBlockSize, blockSize, numSuperBlocks, numBlocks, blocksPerSuperBlock, bvLength;
		int *r_s, *r_b, *r_p;
	public:
		rank_support();
		rank_support(bit_vector& bitvector);
		uint64_t rank1(uint64_t i);
		uint64_t rank0(uint64_t i);
		uint64_t overhead();
		void save(string& fname);
		void load(string& fname);
		string toString();
		void print();
		int getBvLength();
		void freeMemory();
};
