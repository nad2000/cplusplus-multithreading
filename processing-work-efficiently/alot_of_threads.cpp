#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

mutex mtx;

int work(int id) {
  unique_lock<mutex> lock(mtx);
  cout << "Starting " << id << endl;
  lock.unlock();

  this_thread::sleep_for(chrono::seconds(1));
  return id;
}

int main() {
  vector<shared_future<int>> futures;
  for (int i = 0; i < std::thread::hardware_concurrency() * 3; i++) {
    shared_future<int> f = async(launch::async, work, i);
    futures.push_back(f);
  }

  for (auto f : futures) {
    auto res = f.get();
    unique_lock<mutex> lock(mtx);
    cout << "Returned: " << res << endl;
    lock.unlock();
  }
  return 0;
}

