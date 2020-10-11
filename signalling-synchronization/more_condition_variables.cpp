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

    cout << "(t1) acquiring lock" << endl;
    unique_lock<mutex> lock(mtx);
    cout << "(t1) acquired lock" << endl;
    ready = true;
    cout << "(t1) releasing lock" << endl;
    lock.unlock(); // unique_lock needs only unlock
    cout << "(t1) released lock; notifying the main thread" << endl;
    condition.notify_one(); // notifies the waiting thread
    cout << "(t1) notified the main thread" << endl;
  });

  cout << "(main) acquiring lock" << endl;
  unique_lock<mutex> lock(mtx);
  cout << "(main) acquired lock; waiting..." << endl;
  condition.wait(
      lock, [&ready]() { return ready; }); // this releases the lock and waits
  cout << "(main) finished waiting" << endl;
  // for other thread to notify...
  cout << "ready " << ready << endl;
  // cout << fmt::format("ready {}\n", ready);

  t1.join();
  return 0;
}
