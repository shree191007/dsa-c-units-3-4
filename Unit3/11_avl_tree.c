/*==========================================================================
 * FILE  : 11_avl_tree.c
 * TOPIC : AVL TREE (self-balancing Binary Search Tree)
 *==========================================================================
 *
 * THEORY
 * ------
 * An AVL tree (Adelson-Velsky & Landis, 1962) is a BST that keeps itself
 * BALANCED.  For EVERY node the BALANCE FACTOR must be -1, 0 or +1:
 *
 *      balance factor (BF) = height(left subtree) - height(right subtree)
 *
 * A plain BST can degenerate into a linked list (height n) when keys arrive
 * sorted.  An AVL tree guarantees height <= 1.44 * log2(n+2), so every
 * operation stays O(log n).
 *
 * After an insert or delete, walk back up to the root, update heights and
 * check each node.  If |BF| > 1 the node is UNBALANCED and one of four cases
 * applies (z = the unbalanced node, y = its taller child, x = y's taller child):
 *
 *   1. LL case (BF > 1 and key in left-left)    -> single RIGHT rotation at z
 *   2. RR case (BF < -1 and key in right-right) -> single LEFT  rotation at z
 *   3. LR case (BF > 1 and key in left-right)   -> LEFT rotation at y,
 *                                                  then RIGHT rotation at z
 *   4. RL case (BF < -1 and key in right-left)  -> RIGHT rotation at y,
 *                                                  then LEFT  rotation at z
 *
 *   RIGHT rotation at z:                LEFT rotation at z:
 *            z                y                  z                    y
 *           / \              / \                / \                  / \
 *          y   T4    ==>    x   z              T1  y       ==>      z   x
 *         / \              /\  / \                 / \              / \ / \
 *        x   T3          T1 T2 T3 T4              T2  x            T1 T2 T3 T4
 *       / \                                           / \
 *     T1  T2                                         T3 T4
 *
 * ALGORITHMS
 * ----------
 * INSERT : ordinary BST insert, then on the way back up update height and
 *          rebalance each ancestor (at most ONE rotation case is needed).
 * DELETE : ordinary BST delete (leaf / one child / two children via inorder
 *          successor), then update height and rebalance every ancestor on the
 *          way up (may need rotations at several levels). For delete the
 *          case is chosen from the BF of the taller CHILD:
 *              BF(z) > 1  and BF(left)  >= 0 -> LL,   BF(left)  < 0 -> LR
 *              BF(z) < -1 and BF(right) <= 0 -> RR,   BF(right) > 0 -> RL
 *
 * Every rotation is announced on the screen so you can watch the balancing.
 *
 * COMPLEXITY
 * ----------
 *   Search / Insert / Delete : O(log n) (worst case, guaranteed)
 *   Rotation                 : O(1)     Traversals : O(n)    Space : O(n)
 *
 * COMPILE & RUN
 * -------------
 *  gcc 11_avl_tree.c -o avl && ./avl
 *
 * SAMPLE OUTPUT
 * -------------
 *   ===== AVL TREE MENU =====
 *    1. Insert         2. Delete        3. Search
 *    4. Inorder        5. Preorder      6. Postorder
 *    7. Level order    8. Show tree with balance factors
 *    9. Height / count / min / max      10. Verify AVL property
 *    0. Exit
 *   Enter choice: 1
 *   Enter key: 30
 *   Inserted 30 (rotations: 0).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key: 10
 *   Inserted 10 (rotations: 0).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key: 20
 *      [LR imbalance at 30 -> left rotation at 10, then right rotation at 30]
 *   Inserted 20 (rotations: 2).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key: 40
 *   Inserted 40 (rotations: 0).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key: 50
 *      [RR imbalance at 30 -> left rotation]
 *   Inserted 50 (rotations: 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key: 60
 *      [RR imbalance at 20 -> left rotation]
 *   Inserted 60 (rotations: 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *                   60(bf=0)
 *           50(bf=-1)
 *   40(bf=0)
 *                   30(bf=0)
 *           20(bf=0)
 *                   10(bf=0)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 40
 *   Deleted 40 (rotations: 0).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *           60(bf=0)
 *   50(bf=1)
 *                   30(bf=0)
 *           20(bf=0)
 *                   10(bf=0)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 10 20 30 50 60
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *   Level order: 50 20 60 10 30
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 10
 *   Valid AVL tree.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 0
 *   Bye!
 *==========================================================================*/

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
    int height;                 /* height of the subtree rooted here (leaf=1) */
    struct Node *left, *right;
} Node;

