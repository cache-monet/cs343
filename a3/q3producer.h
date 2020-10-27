#ifndef PRODUCER_H
#define PRODUCER_H

extern MPRNG mprng; // use globally defined mprng

_Task Producer {
    BoundedBuffer<int> &buffer;
    int produce;
    int delay;
    void main();
  public:
    Producer ( BoundedBuffer<int> & buffer, const int produce, const int delay);
};

Producer::Producer ( BoundedBuffer<int> & buffer, const int produce, const int delay)
    : buffer(buffer), produce(produce), delay(delay) {};

void Producer::main() {
    for (int i = 1; i <= produce; i++) {
        yield(mprng(delay)); //randomly yield between 0 to delay times
        buffer.insert(i); // insert val into buffer
    }
};
#endif // PRODUCER_H