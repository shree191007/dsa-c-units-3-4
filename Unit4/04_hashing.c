/* Unit 4 - Hashing
   Open addressing: linear probing, quadratic probing, double hashing (insert / search / delete with tombstones)
   Closed addressing: separate chaining. Load factor + rehashing, plus string hashing (djb2, folding). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { EMPTY = 0, FULL = 1, DELETED = 2 };
typedef enum { LINEAR, QUADRATIC, DOUBLE } mode_t_;

typedef struct { int key; int state; } slot_t;
typedef struct { slot_t *t; int size, count; mode_t_ mode; } htable_t;

static int is_prime(int n) { if (n < 2) return 0; for (int i = 2; i * i <= n; i++) if (n % i == 0) return 0; return 1; }
static int next_prime(int n) { while (!is_prime(n)) n++; return n; }
static int prev_prime(int n) { while (n > 2 && !is_prime(n)) n--; return n; }

void ht_init(htable_t *h, int size, mode_t_ m) {
    h->size = next_prime(size); h->count = 0; h->mode = m;
    h->t = calloc(h->size, sizeof(slot_t));
}
void ht_free(htable_t *h) { free(h->t); }

static int h1(htable_t *h, int k) { int r = k % h->size; return r < 0 ? r + h->size : r; }
static int h2(htable_t *h, int k) { int R = prev_prime(h->size - 1); int r = k % R; if (r < 0) r += R; return R - r; }
static int probe(htable_t *h, int k, int i) {                    /* i-th probe position */
    int base = h1(h, k);
    switch (h->mode) {
        case LINEAR:    return (base + i) % h->size;
        case QUADRATIC: return (int)(((long)base + (long)i * i) % h->size);
        default:        return (int)(((long)base + (long)i * h2(h, k)) % h->size);
    }
}
double ht_load(htable_t *h) { return (double)h->count / h->size; }
int ht_search(htable_t *h, int k) {                              /* returns slot index or -1 */
    for (int i = 0; i < h->size; i++) {
        int p = probe(h, k, i);
        if (h->t[p].state == EMPTY) return -1;
        if (h->t[p].state == FULL && h->t[p].key == k) return p;
    }
    return -1;
}
int ht_insert(htable_t *h, int k);
static void rehash(htable_t *h) {
    htable_t n; ht_init(&n, h->size * 2, h->mode);
    for (int i = 0; i < h->size; i++) if (h->t[i].state == FULL) ht_insert(&n, h->t[i].key);
    free(h->t); *h = n;
    printf("   [rehash -> size %d]\n", h->size);
}
int ht_insert(htable_t *h, int k) {                              /* 1 ok, 0 duplicate */
    if (ht_search(h, k) >= 0) return 0;
    double lim = (h->mode == QUADRATIC) ? 0.5 : 0.7;             /* quadratic guaranteed to find slot only if load<0.5 & prime size */
    if ((double)(h->count + 1) / h->size > lim) rehash(h);
    for (int i = 0; i < h->size; i++) {
        int p = probe(h, k, i);
        if (h->t[p].state != FULL) { h->t[p].key = k; h->t[p].state = FULL; h->count++; return 1; }
    }
    rehash(h); return ht_insert(h, k);
}
int ht_delete(htable_t *h, int k) {
    int p = ht_search(h, k);
    if (p < 0) return 0;
    h->t[p].state = DELETED; h->count--; return 1;
}
void ht_print(htable_t *h) {
    for (int i = 0; i < h->size; i++) {
        printf("  [%2d] ", i);
        if (h->t[i].state == FULL) printf("%d\n", h->t[i].key); else if (h->t[i].state == DELETED) puts("<deleted>"); else puts("-");
    }
}

/* ------------------------- separate chaining ------------------------- */
typedef struct chain { int key; struct chain *next; } chain_t;
typedef struct { chain_t **b; int size, count; } chtable_t;

