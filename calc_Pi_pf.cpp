#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

// with promises and futures
//
using namespace std;

double calculate_pi(int terms) {
  double pi = 0.0;
  for (int i = 0; i < terms; i++) {
    int sign = pow(-1, i);
    double term = 1.0 / (i * 2 + 1);
    pi += sign * term;
  }
  return pi * 4.0;
}

int main() {
  promise<double> p;
  auto do_calculation = [&p](int terms) {
    auto result = calculate_pi(terms);
    p.set_value(result);
  };
  thread t1(do_calculation, 1E7);

  future<double> f = p.get_future();
  cout << setprecision(15) << f.get() << endl;

  t1.join();
  return 0;
}
