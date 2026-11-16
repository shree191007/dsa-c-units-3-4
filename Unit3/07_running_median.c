/* Unit 3 (experiential learning) - Running median using a max-heap (lower half) and a min-heap (upper half).
   Heap ordering is chosen through a comparison callback. */
#include <stdio.h>
#include <stdlib.h>

typedef int (*cmp_fn)(int parent, int child);
int max_ok(int p, int c) { return p >= c; }
int min_ok(int p, int c) { return p <= c; }

typedef struct { int *a; int size; cmp_fn ok; } heap_t;
void init(heap_t *h, int cap, cmp_fn f) { h->a = malloc(cap * sizeof(int)); h->size = 0; h->ok = f; }
void deinit(heap_t *h) { free(h->a); }
static void swp(int *x, int *y) { int t = *x; *x = *y; *y = t; }
void push(heap_t *h, int v) {
    int i = h->size++; h->a[i] = v;
    while (i > 0 && !h->ok(h->a[(i - 1) / 2], h->a[i])) { swp(&h->a[i], &h->a[(i - 1) / 2]); i = (i - 1) / 2; }
}
int pop(heap_t *h) {
    int top = h->a[0]; h->a[0] = h->a[--h->size]; int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < h->size && !h->ok(h->a[m], h->a[l])) m = l;
        if (r < h->size && !h->ok(h->a[m], h->a[r])) m = r;
        if (m == i) break;
        swp(&h->a[i], &h->a[m]); i = m;
    }
    return top;
}

typedef struct { heap_t lo, hi; } median_t;      /* lo = max-heap, hi = min-heap */
void med_init(median_t *m, int cap) { init(&m->lo, cap, max_ok); init(&m->hi, cap, min_ok); }
void med_free(median_t *m) { deinit(&m->lo); deinit(&m->hi); }
void med_add(median_t *m, int x) {
    if (m->lo.size == 0 || x <= m->lo.a[0]) push(&m->lo, x); else push(&m->hi, x);
    if (m->lo.size > m->hi.size + 1) push(&m->hi, pop(&m->lo));
    else if (m->hi.size > m->lo.size) push(&m->lo, pop(&m->hi));
}
double med_get(median_t *m) {
    if (m->lo.size > m->hi.size) return m->lo.a[0];
    return (m->lo.a[0] + m->hi.a[0]) / 2.0;
}

int main(void) {
    int stream[] = {5, 15, 1, 3, 8, 7, 9, 10, 20, 2};
    int n = sizeof stream / sizeof *stream;
    median_t m; med_init(&m, n);
    printf("%-8s %s\n", "added", "median");
    for (int i = 0; i < n; i++) { med_add(&m, stream[i]); printf("%-8d %.1f\n", stream[i], med_get(&m)); }
    med_free(&m);
    return 0;
}
