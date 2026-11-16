/*==========================================================================
 * FILE  : 14_graph_adjacency_list.c
 * TOPIC : GRAPH - properties and representation using an ADJACENCY LIST
 *==========================================================================
 *
 * THEORY
 * ------
 * (Definitions of vertex, edge, degree, directed / undirected, weighted,
 *  handshaking lemma, simple / complete / regular graph etc. are explained
 *  in the header of file 13_graph_adjacency_matrix.c - same meaning here.)
 *
 * ADJACENCY LIST REPRESENTATION
 * -----------------------------
 * An array of V linked lists. Entry adjList[u] is the head of a linked list
 * containing every vertex that is adjacent to u (with the weight of the edge).
 *
 *   Graph:  0 - 1,  0 - 2,  1 - 2,  2 - 3
 *
 *      adjList[0] -> [1] -> [2] -> NULL
 *      adjList[1] -> [0] -> [2] -> NULL
 *      adjList[2] -> [0] -> [1] -> [3] -> NULL
 *      adjList[3] -> [2] -> NULL
 *
 *   Undirected edge {u,v} appears TWICE: v in u's list and u in v's list.
 *   Directed   edge u->v appears ONCE : v in u's list only.
 *   A self-loop (u,u) is stored ONCE in u's list.
 *
 * Each list node:   | dest | weight | next |
 *
 * ALGORITHMS
 * ----------
 *  add edge (u,v,w) : append a node (v) to list u (and u to list v if
 *                     undirected). We first check the list to avoid duplicates.
 *  remove edge      : unlink the node from the list(s) and free it.
 *  edge exists?     : search u's list                      -> O(deg(u))
 *  neighbours of u  : just walk u's list                   -> O(deg(u))
 *  out-degree of u  : length of list u
 *  in-degree of u   : count how many lists contain u       -> O(V + E)
 *  count edges      : sum of list lengths (adjust for undirected / loops)
 *
 * COMPLEXITY / WHEN TO USE
 * ------------------------
 *  Space : O(V + E)  <- much better than O(V^2) for SPARSE graphs
 *  add edge : O(deg) here (duplicate check) / O(1) if no check is made
 *  Traversals (DFS / BFS) : O(V + E)  vs  O(V^2) with a matrix
 *  Edge test u-v is slower than a matrix (O(deg(u)) instead of O(1)).
 *
 * COMPILE & RUN
 * -------------
 *  gcc 14_graph_adjacency_list.c -o graph_list && ./graph_list
 *  (vertices are numbered 0 .. V-1)
 *
 * SAMPLE OUTPUT
 * -------------
 *   Number of vertices (1-20): 4
 *   Directed graph?  (1 = yes, 0 = no): 1
 *   Weighted graph?  (1 = yes, 0 = no): 1
 *
 *   ===== GRAPH - ADJACENCY LIST MENU =====
 *    1. Add edge           2. Remove edge        3. Add vertex
 *    4. Display list       5. Does edge u-v exist?  6. Neighbours of a vertex
 *    7. Degrees            8. Graph properties summary
 *    9. Show equivalent matrix
 *    0. Exit
 *   Enter choice: 1
 *   Enter u v weight: 0
 *   1
 *   5
 *   Edge 0->1 added (weight 5).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v weight: 0
 *   2
 *   3
 *   Edge 0->2 added (weight 3).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v weight: 1
 *   2
 *   2
 *   Edge 1->2 added (weight 2).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v weight: 2
 *   3
 *   7
 *   Edge 2->3 added (weight 7).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v weight: 3
 *   0
 *   4
 *   Edge 3->0 added (weight 4).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Adjacency list (directed, weighted):
 *      0 -> [1,w=5] -> [2,w=3] -> NULL
 *      1 -> [2,w=2] -> NULL
 *      2 -> [3,w=7] -> NULL
 *      3 -> [0,w=4] -> NULL
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *   Vertex  In-degree  Out-degree
 *      0        1          2
 *      1        1          1
 *      2        2          1
 *      3        1          1
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   ---- Graph properties ----
 *   Type              : directed, weighted
 *   Vertices |V|      : 4
 *   Edges    |E|      : 5
 *   Sum in-degrees    : 5,  sum out-degrees : 5  (both must equal |E| = 5)
 *   Self-loops        : no (0)
 *   Simple graph      : yes (duplicates are rejected on insert)
 *   Complete graph    : no
 *   Regular graph     : no
 *   Isolated vertices : none
 *   Density           : 0.42  (5 of 12 possible edges)
 *   List memory       : 4 head pointers + 5 list nodes  (grows with |E|, not V^2)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 9
 *   Equivalent adjacency matrix:
 *          0  1  2  3
 *     0 |  0  5  3  0
 *     1 |  0  0  2  0
 *     2 |  0  0  0  7
 *     3 |  4  0  0  0
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter u v: 0
 *   1
 *   Edge removed.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Adjacency list (directed, weighted):
 *      0 -> [2,w=3] -> NULL
 *      1 -> [2,w=2] -> NULL
 *      2 -> [3,w=7] -> NULL
 *      3 -> [0,w=4] -> NULL
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Enter vertex: 2
 *   Neighbours of 2 (out-going): 3(w=7)
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

#define MAXV 20

typedef struct ANode {
    int dest;               /* the adjacent vertex                           */
    int weight;             /* weight of the edge (1 if unweighted)          */
    struct ANode *next;
} ANode;

