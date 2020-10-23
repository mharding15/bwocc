#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "bit_vector.h"
#include "rank_support.h"
#include "math.h"

class select_support
{
	private:
		rank_support rs;
	public:
		select_support();
		select_support(rank_support& rs);
		uint64_t select1(uint64_t i);
		uint64_t select0(uint64_t i);
		uint64_t overhead();
		void save(string& fname);
		void load(string& fname);
		void freeMemory();
};
