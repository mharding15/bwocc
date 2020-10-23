#include "rank_support.h"

rank_support::rank_support() {
}

// constructor, will create the entire rank data structure
rank_support::rank_support(bit_vector& bitvector) {

	bit_vector bv = bitvector;
	bvLength = bv.getLength();
	if (bvLength == 0) {
		return;
	}

	superBlockSize = ceil(log2(bvLength)) * ceil(log2(bvLength)) / 2;
	if (superBlockSize == 0) {
		superBlockSize = 1;
	}
	blockSize = ceil(log2(bvLength)) / 2;
	if (blockSize == 0) {
		blockSize = 1;
	}

	numSuperBlocks = bvLength / superBlockSize, numBlocks = bvLength / blockSize;

	if (bvLength % superBlockSize != 0) {
		numSuperBlocks++;
	}
	if (bvLength % blockSize != 0) {
		numBlocks++;
	}

	blocksPerSuperBlock = superBlockSize / blockSize;
	if (superBlockSize % blockSize != 0) {
		blocksPerSuperBlock++;
	}

	r_s = new int[numSuperBlocks] {};
	r_b = new int[numBlocks] {};
	r_p = new int[bv.getLength()]{};

	int rsTotal = 0, rsIdx = 0, rbTotal = 0, rbIdx = 0, rpTotal = 0;
	for (int i = 0; i < bv.getLength(); i++) {
		// beginning of a superblock
		if (i % superBlockSize == 0) {
			r_s[rsIdx++] = rsTotal;
			rbTotal = 0;
		}
		// beginning of a block
		if (i % blockSize == 0) {
			r_b[rbIdx++] = rbTotal;
			rpTotal = 0;
		}

		rsTotal += bv.getBit(i);
		rbTotal += bv.getBit(i);
		rpTotal += bv.getBit(i);

		r_p[i] = rpTotal;
	}
}

// this method returns the rank of 1's at the give index
uint64_t rank_support::rank1(uint64_t i) {
	// make sure that the position is within the bounds of the bitvector
	if (i >= bvLength) {
		cout << "Position " << i << " is out of bounds of the bit vector." << endl;
		return -1;
	}

	// get the index of the superblock and block
	int superBlock = i / superBlockSize;
	int block = superBlock * blocksPerSuperBlock + (i % superBlockSize) / blockSize;

	return r_s[superBlock] + r_b[block] + r_p[i];
}

// this method returns the rank of 0's at the given index
uint64_t rank_support::rank0(uint64_t i) {
	// make sure that the position is within the bounds of the bitvector
	if (i >= bvLength) {
		cout << "Position " << i << " is out of bounds of the bit vector." << endl;
		return -1;
	}

	return i - rank1(i) + 1;
}

// this method returns the number of bits that are required to store the rank_support data structure
uint64_t rank_support::overhead() {
	uint64_t numBits = 0;
	for (int i = 0; i < numSuperBlocks; i++) {
		if (r_s[i] == 0) {
			numBits += 1;
		} else {
			numBits += ceil(log2(r_s[i]));
		}
	}
	for (int i = 0; i < numBlocks; i++) {
		if (r_b[i] == 0) {
			numBits += 1;
		} else {
			numBits += ceil(log2(r_b[i]));
		}
	}
	for (int i = 0; i < bvLength; i++) {
		if (r_p[i] == 0) {
			numBits += 1;
		} else {
			numBits += ceil(log2(r_p[i]));
		}
	}
	return numBits;
}

// save the rank support data structure
void rank_support::save(string& fname) {
	ofstream outfile;
	outfile.open(fname);
	// store the auxilary data about the bitvector, the super blocks and the blocks
	outfile << bvLength << endl;
	outfile << superBlockSize << endl;
	outfile << numSuperBlocks << endl;
	outfile << blockSize << endl;
	outfile << numBlocks << endl;
	outfile << blocksPerSuperBlock << endl;

	// store the values for the super blocks
	for (int i = 0; i < numSuperBlocks; i++) {
		outfile << r_s[i];
		if (i != numSuperBlocks - 1) {
			outfile << " ";
		} else {
			outfile << endl;
		}
	}

	// store the values for the blocks
	for (int i = 0; i < numBlocks; i++) {
		outfile << r_b[i];
		if (i != numBlocks - 1) {
			outfile << " ";
		}else {
			outfile << endl;
		}
	}

	// store the values for the lookup table
	for (int i = 0; i < bvLength; i++) {
		outfile << r_p[i];
		if (i != bvLength - 1) {
			outfile << " ";
		}else {
			outfile << endl;
		}
	}
	outfile.close();
}

