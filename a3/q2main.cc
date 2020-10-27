#include <iostream>
#include "q2quicksort.h"

using namespace std;

int main(int argc, char * argv[]) {
	int tasks = 1, times = 10000000;
	bool printExtras = !getenv("NO_PRINT_EXTRAS");
	try {
		switch (argc) {
			case 3:
				times = stoi( argv[2] ); if ( times <= 0 ) throw 1;
			case 2:
				tasks = stoi( argv[1] ); if ( tasks <= 0 ) throw 1;
		} // switch
	} catch (...) {
		cout << "Usage: " << argv[0] << " [ tasks (> 0) [times (> 0) ] ]" << endl;
		exit(1);
	} // try
	int arr[7] = {0, 5, 3, 10, 4, 1, 2};
	quicksort(arr, 0, 6, 2); // offset recursive call
	for (int i = 0; i < 7; i++) {
		cout <<  arr[i];
		if (i != 6) cout << ", ";
	}
	cout << endl;
	// uProcessor p[(1<<depth) - 1].
	// {
	// 	Worker
	// }
}

// template<typename T>

// pivot = array[low + ( high - low ) / 2];

// unsigned int uDefaultStackSize() {
// 	return 512 * 1000;		// set all task stack-size to 512K
// }

// uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads

// uTime start = uClock::currTime();
// quicksort( ... );
// cout << "Sort time " << uClock::currTime() - start << " sec." << endl;