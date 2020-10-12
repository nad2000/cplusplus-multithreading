#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

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

mutex mtx;

int work(int id) {
  unique_lock<mutex> lock(mtx);
  cout << "Starting " << id << endl;
  lock.unlock();

  int seconds = int((5.0 * rand()) / RAND_MAX) + 1;
  this_thread::sleep_for(chrono::seconds(seconds));
  return id;
}

int main() {
  BlockingQueue<shared_future<int>> futures(2);
  // if the work is CPU intensive then better use hardware_concurrency()-1
  // if you are waiting for some external resources, better to use x n threads
  int worker_count = thread::hardware_concurrency() * 3;

  thread main([&]() {
    for (int i = 0; i < worker_count; i++) {
      shared_future<int> f = async(launch::async, work, i);
      futures.push(f);
    }
  });

  for (int i = 0; i < worker_count; i++) {
    auto f = futures.front();
    futures.pop();
    auto res = f.get();
    unique_lock<mutex> lock(mtx);
    cout << "Returned: " << res << endl;
    lock.unlock();
  }

  main.join();
  return 0;
}