void ch_init(chtable_t *h, int size) { h->size = next_prime(size); h->count = 0; h->b = calloc(h->size, sizeof(chain_t *)); }
static int ch_hash(chtable_t *h, int k) { int r = k % h->size; return r < 0 ? r + h->size : r; }
int ch_search(chtable_t *h, int k) { for (chain_t *c = h->b[ch_hash(h, k)]; c; c = c->next) if (c->key == k) return 1; return 0; }
int ch_insert(chtable_t *h, int k) {
    if (ch_search(h, k)) return 0;
    int i = ch_hash(h, k); chain_t *n = malloc(sizeof *n); n->key = k; n->next = h->b[i]; h->b[i] = n; h->count++; return 1;
}
int ch_delete(chtable_t *h, int k) {
    chain_t **p = &h->b[ch_hash(h, k)];
    while (*p && (*p)->key != k) p = &(*p)->next;
    if (!*p) return 0;
    chain_t *t = *p; *p = t->next; free(t); h->count--; return 1;
}
void ch_rehash(chtable_t *h) {
    chtable_t n; ch_init(&n, h->size * 2);
    for (int i = 0; i < h->size; i++) for (chain_t *c = h->b[i], *t; c; c = t) { t = c->next; ch_insert(&n, c->key); free(c); }
    free(h->b); *h = n;
}
void ch_print(chtable_t *h) {
    for (int i = 0; i < h->size; i++) { printf("  [%2d] ", i); for (chain_t *c = h->b[i]; c; c = c->next) printf("%d -> ", c->key); puts("NULL"); }
}
void ch_free(chtable_t *h) { for (int i = 0; i < h->size; i++) for (chain_t *c = h->b[i], *t; c; c = t) { t = c->next; free(c); } free(h->b); }

/* ------------------------- string hash functions ------------------------- */
unsigned long djb2(const char *s) { unsigned long h = 5381; int c; while ((c = (unsigned char)*s++)) h = ((h << 5) + h) + c; return h; }
unsigned folding(const char *s, int m) { unsigned sum = 0; for (; *s; s++) sum += (unsigned char)*s; return sum % m; }
unsigned poly(const char *s, int m) { unsigned long h = 0; for (; *s; s++) h = (h * 31 + (unsigned char)*s) % m; return (unsigned)h; }

static void run_open(const char *name, mode_t_ m) {
    int keys[] = {23, 43, 1, 87, 35, 12, 56, 78, 99, 100, 7, 18};
    htable_t h; ht_init(&h, 11, m);
    printf("== %s (initial size %d) ==\n", name, h.size);
    for (int i = 0; i < 12; i++) ht_insert(&h, keys[i]);
    ht_print(&h);
    printf("   load factor %.2f; search 87 -> slot %d; search 55 -> %d\n", ht_load(&h), ht_search(&h, 87), ht_search(&h, 55));
    ht_delete(&h, 87); printf("   after delete 87: search 87 -> %d, search 100 -> %d\n", ht_search(&h, 87), ht_search(&h, 100));
    ht_free(&h);
}
int main(void) {
    run_open("Linear probing", LINEAR);
    run_open("Quadratic probing", QUADRATIC);
    run_open("Double hashing", DOUBLE);

    chtable_t c; ch_init(&c, 7);
    int keys[] = {50, 700, 76, 85, 92, 73, 101, 14, 21};
    for (int i = 0; i < 9; i++) ch_insert(&c, keys[i]);
    puts("== Separate chaining (size 7) =="); ch_print(&c);
    printf("   search 76: %d, delete 85: %d, search 85: %d\n", ch_search(&c, 76), ch_delete(&c, 85), ch_search(&c, 85));
    ch_rehash(&c); puts("   after rehash:"); ch_print(&c);
    ch_free(&c);

    const char *w[] = {"apple", "banana", "cherry"};
    puts("== String hashing (m = 11) ==");
    for (int i = 0; i < 3; i++) printf("   %-7s djb2%%m=%lu folding=%u poly31=%u\n", w[i], djb2(w[i]) % 11, folding(w[i], 11), poly(w[i], 11));
    return 0;
}
