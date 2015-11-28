#include <unistd.h>
#include <algorithm>

#define MAXD 256

using namespace std;

void generatePrime(int* prime, int& idxPrime) {
  bool isPrime[MAXD+1];
  memset(isPrime, true, sizeof(isPrime));
  for (int i=2; i*i<=MAXD; i++) {
    if (isPrime[i]) {
      int k = 3, j = i * 2;
      while (j <= MAXD) {
        isPrime[j] = false;
        j = i * k;
        ++k;
      }
    }
  }
  for (int i=2; i<=MAXD; i++)
    if (isPrime[i]) prime[++idxPrime] = i;
}

int gcd(int a, int b) {
  while (b != 0) {
    int c = a % b;
    a = b;
    b = c;
  }
  return a;
}

void generate_e_d(int& e, int& d, int& n, bool verbose) {
  int prime[MAXD+1];
  int idxPrime = -1;
  if (verbose) printf("    --> Generating primes...");  
  generatePrime(prime, idxPrime);
  if (verbose) printf(" ok\n");

  srand(time(0));
  if (verbose) printf("    --> Generating p and q...");
  int a = prime[rand() % idxPrime + 1];
  int b;
  do {
    b = prime[rand() % idxPrime + 1];
  } while (a == b);
  if (verbose) {
    printf(" ok\n");
    printf("          p = %d\n", a);
    printf("          q = %d\n", b);
  }

  if (verbose) printf("    --> Calculating n and m...");
  n = a * b;
  int m = (a-1) * (b-1);
  a = b = 0;
  if (verbose) {
    printf(" ok\n");
    printf("          n = %d\n", n);
    printf("          m = %d\n", m);
  }
  
  if (verbose) printf("    --> Generating e and d... ok\n");
  int gcd_e;
  do {
    e = prime[rand() % idxPrime + 1];
    gcd_e = gcd(e, m);
    if (verbose) {
      printf("          * Trying e with %d...", e);
      if (e >= m || gcd_e != 1) printf(" no\n");
    }
  } while (e >= m || gcd_e != 1);
  if (verbose) printf(" yes\n");

  int k = 0, mod_d;
  do {
    ++k;
    mod_d = (1 + (k * m)) % e;
    d = (1 + (k * m)) / e;
    if (verbose) {
      printf("          * Trying d with %d...", d);
      if (mod_d != 0) printf(" no\n");
    }
  } while (mod_d != 0);
  if (verbose)  {
    printf(" yes\n\n");
    printf("          e = %d\n", e);
    printf("          d = %d\n", d);
  }
}

