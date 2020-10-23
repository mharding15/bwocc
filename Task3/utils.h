#pragma once
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "bit_vector.h"
#include "rank_support.h"
#include "select_support.h"

using namespace std;

class utils
{
	private:
		int* mergeSortHelper(string word, int* arr, int beg, int end);
		int* merge(string word, int* arr1, int arr1Size, int* arr2, int arr2Size);
	public:
		string getBwt(string word);
		int* mergeSort(string word, int* arr, int size);
		void printArray(int* arr, int size);
		void createOccTable(string input, string output);
		void useOccTable(string fname, string qfname);
		void timeQueryFinding(string fname, string qfname);
		static void processGenomeFile(string input, string output);
		static void createQueries(string input, string output, int setSize, double percentageRandom);
		static string getRandomQuery(int size);
		static void testRankSupport(string& fname);
		static void testSelectSupport(string& fname);
};
