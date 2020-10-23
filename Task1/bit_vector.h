#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#define BLOCK_SIZE 32

using namespace std;

class bit_vector
{
	private:
		int length, numBlocks;
		bitset<BLOCK_SIZE>* bvec;
		string reverseString(string st);
	public:
		bit_vector();
		bit_vector(string& fname);
		bit_vector(int length, string vec);
		bit_vector(int size);
		void setVec(string vec);
		void printVec();
		int getBit(int pos);
		void setBit(int pos);
		void unsetBit(int pos);
		int getLength();
		int getNumBlocks();
		void save(string& fname);
		void load(string& fname);
		string toString();
		void freeMemory();
		static void createTestFiles();
};
