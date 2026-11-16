/*==========================================================================
 * FILE  : 12_splay_tree.c
 * TOPIC : SPLAY TREE (self-adjusting Binary Search Tree)
 *==========================================================================
 *
 * THEORY
 * ------
 * A splay tree is a BST that does NOT store any balance information.
 * Instead, EVERY access (search, insert, delete) moves the touched key to the
 * ROOT by a sequence of rotations called SPLAYING.  Recently used keys stay
 * near the root, so frequently accessed keys are found quickly (good locality
 * of reference, used in caches and network routers).
 *
 * Splaying a node x uses three cases (p = parent of x, g = grandparent):
 *
 *  1. ZIG      : x's parent p is the ROOT       -> one rotation at the root.
 *  2. ZIG-ZIG  : x and p are both LEFT children (or both RIGHT children)
 *                -> rotate g first, THEN rotate p  (two same-direction rotations)
 *  3. ZIG-ZAG  : x is a left child and p is a right child (or vice versa)
 *                -> rotate p, THEN rotate g       (two opposite rotations)
 *
 *   Zig-zig (right-right):          Zig-zag:
 *         g                x              g               x
 *        /                / \            /               / \
 *       p        ==>     ?   p          p       ==>     p   g
 *      /                    / \          \
 *     x                    ?   g          x
 *
 * If the key is not present, the last node visited on the search path is
 * splayed to the root instead.
 *
 * ALGORITHMS
 * ----------
 * SEARCH(k) : splay(k); the key is present iff the new root holds k.
 * INSERT(k) : splay(k) to bring the closest key to the root. If it equals k
 *             stop (duplicate). Otherwise make k the new root and attach the
 *             old root and one of its subtrees as the two children of k.
 * DELETE(k) : splay(k). If k is the root, remove it: if there is no left
 *             subtree the right subtree becomes the tree, otherwise splay the
 *             MAXIMUM of the left subtree (it comes up to the root and has no
 *             right child), then hang the old right subtree below it.
 *
 * The recursive splay() below does exactly the zig / zig-zig / zig-zag steps
 * and prints the case it used.
 *
 * COMPLEXITY
 * ----------
 *   A single operation can take O(n) in the worst case (tree becomes a chain),
 *   BUT the AMORTISED cost is O(log n) per operation over any sequence.
 *   Extra space per node : none (no height / colour field).   Space O(n).
 *
 * COMPILE & RUN
 * -------------
 *  gcc 12_splay_tree.c -o splay && ./splay
 *
 * EXAMPLE: insert 40,20,60; search 20 moves 20 to root.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
static int checked_scanf(const char *format, ...) {
    int required=0;
    for (const char *p=format; *p; ++p) {
        if (*p=='%' && p[1]=='%') { ++p; continue; }
        if (*p=='%' && p[1]!='*') ++required;
    }
    va_list args; va_start(args,format);
    int result=vscanf(format,args); va_end(args);
    if (result!=required) {
        fprintf(stderr,"Input ended or was invalid. Restart and enter the requested values.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

int rotations = 0;                  /* rotations done by the last operation  */

Node *createNode(int key)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) { printf("Out of memory\n"); exit(1); }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/* RIGHT rotation: left child x of y comes up, y goes down to the right.     */
Node *rotateRight(Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    x->right = y;
    rotations++;
    return x;
}

/* LEFT rotation: right child y of x comes up, x goes down to the left.      */
Node *rotateLeft(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    rotations++;
    return y;
}

/* ------------------------------- SPLAY ---------------------------------- */
/* Brings 'key' (or the last node on its search path) to the root of the     */
/* subtree and returns the new root of that subtree.                         */
Node *splay(Node *root, int key)
{
    if (!root || root->key == key) return root;
    if (key < root->key) {
        if (!root->left) return root;
        if (key < root->left->key) { /* zig-zig */
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        } else if (key > root->left->key) { /* zig-zag */
            root->left->right = splay(root->left->right, key);
            if (root->left->right) root->left = rotateLeft(root->left);
        }
        return root->left ? rotateRight(root) : root;
    }
    if (!root->right) return root;
    if (key > root->right->key) { /* zag-zag */
        root->right->right = splay(root->right->right, key);
        root = rotateLeft(root);
    } else if (key < root->right->key) { /* zag-zig */
        root->right->left = splay(root->right->left, key);
        if (root->right->left) root->right = rotateRight(root->right);
    }
    return root->right ? rotateLeft(root) : root;
}

Node *search(Node *root, int key, int *found)
{
    root = splay(root, key);                            /* splay even if absent */
    *found = (root != NULL && root->key == key);
    return root;
}

