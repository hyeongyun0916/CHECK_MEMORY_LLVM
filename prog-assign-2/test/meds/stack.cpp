#include <iostream>
using namespace std;

struct Temp {
  int i;
};

int main(int argc, char **argv) {
  // cout << "start" << endl;
  /*
  struct temp *A;
  double **B;
  unsigned char **q;
  */
  float *fp = new float[6];
  // fp[0] = 0.2;

  // *str = 'c';
  // delete[] str;
  // *str = 'b';
  // cout << *str << endl;
  float fa[3];
  // fa[0] = 0.1;
  char ca[5];
  // logop(1);
  ca[0] = 0xfff;
  // x[1] = 'b';
  // delete[] x;
  cout << ca[0] << endl;
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