/*==========================================================================
 * FILE  : 09_bst_array_sequential_menu.c
 * TOPIC : Binary Search Tree using an ARRAY (sequential / index formula)
 *==========================================================================
 *
 * THEORY
 * ------
 * A binary tree can be stored in a plain array without any pointers.
 * The root is kept at index 0 and the positions of the other nodes follow
 * a fixed formula (0-based indexing):
 *
 *        left  child of node at index i   ->  2*i + 1
 *        right child of node at index i   ->  2*i + 2
 *        parent      of node at index i   ->  (i - 1) / 2
 *
 * A slot that has no node is marked with a special value (EMPTY = -1 here,
 * so this program stores NON-NEGATIVE keys only).
 *
 * Example: insert 50, 30, 70, 20, 40, 60
 *
 *   index : 0   1   2   3   4   5   6
 *   value : 50  30  70  20  40  60  -1
 *
 *                  50
 *                /    \
 *              30      70
 *             /  \    /
 *           20   40  60
 *
 * ADVANTAGES    : no pointers, no malloc, easy parent/child lookup.
 * DISADVANTAGES : wastes memory for skewed trees (a tree of n nodes may
 *                 need 2^n - 1 slots in the worst case), fixed maximum size.
 *                 Inserting 1,2,3,4,... in sorted order quickly overflows.
 *
 * ALGORITHMS
 * ----------
 * INSERT : start at index 0. If the slot is EMPTY, store the key there.
 *          Otherwise move to 2i+1 (key smaller) or 2i+2 (key larger).
 *          If the index goes beyond the array size -> OVERFLOW.
 * SEARCH : same walk; stop at EMPTY (not found) or when key matches.
 * DELETE : 1) leaf         -> mark the slot EMPTY
 *          2) one child    -> MOVE the whole child subtree up one level
 *                             (copy slot by slot to the matching new index)
 *          3) two children -> copy the inorder successor's key into the node,
 *                             then delete the successor (it has no left child)
 * TRAVERSALS : recursive on indices (inorder/preorder/postorder).
 *              LEVEL ORDER is simply a scan of the array from 0 to MAX-1
 *              skipping EMPTY slots (that is the beauty of this layout).
 *
 * COMPLEXITY
 * ----------
 *  Insert / Search / Delete : O(h)  (delete may cost O(size of moved subtree))
 *  Traversals               : O(MAX)
 *  Space                    : O(MAX) fixed, regardless of number of keys
 *
 * COMPILE & RUN
 * -------------
 *  gcc 09_bst_array_sequential_menu.c -o bst_array && ./bst_array
 *
 * SAMPLE OUTPUT
 * -------------
 *   ===== BST USING ARRAY (SEQUENTIAL) MENU =====
 *    1. Insert            2. Delete          3. Search
 *    4. Inorder           5. Preorder        6. Postorder
 *    7. Level order       8. Show array      9. Show tree shape
 *   10. Height / count   11. Min / Max      12. Parent & children of a key
 *    0. Exit
 *   Enter choice: 1
 *   Enter key (non-negative): 50
 *   Inserted 50 at index 0.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key (non-negative): 30
 *   Inserted 30 at index 1.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key (non-negative): 70
 *   Inserted 70 at index 2.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key (non-negative): 20
 *   Inserted 20 at index 3.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key (non-negative): 40
 *   Inserted 40 at index 4.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter key (non-negative): 60
 *   Inserted 60 at index 5.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Index :    0   1   2   3   4   5
 *   Value :   50  30  70  20  40  60
 *   ('-' means the slot is EMPTY)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 9
 *         70
 *               60
 *   50
 *               40
 *         30
 *               20
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 20 30 40 50 60 70
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *   Level order: 50 30 70 20 40 60
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 12
 *   Enter key: 40
 *   Key 40 is at index 4
 *     Parent : 30 (index 1)
 *     Left   : none
 *     Right  : none
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 30
 *   Key 30 deleted.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Index :    0   1   2   3   4   5
 *   Value :   50  40  70  20   -  60
 *   ('-' means the slot is EMPTY)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 20 40 50 60 70
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter key to delete: 50
 *   Key 50 deleted.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Index :    0   1   2   3
 *   Value :   60  40  70  20
 *   ('-' means the slot is EMPTY)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Inorder   : 20 40 60 70
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 10
 *   Height = 3, Nodes = 4
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 11
 *   Min = 20, Max = 70
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

#define MAX   63        /* array size: enough for a full tree of height 6    */
#define EMPTY (-1)      /* marker for an unused slot (keys must be >= 0)     */

