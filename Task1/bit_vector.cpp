#include "bit_vector.h"

bit_vector::bit_vector() {
}

bit_vector::bit_vector(string& fname) {

	fstream infile;
	infile.open(fname);
	string line;
	if (infile.is_open()) {
		getline(infile, line);
		length = stoi(line);
		getline(infile, line);
	} else {
		cout << "Could not open the file." << endl;
		return;
	}

	numBlocks = length / BLOCK_SIZE;
	if (length % BLOCK_SIZE != 0) {
		numBlocks++;
	}

	bvec = new bitset<BLOCK_SIZE>[numBlocks] {};

	// go through the line and get all values (delimited by spaces)
	string delimiter = " ";
	size_t pos = 0;
	for (int i = 0; i < numBlocks; i++) {
		pos = line.find(delimiter);
		bvec[i] = bitset<BLOCK_SIZE>(stoul(line.substr(0, pos)));
		line.erase(0, pos + delimiter.length());
	}
}

// this constructor takes the length of the vector and a space delimited list of ints, each representing 32 bits of the bit vector
bit_vector::bit_vector(int len, string vec){

	length = len;
	numBlocks = length / BLOCK_SIZE;
	if (length % BLOCK_SIZE != 0) {
		numBlocks++;
	}

	bvec = new bitset<BLOCK_SIZE>[numBlocks] {};

	// go through the line and get all values (delimited by spaces)
	string delimiter = " ";
	size_t pos = 0;
	for (int i = 0; i < numBlocks; i++) {
		pos = vec.find(delimiter);
		bvec[i] = bitset<BLOCK_SIZE>(stoul(vec.substr(0, pos)));
		vec.erase(0, pos + delimiter.length());
	}
}

// this constructor makes a bitvector of some size initialized to all 0's
bit_vector::bit_vector(int size) {
	if (size <= 0) {
		cout << "Need at least one bit in the bit_vector" << endl;
		return;
	}

	length = size;
	numBlocks = size / BLOCK_SIZE;
	if (size % BLOCK_SIZE != 0) {
		numBlocks++;
	}

	bvec = new bitset<BLOCK_SIZE>[numBlocks] {};

	// for each block create a new bitset that is set to all 0's
	for (int i = 0; i < numBlocks; i++) {
		int start = i * BLOCK_SIZE;
		bvec[i] = bitset<BLOCK_SIZE>();
	}
}

// would have made this another constructor, but I already had a constructor that had just a string param
void bit_vector::setVec(string bitstring) {
	int len = bitstring.size();
	if (len == 0) {
		cout << "The bit vector cannot be empty." << endl;
		return;
	}

	// this is the length of the bit vector, not the number of blocks we will need
	length = len;

	numBlocks = len / BLOCK_SIZE;
	// if this causes issues (I don't see why it would) I can set it back to just above + 1
	if (len % BLOCK_SIZE != 0) {
		numBlocks++;
	}
	bvec = new bitset<BLOCK_SIZE>[numBlocks] {};

	// for each block, need to reverse the corresponding block in the string so that the first string bit will be at position 0
	for (int i = 0; i < numBlocks; i++) {
		int start = i * BLOCK_SIZE;
		bvec[i] = bitset<BLOCK_SIZE>(reverseString(bitstring.substr(start, BLOCK_SIZE)));
	}
}

// prints a bitstring representation of the bit vector
void bit_vector::printVec() {
	if (bvec == NULL) {
		cout << "The bitvector is not set." << endl;
		return;
	}

	string bitstring = "";
	int remainder = length % BLOCK_SIZE;
	for (int i = 0; i < numBlocks; i++) {
		string currString = reverseString(bvec[i].to_string());
		if (i == (numBlocks - 1) && remainder != 0) {
			currString = currString.substr(0, remainder);
		}
		bitstring += currString;
	}

	cout << "The bit vector is:" << endl;
	cout << bitstring << endl;
}

