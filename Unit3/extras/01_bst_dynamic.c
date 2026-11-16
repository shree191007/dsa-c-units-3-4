/*==========================================================================
 * FILE  : 01_bst_dynamic.c
 * TOPIC : Binary Search Tree (BST) using DYNAMIC ALLOCATION (linked nodes)
 *==========================================================================
 *
 * THEORY
 * ------
 * A Binary Search Tree is a binary tree in which, for EVERY node:
 *      - all keys in its LEFT  subtree are  <  node's key
 *      - all keys in its RIGHT subtree are  >  node's key
 * (duplicates are not stored in this program).
 * Because of this ordering, an INORDER traversal (Left, Root, Right)
 * always prints the keys in sorted (ascending) order.
 *
 * In the dynamic (linked) representation every node is created with malloc()
 * and holds:   data | pointer to left child | pointer to right child
 *
 * ALGORITHMS (summary)
 * --------------------
 * INSERT(key)  : start at root; go left if key < node, right if key > node;
 *                when a NULL link is reached, attach the new node there.
 * SEARCH(key)  : same walk as insert; found if key == node->data.
 * DELETE(key)  : three cases
 *      1) node is a LEAF            -> just free it.
 *      2) node has ONE child        -> replace node by its child.
 *      3) node has TWO children     -> copy the INORDER SUCCESSOR (smallest
 *         key of right subtree) into the node, then delete that successor
 *         from the right subtree (it has at most one child).
 * TRAVERSALS   : Inorder (L,Root,R), Preorder (Root,L,R), Postorder (L,R,Root),
 *                Level order (BFS using a queue), Iterative inorder (stack).
 *
 * OTHER OPERATIONS / APPLICATIONS INCLUDED
 * ----------------------------------------
 *  - height, node count, leaf count, min, max
 *  - inorder successor / predecessor
 *  - k-th smallest element
 *  - Lowest Common Ancestor (LCA)
 *  - print all keys in a range [lo, hi]
 *  - validate that a tree is a BST
 *  - BST SORT (application: sorting numbers with a BST)
 *
 * COMPLEXITY
 * ----------
 *  Insert / Search / Delete : O(h)  (h = height)
 *      best/average (balanced) h = log n  -> O(log n)
 *      worst (skewed, e.g. sorted input) h = n -> O(n)
 *  Traversals               : O(n)
 *  Space                    : O(n) for nodes (+ O(h) recursion stack)
 *
 * COMPILE & RUN
 * -------------
 *  gcc 01_bst_dynamic.c -o bst_dynamic
 *  ./bst_dynamic          (on Windows/MSYS2:  bst_dynamic.exe)
 *
 * SAMPLE OUTPUT
 * -------------
 *   ========== BST (DYNAMIC) MENU ==========
 *    1. Insert              2. Delete            3. Search
 *    4. Inorder             5. Preorder          6. Postorder
 *    7. Level order         8. Iterative inorder 9. Height / counts
 *   10. Min and Max        11. Successor/Predecessor
 *   12. k-th smallest      13. LCA of two keys  14. Print keys in range
 *   15. Is it a BST?       16. Show tree shape  17. BST SORT (application)
 *    0. Exit
 *   Enter choice: 1
 *   Enter key to insert: 50
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 30
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 70
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 20
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 40
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 60
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key to insert: 80
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 20 30 40 50 60 70 80
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Preorder  : 50 30 20 40 70 60 80
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Postorder : 20 40 30 60 80 70 50
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *   Level order: 50 30 70 20 40 60 80
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 9
 *   Height = 3, Nodes = 7, Leaves = 4
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 16
 *   (root at left; right subtree above, left subtree below)
 *               80
 *         70
 *               60
 *   50
 *               40
 *         30
 *               20
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 20
 *   Key 20 deleted.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 30
 *   Key 30 deleted.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 50
 *   Key 50 deleted.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 40 60 70 80
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 16
 *   (root at left; right subtree above, left subtree below)
 *               80
 *         70
 *   60
 *         40
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 3
 *   Enter key to search: 60
 *   Key 60 FOUND.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 11
 *   Enter key: 40
 *   Successor   of 40 = 60
 *   No predecessor for 40
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 12
 *   Enter k: 3
 *   k = 3 -> k-th smallest element = 70
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 13
 *   Enter two keys (both must be in the tree): 60
 *   80
 *   LCA(60, 80) = 60
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 14
 *   Enter lo and hi: 35
 *   65
 *   Keys in [35, 65]: 40 60
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 15
 *   Yes, the tree satisfies the BST property.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 17
 *   How many numbers? 5
 *   Enter 5 numbers (duplicates are dropped): 9
 *   4
 *   1
 *   4
 *   Key 4 already exists (duplicates not allowed).
 *   6
 *   Sorted output: 1 4 6 9
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

/* ---------------------------- NODE DEFINITION --------------------------- */
typedef struct Node {
    int data;               /* the key stored in this node        */
    struct Node *left;      /* pointer to left child  (smaller)   */
    struct Node *right;     /* pointer to right child (larger)    */
} Node;