int tree[MAX];          /* the BST lives entirely in this array              */

/* Mark every slot as empty (an empty tree) */
void initTree(void)
{
    for (int i = 0; i < MAX; i++) tree[i] = EMPTY;
}

/* Is index i a valid slot holding a node? (guards against going past MAX)   */
int isNode(int i) { return i >= 0 && i < MAX && tree[i] != EMPTY; }

/* ------------------------------- INSERT --------------------------------- */
void insert(int key)
{
    int i = 0;
    while (i < MAX && tree[i] != EMPTY) {
        if (key == tree[i]) { printf("Key %d already exists.\n", key); return; }
        i = (key < tree[i]) ? 2 * i + 1 : 2 * i + 2;       /* child index    */
    }
    if (i >= MAX) {                                        /* ran off array  */
        printf("OVERFLOW: no room for %d (tree too deep for array size %d).\n", key, MAX);
        return;
    }
    tree[i] = key;
    printf("Inserted %d at index %d.\n", key, i);
}

/* -------------------------------- SEARCH -------------------------------- */
/* Returns the INDEX of key, or -1 when absent.                             */
int search(int key)
{
    int i = 0;
    while (i < MAX && tree[i] != EMPTY) {
        if (key == tree[i]) return i;
        i = (key < tree[i]) ? 2 * i + 1 : 2 * i + 2;
    }
    return -1;
}

/* Index of the smallest / largest key in the subtree rooted at index i     */
int minIndex(int i) { while (isNode(2 * i + 1)) i = 2 * i + 1; return i; }
int maxIndex(int i) { while (isNode(2 * i + 2)) i = 2 * i + 2; return i; }

/* ------------------------------- DELETE --------------------------------- */
/* Move the entire subtree rooted at index 'from' so that it is rooted at   */
/* index 'to'. Each node keeps the same relative position inside the subtree.*/
/* The source slot is cleared BEFORE recursing, because the destination of a */
/* deeper node can coincide with a slot the source used to occupy.           */
void moveSubtree(int from, int to)
{
    if (!isNode(from)) return;
    int val = tree[from];
    tree[from] = EMPTY;
    tree[to] = val;
    moveSubtree(2 * from + 1, 2 * to + 1);     /* left part moves too       */
    moveSubtree(2 * from + 2, 2 * to + 2);     /* right part moves too      */
}

/* Delete the node stored at index i                                         */
void deleteAt(int i)
{
    int l = 2 * i + 1, r = 2 * i + 2;
    if (!isNode(l) && !isNode(r)) {            /* CASE 1: leaf              */
        tree[i] = EMPTY;
    } else if (!isNode(r)) {                   /* CASE 2a: only left child  */
        moveSubtree(l, i);                     /* pull left subtree up      */
    } else if (!isNode(l)) {                   /* CASE 2b: only right child */
        moveSubtree(r, i);                     /* pull right subtree up     */
    } else {                                   /* CASE 3: two children      */
        int s = minIndex(r);                   /* inorder successor index   */
        tree[i] = tree[s];                     /* copy its key up           */
        deleteAt(s);                           /* remove successor (<=1 child)*/
    }
}

void deleteKey(int key)
{
    int i = search(key);
    if (i == -1) { printf("Key %d not found.\n", key); return; }
    deleteAt(i);
    printf("Key %d deleted.\n", key);
}

/* ----------------------------- TRAVERSALS ------------------------------- */
void inorder(int i)   { if (isNode(i)) { inorder(2*i+1);  printf("%d ", tree[i]); inorder(2*i+2); } }
void preorder(int i)  { if (isNode(i)) { printf("%d ", tree[i]); preorder(2*i+1); preorder(2*i+2); } }
void postorder(int i) { if (isNode(i)) { postorder(2*i+1); postorder(2*i+2); printf("%d ", tree[i]); } }

