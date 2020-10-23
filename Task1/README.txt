README for Programming Task 1

I did not incorporate a command line interface for this task, but the bit_vector and rank_support classes can be used in a program as follows:

The bit_vector class has many forms of initilization. There is a default no param constructor which can be used to create an empty bit_vector object, which you can then initialize with the "setVec()" method by passing it a string containing the bits you want to include in the bitstring (e.g. "001010110011"). There is a construct that takes a length and a string which constists of a space separated list of integers, with each one being converted into a 32 bit "block" of the final bitvector. There is also a constructor very similar to the one that takes length, and the string of ints, except that it takes a file with the length on the first line and then the int list on the second line. Finally there is also a constructor that takes a length only, and initializes the bitvector to be all 0s.

The bit_vector class has the following methods which can be called:
	- int getBit(int pos): returns either a 1 or 0 based on the value of the bit at "pos"
	- void setBit(int pos): set the bit at "pos" to 1
	- void unsetBit(int pos): set the bit at "pos" to 0
	- save(string& fname): save the bit vector to a file
	- load(string& fname): loads the bit vector from a file
	- freeMemory(): The bit_vector allocates memeory for the bitsets it uses to store the full bit vector. Calling this method will free the memory used. 

The rank_support class has a constructor which takes as an argument a bit_vector object. The rank_support structure is then created in this constructor. It also has the following methods which can be used:
	- uint64_t rank1(uint64_t i): get the number of 1's in the bit vector up to and including the given index.
	- uint64_t rank0(uint64_t i): get the number of 0's in the bit vector up to and including the given index.
	- uint64_t overhead(): get the number of bits necessary to store the rank_support structure.
	- void save(string& fname): save the rank_support structure to a file.
	- void load(string& fname): load the rank_support structure from a file.