/* ------------------------------- INSERT --------------------------------- */
Node *insert(Node *root, int key, int *dup)
{
    if (root == NULL) return createNode(key);
    root = splay(root, key);                            /* nearest key -> root */
    if (root->key == key) { *dup = 1; return root; }

    Node *n = createNode(key);
    if (key < root->key) {                              /* old root is larger */
        n->right = root;
        n->left = root->left;
        root->left = NULL;
    } else {                                            /* old root is smaller*/
        n->left = root;
        n->right = root->right;
        root->right = NULL;
    }
    return n;                                           /* new key is the root*/
}

/* ------------------------------- DELETE --------------------------------- */
Node *deleteKey(Node *root, int key, int *found)
{
    *found = 0;
    if (root == NULL) return NULL;
    root = splay(root, key);
    if (root->key != key) return root;                  /* not present        */
    *found = 1;

    Node *temp = root;
    if (root->left == NULL) {
        root = root->right;                             /* nothing on the left*/
    } else {
        root = splay(root->left, key);                  /* key > all of left, so
                                                           its MAX comes to root;
                                                           that max has no right
                                                           child                */
        root->right = temp->right;
    }
    free(temp);
    return root;
}

/* ----------------------------- TRAVERSALS ------------------------------- */
void inorder(Node *r)   { if (r) { inorder(r->left);  printf("%d ", r->key); inorder(r->right); } }
void preorder(Node *r)  { if (r) { printf("%d ", r->key); preorder(r->left); preorder(r->right); } }
void postorder(Node *r) { if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->key); } }

int height(Node *r)
{
    if (!r) return 0;
    int a = height(r->left), b = height(r->right);
    return 1 + (a > b ? a : b);
}
int countNodes(Node *r) { return r ? 1 + countNodes(r->left) + countNodes(r->right) : 0; }

void printTree(Node *r, int space)
{
    if (!r) return;
    printTree(r->right, space + 6);
    printf("%*s%d\n", space, "", r->key);
    printTree(r->left, space + 6);
}

void freeTree(Node *r) { if (r) { freeTree(r->left); freeTree(r->right); free(r); } }

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    Node *root = NULL;
    int choice, x, flag;

    while (1) {
        printf("\n===== SPLAY TREE MENU =====\n");
        printf(" 1. Insert        2. Delete       3. Search (splays the key)\n");
        printf(" 4. Inorder       5. Preorder     6. Postorder\n");
        printf(" 7. Show tree     8. Height / count / current root\n");
        printf(" 9. Access minimum (splay min)    10. Access maximum (splay max)\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter key: "); checked_scanf("%d", &x);
            flag = 0; rotations = 0;
            root = insert(root, x, &flag);
            if (flag) printf("Key %d already exists (it is now the root).\n", x);
            else printf("Inserted %d; it is now the root. (rotations: %d)\n", x, rotations);
            break;
        case 2:
            printf("Enter key to delete: "); checked_scanf("%d", &x);
            rotations = 0;
            root = deleteKey(root, x, &flag);
            if (flag) printf("Deleted %d. (rotations: %d)\n", x, rotations);
            else printf("Key %d not found (last visited node was splayed to root).\n", x);
            break;
        case 3:
            printf("Enter key to search: "); checked_scanf("%d", &x);
            rotations = 0;
            root = search(root, x, &flag);
            if (flag) printf("Key %d FOUND and moved to the root. (rotations: %d)\n", x, rotations);
            else printf("Key %d NOT found.%s\n", x, root ? " Last node on the path was splayed to the root." : "");
            break;
        case 4: printf("Inorder   : "); inorder(root);   printf("\n"); break;
        case 5: printf("Preorder  : "); preorder(root);  printf("\n"); break;
        case 6: printf("Postorder : "); postorder(root); printf("\n"); break;
        case 7: if (!root) printf("(empty)\n"); else printTree(root, 0); break;
        case 8:
            printf("Height = %d, Nodes = %d", height(root), countNodes(root));
            if (root) printf(", Root = %d", root->key);
            printf("\n");
            break;
        case 9:
            if (!root) { printf("Tree is empty.\n"); break; }
            { Node *m = root; while (m->left) m = m->left;
              rotations = 0; root = splay(root, m->key);
              printf("Minimum = %d is now the root.\n", root->key); }
            break;
        case 10:
            if (!root) { printf("Tree is empty.\n"); break; }
            { Node *m = root; while (m->right) m = m->right;
              rotations = 0; root = splay(root, m->key);
              printf("Maximum = %d is now the root.\n", root->key); }
            break;
        case 0: freeTree(root); printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    freeTree(root);
    return 0;
}
