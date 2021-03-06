#include <chrono>
#include <condition_variable>
#include <exception>
// #include <fmt/format.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

int main() {

  // bool ready = false;
  // thread_local bool ready = false;  -- each thread would get an own copy
  // atomic<bool> ready = false;
  condition_variable condition;
  mutex mtx;
  bool ready = false;

  thread t1([&]() {
    this_thread::sleep_for(chrono::milliseconds(2000));
    unique_lock<mutex> lock(mtx);
    ready = true;
    lock.unlock();          // unique_lock needs only unlock
    condition.notify_one(); // notifies the waiting thread
  });

  unique_lock<mutex> lock(mtx);
  while (!ready) {
    condition.wait(lock); // this releases the lock and waits
    // for other thread to notify...
  }
  cout << "ready " << ready << endl;
  // cout << fmt::format("ready {}\n", ready);

  t1.join();
  return 0;
}