ANode *adjList[MAXV];       /* adjList[u] = head of u's linked list          */
int V = 0;
int directed = 0;
int weighted = 0;

int validVertex(int v) { return v >= 0 && v < V; }

ANode *newANode(int dest, int w)
{
    ANode *n = (ANode *)malloc(sizeof(ANode));
    if (!n) { printf("Out of memory\n"); exit(1); }
    n->dest = dest; n->weight = w; n->next = NULL;
    return n;
}

/* Find the node for v inside u's list (NULL if edge u->v does not exist)    */
ANode *findEdge(int u, int v)
{
    for (ANode *p = adjList[u]; p; p = p->next) if (p->dest == v) return p;
    return NULL;
}

/* Append v (with weight w) at the END of u's list so the order of input is
   preserved. Tail walk costs O(deg(u)).                                      */
void appendNode(int u, int v, int w)
{
    ANode *n = newANode(v, w);
    if (!adjList[u]) { adjList[u] = n; return; }
    ANode *p = adjList[u];
    while (p->next) p = p->next;
    p->next = n;
}

/* ------------------------------ ADD EDGE -------------------------------- */
void addEdge(int u, int v, int w)
{
    if (!validVertex(u) || !validVertex(v)) { printf("Invalid vertex (use 0..%d).\n", V - 1); return; }
    if (w <= 0) { printf("Weight must be positive.\n"); return; }

    ANode *e = findEdge(u, v);
    if (e) {                                            /* already present    */
        e->weight = w;
        if (!directed && u != v) findEdge(v, u)->weight = w;
        printf("Edge already existed; weight updated.\n");
        return;
    }
    appendNode(u, v, w);
    if (!directed && u != v) appendNode(v, u, w);       /* mirror edge        */
    printf("Edge %d%s%d added (weight %d).\n", u, directed ? "->" : "-", v, w);
}

/* ---------------------------- REMOVE EDGE ------------------------------- */
/* Unlink v from u's list (classic linked-list deletion).                    */
int unlinkNode(int u, int v)
{
    ANode *prev = NULL, *p = adjList[u];
    while (p && p->dest != v) { prev = p; p = p->next; }
    if (!p) return 0;
    if (prev) prev->next = p->next; else adjList[u] = p->next;
    free(p);
    return 1;
}

