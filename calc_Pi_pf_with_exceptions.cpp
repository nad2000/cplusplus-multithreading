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
  promise<double> p;
  auto do_calculation = [&p](int terms) {
    try {
      auto result = calculate_pi(terms);
      p.set_value(result);
    } catch (const exception &e) { // OR catch(...)
      cerr << "ERROR(do_calculation): " << e.what() << endl;
      // promise should be kept
      p.set_exception(current_exception());
    }
  };

  thread t1(do_calculation, 0);

  future<double> f = p.get_future();
  try {
    cout << setprecision(15) << f.get() << endl;
  } catch (const exception &e) {
    cerr << "ERROR(main): " << e.what() << endl;
  }

  t1.join();
  return 0;
}