int rotationCount = 0;          /* rotations done by the last operation       */

int max(int a, int b) { return a > b ? a : b; }

/* Height of an empty subtree is 0 */
int height(Node *n) { return n ? n->height : 0; }

/* Balance factor = height(left) - height(right) */
int balanceFactor(Node *n) { return n ? height(n->left) - height(n->right) : 0; }

void updateHeight(Node *n) { n->height = 1 + max(height(n->left), height(n->right)); }

Node *createNode(int key)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) { printf("Out of memory\n"); exit(1); }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

/* ------------------------------ ROTATIONS ------------------------------- */
/* RIGHT rotation: z's left child y becomes the new root of the subtree.     */
Node *rotateRight(Node *z)
{
    Node *y = z->left;
    Node *T3 = y->right;
    y->right = z;               /* z becomes right child of y                */
    z->left = T3;               /* y's old right subtree is adopted by z     */
    updateHeight(z);            /* z is lower now: update it FIRST           */
    updateHeight(y);
    rotationCount++;
    return y;                   /* y is the new subtree root                 */
}

/* LEFT rotation: z's right child y becomes the new root of the subtree.     */
Node *rotateLeft(Node *z)
{
    Node *y = z->right;
    Node *T2 = y->left;
    y->left = z;
    z->right = T2;
    updateHeight(z);
    updateHeight(y);
    rotationCount++;
    return y;
}

/* ------------------------------ REBALANCE ------------------------------- */
/* Called on every node while returning from insert / delete.                */
Node *rebalance(Node *n)
{
    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1) {                                       /* left-heavy        */
        if (balanceFactor(n->left) >= 0) {              /* LL case           */
            printf("   [LL imbalance at %d -> right rotation]\n", n->key);
            return rotateRight(n);
        }
        printf("   [LR imbalance at %d -> left rotation at %d, then right rotation at %d]\n",
               n->key, n->left->key, n->key);
        n->left = rotateLeft(n->left);                  /* LR case           */
        return rotateRight(n);
    }
    if (bf < -1) {                                      /* right-heavy       */
        if (balanceFactor(n->right) <= 0) {             /* RR case           */
            printf("   [RR imbalance at %d -> left rotation]\n", n->key);
            return rotateLeft(n);
        }
        printf("   [RL imbalance at %d -> right rotation at %d, then left rotation at %d]\n",
               n->key, n->right->key, n->key);
        n->right = rotateRight(n->right);               /* RL case           */
        return rotateLeft(n);
    }
    return n;                                           /* already balanced  */
}

/* ------------------------------- INSERT --------------------------------- */
Node *insert(Node *root, int key, int *dup)
{
    if (!root) return createNode(key);
    if (key < root->key)      root->left  = insert(root->left,  key, dup);
    else if (key > root->key) root->right = insert(root->right, key, dup);
    else { *dup = 1; return root; }                     /* no duplicates     */
    return rebalance(root);
}

Node *minNode(Node *n) { while (n && n->left) n = n->left; return n; }
Node *maxNode(Node *n) { while (n && n->right) n = n->right; return n; }

/* ------------------------------- DELETE --------------------------------- */
Node *deleteNode(Node *root, int key)
{
    if (!root) return NULL;
    if (key < root->key)      root->left  = deleteNode(root->left, key);
    else if (key > root->key) root->right = deleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {              /* 0 or 1 child      */
            Node *child = root->left ? root->left : root->right;
            free(root);
            return child;                               /* may be NULL       */
        }
        Node *s = minNode(root->right);                 /* 2 children        */
        root->key = s->key;
        root->right = deleteNode(root->right, s->key);
    }
    return rebalance(root);
}

