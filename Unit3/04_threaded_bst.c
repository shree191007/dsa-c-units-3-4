/* Unit 3 - Inorder threaded binary search tree (double threaded)
   insert, search, inorder (no stack/recursion), preorder, successor/predecessor, reverse inorder, delete, free */
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    struct node *left, *right;
    int lthread, rthread;          /* 1 => pointer is a thread (to pred/succ), 0 => real child */
} node_t;

static node_t *create(int k) {
    node_t *n = malloc(sizeof *n);
    if (!n) { puts("no more memory"); exit(1); }
    n->key = k; n->left = n->right = NULL; n->lthread = n->rthread = 1; return n;
}
static node_t *leftmost(node_t *t)  { if (!t) return NULL; while (!t->lthread) t = t->left;  return t; }
static node_t *rightmost(node_t *t) { if (!t) return NULL; while (!t->rthread) t = t->right; return t; }

node_t *insert(node_t *root, int k) {
    node_t *p = NULL, *c = root;
    while (c) {
        if (k == c->key) return root;               /* no duplicates */
        p = c;
        if (k < c->key) { if (!c->lthread) c = c->left; else break; }
        else            { if (!c->rthread) c = c->right; else break; }
    }
    node_t *n = create(k);
    if (!p) return n;
    if (k < p->key) { n->left = p->left; n->right = p; p->left = n; p->lthread = 0; }
    else            { n->right = p->right; n->left = p; p->right = n; p->rthread = 0; }
    return root;
}
node_t *search(node_t *t, int k) {
    while (t) {
        if (k == t->key) return t;
        if (k < t->key) t = t->lthread ? NULL : t->left;
        else            t = t->rthread ? NULL : t->right;
    }
    return NULL;
}
node_t *succ(node_t *n) { return n->rthread ? n->right : leftmost(n->right); }
node_t *pred(node_t *n) { return n->lthread ? n->left : rightmost(n->left); }

void inorder(node_t *root)         { for (node_t *t = leftmost(root);  t; t = succ(t)) printf("%d ", t->key); }
void reverse_inorder(node_t *root) { for (node_t *t = rightmost(root); t; t = pred(t)) printf("%d ", t->key); }
void preorder(node_t *root) {
    node_t *t = root;
    while (t) {
        printf("%d ", t->key);
        if (!t->lthread) t = t->left;
        else if (!t->rthread) t = t->right;
        else {                                        /* leaf: follow threads up to a node with a right child */
            while (t && t->rthread) t = t->right;
            if (t) t = t->right;
        }
    }
}

/* delete: handle by cases using parent tracking */
static int is_left_child(node_t *par, node_t *c) { return !par->lthread && par->left == c; }
static node_t *case_leaf(node_t *root, node_t *par, node_t *c) {
    if (!par) { free(c); return NULL; }
    if (is_left_child(par, c)) { par->lthread = 1; par->left = c->left; }
    else                       { par->rthread = 1; par->right = c->right; }
    free(c); return root;
}
static node_t *case_one_child(node_t *root, node_t *par, node_t *c) {
    node_t *child;
    if (!c->lthread) { child = c->left;  rightmost(child)->right = c->right; }   /* fix successor-thread */
    else             { child = c->right; leftmost(child)->left = c->left; }      /* fix predecessor-thread */
    if (!par) root = child;
    else if (is_left_child(par, c)) par->left = child;
    else par->right = child;
    free(c); return root;
}
node_t *delete_key(node_t *root, int k) {
    node_t *par = NULL, *c = root;
    while (c && c->key != k) {
        par = c;
        if (k < c->key) c = c->lthread ? NULL : c->left;
        else            c = c->rthread ? NULL : c->right;
    }
    if (!c) return root;
    if (!c->lthread && !c->rthread) {                 /* two children: copy successor, delete it */
        node_t *sp = c, *s = c->right;
        while (!s->lthread) { sp = s; s = s->left; }
        c->key = s->key; c = s; par = sp;
    }
    if (c->lthread && c->rthread) return case_leaf(root, par, c);
    return case_one_child(root, par, c);
}
void destroy(node_t *root) {
    node_t *t = leftmost(root);
    while (t) { node_t *n = succ(t); free(t); t = n; }
}

int main(void) {
    node_t *root = NULL;
    int a[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65};
    for (int i = 0; i < 10; i++) root = insert(root, a[i]);
    printf("Inorder          : "); inorder(root); puts("");
    printf("Reverse inorder  : "); reverse_inorder(root); puts("");
    printf("Preorder         : "); preorder(root); puts("");
    node_t *n = search(root, 40);
    printf("40 found, succ=%d pred=%d\n", succ(n)->key, pred(n)->key);
    int dels[] = {20, 30, 50, 80};
    for (int i = 0; i < 4; i++) {
        root = delete_key(root, dels[i]);
        printf("After deleting %d: ", dels[i]); inorder(root); puts("");
    }
    destroy(root);
    return 0;
}
