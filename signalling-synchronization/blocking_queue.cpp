#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// Java has a blocking threadsafe queue..
// And the 'Boost' library also have already built-in blocking
// queue.

using namespace std;

// an object/function etc is said to be 'thread safe'
// if it can be used by multiple threads...
template <typename E> class BlockingQueue {
private:
  int _max_size;
  condition_variable _cond;
  mutex _mtx;
  queue<E> _queue;

public:
  int size() {
    lock_guard<mutex> lock(_mtx); // lock the queue
    return _queue.size();
  }
  void push(E e) {
    unique_lock<mutex> lock(_mtx);
    _cond.wait(lock, [this]() { return _queue.size() < _max_size; });
    _queue.push(e);

    lock.unlock();
    _cond.notify_one();
  }
  E front() {

    unique_lock<mutex> lock(_mtx);
    _cond.wait(lock, [this]() { return !_queue.empty(); });
    return _queue.front();
  }
  void pop() {

    unique_lock<mutex> lock(_mtx);
    _cond.wait(lock, [this]() { return !_queue.empty(); });

    _queue.pop();

    lock.unlock();
    _cond.notify_one();
  }
  BlockingQueue(int max_size) : _max_size(max_size) {}
};

int main() {

  BlockingQueue<int> q(3);

  // Producer (thread):
  // thread t1(&BlockingQueue<int>::push, &q, 7); // we could call bind too
  thread t1([&q]() {
    for (int i = 0; i < 10; i++) {
      cout << "PUSH(" << q.size() << "): " << i << endl;
      q.push(i);
    }
  });
  // Consumer (thread):
  // thread t2(&BlockingQueue<int>::pop, &q);
  thread t2([&q]() {
    for (int i = 0; i < 10; i++) {
      int e = q.front();
      q.pop();
      cout << "POP(" << q.size() << "): " << e << endl;
    }
  });

  t1.join();
  t2.join();

  return 0;
}

