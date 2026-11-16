/*==========================================================================
 * FILE  : 16_graph_bfs.c
 * TOPIC : GRAPH TRAVERSAL - BREADTH FIRST SEARCH (BFS)
 *==========================================================================
 *
 * THEORY
 * ------
 * BFS explores a graph LEVEL BY LEVEL: first the start vertex, then all its
 * neighbours (distance 1), then all their unvisited neighbours (distance 2),
 * and so on - like ripples spreading in water.
 * It uses a QUEUE (FIFO) and a VISITED array.  A vertex is marked visited when
 * it is ENQUEUED (not when dequeued) so it can never enter the queue twice.
 *
 *   Graph:      0 --- 1          BFS from 0 (neighbours in increasing order):
 *               |     |              level 0: 0
 *               2 --- 3 --- 4          level 1: 1 2
 *               (edges: 0-1, 0-2, 1-3, 2-3, 3-4)    level 2: 3
 *                                       level 3: 4        order: 0 1 2 3 4
 *
 * In an UNWEIGHTED graph BFS finds the SHORTEST PATH (fewest edges) from the
 * start vertex to every reachable vertex - DFS cannot guarantee this.
 *
 * The graph is stored in BOTH representations (adjacency matrix and
 * adjacency list built from the same edges). Neighbours are kept in ascending
 * order so that both versions give the same visiting order.
 *
 * ALGORITHM
 * ---------
 *    BFS(start):
 *        mark start visited, dist[start] = 0, parent[start] = -1
 *        enqueue(start)
 *        while queue not empty:
 *            u = dequeue()
 *            print u
 *            for each neighbour v of u:
 *                if v not visited:
 *                    mark v visited, dist[v] = dist[u] + 1, parent[v] = u
 *                    enqueue(v)
 *
 *  Applications coded below
 *     - level order / distance of every vertex from the source
 *     - shortest path between two vertices (follow the parent[] links backwards)
 *     - connected components / BFS forest
 *     - BIPARTITE test: colour the source 0, neighbours 1, their neighbours 0 ...
 *       if two adjacent vertices ever get the same colour the graph is NOT
 *       bipartite (it has an odd cycle).  (undirected graphs)
 *
 * COMPLEXITY
 * ----------
 *   Adjacency LIST   : O(V + E)      Adjacency MATRIX : O(V^2)
 *   Space            : O(V) for the queue, visited, distance, parent arrays
 *
 * COMPILE & RUN
 * -------------
 *  gcc 16_graph_bfs.c -o bfs && ./bfs      (vertices are numbered 0 .. V-1)
 *
 * SAMPLE OUTPUT
 * -------------
 *   Number of vertices (1-20): 6
 *   Directed graph? (1 = yes, 0 = no): 0
 *
 *   ===== GRAPH - BFS MENU =====
 *    1. Add edge                    2. Display graph
 *    3. BFS (adjacency list)        4. BFS (adjacency matrix)
 *    5. Levels and distances        6. Shortest path between two vertices
 *    7. Components / BFS forest     8. Bipartite test (undirected)
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
 *   Enter choice: 3
 *   Enter start vertex: 0
 *   BFS (list)   : 0 1 2 3 4
 *   Not reachable from the start: 5
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Enter start vertex: 0
 *   BFS (matrix) : 0 1 2 3 4
 *   Not reachable from the start: 5
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Enter source vertex: 0
 *   BFS order: 0 1 2 3 4
 *     Level 0: 0
 *     Level 1: 1 2
 *     Level 2: 3
 *     Level 3: 4
 *   Vertex  distance  parent
 *      0        0       -1
 *      1        1        0
 *      2        1        0
 *      3        2        1
 *      4        3        3
 *      5      inf        -
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Enter source and target: 0
 *   4
 *   Shortest path (3 edges): 0 -> 1 -> 3 -> 4
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *     Component 1: 0 1 2 3 4
 *     Component 2: 5
 *   Number of connected components = 2  -> graph is NOT connected
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *     Set A (colour 0): 0 3 5
 *     Set B (colour 1): 1 2 4
 *   The graph IS bipartite.
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

typedef struct ANode { int dest; struct ANode *next; } ANode;

int adj[MAXV][MAXV];            /* adjacency matrix                          */
ANode *adjList[MAXV];           /* adjacency list (sorted)                   */
int V = 0, directed = 0;

int visited[MAXV], dist[MAXV], parent[MAXV];
int quiet = 0;                  /* 1 = do not print the visiting order       */

/* ---------------------------- QUEUE (array) ----------------------------- */
/* Every vertex is enqueued at most once, so MAXV slots are enough.          */
int queue[MAXV], front = 0, rear = 0;
void enqueue(int x)  { queue[rear++] = x; }
int  dequeue(void)   { return queue[front++]; }
int  queueEmpty(void){ return front == rear; }
void resetQueue(void){ front = rear = 0; }

/* Reset all per-search arrays before a new BFS */
void resetSearch(void)
{
    memset(visited, 0, sizeof visited);
    for (int i = 0; i < V; i++) { dist[i] = -1; parent[i] = -1; }
    resetQueue();
}

/* ---------------------------- BUILD THE GRAPH --------------------------- */
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

/* ===================== 1. BFS - ADJACENCY LIST ============================ */
/* Explores only the part of the graph reachable from 'start'.               */
void bfsList(int start)
{
    visited[start] = 1; dist[start] = 0; parent[start] = -1;
    enqueue(start);
    while (!queueEmpty()) {
        int u = dequeue();
        if (!quiet) printf("%d ", u);
        for (ANode *p = adjList[u]; p; p = p->next) {
            int v = p->dest;
            if (!visited[v]) {
                visited[v] = 1;             /* mark when ENQUEUED            */
                dist[v] = dist[u] + 1;
                parent[v] = u;
                enqueue(v);
            }
        }
    }
}

