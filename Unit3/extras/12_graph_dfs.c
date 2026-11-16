/*==========================================================================
 * FILE  : 12_graph_dfs.c
 * TOPIC : GRAPH TRAVERSAL - DEPTH FIRST SEARCH (DFS)
 *==========================================================================
 *
 * THEORY
 * ------
 * DFS explores a graph by going as DEEP as possible along one path before
 * BACKTRACKING.  From the current vertex it moves to an unvisited neighbour,
 * and only when a vertex has no unvisited neighbours left does it return to
 * the previous vertex and try its next neighbour.
 * It uses a STACK (the recursion call stack, or an explicit stack) and a
 * VISITED array so that no vertex is processed twice (graphs may have cycles).
 *
 *   Graph:      0 --- 1          DFS from 0 (neighbours in increasing order):
 *               |     |              0 -> 1 -> 3 -> 2 -> 4
 *               2 --- 3 --- 4
 *               (edges: 0-1, 0-2, 1-3, 2-3, 3-4)
 *
 * This program keeps the graph in BOTH representations (adjacency matrix and
 * adjacency list, built together from the same edges).  Neighbour lists are
 * kept in ASCENDING order so that all versions give the same visiting order.
 *
 * ALGORITHMS
 * ----------
 *  DFS (recursive):                      DFS (iterative, explicit stack):
 *     dfs(u):                               push(start)
 *        mark u visited, print u            while stack not empty:
 *        for each neighbour v of u:            u = pop()
 *            if v not visited: dfs(v)          if u visited: continue
 *                                              mark u visited, print u
 *                                              push all unvisited neighbours
 *                                              (in REVERSE order so the smallest
 *                                               neighbour is popped first)
 *
 *  Extra: DISCOVERY / FINISH times and EDGE CLASSIFICATION
 *     tree edge    : edge that leads to a newly discovered vertex
 *     back edge    : edge to an ancestor that is still being explored (=> CYCLE)
 *     forward edge : (directed) edge to an already finished DESCENDANT
 *     cross edge   : (directed) edge to an already finished non-descendant
 *
 *  Applications coded below
 *     - connected components / DFS forest (restart DFS from every unvisited vertex)
 *     - path existence between two vertices
 *     - cycle detection:  undirected : a visited neighbour that is not the
 *                                      parent means a cycle
 *                         directed   : an edge to a vertex still ON the
 *                                      current recursion path (colour GREY)
 *
 * COMPLEXITY
 * ----------
 *   Adjacency LIST   : O(V + E)  time  (each vertex and edge handled once)
 *   Adjacency MATRIX : O(V^2)    time  (a whole row is scanned per vertex)
 *   Space            : O(V) extra (visited array + stack / recursion depth)
 *
 * COMPILE & RUN
 * -------------
 *  gcc 12_graph_dfs.c -o dfs && ./dfs      (vertices are numbered 0 .. V-1)
 *
 * SAMPLE OUTPUT
 * -------------
 *   === Run 1 ===
 *   Number of vertices (1-20): 6
 *   Directed graph? (1 = yes, 0 = no): 0
 *
 *   ===== GRAPH - DFS MENU =====
 *    1. Add edge                      2. Display graph
 *    3. DFS recursive (adj. list)     4. DFS recursive (adj. matrix)
 *    5. DFS iterative (stack)         6. DFS with discovery/finish times + edge types
 *    7. Components / DFS forest       8. Path between two vertices
 *    9. Detect a cycle
 *    0. Exit
 *   Enter choice: 1
 *   Enter u v: 0
 *   1
 *   Edge 0-1 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 0
 *   2
 *   Edge 0-2 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 1
 *   3
 *   Edge 1-3 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 2
 *   3
 *   Edge 2-3 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 3
 *   4
 *   Edge 3-4 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Adjacency list:
 *      0 -> 1 2
 *      1 -> 0 3
 *      2 -> 0 3
 *      3 -> 1 2 4
 *      4 -> 3
 *      5 ->
 *   Adjacency matrix:
 *         0 1 2 3 4 5
 *     0 | 0 1 1 0 0 0
 *     1 | 1 0 0 1 0 0
 *     2 | 1 0 0 1 0 0
 *     3 | 0 1 1 0 1 0
 *     4 | 0 0 0 1 0 0
 *     5 | 0 0 0 0 0 0
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 3
 *   Enter start vertex: 0
 *   DFS (list)   : 0 1 3 2 4
 *   Not reachable from the start: 5
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Enter start vertex: 0
 *   DFS (matrix) : 0 1 3 2 4
 *   Not reachable from the start: 5
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Enter start vertex: 0
 *   DFS (stack)  : 0 1 3 2 4
 *   Not reachable from the start: 5
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Enter start vertex: 0
 *   Exploring from 0:
 *      edge 0-1 : TREE edge
 *      edge 1-3 : TREE edge
 *      edge 3-2 : TREE edge
 *      edge 2-0 : BACK edge (forms a cycle)
 *      edge 3-4 : TREE edge
 *   Vertex  discovery  finish  parent
 *      0        1        10     -1
 *      1        2         9      0
 *      2        4         5      3
 *      3        3         8      1
 *      4        6         7      3
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *     Component 1: 0 1 3 2 4
 *     Component 2: 5
 *   Number of connected components = 2  -> graph is NOT connected
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Enter source and target: 0
 *   4
 *   A path exists (DFS path, not necessarily the shortest): 0 -> 1 -> 3 -> 4
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   Enter source and target: 0
 *   5
 *   No path from 0 to 5.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 9
 *   The graph CONTAINS a cycle.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 0
 *   Bye!
 *
 *   === Run 2 ===
 *   Number of vertices (1-20): 4
 *   Directed graph? (1 = yes, 0 = no): 1
 *
 *   ===== GRAPH - DFS MENU =====
 *    1. Add edge                      2. Display graph
 *    3. DFS recursive (adj. list)     4. DFS recursive (adj. matrix)
 *    5. DFS iterative (stack)         6. DFS with discovery/finish times + edge types
 *    7. Components / DFS forest       8. Path between two vertices
 *    9. Detect a cycle
 *    0. Exit
 *   Enter choice: 1
 *   Enter u v: 0
 *   1
 *   Edge 0->1 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 1
 *   2
 *   Edge 1->2 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 2
 *   0
 *   Edge 2->0 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 0
 *   3
 *   Edge 0->3 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 3
 *   2
 *   Edge 3->2 added.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Enter start vertex: 0
 *   Exploring from 0:
 *      edge 0->1 : TREE edge
 *      edge 1->2 : TREE edge
 *      edge 2->0 : BACK edge (forms a cycle)
 *      edge 0->3 : TREE edge
 *      edge 3->2 : CROSS edge
 *   Vertex  discovery  finish  parent
 *      0        1         8     -1
 *      1        2         5      0
 *      2        3         4      1
 *      3        6         7      0
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 9
 *   The graph CONTAINS a cycle.
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
#include <string.h>

#define MAXV 20

/* ---------------- graph storage: matrix AND list together --------------- */
typedef struct ANode { int dest; struct ANode *next; } ANode;

