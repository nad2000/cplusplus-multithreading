#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

double calculate_pi(int terms) {
  double pi = 0.0;
  for (int i = 0; i < terms; i++) {
    int sign = pow(-1, i);
    double term = 1.0 / (i * 2 + 1);
    // if (i & 0x1)
    //   pi -= term;
    // else
    //   pi += term;
    pi += sign * term;
  }
  return pi * 4.0;
}

int main() {
  cout << setprecision(15) << calculate_pi(1E7) << endl;
  return 0;
}