/* ===================== 2. BFS - ADJACENCY MATRIX ========================== */
void bfsMatrix(int start)
{
    visited[start] = 1; dist[start] = 0; parent[start] = -1;
    enqueue(start);
    while (!queueEmpty()) {
        int u = dequeue();
        printf("%d ", u);
        for (int v = 0; v < V; v++) {       /* scan the whole row: O(V)      */
            if (adj[u][v] && !visited[v]) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                enqueue(v);
            }
        }
    }
}

void reportUnreached(void)
{
    int any = 0;
    for (int i = 0; i < V; i++) if (!visited[i]) { if (!any) printf("Not reachable from the start: "); printf("%d ", i); any = 1; }
    if (any) printf("\n");
}

/* ===================== 3. LEVELS AND DISTANCES ============================ */
void showLevels(int start)
{
    resetSearch();
    printf("BFS order: "); bfsList(start); printf("\n");
    int maxd = 0;
    for (int i = 0; i < V; i++) if (dist[i] > maxd) maxd = dist[i];
    for (int d = 0; d <= maxd; d++) {
        printf("  Level %d: ", d);
        for (int i = 0; i < V; i++) if (dist[i] == d) printf("%d ", i);
        printf("\n");
    }
    printf("Vertex  distance  parent\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] >= 0) printf("%4d %8d %8d\n", i, dist[i], parent[i]);
        else printf("%4d %8s %8s\n", i, "inf", "-");
    }
}

/* ===================== 4. SHORTEST PATH (unweighted) ====================== */
void printPath(int v)                       /* follow parent links back      */
{
    if (parent[v] != -1) { printPath(parent[v]); printf(" -> "); }
    printf("%d", v);
}

void shortestPath(int s, int t)
{
    resetSearch();
    quiet = 1; bfsList(s); quiet = 0;       /* run BFS without printing      */
    if (dist[t] < 0) { printf("No path from %d to %d.\n", s, t); return; }
    printf("Shortest path (%d edge%s): ", dist[t], dist[t] == 1 ? "" : "s");
    printPath(t);
    printf("\n");
}

/* ===================== 5. CONNECTED COMPONENTS ============================ */
void components(void)
{
    resetSearch();
    int count = 0;
    for (int s = 0; s < V; s++) {
        if (visited[s]) continue;
        printf("  %s %d: ", directed ? "BFS tree" : "Component", ++count);
        bfsList(s);
        printf("\n");
    }
    if (!directed) printf("Number of connected components = %d  -> graph is %s\n",
                          count, count == 1 ? "CONNECTED" : "NOT connected");
    else printf("(%d BFS trees)\n", count);
}

/* ===================== 6. BIPARTITE TEST ================================== */
int isBipartite(void)
{
    int colour[MAXV];
    for (int i = 0; i < V; i++) colour[i] = -1;         /* -1 = uncoloured    */
    for (int s = 0; s < V; s++) {
        if (colour[s] != -1) continue;
        resetQueue();
        colour[s] = 0; enqueue(s);
        while (!queueEmpty()) {
            int u = dequeue();
            for (ANode *p = adjList[u]; p; p = p->next) {
                int v = p->dest;
                if (colour[v] == -1) { colour[v] = 1 - colour[u]; enqueue(v); }
                else if (colour[v] == colour[u]) return 0;   /* odd cycle     */
            }
        }
    }
    printf("  Set A (colour 0): "); for (int i = 0; i < V; i++) if (colour[i] == 0) printf("%d ", i);
    printf("\n  Set B (colour 1): "); for (int i = 0; i < V; i++) if (colour[i] == 1) printf("%d ", i);
    printf("\n");
    return 1;
}

int readVertex(const char *msg)
{
    int s;
    printf("%s", msg);
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
        printf("\n===== GRAPH - BFS MENU =====\n");
        printf(" 1. Add edge                    2. Display graph\n");
        printf(" 3. BFS (adjacency list)        4. BFS (adjacency matrix)\n");
        printf(" 5. Levels and distances        6. Shortest path between two vertices\n");
        printf(" 7. Components / BFS forest     8. Bipartite test (undirected)\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1: printf("Enter u v: "); checked_scanf("%d %d", &u, &v); addEdge(u, v); break;
        case 2: display(); break;
        case 3:
            if ((s = readVertex("Enter start vertex: ")) < 0) break;
            resetSearch(); printf("BFS (list)   : "); bfsList(s); printf("\n"); reportUnreached();
            break;
        case 4:
            if ((s = readVertex("Enter start vertex: ")) < 0) break;
            resetSearch(); printf("BFS (matrix) : "); bfsMatrix(s); printf("\n"); reportUnreached();
            break;
        case 5:
            if ((s = readVertex("Enter source vertex: ")) < 0) break;
            showLevels(s);
            break;
        case 6:
            printf("Enter source and target: "); checked_scanf("%d %d", &u, &v);
            if (u < 0 || u >= V || v < 0 || v >= V) { printf("Invalid vertex.\n"); break; }
            shortestPath(u, v);
            break;
        case 7: components(); break;
        case 8:
            if (directed) { printf("Bipartite test here is for undirected graphs.\n"); break; }
            printf(isBipartite() ? "The graph IS bipartite.\n" : "The graph is NOT bipartite (odd cycle found).\n");
            break;
        case 0: freeGraph(); printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    freeGraph();
    return 0;
}
