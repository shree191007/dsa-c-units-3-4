/*==========================================================================
 * FILE  : 10_priority_queue_menu.c
 * TOPIC : PRIORITY QUEUE implemented with a binary HEAP
 *         (min-priority queue and max-priority queue)
 *==========================================================================
 *
 * THEORY
 * ------
 * A PRIORITY QUEUE is an abstract data type where every element carries a
 * PRIORITY and the element that is removed next is always the one with the
 * best priority (not the oldest one, as in an ordinary FIFO queue).
 *
 *      MIN-priority queue : smallest priority number is served first
 *                           (e.g. 1 = most urgent)           -> MIN-heap
 *      MAX-priority queue : largest  priority number is served first
 *                           (e.g. 10 = most urgent)          -> MAX-heap
 *
 * Why a heap?  Compare the ways to implement a priority queue:
 *
 *      structure          enqueue     dequeue(best)    peek
 *      unsorted array      O(1)         O(n)           O(n)
 *      sorted array        O(n)         O(1)           O(1)
 *      HEAP              O(log n)     O(log n)         O(1)     <- balanced
 *
 * Each element here is a record  { name, priority, seq }.
 * 'seq' is an arrival counter used as a TIE-BREAKER so that elements of
 * EQUAL priority leave in first-come-first-served order (a plain heap is not
 * stable by itself).  Comparison rule:
 *      a is served before b  if  a has better priority,
 *                            or  equal priority and a arrived earlier.
 *
 * Heap layout: 1-based array, parent(i)=i/2, left(i)=2i, right(i)=2i+1.
 *
 * OPERATIONS
 * ----------
 *   enqueue(name, priority)  : insert at the end, SIFT UP
 *   dequeue()                : take root, move last to root, SIFT DOWN
 *   peek()                   : look at root without removing
 *   changePriority(name, p)  : find element, update priority, then sift up
 *                              or down (decrease-key / increase-key)
 *   display                  : array order + the order in which items would
 *                              be served (done on a copy; queue unchanged)
 *   APPLICATION: a small task scheduler that runs all queued tasks in order.
 *
 * COMPLEXITY
 * ----------
 *   enqueue O(log n)   dequeue O(log n)   peek O(1)
 *   changePriority O(n) to find + O(log n) to fix     Space O(n)
 *
 * COMPILE & RUN
 * -------------
 *  gcc 10_priority_queue_menu.c -o pq && ./pq
 *
 * SAMPLE OUTPUT
 * -------------
 *   ===== PRIORITY QUEUE (HEAP) MENU  [MIN-PQ: smaller number = higher priority] =====
 *    1. Enqueue (name, priority)   2. Dequeue (serve next)
 *    3. Peek                       4. Change priority of an item
 *    5. Display                    6. Switch MIN <-> MAX priority queue
 *    7. Size / isEmpty / isFull    8. Run scheduler (serve ALL in order)
 *    0. Exit
 *   Enter choice: 1
 *   Enter name (no spaces) and priority: Surgery
 *   1
 *   Enqueued Surgery with priority 1.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter name (no spaces) and priority: Checkup
 *   5
 *   Enqueued Checkup with priority 5.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter name (no spaces) and priority: Fracture
 *   2
 *   Enqueued Fracture with priority 2.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter name (no spaces) and priority: Flu
 *   5
 *   Enqueued Flu with priority 5.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter name (no spaces) and priority: Burn
 *   2
 *   Enqueued Burn with priority 2.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Heap array order (index: name/priority):
 *     [1: Surgery/1] [2: Burn/2] [3: Fracture/2] [4: Flu/5] [5: Checkup/5]
 *   Service order (next served first):
 *     Surgery(1) Fracture(2) Burn(2) Checkup(5) Flu(5)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 3
 *   Next to be served: Surgery (priority 1)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Served: Surgery (priority 1)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Enter name and new priority: Flu
 *   1
 *   Priority of Flu updated.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Heap array order (index: name/priority):
 *     [1: Flu/1] [2: Fracture/2] [3: Checkup/5] [4: Burn/2]
 *   Service order (next served first):
 *     Flu(1) Fracture(2) Burn(2) Checkup(5)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Switched. Existing items were re-heapified.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Heap array order (index: name/priority):
 *     [1: Checkup/5] [2: Fracture/2] [3: Flu/1] [4: Burn/2]
 *   Service order (next served first):
 *     Checkup(5) Fracture(2) Burn(2) Flu(1)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Scheduler running tasks in priority order:
 *     Step 1: running Checkup    (priority 5)
 *     Step 2: running Fracture   (priority 2)
 *     Step 3: running Burn       (priority 2)
 *     Step 4: running Flu        (priority 1)
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
#include <string.h>

#define CAP 50

typedef struct {
    char name[32];      /* task / patient / job name                         */
    int  priority;      /* the priority number                               */
    int  seq;           /* arrival order, used to break ties                 */
} Item;

Item pq[CAP + 1];       /* heap array, indices 1..size                       */
int size = 0;
int isMin = 1;          /* 1 = min-priority queue, 0 = max-priority queue    */
int arrivalCounter = 0; /* increases with every enqueue                      */

