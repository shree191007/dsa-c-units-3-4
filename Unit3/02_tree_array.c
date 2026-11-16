/* Unit 3 - Array implementation of trees
   (a) Complete binary tree in an array: index math, traversals
   (b) BST stored in an array (index 1-based, 0 = empty)  */
#include <stdio.h>
#include <string.h>

#define MAX 127

/* ---------- (a) generic binary tree in array (1-based, 0 means no node) ---------- */
typedef struct { int a[MAX + 1]; int used[MAX + 1]; } atree_t;

static int L(int i) { return 2 * i; }
static int R(int i) { return 2 * i + 1; }
static int P(int i) { return i / 2; }

void at_init(atree_t *t) { memset(t, 0, sizeof *t); }
void at_set(atree_t *t, int i, int v) { if (i <= MAX) { t->a[i] = v; t->used[i] = 1; } }
int  at_has(atree_t *t, int i) { return i <= MAX && t->used[i]; }

void at_inorder(atree_t *t, int i)   { if (at_has(t, i)) { at_inorder(t, L(i)); printf("%d ", t->a[i]); at_inorder(t, R(i)); } }
void at_preorder(atree_t *t, int i)  { if (at_has(t, i)) { printf("%d ", t->a[i]); at_preorder(t, L(i)); at_preorder(t, R(i)); } }
void at_postorder(atree_t *t, int i) { if (at_has(t, i)) { at_postorder(t, L(i)); at_postorder(t, R(i)); printf("%d ", t->a[i]); } }
void at_levelorder(atree_t *t) { for (int i = 1; i <= MAX; i++) if (t->used[i]) printf("%d ", t->a[i]); }
int  at_height(atree_t *t, int i) {
    if (!at_has(t, i)) return -1;
    int l = at_height(t, L(i)), r = at_height(t, R(i));
    return 1 + (l > r ? l : r);
}

/* ---------- (b) BST in array ---------- */
typedef struct { int key[MAX + 1]; int used[MAX + 1]; } abst_t;

void ab_init(abst_t *t) { memset(t, 0, sizeof *t); }
int ab_insert(abst_t *t, int k) {
    int i = 1;
    while (i <= MAX && t->used[i]) {
        if (k == t->key[i]) return 0;
        i = k < t->key[i] ? L(i) : R(i);
    }
    if (i > MAX) return -1;                  /* skewed tree overflowed array */
    t->key[i] = k; t->used[i] = 1; return 1;
}
int ab_search(abst_t *t, int k) {
    int i = 1;
    while (i <= MAX && t->used[i]) {
        if (k == t->key[i]) return i;
        i = k < t->key[i] ? L(i) : R(i);
    }
    return 0;
}
void ab_inorder(abst_t *t, int i) {
    if (i > MAX || !t->used[i]) return;
    ab_inorder(t, L(i)); printf("%d ", t->key[i]); ab_inorder(t, R(i));
}

int main(void) {
    atree_t t; at_init(&t);
    int vals[] = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 7; i++) at_set(&t, i + 1, vals[i]);
    printf("Node 5: parent=%d left=%d right=%d\n", t.a[P(5)], L(5), R(5));
    printf("Inorder   : "); at_inorder(&t, 1); puts("");
    printf("Preorder  : "); at_preorder(&t, 1); puts("");
    printf("Postorder : "); at_postorder(&t, 1); puts("");
    printf("Levelorder: "); at_levelorder(&t); puts("");
    printf("Height    : %d\n", at_height(&t, 1));

    abst_t b; ab_init(&b);
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) ab_insert(&b, keys[i]);
    printf("Array BST inorder: "); ab_inorder(&b, 1); puts("");
    printf("search 60 -> index %d, search 65 -> index %d\n", ab_search(&b, 60), ab_search(&b, 65));
    return 0;
}