/* Level order = left-to-right scan of the array, ignoring empty slots       */
void levelOrder(void)
{
    for (int i = 0; i < MAX; i++)
        if (tree[i] != EMPTY) printf("%d ", tree[i]);
}

/* ------------------------- HEIGHT / COUNT ------------------------------- */
int height(int i)
{
    if (!isNode(i)) return 0;
    int hl = height(2 * i + 1), hr = height(2 * i + 2);
    return 1 + (hl > hr ? hl : hr);
}
int countNodes(int i)
{
    return isNode(i) ? 1 + countNodes(2 * i + 1) + countNodes(2 * i + 2) : 0;
}

/* Show the array contents (only up to the last used slot) with indices     */
void displayArray(void)
{
    int last = -1;
    for (int i = 0; i < MAX; i++) if (tree[i] != EMPTY) last = i;
    if (last == -1) { printf("(empty tree)\n"); return; }
    printf("Index : "); for (int i = 0; i <= last; i++) printf("%4d", i);
    printf("\nValue : "); for (int i = 0; i <= last; i++) {
        if (tree[i] == EMPTY) printf("   -"); else printf("%4d", tree[i]);
    }
    printf("\n('-' means the slot is EMPTY)\n");
}

/* Sideways tree picture (right subtree on top)                              */
void printTree(int i, int space)
{
    if (!isNode(i)) return;
    printTree(2 * i + 2, space + 6);
    printf("%*s%d\n", space, "", tree[i]);
    printTree(2 * i + 1, space + 6);
}

/* Parent / children of a key, using the index formulas                      */
void showRelatives(int key)
{
    int i = search(key);
    if (i == -1) { printf("Key %d not found.\n", key); return; }
    printf("Key %d is at index %d\n", key, i);
    if (i == 0) printf("  Parent : none (it is the root)\n");
    else        printf("  Parent : %d (index %d)\n", tree[(i - 1) / 2], (i - 1) / 2);
    if (isNode(2*i+1)) printf("  Left   : %d (index %d)\n", tree[2*i+1], 2*i+1); else printf("  Left   : none\n");
    if (isNode(2*i+2)) printf("  Right  : %d (index %d)\n", tree[2*i+2], 2*i+2); else printf("  Right  : none\n");
}

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    int choice, x;
    initTree();

    while (1) {
        printf("\n===== BST USING ARRAY (SEQUENTIAL) MENU =====\n");
        printf(" 1. Insert            2. Delete          3. Search\n");
        printf(" 4. Inorder           5. Preorder        6. Postorder\n");
        printf(" 7. Level order       8. Show array      9. Show tree shape\n");
        printf("10. Height / count   11. Min / Max      12. Parent & children of a key\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter key (non-negative): "); checked_scanf("%d", &x);
            if (x < 0) printf("Only non-negative keys are allowed.\n"); else insert(x);
            break;
        case 2: printf("Enter key to delete: "); checked_scanf("%d", &x); deleteKey(x); break;
        case 3:
            printf("Enter key to search: "); checked_scanf("%d", &x);
            { int i = search(x);
              if (i >= 0) printf("Key %d FOUND at index %d.\n", x, i); else printf("Key %d NOT found.\n", x); }
            break;
        case 4: printf("Inorder   : "); inorder(0);   printf("\n"); break;
        case 5: printf("Preorder  : "); preorder(0);  printf("\n"); break;
        case 6: printf("Postorder : "); postorder(0); printf("\n"); break;
        case 7: printf("Level order: "); levelOrder(); printf("\n"); break;
        case 8: displayArray(); break;
        case 9: if (!isNode(0)) printf("(empty)\n"); else printTree(0, 0); break;
        case 10: printf("Height = %d, Nodes = %d\n", height(0), countNodes(0)); break;
        case 11:
            if (!isNode(0)) printf("Tree is empty.\n");
            else printf("Min = %d, Max = %d\n", tree[minIndex(0)], tree[maxIndex(0)]);
            break;
        case 12: printf("Enter key: "); checked_scanf("%d", &x); showRelatives(x); break;
        case 0: printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