void removeEdge(int u, int v)
{
    if (!validVertex(u) || !validVertex(v)) { printf("Invalid vertex.\n"); return; }
    if (!unlinkNode(u, v)) { printf("No such edge.\n"); return; }
    if (!directed && u != v) unlinkNode(v, u);
    printf("Edge removed.\n");
}

void addVertex(void)
{
    if (V == MAXV) { printf("Maximum of %d vertices reached.\n", MAXV); return; }
    adjList[V++] = NULL;
    printf("Vertex %d added.\n", V - 1);
}

/* ------------------------------- DISPLAY -------------------------------- */
void displayList(void)
{
    printf("Adjacency list (%s%s):\n", directed ? "directed" : "undirected", weighted ? ", weighted" : "");
    for (int u = 0; u < V; u++) {
        printf("  %2d -> ", u);
        for (ANode *p = adjList[u]; p; p = p->next) {
            if (weighted) printf("[%d,w=%d] -> ", p->dest, p->weight);
            else printf("[%d] -> ", p->dest);
        }
        printf("NULL\n");
    }
}

/* ---------------------------- DEGREE helpers ---------------------------- */
int outDegree(int u) { int d = 0; for (ANode *p = adjList[u]; p; p = p->next) d++; return d; }
int inDegree(int v)
{
    int d = 0;
    for (int u = 0; u < V; u++) if (findEdge(u, v)) d++;
    return d;
}
/* undirected degree: a self-loop is stored once but counts 2                 */
int degree(int u) { return outDegree(u) + (findEdge(u, u) ? 1 : 0); }

void showNeighbours(int u)
{
    if (!validVertex(u)) { printf("Invalid vertex.\n"); return; }
    printf("Neighbours of %d%s: ", u, directed ? " (out-going)" : "");
    if (!adjList[u]) printf("none");
    for (ANode *p = adjList[u]; p; p = p->next) {
        if (weighted) printf("%d(w=%d) ", p->dest, p->weight); else printf("%d ", p->dest);
    }
    printf("\n");
}

void showDegrees(void)
{
    if (directed) {
        printf("Vertex  In-degree  Out-degree\n");
        for (int v = 0; v < V; v++) printf("%4d %8d %10d\n", v, inDegree(v), outDegree(v));
    } else {
        printf("Vertex  Degree\n");
        for (int v = 0; v < V; v++) printf("%4d %8d\n", v, degree(v));
    }
}

/* ------------------------- PROPERTIES SUMMARY --------------------------- */
void showProperties(void)
{
    int loops = 0, nonLoopEntries = 0, edges;

    for (int u = 0; u < V; u++)
        for (ANode *p = adjList[u]; p; p = p->next) {
            if (p->dest == u) loops++; else nonLoopEntries++;
        }
    edges = directed ? nonLoopEntries + loops : nonLoopEntries / 2 + loops;

    printf("---- Graph properties ----\n");
    printf("Type              : %s%s\n", directed ? "directed" : "undirected", weighted ? ", weighted" : ", unweighted");
    printf("Vertices |V|      : %d\n", V);
    printf("Edges    |E|      : %d\n", edges);

    if (directed) {
        int si = 0, so = 0;
        for (int v = 0; v < V; v++) { si += inDegree(v); so += outDegree(v); }
        printf("Sum in-degrees    : %d,  sum out-degrees : %d  (both must equal |E| = %d)\n", si, so, edges);
    } else {
        int sum = 0;
        for (int v = 0; v < V; v++) sum += degree(v);
        printf("Sum of degrees    : %d  (= 2*|E| = %d, handshaking lemma %s)\n",
               sum, 2 * edges, sum == 2 * edges ? "holds" : "FAILS");
    }

    printf("Self-loops        : %s (%d)\n", loops ? "yes" : "no", loops);
    printf("Simple graph      : %s\n", loops ? "no (has self-loops)" : "yes (duplicates are rejected on insert)");

    int complete = 1;
    for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) if (i != j && !findEdge(i, j)) complete = 0;
    printf("Complete graph    : %s\n", (complete && V > 0) ? "yes" : "no");

    int regular = 1, d0 = V ? (directed ? outDegree(0) : degree(0)) : 0;
    for (int v = 0; v < V; v++) {
        if (directed) { if (outDegree(v) != d0 || inDegree(v) != d0) regular = 0; }
        else if (degree(v) != d0) regular = 0;
    }
    printf("Regular graph     : %s", (regular && V > 0) ? "yes" : "no");
    if (regular && V > 0) printf(" (every vertex has degree %d)", d0);
    printf("\n");

    printf("Isolated vertices : ");
    int iso = 0;
    for (int v = 0; v < V; v++)
        if ((directed ? inDegree(v) + outDegree(v) : degree(v)) == 0) { printf("%d ", v); iso = 1; }
    if (!iso) printf("none");
    printf("\n");

    int maxEdges = directed ? V * (V - 1) : V * (V - 1) / 2;
    int realEdges = directed ? nonLoopEntries : nonLoopEntries / 2;
    if (maxEdges > 0)
        printf("Density           : %.2f  (%d of %d possible edges)\n", (double)realEdges / maxEdges, realEdges, maxEdges);

    int nodes = loops + nonLoopEntries;
    printf("List memory       : %d head pointers + %d list nodes  (grows with |E|, not V^2)\n", V, nodes);
}