Node *search(Node *r, int key)
{
    while (r && r->key != key) r = key < r->key ? r->left : r->right;
    return r;
}

/* ----------------------------- TRAVERSALS ------------------------------- */
void inorder(Node *r)   { if (r) { inorder(r->left);  printf("%d ", r->key); inorder(r->right); } }
void preorder(Node *r)  { if (r) { printf("%d ", r->key); preorder(r->left); preorder(r->right); } }
void postorder(Node *r) { if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->key); } }

int countNodes(Node *r);
void levelOrder(Node *root)
{
    if (!root) { printf("(empty)"); return; }
    Node **q = malloc((size_t)countNodes(root) * sizeof *q);
    if (!q) { fprintf(stderr,"Out of memory\n"); exit(EXIT_FAILURE); }
    int f = 0, r = 0;
    q[r++] = root;
    while (f < r) {
        Node *c = q[f++];
        printf("%d ", c->key);
        if (c->left)  q[r++] = c->left;
        if (c->right) q[r++] = c->right;
    }
    free(q);
}

int countNodes(Node *r) { return r ? 1 + countNodes(r->left) + countNodes(r->right) : 0; }

/* Sideways picture; each key is followed by its balance factor in brackets  */
void printTree(Node *r, int space)
{
    if (!r) return;
    printTree(r->right, space + 8);
    printf("%*s%d(bf=%d)\n", space, "", r->key, balanceFactor(r));
    printTree(r->left, space + 8);
}

/* Check that every node really has |BF| <= 1 and heights are correct        */
int isAVL(Node *r)
{
    if (!r) return 1;
    int bf = height(r->left) - height(r->right);
    if (bf < -1 || bf > 1) return 0;
    if (r->height != 1 + max(height(r->left), height(r->right))) return 0;
    return isAVL(r->left) && isAVL(r->right);
}

void freeTree(Node *r) { if (r) { freeTree(r->left); freeTree(r->right); free(r); } }

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    Node *root = NULL;
    int choice, x;

    while (1) {
        printf("\n===== AVL TREE MENU =====\n");
        printf(" 1. Insert         2. Delete        3. Search\n");
        printf(" 4. Inorder        5. Preorder      6. Postorder\n");
        printf(" 7. Level order    8. Show tree with balance factors\n");
        printf(" 9. Height / count / min / max      10. Verify AVL property\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1: {
            int dup = 0;
            printf("Enter key: "); checked_scanf("%d", &x);
            rotationCount = 0;
            root = insert(root, x, &dup);
            if (dup) printf("Key %d already exists.\n", x);
            else printf("Inserted %d (rotations: %d).\n", x, rotationCount);
            break;
        }
        case 2:
            printf("Enter key to delete: "); checked_scanf("%d", &x);
            if (!search(root, x)) printf("Key %d not found.\n", x);
            else {
                rotationCount = 0;
                root = deleteNode(root, x);
                printf("Deleted %d (rotations: %d).\n", x, rotationCount);
            }
            break;
        case 3:
            printf("Enter key: "); checked_scanf("%d", &x);
            printf(search(root, x) ? "Key %d FOUND.\n" : "Key %d NOT found.\n", x);
            break;
        case 4: printf("Inorder   : "); inorder(root);   printf("\n"); break;
        case 5: printf("Preorder  : "); preorder(root);  printf("\n"); break;
        case 6: printf("Postorder : "); postorder(root); printf("\n"); break;
        case 7: printf("Level order: "); levelOrder(root); printf("\n"); break;
        case 8: if (!root) printf("(empty)\n"); else printTree(root, 0); break;
        case 9:
            printf("Height = %d, Nodes = %d\n", height(root), countNodes(root));
            if (root) printf("Min = %d, Max = %d\n", minNode(root)->key, maxNode(root)->key);
            break;
        case 10: printf(isAVL(root) ? "Valid AVL tree.\n" : "NOT a valid AVL tree!\n"); break;
        case 0: freeTree(root); printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    freeTree(root);
    return 0;
}