/* Create a new node: allocate memory, store key, make both links NULL */
Node *createNode(int key)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {                        /* malloc can fail -> always check */
        printf("Memory allocation failed!\n");
        exit(1);
    }
    n->data = key;
    n->left = n->right = NULL;
    return n;
}

/* ------------------------------- INSERT --------------------------------- */
/* Recursive insert. Returns the (possibly new) root of the subtree.        */
Node *insert(Node *root, int key)
{
    if (root == NULL)                       /* empty spot found -> new node  */
        return createNode(key);
    if (key < root->data)
        root->left = insert(root->left, key);       /* go to left subtree  */
    else if (key > root->data)
        root->right = insert(root->right, key);     /* go to right subtree */
    else
        printf("Key %d already exists (duplicates not allowed).\n", key);
    return root;
}

/* ------------------------------- SEARCH --------------------------------- */
/* Iterative search: returns pointer to node holding key, or NULL.          */
Node *search(Node *root, int key)
{
    while (root != NULL) {
        if (key == root->data) return root;
        root = (key < root->data) ? root->left : root->right;
    }
    return NULL;
}

/* ---------------------------- MIN and MAX ------------------------------- */
/* Smallest key = keep going left.  Largest key = keep going right.         */
Node *findMin(Node *root)
{
    while (root != NULL && root->left != NULL) root = root->left;
    return root;
}
Node *findMax(Node *root)
{
    while (root != NULL && root->right != NULL) root = root->right;
    return root;
}

