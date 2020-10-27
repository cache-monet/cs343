#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

using namespace std;


#ifdef BUSY                            // busy waiting implementation
template<typename T>
class BoundedBuffer {
    uOwnerLock buf_lock;
    uCondLock cons_lock;
    uCondLock prod_lock;
    const unsigned int size; // max capacity of buffer
    T* container; // fixed size container
    int front = 0, rear = 0; // track where to insert and remove
    unsigned int cur_size = 0 ; // current size of container
  public:
    BoundedBuffer( const unsigned int size = 10 );
    ~BoundedBuffer();
    void insert( T elem );
    T remove();
};

template<typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size) : size(size) {
  container = new T[size]; // resize to full capacity
}

template<typename T>
BoundedBuffer<T>::~BoundedBuffer() {
  delete container;
};

template<typename T>
void BoundedBuffer<T>::insert( T elem ) {
    // Only one consumer (exclusive) OR  producer can be use the buffer so we don't need additional locks for 
    // front, rear, cur_size
    buf_lock.acquire();
    while (cur_size == size) prod_lock.wait(buf_lock); // busy wait while container is full
    // insert
    container[front] = elem;
    front = (front + 1) % size; cur_size++;
    cons_lock.signal(); // alert consumer
    buf_lock.release();
};

template<typename T>
T BoundedBuffer<T>::remove() {
    buf_lock.acquire();
    T retval;
    while (cur_size == 0) cons_lock.wait(buf_lock); // busy wait while container is full
    retval = container[rear];
    rear = (rear + 1) % size; cur_size--; // update queue vals
    prod_lock.signal(); // alert producer
    buf_lock.release();
    return retval;
};
#endif // BUSY

#ifdef NOBUSY                          // no busy waiting implementation
template<typename T>
class BoundedBuffer {
    uOwnerLock buf_lock;
    uCondLock cons_lock;
    uCondLock prod_lock;
    
    uCondLock barge_lock;
    bool cons_signal = false;
    bool prod_signal = false;
    bool barge_signal = false;

    T* container; // fixed size container
    const unsigned int size; // max capacity of buffer
    unsigned int cur_size = 0 ; // current size of container
    int front = 0, rear = 0; // track where to insert and remove

  public:
    BoundedBuffer( const unsigned int size = 10 );
    ~BoundedBuffer();
    void insert( T elem );
    T remove();
};

template<typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size) : size(size) {
  container = new T[size]; // dynamically allocate container size
}

template<typename T>
BoundedBuffer<T>::~BoundedBuffer() {
  delete container;
};

template<typename T>
void BoundedBuffer<T>::insert( T elem ) {
    buf_lock.acquire();

    if (cons_signal || prod_signal || barge_signal) { //.cant' barge through
      barge_lock.wait(buf_lock);
      // wakes upon receiving signal
      if (barge_lock.empty()) barge_signal = false;
    }

    if (cur_size == size) { // full buffer need to wait for consumers
      if (!(cons_signal || barge_lock.empty())) barge_lock.signal(); // no consumers working; bargers could be consumers
      prod_lock.wait(buf_lock);
    }

    // push items
    container[front] = elem;
    front = (front + 1) % size; cur_size++;

    if (!cons_lock.empty()) { // consumers has priority over bargers
      cons_signal = true;
      cons_lock.signal(); // alert consumer
    } else if (!barge_lock.empty()) {
      barge_signal = true;
      barge_lock.signal(); // alert consumer
    } 
    prod_signal = false;
    buf_lock.release();
};

template<typename T>
T BoundedBuffer<T>::remove() {
    buf_lock.acquire();
    if (cons_signal || prod_signal || barge_signal) { //.cant' barge through
      barge_lock.wait(buf_lock);
      if (barge_lock.empty()) barge_signal = false;
    }

    if (cur_size == 0) { // empty buffer wait for producers
      if (!(prod_signal || barge_lock.empty())) barge_lock.signal(); // bargers could be producers
      cons_lock.wait(buf_lock);
    }

    // set return value
    T retval;
    retval = container[rear];
    rear = (rear + 1) % size; cur_size--; // update queue vals

    if (!prod_lock.empty()) { // consumers has priority over bargers
      prod_signal = true;
      prod_lock.signal(); // alert consumer
    } else if (!barge_lock.empty()) { // some bargers are consumers
      barge_signal = true;
      barge_lock.signal(); // alert barger
    }
    cons_signal = false; // signal finished
    buf_lock.release();
    return retval;
};
#endif // NOBUSY

#endif // BOUNDED_BUFFER_H