#include <iostream>
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
  queue<E> _queue;

public:
  void push(E e) { _queue.push(e); }
  E pop() {
    auto e = _queue.front();
    _queue.pop();
    return e;
  }
  BlockingQueue(int max_size) : _max_size(max_size) {}
};

int main() {

  BlockingQueue<int> q(10);

  thread t1(&BlockingQueue<int>::push, &q, 7); // we could call bind too
  thread t2(&BlockingQueue<int>::pop, &q);

  t1.join();
  t2.join();

  return 0;
}

