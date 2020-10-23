#include "utils.h"

// this method takes a string from a file, creates the BWT and makes a bit vector for each character
void utils::createOccTable(string input, string output) {
	// obtain the input from the input file
	fstream infile;
	infile.open(input, ios::in);

	string word;
	getline(infile, word);
	infile.close();

	// get the BWT of the word
	string BWT = getBwt(word + "$");

	map<char, bit_vector> charMap;
	map<char, bit_vector>::iterator it;

	map<char, int> occMap;
	map<char, int>::iterator occIt;

	// create bit vector of 0's for each character in the bwt
	int sigma = 0;
	for (int i = 0; i < BWT.size(); i++) {
		// don't need to include $ as a character, there is only 1 and we always know where it is in the first column of bwt
		if (BWT.at(i) != '$') {
			it = charMap.find(BWT.at(i));
			// the char does not exist in the map
			if (it == charMap.end()) {
				// not sure this will actually work, might need to use 'new'
				bit_vector b(BWT.size());
				b.setBit(i);
				charMap[BWT.at(i)] = b;

				occMap[BWT.at(i)] = 1;
				sigma++;
			// the char does exist and
			}else {
				it->second.setBit(i);
				occIt = occMap.find(BWT.at(i));
				occIt->second++;
			}
		}
	}

	// now save the bitvectors
	ofstream outfile;
	outfile.open(output);

	outfile << sigma << endl;
	outfile << BWT.size() << endl;

	// for each character in the map, store the bit vector associated with it to a file
	int runningTotal = 1;
	for (map<char, bit_vector>::iterator it = charMap.begin(); it != charMap.end(); ++it) {
		// will store the character, the first row of that character, and the bitvector itself
		outfile << it->first << endl;
		outfile << runningTotal << endl;
		outfile << occMap.find(it->first)->second << endl;
		runningTotal += occMap.find(it->first)->second;
		outfile << it->second.toString();
		it->second.freeMemory();
	}

	outfile.close();

	cout << sigma << endl;
	cout << word.size() << endl;
}

// this function takes the name of the bwt occ table stored file and the file with the queries and outputs the rows in the bwt of the queries
void utils::useOccTable(string fname, string qfname) {
	ifstream infile;
	infile.open(fname, ios::in);

	int bvLength, sigma, startIdx, count;
	char c;
	string line;

	map<char, pair<int, int>> firstCol;
	map<char, rank_support> rankSupportMap;

	if (infile.is_open()) {
		getline(infile, line);
		sigma = stoi(line);
		getline(infile, line);
		bvLength = stoi(line);

		for (int i = 0; i < sigma; i++) {
			getline(infile, line);
			c = line.at(0);
			getline(infile, line);
			startIdx = stoi(line);
			getline(infile, line);
			count = stoi(line);
			getline(infile, line);
			bit_vector b(bvLength, line);

			// store the starting index and count of the character in the first column of the bwt
			firstCol[c] = pair<int, int>(startIdx, count);

			// store the rank_support structure for the character
			rank_support rs(b);
			rankSupportMap[c] = rs;

			b.freeMemory();
		}

		infile.close();
	}
	else {
		cout << "Could not open the bwt structure file." << endl;
		return;
	}

	// now that we have the rank support structure for each char, can do backwards searching
	infile.open(qfname, ios::in);
	if (infile.is_open()) {
		// for each line (query)
		while (getline(infile, line)) {
			int queryLength = line.size();
			char curr = line.at(queryLength - 1), next;
			int firstIdx, lastIdx, firstRank = -1, lastRank = -1;
			pair<int, int> p;
			bool found = true;

			// go backwards through the query
			for (int i = queryLength - 2; i >= 0; i--) {
				// get the pair which contains the first index of the char in bwt and num occurrences of char in bwt
				p = firstCol.find(curr)->second;
				if (i == queryLength - 2) {
					firstIdx = p.first;
					lastIdx = firstIdx + p.second - 1;
				} else {
					firstIdx = p.first + firstRank - 1;
					lastIdx = p.first + lastRank - 1;
				}
				next = line.at(i);

				firstRank = rankSupportMap.find(next)->second.rank1(firstIdx - 1);
				lastRank = rankSupportMap.find(next)->second.rank1(lastIdx);

				// if this is the case, then the query does not exist in the string
				if (firstRank == lastRank) {
					found = false;
					break;
				}
				firstRank++;
				curr = next;
			}
			if (found) {
				lastRank++;
			}
			cout << firstRank << "\t" << lastRank << endl;
		}

	} else {
		cout << "Could not open the queries file." << endl;
		return;
	}

	for (map<char, rank_support>::iterator it = rankSupportMap.begin(); it != rankSupportMap.end(); ++it) {
		it->second.freeMemory();
	}
}

// this method is used to time the queries
void utils::timeQueryFinding(string fname, string qfname) {

	cout << "The structure name: " << fname << ". The qfname: " << qfname << endl;

	auto start = chrono::steady_clock::now();
	useOccTable(fname, qfname);
	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
}

