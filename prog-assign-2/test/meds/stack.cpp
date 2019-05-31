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
  /*
  float *fp = new float[999];
  // *fp = 1.0;
  cout << "temp" << endl;
  delete[] fp;
  */
  // fp[0] = 0.2;
  /*
  char *str;
  *str = 'c';
  delete str;
  */
  // *str = 'b';
  // cout << *str << endl;
  /*
  float fa[3];
  // fa[0] = 0.1;
  int ia[5];
  // logop(1);
  ia[0] = 1;
  // x[1] = 'b';
  // delete[] x;
  cout << ia << endl;
  */
  // printf("%p\t%c\n", x, *(x+1));
  // int *b;
  int a[10];
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
  //long long b = 10;
  
  a[1] = 5;
  cout << "a[1]: " << a[1] << endl;
  a[argc * 32] = 1;
  cout << "a[" << argc << " * 32]: " << a[argc * 32] << endl;
  cout << "main: " << a << "\t" << &a[1] << endl;
  
  // CHECK: {{AddressSanitizer}}
  return 0;
}