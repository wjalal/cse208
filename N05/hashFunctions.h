#include <cstdlib>
#include <ctime>

using namespace std;

long long int power (long long int a, long long int b) {
    if (b == 0) return 1;
    long long int x = power (a, b/2);
    x *= x;
    if (b%2 == 1) x *= a;
    return x;
};

unsigned int h2 (char* a, int N) {
    unsigned long long int x = 5381;
    while (int c = *a++) x = ((x << 5) + x) + c;
    return (unsigned int) (x % N);
};

unsigned int h1 (char* a, int N) {
    int i, z = 31;
    long long int x = 0;
    for (i=0; a[i]!='\0'; i++) x += a[i] * power (z, i);
    return (unsigned int) (x);
};

