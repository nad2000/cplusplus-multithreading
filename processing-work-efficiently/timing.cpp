#include <chrono>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

mutex mtx;

double calculate_pi(int terms, int start, int skip) {
  double pi = 0.0;
  for (int i = start; i < terms; i += skip) {
    int sign = pow(-1, i);
    double term = 1.0 / (i * 2 + 1);
    pi += sign * term;
  }
  return pi * 4.0;
}

int main() {

  vector<shared_future<double>> futures;
  const int CONCURRENCY = thread::hardware_concurrency();
  // const int CONCURRENCY = 1;

  auto start = chrono::steady_clock::now();
  for (int i = 0; i < CONCURRENCY; i++) {
    shared_future<double> f =
        async(launch::async, calculate_pi, 1E8, i, CONCURRENCY);
    futures.push_back(f);
  }

  double sum = 0.0;
  for (auto f : futures) {
    auto res = f.get();
    /* unique_lock<mutex> lock(mtx); */
    /* cout << "Returned: " << setprecision(15) << res << endl; */
    /* lock.unlock(); */
    sum += res;
  }

  auto end = chrono::steady_clock::now();
  auto duration =
      chrono::duration_cast<chrono::milliseconds>(end - start).count();
  cout << "PI:         " << setprecision(15) << sum << endl;
  cout << "COMPARE PI: " << setprecision(15) << M_PI << endl;
  cout << "DURATION:   " << setprecision(15) << duration << endl;
  return 0;
}
