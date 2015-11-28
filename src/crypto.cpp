#include <stdio.h>
#include <string>

#include "generate.h"
#include "utility.h"

#define square(X) (X)*(X)

using namespace std;

int bigmod(int b, int p, int m) {
  if (p == 0) return 1;
  else if (p%2 == 0) return square(bigmod(b, p/2, m)) % m;
  else return ((b % m) * bigmod(b, p-1, m)) % m;
}

int encrypt(string f1, string f2, int e, int n) {
  FILE *fin, *fout;
  fin = fopen(f1.c_str(), "r");
  fout = fopen(f2.c_str(), "w");
  if (fin == NULL || fout == NULL) return 0;
  char ch;
  int counter = 0;
  while (fscanf(fin, "%c", &ch) != EOF) {
    int dx = ch;
    int bgmod = bigmod(dx, e, n);
    if (counter != 0) fprintf(fout, " ");
    ++counter;
    fprintf(fout, "0x%X", bgmod);
  }
  fprintf(fout, "\n");
  fclose(fin);
  fclose(fout);
  return 1;
}

int decrypt(string f1, string f2, int d, int n) {
  FILE *fin, *fout;
  fin = fopen(f1.c_str(), "r");
  fout = fopen(f2.c_str(), "w");
  if (fin == NULL || fout == NULL) return 0;
  char st[500], hex[33];
  while (fscanf(fin, " %s", st) != EOF) {
    int idx = 0, p = 0;
    for (int i=0; i<strlen(st); i++) {
      if (p) {
        hex[idx] = st[i];
        hex[idx+1] = '\0';
        ++idx;
      }
      if (st[i] == 'x') {
        hex[idx] = '\0';
        p = 1;
      }
    }
    int dec = hexToDec(hex);
    int bgmod = bigmod(dec, d, n);
    fprintf(fout, "%c", (char)bgmod);
  }
  fclose(fin);
  fclose(fout);
  return 1;
}

