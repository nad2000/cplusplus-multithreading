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

void work(int &count, mutex &mtx) {
  // mtx.lock(); // not a good idea - threads wouldn't interleave
  for (int i = 0; i < ITERATIONS; i++) {
    mtx.lock();
    ++count; // critical section...
    mtx.unlock();
  }
  // mtx.unlock();
};

int main() {
  int count = 0;
  mutex mtx;

  // std::ref - reference wrapper
  thread t1(work, ref(count), ref(mtx));
  cout << "COUNT: " << count << endl;
  thread t2(work, ref(count), ref(mtx));
  cout << "COUNT: " << count << endl;

  t1.join();
  t2.join();

  cout << "COUNT: " << count << endl;

  return 0;
}