int adj[MAXV][MAXV];            /* adjacency matrix (1 = edge)               */
ANode *adjList[MAXV];           /* adjacency list, kept sorted by vertex id  */
int V = 0, directed = 0;

int visited[MAXV];              /* 0 = not visited, 1 = visited              */

/* Insert v into u's list in ASCENDING order (so matrix & list give the same
   traversal order).                                                          */
void listInsertSorted(int u, int v)
{
    ANode *n = (ANode *)malloc(sizeof(ANode));
    if (!n) { printf("Out of memory\n"); exit(1); }
    n->dest = v;
    if (!adjList[u] || v < adjList[u]->dest) { n->next = adjList[u]; adjList[u] = n; return; }
    ANode *p = adjList[u];
    while (p->next && p->next->dest < v) p = p->next;
    n->next = p->next;
    p->next = n;
}

void addEdge(int u, int v)
{
    if (u < 0 || u >= V || v < 0 || v >= V) { printf("Invalid vertex (use 0..%d).\n", V - 1); return; }
    if (adj[u][v]) { printf("Edge already exists.\n"); return; }
    adj[u][v] = 1;           listInsertSorted(u, v);
    if (!directed && u != v) { adj[v][u] = 1; listInsertSorted(v, u); }
    printf("Edge %d%s%d added.\n", u, directed ? "->" : "-", v);
}

