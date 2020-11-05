#include <iostream>
#include <vector>
#include "MPRNG.h"
#include "q3buffer.h"
#include "q3consumer.h"
#include "q3producer.h"

using namespace std;

MPRNG mprng; // globally defined mprng

int main (int argc, char * argv[] ) {
    int cons = 5, prod = 5;
    int produce = 3;
    int buf_size = 10;
    int delay = -1;
    int processors = 1;
    try {
        switch ( argc ) {
            case 7: processors = stoi(argv[6]); if (processors < 1) throw 1;
            case 6: delay = stoi(argv[5]); if (delay < 1 ) throw 1;
            case 5: buf_size = stoi(argv[4]); if (buf_size < 1) throw 1;
            case 4: produce = stoi(argv[3]); if (produce < 1 ) throw 1;
            case 3: prod = stoi(argv[2]); if (prod < 1 ) throw 1;
            case 2: cons = stoi(argv[1]); if (cons < 1 ) throw 1;
        }
    } catch (...) {
        cout << "Usage:" << argv[0] << "[ Cons (> 0) [ Prods (> 0) [ Produce (> 0) [ BufferSize (> 0) [ Delay (> 0) [ Processors (> 0) ] ] ] ] ] ]" << endl;
		exit ( 1 );
    }
    uProcessor p[processors - 1] __attribute__(( unused )); // multi-processor
    if (delay == -1) delay = cons + prod; // delay value not specified; use default
    
    int total_sum = 0;
    int counters[cons] = {0};
    // create buffer producers and consumers
    BoundedBuffer<int> buffer(buf_size);

    vector<Producer*> producers;
    for (int i = 0; i < prod; i++) producers.push_back(new Producer(buffer, produce, delay));
    vector<Consumer*> consumers;
    for (int i = 0; i < cons; i++) consumers.push_back(new Consumer(buffer, delay, SENTINEL, counters[i])); // getting sentinel from compiler

    // Critical path is the producer task that takes the longest to terminate
    for (int i = 0; i < prod; i++ ) {
  	    delete producers[i]; // use magic in delete to wait until Producer tasks have finished
    }
    for (int i = 0; i < cons; i++ ) {
        buffer.insert(SENTINEL); // get 
    }
    for (int i = 0; i < cons; i++ ) {
  	    delete consumers[i];
        // cout << "i: " << i << " sum: " << counters[i] << endl;
        total_sum += counters[i];
    }
    cout << "total: " << total_sum << endl;
} // main
// end of file