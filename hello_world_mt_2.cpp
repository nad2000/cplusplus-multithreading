// Compilation clang++ -Wall -std=c++20 -lpthread  hello_world_mt.cpp && ./a.out

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

void work() {
  for (int i = 0; i < 10; i++) {
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Loop " << i << endl;
  }
}

int main() {
  thread t1(work);
  thread t2(work);

  t1.join();
  t2.join();

  return 0;
}
