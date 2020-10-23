README for Programming Task 2

I did not incorporate a command line interface for this task, but to use the select_support class you can simply include "select_support.h". The constructor for the select_support class takes a rank_support object as a parameter. It uses this object to do a binary search using rank queries on the bit vector, which is referenced in the rank_support object. You can then call the following methods on the select_support object:
	- uint64_t select1(uint64_t i): get the index associated with the 1 rank given by i.
	- uint64_t select0(uint64_t i): get the index associated with the 0 rank given by i.
	- uint64_t overhead(): get the amount of bits necessary to store the select_support data structure.
	- save(string& fname): save the select_support structure to a file.
	- load(string& fname): load teh select_support structure from a file.
