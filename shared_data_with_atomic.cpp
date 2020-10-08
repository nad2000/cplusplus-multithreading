// Compilation:
// clang++ -Wall -std=c++20 -lpthread  hello_world_mt_2.cpp && ./a.out
/* demonstration of the data races using shared data */

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

int main() {
  atomic<int> count = 0;
  const int ITERATIONS = 1E6;
  thread t1([&count]() {
    for (int i = 0; i < ITERATIONS; i++)
      ++count;
  });
  cout << "COUNT: " << count << endl;
  thread t2([&count]() {
    for (int i = 0; i < ITERATIONS; i++)
      ++count;
  });
  cout << "COUNT: " << count << endl;

  t1.join();
  t2.join();

  cout << "COUNT: " << count << endl;

  return 0;
}