// simply reverses the given string, used when printing the bitvector so it will make it more readable
string bit_vector::reverseString(string st) {
	string reversed = "";
	for (int i = st.size() - 1; i >= 0; i--) {
		reversed += st.at(i);
	}
	return reversed;
}

// this function returns 0 or 1 based on whatever the bit at the given index is
int bit_vector::getBit(int pos){
	if (pos >= length || pos < 0) {
		cout << "Position " << pos << " is out of bounds." << endl;
		return -1;
	}
	int block = pos / BLOCK_SIZE, posInBlock = pos % BLOCK_SIZE;
	if (bvec[block].test(posInBlock)) {
		return 1;
	} else {
		return 0;
	}
}

// set the bit at the give index to 1
void bit_vector::setBit(int pos) {
	if (pos >= length || pos < 0) {
		cout << "Position " << pos << " is out of bounds. Bit was not set." << endl;
		return;
	}
	int block = pos / BLOCK_SIZE, posInBlock = pos % BLOCK_SIZE;
	bvec[block].set(posInBlock);
}

// set the bit at the given index to 0
void bit_vector::unsetBit(int pos) {
	if (pos > length || pos < 0) {
		cout << "Position " << pos << " is out of bounds. Bit was not set." << endl;
		return;
	}
	int block = pos / BLOCK_SIZE, posInBlock = pos % BLOCK_SIZE;
	bvec[block].reset(posInBlock);
}

int bit_vector::getLength() {
	return length;
}

int bit_vector::getNumBlocks() {
	return numBlocks;
}

// saves the bit vector to a file
void bit_vector::save(string& fname) {
	ofstream outfile;
	outfile.open(fname);
	cout << "Saving the length and it is: " << length << endl;
	outfile << length << endl;
	cout << "Saving numBlocks and it is: " << numBlocks << endl;
	outfile << numBlocks << endl;
	for (int i = 0; i < numBlocks; i++) {
		outfile << bvec[i].to_ulong();
		if (i != numBlocks - 1) {
			outfile << " ";
		}
	}
	outfile.close();
}

// loads the bit vector from a file
void bit_vector::load(string& fname) {
	ifstream infile;
	infile.open(fname, ios::in);

	string line;
	getline(infile, line);
	length = stoi(line);

	getline(infile, line);
	numBlocks = stoi(line);

	bvec = new bitset<BLOCK_SIZE>[numBlocks] {};
	if (infile.is_open()) {
		string delimiter = " ";
		getline(infile, line);
		// go through the line and get all values (delimited by spaces)
		size_t pos = 0;
		for (int i = 0; i < numBlocks; i++) {
			pos = line.find(delimiter);
			bvec[i] = bitset<BLOCK_SIZE>(stoi(line.substr(0, pos)));
			line.erase(0, pos + delimiter.length());
		}
	} else {
		cout << "Problem opening the file..." << endl;
		return;
	}
}

// this function is basically just used to assist in storing bit vectors for use in other objects
string bit_vector::toString() {
	string ret = "";
	for (int i = 0; i < numBlocks; i++) {
		ret += to_string(bvec[i].to_ulong());
		if (i != numBlocks - 1) {
			ret += " ";
		} else {
			ret += "\n";
		}
	}
	return ret;
}

// need to free the memory used buy the bit vector
void bit_vector::freeMemory() {
	delete[] bvec;
}

// this method helps in creating bit vectors that can be used to test rank_support and select_support
void bit_vector::createTestFiles() {
	ofstream outfile;
	int size = 32;

	/* initialize random seed: */
	srand(time(NULL));

	while (size < 2000000) {
		cout << "Size is: " << size << endl;
		int nums = size / BLOCK_SIZE;
		string fname = "bv_test_" + to_string(size) + ".txt";
		outfile.open(fname);

		outfile << size << endl;

		cout << "Creating " << nums << " random numbers." << endl;
		for (int i = 0; i < nums; i++) {
			outfile << rand();
			if (i < nums - 1) {
				outfile << " ";
			}
		}
		outfile.close();
		size *= 2;
	}

}