/* Should item a be served BEFORE item b?                                    */
int before(Item a, Item b)
{
    if (a.priority != b.priority)
        return isMin ? (a.priority < b.priority) : (a.priority > b.priority);
    return a.seq < b.seq;                               /* tie: older first  */
}

void swapItems(Item *a, Item *b) { Item t = *a; *a = *b; *b = t; }

void siftUp(int i)
{
    while (i > 1 && before(pq[i], pq[i / 2])) {
        swapItems(&pq[i], &pq[i / 2]);
        i /= 2;
    }
}

void siftDown(int i, int n)
{
    while (2 * i <= n) {
        int c = 2 * i;
        if (c + 1 <= n && before(pq[c + 1], pq[c])) c++;    /* better child  */
        if (!before(pq[c], pq[i])) break;
        swapItems(&pq[i], &pq[c]);
        i = c;
    }
}

/* ------------------------------ ENQUEUE --------------------------------- */
int enqueue(const char *name, int priority)
{
    if (size == CAP) return 0;                          /* queue full        */
    size++;
    strncpy(pq[size].name, name, 31);
    pq[size].name[31] = '\0';
    pq[size].priority = priority;
    pq[size].seq = arrivalCounter++;
    siftUp(size);
    return 1;
}

/* ------------------------------ DEQUEUE --------------------------------- */
Item dequeue(void)
{
    Item top = pq[1];
    pq[1] = pq[size--];                                 /* last item -> root */
    siftDown(1, size);
    return top;
}

/* ---------------------------- CHANGE PRIORITY --------------------------- */
int changePriority(const char *name, int newPriority)
{
    for (int i = 1; i <= size; i++) {
        if (strcmp(pq[i].name, name) == 0) {
            pq[i].priority = newPriority;
            siftUp(i);                                  /* one of these two  */
            siftDown(i, size);                          /* will do the work  */
            return 1;
        }
    }
    return 0;
}

/* ------------------------------- DISPLAY -------------------------------- */
void display(void)
{
    if (size == 0) { printf("(queue is empty)\n"); return; }

    printf("Heap array order (index: name/priority):\n  ");
    for (int i = 1; i <= size; i++) printf("[%d: %s/%d] ", i, pq[i].name, pq[i].priority);
    printf("\n");

    /* Show service order without destroying the queue: work on a copy       */
    Item backup[CAP + 1]; int n = size;
    memcpy(backup, pq, sizeof(pq));
    printf("Service order (next served first):\n  ");
    while (size > 0) { Item t = dequeue(); printf("%s(%d) ", t.name, t.priority); }
    printf("\n");
    memcpy(pq, backup, sizeof(pq));
    size = n;
}

/* Switch between min and max priority queue: rebuild the heap bottom-up     */
void switchType(void)
{
    isMin = !isMin;
    for (int i = size / 2; i >= 1; i--) siftDown(i, size);
}

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    int choice, p;
    char name[64];

    while (1) {
        printf("\n===== PRIORITY QUEUE (HEAP) MENU  [%s] =====\n",
               isMin ? "MIN-PQ: smaller number = higher priority"
                     : "MAX-PQ: larger number = higher priority");
        printf(" 1. Enqueue (name, priority)   2. Dequeue (serve next)\n");
        printf(" 3. Peek                       4. Change priority of an item\n");
        printf(" 5. Display                    6. Switch MIN <-> MAX priority queue\n");
        printf(" 7. Size / isEmpty / isFull    8. Run scheduler (serve ALL in order)\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter name (no spaces) and priority: ");
            checked_scanf("%63s %d", name, &p);
            if (enqueue(name, p)) printf("Enqueued %s with priority %d.\n", name, p);
            else printf("Queue is FULL.\n");
            break;
        case 2:
            if (size == 0) printf("Queue is empty.\n");
            else { Item t = dequeue(); printf("Served: %s (priority %d)\n", t.name, t.priority); }
            break;
        case 3:
            if (size == 0) printf("Queue is empty.\n");
            else printf("Next to be served: %s (priority %d)\n", pq[1].name, pq[1].priority);
            break;
        case 4:
            printf("Enter name and new priority: "); checked_scanf("%63s %d", name, &p);
            printf(changePriority(name, p) ? "Priority of %s updated.\n" : "%s not found.\n", name);
            break;
        case 5: display(); break;
        case 6: switchType(); printf("Switched. Existing items were re-heapified.\n"); break;
        case 7:
            printf("Size = %d, empty = %s, full = %s\n", size,
                   size == 0 ? "yes" : "no", size == CAP ? "yes" : "no");
            break;
        case 8:
            if (size == 0) { printf("Nothing to run.\n"); break; }
            printf("Scheduler running tasks in priority order:\n");
            for (int step = 1; size > 0; step++) {
                Item t = dequeue();
                printf("  Step %d: running %-10s (priority %d)\n", step, t.name, t.priority);
            }
            break;
        case 0: printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