void display(void)
{
    printf("Adjacency list:\n");
    for (int u = 0; u < V; u++) {
        printf("  %2d -> ", u);
        for (ANode *p = adjList[u]; p; p = p->next) printf("%d ", p->dest);
        printf("\n");
    }
    printf("Adjacency matrix:\n     ");
    for (int j = 0; j < V; j++) printf("%2d", j);
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%3d |", i);
        for (int j = 0; j < V; j++) printf("%2d", adj[i][j]);
        printf("\n");
    }
}

/* Print the vertices a traversal could not reach (graph not connected)      */
void reportUnreached(void)
{
    int any = 0;
    for (int i = 0; i < V; i++) if (!visited[i]) { if (!any) printf("Not reachable from the start: "); printf("%d ", i); any = 1; }
    if (any) printf("\n");
}

/* ===================== 1. RECURSIVE DFS - ADJACENCY LIST ================== */
void dfsList(int u)
{
    visited[u] = 1;                         /* mark BEFORE exploring         */
    printf("%d ", u);                       /* "process" the vertex          */
    for (ANode *p = adjList[u]; p; p = p->next)
        if (!visited[p->dest]) dfsList(p->dest);        /* go deeper         */
}

/* ===================== 2. RECURSIVE DFS - ADJACENCY MATRIX ================ */
void dfsMatrix(int u)
{
    visited[u] = 1;
    printf("%d ", u);
    for (int v = 0; v < V; v++)             /* scan the whole row: O(V)      */
        if (adj[u][v] && !visited[v]) dfsMatrix(v);
}

/* ===================== 3. ITERATIVE DFS - EXPLICIT STACK ================== */
void dfsIterative(int start)
{
    int stack[MAXV * MAXV + 1], top = -1;   /* each edge may push once       */
    stack[++top] = start;
    while (top >= 0) {
        int u = stack[top--];               /* pop                           */
        if (visited[u]) continue;           /* may have been pushed twice    */
        visited[u] = 1;
        printf("%d ", u);

        int nb[MAXV], cnt = 0;              /* collect neighbours in order   */
        for (ANode *p = adjList[u]; p; p = p->next) nb[cnt++] = p->dest;
        for (int i = cnt - 1; i >= 0; i--)  /* push in REVERSE so smallest   */
            if (!visited[nb[i]]) stack[++top] = nb[i];  /* neighbour pops first*/
    }
}

/* ============ 4. DFS WITH TIMESTAMPS AND EDGE CLASSIFICATION ============== */
int color[MAXV];            /* 0 = WHITE (undiscovered), 1 = GREY (in progress),
                               2 = BLACK (finished)                           */
int disc[MAXV], fin[MAXV], parent[MAXV], timer;

void dfsTimes(int u)
{
    color[u] = 1;
    disc[u] = ++timer;                              /* discovery time         */
    for (ANode *p = adjList[u]; p; p = p->next) {
        int v = p->dest;
        if (color[v] == 0) {                        /* new vertex             */
            printf("   edge %d%s%d : TREE edge\n", u, directed ? "->" : "-", v);
            parent[v] = u;
            dfsTimes(v);
        } else if (color[v] == 1) {                 /* ancestor still open    */
            if (directed || v != parent[u])
                printf("   edge %d%s%d : BACK edge (forms a cycle)\n", u, directed ? "->" : "-", v);
        } else if (directed) {                      /* finished vertex        */
            printf("   edge %d->%d : %s edge\n", u, v, disc[u] < disc[v] ? "FORWARD" : "CROSS");
        }
    }
    color[u] = 2;
    fin[u] = ++timer;                               /* finish time            */
}

/* ===================== 5. CONNECTED COMPONENTS / DFS FOREST =============== */
void components(void)
{
    memset(visited, 0, sizeof visited);
    int count = 0;
    for (int s = 0; s < V; s++) {
        if (visited[s]) continue;
        printf("  %s %d: ", directed ? "DFS tree" : "Component", ++count);
        dfsList(s);
        printf("\n");
    }
    if (directed) printf("(%d DFS trees; strongly connected components need Kosaraju/Tarjan)\n", count);
    else printf("Number of connected components = %d  -> graph is %s\n",
                count, count == 1 ? "CONNECTED" : "NOT connected");
}

/* ===================== 6. PATH EXISTENCE (backtracking DFS) =============== */
int dfsPath(int u, int target, int path[], int *len)
{
    visited[u] = 1;
    path[(*len)++] = u;                     /* extend the current path       */
    if (u == target) return 1;
    for (ANode *p = adjList[u]; p; p = p->next)
        if (!visited[p->dest] && dfsPath(p->dest, target, path, len)) return 1;
    (*len)--;                               /* dead end: BACKTRACK           */
    return 0;
}