// this method calls the helper method that creates the BWT
string utils::getBwt(string word) {
	int* arr = new int[word.size()]{};
	for (int i = 0; i < word.size(); i++) {
		arr[i] = i;
	}
	int* sortedArray = mergeSort(word, arr, word.size());
	string bwt = "";
	for (int i = 0; i < word.size(); i++) {
		int pos = word.size() - 1;
		if (sortedArray[i] != 0) {
			pos = (sortedArray[i] - 1) % word.size();
		}
		bwt += word.at(pos);
	}
	delete[] arr;
	delete[] sortedArray;

	return bwt;
}

// the method that does the merge sort (although, the helper really does all the work)
int* utils::mergeSort(string word, int* arr, int size) {
	return mergeSortHelper(word, arr, 0, size - 1);
}

// the real workhorse of the merge sort algorithm
int* utils::mergeSortHelper(string word, int* arr, int beg, int end) {

	if (beg == end) {
		return new int[1]{ arr[beg] };
	}
	int mid = (beg + end) / 2;
	int* arr1 = mergeSortHelper(word, arr, beg, mid);
	int* arr2 = mergeSortHelper(word, arr, mid + 1, end);

	return merge(word, arr1, mid - beg + 1, arr2, end - (mid + 1) + 1);
}

// this method merges two sorted arrays into one sorted array
int* utils::merge(string word, int* arr1, int arr1Size, int* arr2, int arr2Size) {

	int totalSize = arr1Size + arr2Size;
	int* merged = new int[totalSize] {};
	int arr1Pos = 0, arr2Pos = 0;
	for (int i = 0; i < totalSize; i++) {
		// not done with arr1
		if (arr1Pos < arr1Size) {
			// not done with arr2
			if (arr2Pos < arr2Size) {
				int a1 = arr1[arr1Pos], a2 = arr2[arr2Pos];
				// need to compare the substrings starting at the positions and going to the end of the string
				if (word.substr(a1, word.size() - a1) <= word.substr(a2, word.size() - a2)) {
					merged[i] = arr1[arr1Pos++];
				} else {
					merged[i] = arr2[arr2Pos++];
				}
			// arr2 is done, so just use arr1 value
			} else {
				merged[i] = arr1[arr1Pos++];
			}
		// we are done with arr1
		} else {
			merged[i] = arr2[arr2Pos++];
		}
	}
	delete[] arr1;
	delete[] arr2;

	return merged;
}

// used for debugging purposes
void utils::printArray(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

// I got the genome file in a bunch of lines, I made this to basically concatenate the lines into a single line
void utils::processGenomeFile(string input, string output) {
	fstream infile;
	infile.open(input, ios::in);
	string line;

	ofstream outfile;
	outfile.open(output);

	if (infile.is_open()) {
		while (getline(infile, line)) {
			outfile << line;
		}
	}
	else {
		cout << "The file could not be opened." << endl;
		return;
	}

	infile.close();
	outfile.close();
}

// this method creates queries to use when testing the backwards search
void utils::createQueries(string input, string output, int setSize, double percentageRandom) {
	srand(time(NULL));

	cout << "Creating queries for file " << output << endl;

	fstream infile;
	infile.open(input);
	int querySize = 60, r;

	if (infile.is_open()) {
		ofstream outfile;
		outfile.open(output);

		string genome;
		getline(infile, genome);
		int length = genome.size();
		int numRandom = setSize * percentageRandom;
		int numNotRandom = setSize - numRandom;

		for (int i = 0; i < numNotRandom; i++) {
			r = rand() % (length - querySize);
			if (r < 0 || r >= length - querySize) {
				cout << "Out of bounds, and r is: " << r << endl;
			}
			outfile << genome.substr(r, querySize) << endl;
		}
		for (int i = 0; i < numRandom; i++) {
			outfile << getRandomQuery(querySize);
			if (i != numRandom - 1) {
				outfile << endl;
			}
		}
	} else {
		cout << "The input file could not be opened." << endl;
	}
}

string utils::getRandomQuery(int n) {

	string ret = "";
	int r;
	for (int i = 0; i < n; i++) {
		r = rand() % 4;
		if (r == 0) {
			ret += "A";
		}
		else if (r == 1) {
			ret += "C";
		}
		else if (r == 2) {
			ret += "G";
		}
		else if (r == 3) {
			ret += "T";
		}
	}
	return ret;
}

// the benchmark method for rank_support
void utils::testRankSupport(string& fname) {

	cout << "In testRankSupport, about to test with file: " << fname << endl;

	/* initialize random seed: */
	srand(time(NULL));
	int r;

	bit_vector b(fname);
	rank_support rs(b);
	auto start = chrono::steady_clock::now();
	for (int i = 0; i < 1000000; i++) {
		r = rand() % b.getLength();
		rs.rank1(r);
	}
	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	cout << "Overhead is: " << rs.overhead() << endl;

	rs.freeMemory();
	b.freeMemory();
}

// the benchmark method for select
void utils::testSelectSupport(string& fname) {

	cout << "In testSelectSupport, about to test with file: " << fname << endl;

	/* initialize random seed: */
	srand(time(NULL));
	int r;

	bit_vector b(fname);
	rank_support rs(b);
	select_support ss(rs);

	auto start = chrono::steady_clock::now();
	for (int i = 0; i < 1000000; i++) {
		r = rand() % b.getLength()/2;
		ss.select1(r);
	}
	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	cout << "Overhead is: " << rs.overhead() << endl;

	rs.freeMemory();
	b.freeMemory();
}
