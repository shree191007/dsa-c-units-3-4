/* Unit 3 - Heap (min/max chosen by a comparison callback), heapify, build-heap,
   heap sort, decrease/update key, and a Priority Queue built on the heap */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*cmp_fn)(int parent, int child);     /* return >0 if parent must stay above child */
int max_cmp(int p, int c) { return p >= c; }
int min_cmp(int p, int c) { return p <= c; }

typedef struct { int *a; int size, cap; cmp_fn ok; } heap_t;

void h_init(heap_t *h, int cap, cmp_fn f) { h->a = malloc(cap * sizeof(int)); h->size = 0; h->cap = cap; h->ok = f; }
void h_free(heap_t *h) { free(h->a); h->a = NULL; h->size = h->cap = 0; }
static void swap(int *x, int *y) { int t = *x; *x = *y; *y = t; }

void sift_up(heap_t *h, int i) {
    while (i > 0 && !h->ok(h->a[(i - 1) / 2], h->a[i])) { swap(&h->a[i], &h->a[(i - 1) / 2]); i = (i - 1) / 2; }
}
void sift_down(int *a, int n, int i, cmp_fn ok) {              /* "heapify" one node */
    for (;;) {
        int l = 2 * i + 1, r = l + 1, top = i;
        if (l < n && !ok(a[top], a[l])) top = l;
        if (r < n && !ok(a[top], a[r])) top = r;
        if (top == i) return;
        swap(&a[i], &a[top]); i = top;
    }
}
int h_insert(heap_t *h, int v) {
    if (h->size == h->cap) { h->cap *= 2; h->a = realloc(h->a, h->cap * sizeof(int)); }
    h->a[h->size] = v; sift_up(h, h->size++); return 1;
}
int h_peek(heap_t *h, int *out) { if (!h->size) return 0; *out = h->a[0]; return 1; }
int h_extract(heap_t *h, int *out) {
    if (!h->size) return 0;
    *out = h->a[0]; h->a[0] = h->a[--h->size]; sift_down(h->a, h->size, 0, h->ok); return 1;
}
void h_build(heap_t *h, const int *src, int n) {               /* bottom-up O(n) build */
    h->size = 0;
    while (h->cap < n) { h->cap *= 2; h->a = realloc(h->a, h->cap * sizeof(int)); }
    memcpy(h->a, src, n * sizeof(int)); h->size = n;
    for (int i = n / 2 - 1; i >= 0; i--) sift_down(h->a, n, i, h->ok);
}
int h_update(heap_t *h, int idx, int nv) {                      /* change key at idx (increase or decrease) */
    if (idx < 0 || idx >= h->size) return 0;
    h->a[idx] = nv; sift_up(h, idx); sift_down(h->a, h->size, idx, h->ok); return 1;
}
int h_delete_at(heap_t *h, int idx) {
    if (idx < 0 || idx >= h->size) return 0;
    h->a[idx] = h->a[--h->size];
    if (idx < h->size) { sift_up(h, idx); sift_down(h->a, h->size, idx, h->ok); }
    return 1;
}
void h_print(heap_t *h) { for (int i = 0; i < h->size; i++) printf("%d ", h->a[i]); puts(""); }

/* turn an arbitrary array into min/max heap according to callback */
void heapify_array(int *a, int n, cmp_fn ok) { for (int i = n / 2 - 1; i >= 0; i--) sift_down(a, n, i, ok); }

/* heap sort: ascending uses max-heap, descending uses min-heap */
void heap_sort(int *a, int n, int ascending) {
    cmp_fn f = ascending ? max_cmp : min_cmp;
    heapify_array(a, n, f);
    for (int e = n - 1; e > 0; e--) { swap(&a[0], &a[e]); sift_down(a, e, 0, f); }
}

/* ------------------ Priority queue (min priority number = served first) ------------------ */
typedef struct { int prio; char name[32]; } item_t;
typedef struct { item_t *a; int size, cap; } pq_t;

void pq_init(pq_t *q, int cap) { q->a = malloc(cap * sizeof(item_t)); q->size = 0; q->cap = cap; }
void pq_free(pq_t *q) { free(q->a); }
static void iswap(item_t *x, item_t *y) { item_t t = *x; *x = *y; *y = t; }
void pq_enqueue(pq_t *q, int prio, const char *name) {
    if (q->size == q->cap) { q->cap *= 2; q->a = realloc(q->a, q->cap * sizeof(item_t)); }
    int i = q->size++;
    q->a[i].prio = prio; strncpy(q->a[i].name, name, 31); q->a[i].name[31] = 0;
    while (i > 0 && q->a[(i - 1) / 2].prio > q->a[i].prio) { iswap(&q->a[i], &q->a[(i - 1) / 2]); i = (i - 1) / 2; }
}
int pq_dequeue(pq_t *q, item_t *out) {
    if (!q->size) return 0;
    *out = q->a[0]; q->a[0] = q->a[--q->size];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < q->size && q->a[l].prio < q->a[m].prio) m = l;
        if (r < q->size && q->a[r].prio < q->a[m].prio) m = r;
        if (m == i) break;
        iswap(&q->a[i], &q->a[m]); i = m;
    }
    return 1;
}

int main(void) {
    int data[] = {15, 3, 17, 10, 84, 19, 6, 22, 9};
    int n = 9, v;

    heap_t mx, mn; h_init(&mx, 4, max_cmp); h_init(&mn, 4, min_cmp);
    for (int i = 0; i < n; i++) { h_insert(&mx, data[i]); h_insert(&mn, data[i]); }
    printf("Max-heap array: "); h_print(&mx);
    printf("Min-heap array: "); h_print(&mn);
    h_peek(&mx, &v); printf("max=%d ", v); h_peek(&mn, &v); printf("min=%d\n", v);
    printf("Extract from max-heap: "); while (h_extract(&mx, &v)) printf("%d ", v); puts("");

    heap_t b; h_init(&b, 2, min_cmp); h_build(&b, data, n);
    printf("Bottom-up built min-heap: "); h_print(&b);
    h_update(&b, 3, 1); printf("After update idx3 -> 1  : "); h_print(&b);
    h_delete_at(&b, 0); printf("After delete root       : "); h_print(&b);

    int s[] = {15, 3, 17, 10, 84, 19, 6, 22, 9};
    heap_sort(s, n, 1); printf("Heap sort ascending : "); for (int i = 0; i < n; i++) printf("%d ", s[i]); puts("");
    heap_sort(s, n, 0); printf("Heap sort descending: "); for (int i = 0; i < n; i++) printf("%d ", s[i]); puts("");

    pq_t q; pq_init(&q, 2);
    pq_enqueue(&q, 3, "write report"); pq_enqueue(&q, 1, "fix outage"); pq_enqueue(&q, 2, "code review"); pq_enqueue(&q, 5, "lunch");
    item_t it; puts("Priority queue order:");
    while (pq_dequeue(&q, &it)) printf("  [%d] %s\n", it.prio, it.name);
    h_free(&mx); h_free(&mn); h_free(&b); pq_free(&q);
    return 0;
}
