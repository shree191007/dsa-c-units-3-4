/* Unit 3 (experiential learning) - callbacks in C: mymap, myfilter, myreduce
   + uses: digit sum, reverse digits, odd filter, prime filter, sum/max reduce */
#include <stdio.h>
#include <stdlib.h>

typedef int (*map_fn)(int);
typedef int (*pred_fn)(int);
typedef int (*reduce_fn)(int acc, int x);

void mymap(const int *in, int *out, int n, map_fn f) { for (int i = 0; i < n; i++) out[i] = f(in[i]); }

/* returns number of elements kept */
int myfilter(const int *in, int *out, int n, pred_fn p) {
    int k = 0;
    for (int i = 0; i < n; i++) if (p(in[i])) out[k++] = in[i];
    return k;
}
int myreduce(const int *in, int n, int init, reduce_fn f) {
    int acc = init;
    for (int i = 0; i < n; i++) acc = f(acc, in[i]);
    return acc;
}

int digit_sum(int x) { int s = 0; x = abs(x); while (x) { s += x % 10; x /= 10; } return s; }
int reverse_num(int x) { int s = x < 0 ? -1 : 1, r = 0; x = abs(x); while (x) { r = r * 10 + x % 10; x /= 10; } return s * r; }
int is_odd(int x) { return x % 2 != 0; }
int is_prime(int x) { if (x < 2) return 0; for (int i = 2; i * i <= x; i++) if (x % i == 0) return 0; return 1; }
int add(int a, int b) { return a + b; }
int max2(int a, int b) { return a > b ? a : b; }

static void show(const char *t, const int *a, int n) { printf("%-22s: ", t); for (int i = 0; i < n; i++) printf("%d ", a[i]); puts(""); }

int main(void) {
    int in[] = {123, 45, 7, 982, 13, 100, 29, 64};
    int n = 8, out[8], k;
    show("input", in, n);
    mymap(in, out, n, digit_sum);   show("digit sums", out, n);
    mymap(in, out, n, reverse_num); show("reversed numbers", out, n);
    k = myfilter(in, out, n, is_odd);   show("odd only", out, k);
    k = myfilter(in, out, n, is_prime); show("primes only", out, k);
    printf("%-22s: %d\n", "sum (reduce)", myreduce(in, n, 0, add));
    printf("%-22s: %d\n", "max (reduce)", myreduce(in, n, in[0], max2));
    return 0;
}
