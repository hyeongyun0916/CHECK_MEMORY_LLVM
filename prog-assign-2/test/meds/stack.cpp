#include <iostream>
#include "../../pass/runtime/loaop.hpp"
using namespace std;

// extern int logop(int);

struct Temp {
  int i;
};

int main(int argc, char **argv) {
  // cout << "start" << endl;
  /*
  struct temp *A;
  double **B;
  unsigned char **q;
  float *str = new float[6];
  */

  // *str = 'c';
  // delete[] str;
  // *str = 'b';
  // cout << *str << endl;
  float f[3];
  f[0] = 0.1;
  char x[5];
  // logop(1);
  x[0] = 'a';
  // x[1] = 'b';
  // delete[] x;
  // cout << x[0] << endl;
  // printf("%p\t%c\n", x, *(x+1));
  /*
  char b[10];
  char c[10];
  char d[10];
  char e[10];
  char f[10];
  char g[10];
  char h[10];
  char i[10];
  */
  // a[argc * 32] = 'a';
  // CHECK: {{AddressSanitizer}}
  return 0;
}