/* ------------------------------- DELETE --------------------------------- */
Node *deleteNode(Node *root, int key)
{
    if (root == NULL) return NULL;          /* key not present               */

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {                                  /* ---- node to delete found ---- */
        /* CASE 1 & 2: zero or one child -> splice the node out             */
        if (root->left == NULL) {
            Node *temp = root->right;       /* may be NULL (leaf case)       */
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        /* CASE 3: two children -> use inorder successor                    */
        Node *succ = findMin(root->right);          /* smallest in right    */
        root->data = succ->data;                    /* copy its key up      */
        root->right = deleteNode(root->right, succ->data); /* remove it     */
    }
    return root;
}

/* ----------------------------- TRAVERSALS ------------------------------- */
void inorder(Node *r)   { if (r) { inorder(r->left);  printf("%d ", r->data); inorder(r->right); } }
void preorder(Node *r)  { if (r) { printf("%d ", r->data); preorder(r->left); preorder(r->right); } }
void postorder(Node *r) { if (r) { postorder(r->left); postorder(r->right); printf("%d ", r->data); } }

/* ------------------------- COUNTING / HEIGHT ---------------------------- */
int countNodes(Node *r) { return r ? 1 + countNodes(r->left) + countNodes(r->right) : 0; }
int countLeaves(Node *r)
{
    if (r == NULL) return 0;
    if (r->left == NULL && r->right == NULL) return 1;   /* a leaf          */
    return countLeaves(r->left) + countLeaves(r->right);
}
/* height = number of NODES on the longest root-to-leaf path (empty = 0)    */
int height(Node *r)
{
    if (r == NULL) return 0;
    int hl = height(r->left), hr = height(r->right);
    return 1 + (hl > hr ? hl : hr);
}

/* ------------------------ LEVEL ORDER (queue) --------------------------- */
/* Visit nodes level by level using an array-based queue of node pointers.  */
void levelOrder(Node *root)
{
    if (root == NULL) { printf("(empty)"); return; }
    int n = countNodes(root);
    Node **q = (Node **)malloc(n * sizeof(Node *));
    if (!q) { fprintf(stderr,"Out of memory\n"); exit(EXIT_FAILURE); }
    int front = 0, rear = 0;
    q[rear++] = root;
    while (front < rear) {
        Node *cur = q[front++];                     /* dequeue              */
        printf("%d ", cur->data);
        if (cur->left)  q[rear++] = cur->left;      /* enqueue children     */
        if (cur->right) q[rear++] = cur->right;
    }
    free(q);
}

/* ---------------------- ITERATIVE INORDER (stack) ----------------------- */
/* Same output as recursive inorder but uses an explicit stack (no recursion)*/
void inorderIterative(Node *root)
{
    int n = countNodes(root);
    if (n == 0) { printf("(empty)"); return; }
    Node **stack = (Node **)malloc(n * sizeof(Node *));
    if (!stack) { fprintf(stderr,"Out of memory\n"); exit(EXIT_FAILURE); }
    int top = -1;
    Node *cur = root;
    while (cur != NULL || top >= 0) {
        while (cur != NULL) {               /* go as far left as possible   */
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];                 /* pop, visit                   */
        printf("%d ", cur->data);
        cur = cur->right;                   /* then handle right subtree    */
    }
    free(stack);
}

/* ------------------- INORDER SUCCESSOR / PREDECESSOR -------------------- */
/* Successor = smallest key strictly greater than 'key'.                    */
Node *inorderSuccessor(Node *root, int key)
{
    Node *succ = NULL;
    while (root != NULL) {
        if (key < root->data) { succ = root; root = root->left; }
        else root = root->right;
    }
    return succ;
}
/* Predecessor = largest key strictly smaller than 'key'.                   */
Node *inorderPredecessor(Node *root, int key)
{
    Node *pred = NULL;
    while (root != NULL) {
        if (key > root->data) { pred = root; root = root->right; }
        else root = root->left;
    }
    return pred;
}

/* ------------------------- K-TH SMALLEST ELEMENT ------------------------ */
/* Inorder visits keys in sorted order, so the k-th visited node is the     */
/* k-th smallest. 'count' tracks how many nodes have been visited so far.   */
void kthHelper(Node *r, int k, int *count, int *result, int *found)
{
    if (r == NULL || *found) return;
    kthHelper(r->left, k, count, result, found);
    if (!*found && ++(*count) == k) { *result = r->data; *found = 1; return; }
    kthHelper(r->right, k, count, result, found);
}

/* --------------------- LOWEST COMMON ANCESTOR (LCA) --------------------- */
/* In a BST the LCA of a and b is the first node whose key lies between     */
/* them (or equals one of them) while walking down from the root.           */
Node *lca(Node *root, int a, int b)
{
    while (root != NULL) {
        if (a < root->data && b < root->data) root = root->left;
        else if (a > root->data && b > root->data) root = root->right;
        else return root;                   /* the paths split here         */
    }
    return NULL;
}

/* ------------------------- PRINT KEYS IN A RANGE ------------------------ */
void printRange(Node *r, int lo, int hi)
{
    if (r == NULL) return;
    if (lo < r->data) printRange(r->left, lo, hi);          /* left may help */
    if (lo <= r->data && r->data <= hi) printf("%d ", r->data);
    if (hi > r->data) printRange(r->right, lo, hi);         /* right may help*/
}

/* ------------------------- VALIDATE BST PROPERTY ------------------------ */
/* Every node must lie strictly between (lo, hi) inherited from ancestors.  */
int isBSTUtil(Node *r, long long lo, long long hi)
{
    if (r == NULL) return 1;
    if (r->data <= lo || r->data >= hi) return 0;
    return isBSTUtil(r->left, lo, r->data) && isBSTUtil(r->right, r->data, hi);
}
int isBST(Node *root) { return isBSTUtil(root, -2147483649LL, 2147483648LL); }

/* ---------------------- SIDEWAYS TREE DISPLAY --------------------------- */
/* Reverse inorder (R, Root, L) with indentation: root at left, right child */
/* printed above, left child below.                                          */
void printTree(Node *r, int space)
{
    if (r == NULL) return;
    printTree(r->right, space + 6);
    printf("%*s%d\n", space, "", r->data);
    printTree(r->left, space + 6);
}

/* Free every node (postorder, because children must be freed first)        */
void freeTree(Node *r)
{
    if (r) { freeTree(r->left); freeTree(r->right); free(r); }
}

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    Node *root = NULL;
    int choice, x, y;

    while (1) {
        printf("\n========== BST (DYNAMIC) MENU ==========\n");
        printf(" 1. Insert              2. Delete            3. Search\n");
        printf(" 4. Inorder             5. Preorder          6. Postorder\n");
        printf(" 7. Level order         8. Iterative inorder 9. Height / counts\n");
        printf("10. Min and Max        11. Successor/Predecessor\n");
        printf("12. k-th smallest      13. LCA of two keys  14. Print keys in range\n");
        printf("15. Is it a BST?       16. Show tree shape  17. BST SORT (application)\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter key to insert: "); checked_scanf("%d", &x);
            root = insert(root, x);
            break;
        case 2:
            printf("Enter key to delete: "); checked_scanf("%d", &x);
            if (search(root, x) == NULL) printf("Key %d not found.\n", x);
            else { root = deleteNode(root, x); printf("Key %d deleted.\n", x); }
            break;
        case 3:
            printf("Enter key to search: "); checked_scanf("%d", &x);
            printf(search(root, x) ? "Key %d FOUND.\n" : "Key %d NOT found.\n", x);
            break;
        case 4: printf("Inorder   : "); if (root) inorder(root); else printf("(empty)"); printf("\n"); break;
        case 5: printf("Preorder  : "); if (root) preorder(root); else printf("(empty)"); printf("\n"); break;
        case 6: printf("Postorder : "); if (root) postorder(root); else printf("(empty)"); printf("\n"); break;
        case 7: printf("Level order: "); levelOrder(root); printf("\n"); break;
        case 8: printf("Iterative inorder: "); inorderIterative(root); printf("\n"); break;
        case 9:
            printf("Height = %d, Nodes = %d, Leaves = %d\n",
                   height(root), countNodes(root), countLeaves(root));
            break;
        case 10:
            if (root == NULL) printf("Tree is empty.\n");
            else printf("Min = %d, Max = %d\n", findMin(root)->data, findMax(root)->data);
            break;
        case 11: {
            printf("Enter key: "); checked_scanf("%d", &x);
            Node *s = inorderSuccessor(root, x), *p = inorderPredecessor(root, x);
            if (s) printf("Successor   of %d = %d\n", x, s->data); else printf("No successor for %d\n", x);
            if (p) printf("Predecessor of %d = %d\n", x, p->data); else printf("No predecessor for %d\n", x);
            break;
        }
        case 12: {
            int count = 0, result = 0, found = 0;
            printf("Enter k: "); checked_scanf("%d", &x);
            kthHelper(root, x, &count, &result, &found);
            if (found) printf("k = %d -> k-th smallest element = %d\n", x, result);
            else printf("Tree has fewer than %d nodes.\n", x);
            break;
        }
        case 13:
            printf("Enter two keys (both must be in the tree): "); checked_scanf("%d %d", &x, &y);
            if (search(root, x) && search(root, y)) printf("LCA(%d, %d) = %d\n", x, y, lca(root, x, y)->data);
            else printf("Both keys must exist in the tree.\n");
            break;
        case 14:
            printf("Enter lo and hi: "); checked_scanf("%d %d", &x, &y);
            printf("Keys in [%d, %d]: ", x, y); printRange(root, x, y); printf("\n");
            break;
        case 15:
            printf(isBST(root) ? "Yes, the tree satisfies the BST property.\n"
                               : "No, it is NOT a BST.\n");
            break;
        case 16:
            if (root == NULL) printf("(empty)\n");
            else { printf("(root at left; right subtree above, left subtree below)\n"); printTree(root, 0); }
            break;
        case 17: {
            /* APPLICATION: BST sort = insert all numbers, then inorder.     */
            int n, i, v;
            Node *tmp = NULL;
            printf("How many numbers? "); checked_scanf("%d", &n);
            printf("Enter %d numbers (duplicates are dropped): ", n);
            for (i = 0; i < n; i++) { checked_scanf("%d", &v); tmp = insert(tmp, v); }
            printf("Sorted output: "); inorder(tmp); printf("\n");
            freeTree(tmp);
            break;
        }
        case 0:
            freeTree(root);
            printf("Bye!\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
    freeTree(root);
    return 0;
}
