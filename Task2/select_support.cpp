#include "select_support.h"

select_support::select_support() {
}

select_support::select_support(rank_support& rs) {
	this->rs = rs;
}

// uses the rank_select to do a binary search
uint64_t select_support::select1(uint64_t i) {
	uint64_t length = rs.getBvLength();
	uint64_t left = 0, mid = length/2, right = length - 1;
	uint64_t response = 0;

	while (true) {
		uint64_t rank = rs.rank1(mid);
		// the ith 1 must be before the current position
		if (i < rank) {
			if (left == mid) {
				break;
			} else {
				right = mid;
				mid = (left + mid) / 2;
			}
		// the ith 1 must be after the current position
		} else if (i > rank) {
			if (mid == right) {
				break;
			}else {
				left = mid + 1;
				mid = (left + right) / 2;
			}
		// we could be at the ith 1, but could also be at a 0 that follows the ith 1
		} else {
			// the ith 1 has been found
			if (mid == 0 || rs.rank1(mid - 1) == (i - 1)) {
				response = mid;
				break;
			// we are getting close, but must be at a 0 after the ith 1
			} else {
				right = mid;
				mid = (left + mid) / 2;
			}
		}
	}
	return response;
}

uint64_t select_support::select0(uint64_t i) {
	uint64_t length = rs.getBvLength();
	uint64_t left = 0, mid = length / 2, right = length;
	uint64_t response = 0;

	while (true) {
		uint64_t rank = rs.rank0(mid);
		// the ith 0 must be before the current position
		if (i < rank) {
			if (left == mid) {
				break;
			}
			else {
				right = mid;
				mid = (left + mid) / 2;
			}
		// the ith 0 must be after the current position
		} else if (i > rank) {
			if (mid == right) {
				break;
			}
			else {
				left = mid + 1;
				mid = (left + right) / 2;
			}
		// we could be at the ith 0, but could also be at a 1 that follows the ith 0
		} else {
			// the ith 0 has been found
			if (mid == 0 || rs.rank0(mid - 1) == (i - 1)) {
				response = mid;
				break;
			// we are getting close, but must be at a 1 after the ith 0
			}
			else {
				right = mid;
				mid = (left + mid) / 2;
			}
		}
	}

	return response;
}

uint64_t select_support::overhead() {
	return rs.overhead();
}

void select_support::save(string& fname) {
	rs.save(fname);
}

void select_support::load(string& fname) {
	rs.load(fname);
}

void select_support::freeMemory() {
	rs.freeMemory();
}