// load the rank support data structure from a file
void rank_support::load(string& fname) {
	ifstream infile;
	infile.open(fname, ios::in);

	if (infile.is_open()) {
		string line;

		getline(infile, line);
		bvLength = stoi(line);

		getline(infile, line);
		superBlockSize = stoi(line);

		getline(infile, line);
		numSuperBlocks = stoi(line);

		getline(infile, line);
		blockSize = stoi(line);

		getline(infile, line);
		numBlocks = stoi(line);

		getline(infile, line);
		blocksPerSuperBlock = stoi(line);

		// get the superblock values
		string delimiter = " ";
		getline(infile, line);
		r_s = new int[numSuperBlocks] {};
		// go through the line and get all values (delimited by spaces)
		size_t pos = 0;
		for (int i = 0; i < numSuperBlocks; i++) {
			pos = line.find(delimiter);
			r_s[i] = stoi(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}

		// get the block values
		getline(infile, line);
		r_b = new int[numBlocks] {};
		// go through the line and get all values (delimited by spaces)
		pos = 0;
		for (int i = 0; i < numBlocks; i++) {
			pos = line.find(delimiter);
			r_b[i] = stoi(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}

		// get the final lookup table values
		getline(infile, line);
		r_p = new int[bvLength]{};
		// go through the line and get all values (delimited by spaces)
		pos = 0;
		for (int i = 0; i < bvLength; i++) {
			pos = line.find(delimiter);
			r_p[i] = stoi(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());
		}
	} else {
		cout << "Problem opening the file..." << endl;
		return;
	}
}

// this method is mostly used to store the rank_support structure in a different object
string rank_support::toString() {
	string ret = "";
	ret +=  bvLength + "\n";
	ret += superBlockSize + "\n";
	ret += numSuperBlocks + "\n";
	ret += blockSize + "\n";
	ret += numBlocks + "\n";
	ret += blocksPerSuperBlock + "\n";

	// read the values for the super blocks
	for (int i = 0; i < numSuperBlocks; i++) {
		ret += r_s[i];
		if (i != numSuperBlocks - 1) {
			ret += " ";
		}
		else {
			ret += "\n";
		}
	}
	// read the values for the blocks
	for (int i = 0; i < numBlocks; i++) {
		ret += r_b[i];
		if (i != numBlocks - 1) {
			ret += " ";
		}
		else {
			ret += "\n";
		}
	}
	// read the values for the lookup table
	for (int i = 0; i < bvLength; i++) {
		ret += r_p[i];
		if (i != bvLength - 1) {
			ret += " ";
		}
		else {
			ret += "\n";
		}
	}
	return ret;
}

// mostly for debugging purposes
void rank_support::print() {
	if (r_s == NULL) {
		cout << "The superblocks are not initialized..." << endl;
		return;
	}
	if (r_b == NULL) {
		cout << "The blocks are not initialized..." << endl;
		return;
	}
	if (r_p == NULL) {
		cout << "The final lookup tables are not initialized..." << endl;
		return;
	}
	cout << "The bit vector length is: " << bvLength << endl;
	cout << "There are " << blocksPerSuperBlock << " blocks per super block." << endl;
	cout << "The superblockSize is: " << superBlockSize << " and there are " << numSuperBlocks << " super blocks." << endl;
	cout << "Super blocks are: " << endl;
	for (int i = 0; i < numSuperBlocks; i++) {
		cout << r_s[i] << " ";
	}
	cout << endl;

	cout << "The blockSize is: " << blockSize << " and there are " << numBlocks << " blocks." << endl;
	cout << "The blocks are:" << endl;
	for (int i = 0; i < numBlocks; i++) {
		cout << r_b[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < bvLength; i++) {
		cout << r_p[i] << " ";
	}
	cout << endl;
}

int rank_support::getBvLength() {
	return bvLength;
}

// destructor to free the memory used
void rank_support::freeMemory() {
	delete[] r_s;
	delete[] r_b;
	delete[] r_p;
}
