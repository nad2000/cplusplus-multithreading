#include <chrono>
#include <cmath>
#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

int main() {

  // bool ready = false;
  // thread_local bool ready = false;  -- each thread would get an own copy
  atomic<bool> ready = false;
  thread t1([&]() {
    this_thread::sleep_for(chrono::milliseconds(2000));
    ready = true;
  });
  // main thread waits till the work gets done
  while (!ready) {
    this_thread::sleep_for(chrono::milliseconds(200));
  }
  cout << "ready " << ready << endl;
  t1.join();
  return 0;
}
