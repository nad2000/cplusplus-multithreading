// Compilation:
// clang++ -Wall -std=c++20 -lpthread  hello_world_mt_2.cpp && ./a.out
/* demonstration of the data races using shared data
 * mutex - mutual exclusion (not a synchronization):
 * https://www.youtube.com/watch?v=jkRN9zcLH1s
 * */

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
const int ITERATIONS = 1E6;

// Resource acquisition is initialization (RAII)
// Acquire the lock (resource) at the initialization and when it
// goes out of scope, it gets released.

// callable object is an instance of a class that overrides call method
class App {
private:
  mutex mtx;
  int count = 0;

public:
  void operator()() {

    // mtx.lock(); // not a good idea - threads wouldn't interleave
    for (int i = 0; i < ITERATIONS; i++) {
      lock_guard<mutex> guard(mtx); // the guard attempts to acquire the lock
      // unique_guard<mutex> guard(mtx); // richer functionality
      ++count; // critical section...
      // the section ends here and the lock gets released
    }
    // mtx.unlock();
  }
  int getCount() { return count; }
};

int main() {

  App app;

  // std::ref - reference wrapper
  thread t1(ref(app));
  cout << "COUNT: " << app.getCount() << endl;
  thread t2(ref(app));
  cout << "COUNT: " << app.getCount() << endl;

  t1.join();
  t2.join();

  cout << "COUNT: " << app.getCount() << endl;

  return 0;
}