/* Show the equivalent matrix so the two representations can be compared     */
void showAsMatrix(void)
{
    printf("Equivalent adjacency matrix:\n     ");
    for (int j = 0; j < V; j++) printf("%3d", j);
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%3d |", i);
        for (int j = 0; j < V; j++) { ANode *e = findEdge(i, j); printf("%3d", e ? e->weight : 0); }
        printf("\n");
    }
}

void freeGraph(void)
{
    for (int u = 0; u < V; u++) {
        ANode *p = adjList[u];
        while (p) { ANode *t = p; p = p->next; free(t); }
        adjList[u] = NULL;
    }
}

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    int choice, u, v, w;

    printf("Number of vertices (1-%d): ", MAXV);
    if (checked_scanf("%d", &V) != 1 || V < 1 || V > MAXV) { printf("Invalid.\n"); return 1; }
    printf("Directed graph?  (1 = yes, 0 = no): ");  checked_scanf("%d", &directed);
    printf("Weighted graph?  (1 = yes, 0 = no): ");  checked_scanf("%d", &weighted);
    for (int i = 0; i < V; i++) adjList[i] = NULL;

    while (1) {
        printf("\n===== GRAPH - ADJACENCY LIST MENU =====\n");
        printf(" 1. Add edge           2. Remove edge        3. Add vertex\n");
        printf(" 4. Display list       5. Does edge u-v exist?  6. Neighbours of a vertex\n");
        printf(" 7. Degrees            8. Graph properties summary\n");
        printf(" 9. Show equivalent matrix\n 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter u v%s: ", weighted ? " weight" : ""); w = 1;
            if (weighted) checked_scanf("%d %d %d", &u, &v, &w); else checked_scanf("%d %d", &u, &v);
            addEdge(u, v, w);
            break;
        case 2: printf("Enter u v: "); checked_scanf("%d %d", &u, &v); removeEdge(u, v); break;
        case 3: addVertex(); break;
        case 4: displayList(); break;
        case 5:
            printf("Enter u v: "); checked_scanf("%d %d", &u, &v);
            if (!validVertex(u) || !validVertex(v)) printf("Invalid vertex.\n");
            else printf(findEdge(u, v) ? "Yes, edge exists (found by walking u's list).\n" : "No edge between them.\n");
            break;
        case 6: printf("Enter vertex: "); checked_scanf("%d", &u); showNeighbours(u); break;
        case 7: showDegrees(); break;
        case 8: showProperties(); break;
        case 9: showAsMatrix(); break;
        case 0: freeGraph(); printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    freeGraph();
    return 0;
}
