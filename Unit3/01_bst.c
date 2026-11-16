/* Unit 3 - Binary Search Tree (linked): insert, search, delete, traversals,
   height, counts, min/max, mirror, LCA, floor/ceil, is-BST, kth smallest */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node { int key; struct node *left, *right; } node_t;

static node_t *new_node(int k) {
    node_t *n = malloc(sizeof *n);
    if (!n) { puts("no more memory"); exit(1); }
    n->key = k; n->left = n->right = NULL; return n;
}
node_t *insert(node_t *r, int k) {
    if (!r) return new_node(k);
    if (k < r->key) r->left = insert(r->left, k);
    else if (k > r->key) r->right = insert(r->right, k);   /* duplicates ignored */
    return r;
}
node_t *insert_iter(node_t *r, int k) {
    node_t *n = new_node(k), *p = NULL, *c = r;
    while (c) { p = c; c = k < c->key ? c->left : c->right; }
    if (!p) return n;
    if (k < p->key) p->left = n; else p->right = n;
    return r;
}
node_t *search(node_t *r, int k) {
    while (r && r->key != k) r = k < r->key ? r->left : r->right;
    return r;
}
node_t *find_min(node_t *r) { while (r && r->left) r = r->left; return r; }
node_t *find_max(node_t *r) { while (r && r->right) r = r->right; return r; }

node_t *delete_key(node_t *r, int k) {
    if (!r) return NULL;
    if (k < r->key) r->left = delete_key(r->left, k);
    else if (k > r->key) r->right = delete_key(r->right, k);
    else {
        if (!r->left) { node_t *t = r->right; free(r); return t; }
        if (!r->right) { node_t *t = r->left; free(r); return t; }
        node_t *s = find_min(r->right);          /* inorder successor */
        r->key = s->key;
        r->right = delete_key(r->right, s->key);
    }
    return r;
}
void inorder(node_t *r)   { if (r) { inorder(r->left); printf("%d ", r->key); inorder(r->right); } }
void preorder(node_t *r)  { if (r) { printf("%d ", r->key); preorder(r->left); preorder(r->right); } }
void postorder(node_t *r) { if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->key); } }

int height(node_t *r) { if (!r) return -1; int a = height(r->left), b = height(r->right); return 1 + (a > b ? a : b); }
int count_nodes(node_t *r) { return r ? 1 + count_nodes(r->left) + count_nodes(r->right) : 0; }
int count_leaves(node_t *r) { if (!r) return 0; if (!r->left && !r->right) return 1; return count_leaves(r->left) + count_leaves(r->right); }

void level_order(node_t *r) {
    int n = count_nodes(r), f = 0, b = 0;
    if (!n) return;
    node_t **q = malloc(n * sizeof *q);
    q[b++] = r;
    while (f < b) {
        node_t *c = q[f++]; printf("%d ", c->key);
        if (c->left) q[b++] = c->left;
        if (c->right) q[b++] = c->right;
    }
    free(q);
}
/* iterative inorder using explicit stack */
void inorder_iter(node_t *r) {
    int n = count_nodes(r), top = 0;
    node_t **st = malloc((n + 1) * sizeof *st);
    node_t *c = r;
    while (c || top) {
        while (c) { st[top++] = c; c = c->left; }
        c = st[--top]; printf("%d ", c->key); c = c->right;
    }
    free(st);
}
void mirror(node_t *r) {
    if (!r) return;
    node_t *t = r->left; r->left = r->right; r->right = t;
    mirror(r->left); mirror(r->right);
}
node_t *lca(node_t *r, int a, int b) {
    while (r) {
        if (a < r->key && b < r->key) r = r->left;
        else if (a > r->key && b > r->key) r = r->right;
        else return r;
    }
    return NULL;
}
int is_bst(node_t *r, long lo, long hi) {
    if (!r) return 1;
    if (r->key <= lo || r->key >= hi) return 0;
    return is_bst(r->left, lo, r->key) && is_bst(r->right, r->key, hi);
}
int floor_key(node_t *r, int k) { int res = INT_MIN; while (r) { if (r->key == k) return k; if (r->key < k) { res = r->key; r = r->right; } else r = r->left; } return res; }
int ceil_key(node_t *r, int k)  { int res = INT_MAX; while (r) { if (r->key == k) return k; if (r->key > k) { res = r->key; r = r->left; } else r = r->right; } return res; }
static void kth(node_t *r, int *k, int *ans) {
    if (!r || *k <= 0) return;
    kth(r->left, k, ans);
    if (--(*k) == 0) { *ans = r->key; return; }
    kth(r->right, k, ans);
}
int kth_smallest(node_t *r, int k) { int ans = INT_MIN; kth(r, &k, &ans); return ans; }
void destroy(node_t *r) { if (r) { destroy(r->left); destroy(r->right); free(r); } }

int main(void) {
    int a[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65};
    node_t *root = NULL;
    for (int i = 0; i < 10; i++) root = insert(root, a[i]);
    printf("Inorder   : "); inorder(root); puts("");
    printf("Preorder  : "); preorder(root); puts("");
    printf("Postorder : "); postorder(root); puts("");
    printf("Level     : "); level_order(root); puts("");
    printf("Inorder (iterative): "); inorder_iter(root); puts("");
    printf("Height=%d nodes=%d leaves=%d min=%d max=%d\n", height(root), count_nodes(root), count_leaves(root), find_min(root)->key, find_max(root)->key);
    printf("search 60: %s, search 99: %s\n", search(root, 60) ? "found" : "no", search(root, 99) ? "found" : "no");
    printf("LCA(35,45)=%d LCA(20,45)=%d\n", lca(root, 35, 45)->key, lca(root, 20, 45)->key);
    printf("floor(66)=%d ceil(66)=%d 3rd smallest=%d\n", floor_key(root, 66), ceil_key(root, 66), kth_smallest(root, 3));
    printf("is BST: %d\n", is_bst(root, LONG_MIN, LONG_MAX));
    root = delete_key(root, 20); root = delete_key(root, 30); root = delete_key(root, 50);
    printf("After deleting 20,30,50: "); inorder(root); puts("");
    mirror(root); printf("Mirror inorder: "); inorder(root); puts("");
    destroy(root);
    return 0;
}
