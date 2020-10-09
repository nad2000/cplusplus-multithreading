#include <cmath>
#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

// with promises and futures
//
using namespace std;

double calculate_pi(int terms) {
  if (terms < 1)
    throw runtime_error("Terms cannot be less than 1");
  double pi = 0.0;
  for (int i = 0; i < terms; i++) {
    int sign = pow(-1, i);
    double term = 1.0 / (i * 2 + 1);
    pi += sign * term;
  }
  return pi * 4.0;
}

int main() {

  packaged_task<double(int)> task1(calculate_pi);
  future<double> f = task1.get_future();
  // since we don't need to refer any more we can use 'move'
  // to create a shallow copy...
  thread t1(move(task1), 1E7);
  t1.join();

  try {
    cout << setprecision(15) << f.get() << endl;
  } catch (const exception &e) {
    cerr << "ERROR(main): " << e.what() << endl;
  }

  return 0;
}
