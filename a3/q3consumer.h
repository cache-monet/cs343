#ifndef CONSUMER_H
#define CONSUMER_H
#include <iostream>
using namespace std;

extern MPRNG mprng;

_Task Consumer {
    BoundedBuffer<int> &buffer;
    int delay;
    int sentinel;
    int& sum;
    void main();
  public:
    Consumer(BoundedBuffer<int> &buffer, const int delay, const int sentinel, int &sum);
};

Consumer::Consumer(BoundedBuffer<int> &buffer, const int delay, const int sentinel, int &sum)
    : buffer(buffer), delay(delay), sentinel(sentinel), sum(sum) {}

void Consumer::main() {
  for (;;) {
    yield(mprng(delay));
    int val = buffer.remove();
    if (val == sentinel) return; // received signal to terminate task
    sum += val; // 
  }
}

#endif //CONSUMER_H 