/* ===================== 7. CYCLE DETECTION ================================= */
int cycleUndirected(int u, int par)
{
    visited[u] = 1;
    for (ANode *p = adjList[u]; p; p = p->next) {
        int v = p->dest;
        if (!visited[v]) { if (cycleUndirected(v, u)) return 1; }
        else if (v != par) return 1;        /* visited non-parent => cycle   */
    }
    return 0;
}
int cycleDirected(int u)
{
    color[u] = 1;                           /* on the current path           */
    for (ANode *p = adjList[u]; p; p = p->next) {
        int v = p->dest;
        if (color[v] == 1) return 1;        /* back edge to GREY vertex      */
        if (color[v] == 0 && cycleDirected(v)) return 1;
    }
    color[u] = 2;
    return 0;
}
int hasCycle(void)
{
    memset(visited, 0, sizeof visited);
    memset(color, 0, sizeof color);
    for (int s = 0; s < V; s++) {
        if (directed) { if (color[s] == 0 && cycleDirected(s)) return 1; }
        else          { if (!visited[s] && cycleUndirected(s, -1)) return 1; }
    }
    return 0;
}

int readStart(void)
{
    int s;
    printf("Enter start vertex: ");
    if (checked_scanf("%d", &s) != 1 || s < 0 || s >= V) { printf("Invalid vertex.\n"); return -1; }
    return s;
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
    int choice, u, v, s;

    printf("Number of vertices (1-%d): ", MAXV);
    if (checked_scanf("%d", &V) != 1 || V < 1 || V > MAXV) { printf("Invalid.\n"); return 1; }
    printf("Directed graph? (1 = yes, 0 = no): "); checked_scanf("%d", &directed);

    while (1) {
        printf("\n===== GRAPH - DFS MENU =====\n");
        printf(" 1. Add edge                      2. Display graph\n");
        printf(" 3. DFS recursive (adj. list)     4. DFS recursive (adj. matrix)\n");
        printf(" 5. DFS iterative (stack)         6. DFS with discovery/finish times + edge types\n");
        printf(" 7. Components / DFS forest       8. Path between two vertices\n");
        printf(" 9. Detect a cycle\n 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1: printf("Enter u v: "); checked_scanf("%d %d", &u, &v); addEdge(u, v); break;
        case 2: display(); break;
        case 3:
            if ((s = readStart()) < 0) break;
            memset(visited, 0, sizeof visited);
            printf("DFS (list)   : "); dfsList(s); printf("\n"); reportUnreached();
            break;
        case 4:
            if ((s = readStart()) < 0) break;
            memset(visited, 0, sizeof visited);
            printf("DFS (matrix) : "); dfsMatrix(s); printf("\n"); reportUnreached();
            break;
        case 5:
            if ((s = readStart()) < 0) break;
            memset(visited, 0, sizeof visited);
            printf("DFS (stack)  : "); dfsIterative(s); printf("\n"); reportUnreached();
            break;
        case 6:
            if ((s = readStart()) < 0) break;
            memset(color, 0, sizeof color); timer = 0;
            for (int i = 0; i < V; i++) parent[i] = -1;
            printf("Exploring from %d:\n", s);
            dfsTimes(s);
            printf("Vertex  discovery  finish  parent\n");
            for (int i = 0; i < V; i++)
                if (color[i]) printf("%4d %8d %9d %6d\n", i, disc[i], fin[i], parent[i]);
            break;
        case 7: components(); break;
        case 8: {
            int path[MAXV], len = 0;
            printf("Enter source and target: "); checked_scanf("%d %d", &u, &v);
            if (u < 0 || u >= V || v < 0 || v >= V) { printf("Invalid vertex.\n"); break; }
            memset(visited, 0, sizeof visited);
            if (dfsPath(u, v, path, &len)) {
                printf("A path exists (DFS path, not necessarily the shortest): ");
                for (int i = 0; i < len; i++) printf("%d%s", path[i], i < len - 1 ? " -> " : "\n");
            } else printf("No path from %d to %d.\n", u, v);
            break;
        }
        case 9: printf(hasCycle() ? "The graph CONTAINS a cycle.\n" : "The graph has NO cycle.\n"); break;
        case 0: freeGraph(); printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    freeGraph();
    return 0;
}
