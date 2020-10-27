#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <iostream>
using namespace std;

#ifdef CBEGIN
#include <uCobegin.h>

template<typename T>
void quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ) {
    if (high == (unsigned int)-1) return; // sorting empty arr (length-1 underflowed)
    uThisTask().verify();

    auto swap = [](T& a, T& b) {
        T temp = a;
        a = b; b = temp;
    };

	unsigned int i = low, j = high;
	T pivot = values[low + (high - low) / 2];
	// partition
	while (i <= j ) {
		while (values[i] < pivot) ++i;
		while (values[j] > pivot) --j;
        // cout << "i: " << i << " j: " << j << endl;
		if (i <= j) {
			swap(values[i], values[j]);
            ++i;
            if (j != 0 ) --j; // watch out for unsigned underflow
		}
	}
	// recursion
    if (depth == 0) { // execute sequentially
	    if (j > low ) quicksort(values, low, j, depth);
        if (i < high) quicksort(values, i, high, depth);
    } else { 
        COBEGIN // create a thread for each partition
            BEGIN if (j > low ) quicksort(values, low, j, depth-1); END
            BEGIN if (i < high) quicksort(values, i, high, depth-1); END
        COEND
    }
}
#endif // CBEGIN

#ifdef ACTOR
#include <uActor.h>

template<typename T>
void quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ) {
    if (high == (unsigned int)-1) return; // sorting empty arr (length-1 underflowed)
    struct SortMsg : public uActor::Message { 
        T* values;
        unsigned int low;
        unsigned int high;
        unsigned int depth;
        SortMsg( T* values, unsigned int low, unsigned int high, unsigned int depth) : Message(uActor::Delete),
            values(values), low(low), high(high), depth(depth){}
    }; // SortMsg

    _Actor Sort {
        uActor::Allocation receive( uActor::Message & msg ) { // receive base type
            // move partitioning and swap into one function
            function <tuple<unsigned int, unsigned int>(T*, unsigned int, unsigned int)> partition = [&](
                T* values, unsigned int low, unsigned int high
            ) {
                auto swap = [](T& a, T& b) {
                    T temp = a;
                    a = b; b = temp;
                };
                unsigned int i = low, j = high;
                T pivot = values[i + (j - i) / 2];
	            while (i <= j ) {
		            while (values[i] < pivot) ++i;
		            while (values[j] > pivot) --j;
		            if (i <= j) {
                        // swap values
                        swap(values[i], values[j]);
                        ++i;
                        if (j != 0 ) --j; // watch out for unsigned underflow
		            }
	            }
                return tuple(i, j);
            };
            // used when depth = 0; finish running the partition (and any recursive calls) on current actor
            function<void(T*, unsigned int, unsigned int)> recurse = [&](T* values, unsigned int low, unsigned int high) {
                auto [i, j] = partition(values, low, high);
                if (j > low ) recurse(values, low, j);
                if (i < high) recurse(values, i, high);
            };

            Case( SortMsg, msg ) {
                T* values = msg_d->values;
                unsigned int low = msg_d->low, high = msg_d->high, depth = msg_d->depth;
                if (depth == 0) { // execute sequentially
                    recurse(values, low, high);
                } else { 
                    auto [i, j] = partition(values, low, high);
                    // create a new actor for each partition
                    if (j > low ) *new Sort() | *new SortMsg(values, low, j, depth-1) | uActor::stopMsg;
                    if (i < high) *new Sort() | *new SortMsg(values, i, high, depth-1) | uActor::stopMsg;
                }
            } else Case( uActor::StopMsg, msg ) return uActor::Delete; // delete actor
            return uActor::Nodelete;
        }
    }; // Sort
    // start the engine
    uActorStart();
    *new Sort() | *new SortMsg(values, low, high, depth) | uActor::stopMsg;
    uActorStop();
} // quicksort
#endif // ACTOR

#ifdef TASK
template<typename T>
void quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ) {
    if (high == (unsigned int)-1) return; // sorting empty arr (length-1 underflowed)
    _Task QuickSort {
        T* values;
        unsigned int low;
        unsigned int high;
        unsigned int depth;

        void swap (T& a, T& b) {
            T temp = a;
            a = b; b = temp;
        } // swap

        tuple<unsigned int, unsigned int> partition(unsigned int low, unsigned int high) {
	        unsigned int i = low, j = high;
	        T pivot = values[low + (high - low) / 2];
	        // partition
	        while (i <= j ) {
		        while (values[i] < pivot) ++i;
		        while (values[j] > pivot) --j;
		        if (i <= j) {
			        swap(values[i], values[j]);
                    ++i;
                    if (j != 0 ) --j; // watch out for unsigned underflow
		        }
	        }
            return tuple(i, j);
        } // partition

        void recurse(unsigned int low, unsigned int high) {
            auto [i, j] = partition(low, high);
            if (j > low ) recurse(low, j);
            if (i < high) recurse(i, high);
        } // recurse

        void main() {
            uThisTask().verify();
            // do both partitions on current task
            if (depth == 0) {
                recurse(low, high);
            } else {
                auto [i, j] = partition(low, high);
                if (j > low) QuickSort left(values, low, j, depth-1); // WLOG create a new task for the left partition
                if (i < high) recurse(i, high); // resume right partition on current task
            }
        } // main
      public:
        // constructor
        QuickSort(T* values, unsigned int low, unsigned int high, unsigned int depth) :
            values(values), low(low), high(high), depth(depth) {}
    }; // QuickSort

    QuickSort init(values, low, high, depth);
}

#endif // TASK

#endif // QUICKSORT_H
