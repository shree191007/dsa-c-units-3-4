/*==========================================================================
 * FILE  : 13_graph_adjacency_matrix.c
 * TOPIC : GRAPH - properties and representation using an ADJACENCY MATRIX
 *==========================================================================
 *
 * THEORY (graph basics and properties)
 * ------------------------------------
 * A GRAPH G = (V, E) has a set V of VERTICES (nodes) and a set E of EDGES
 * (links between pairs of vertices).
 *
 *   UNDIRECTED graph : edge {u,v} works both ways.
 *   DIRECTED graph (digraph) : edge (u -> v) works one way only.
 *   WEIGHTED graph   : every edge has a number (cost / distance).
 *
 * Important terms
 *   Adjacent vertices : joined by an edge.         Self-loop : edge (v,v).
 *   DEGREE of v (undirected) : number of edges touching v (a self-loop adds 2).
 *   IN-degree / OUT-degree (directed) : edges coming into / going out of v.
 *   HANDSHAKING LEMMA : sum of all degrees = 2 * |E|  (undirected).
 *                       sum of in-degrees = sum of out-degrees = |E| (directed).
 *   Simple graph : no self-loops and no parallel (duplicate) edges.
 *   Complete graph K_n : every pair of distinct vertices is joined.
 *       Max edges: n(n-1)/2 (undirected), n(n-1) (directed).
 *   Regular graph : every vertex has the same degree.
 *   Isolated vertex : degree 0.
 *   Density = |E| / (max possible edges). Dense ~ 1, sparse ~ 0.
 *   Path, cycle, connected graph - see the DFS / BFS files.
 *
 * ADJACENCY MATRIX REPRESENTATION
 * -------------------------------
 * A V x V two-dimensional array  adj[i][j]:
 *      adj[i][j] = 0        -> no edge from i to j
 *      adj[i][j] = w (> 0)  -> edge from i to j (w = 1 for unweighted graphs,
 *                              w = weight for weighted graphs)
 * For an UNDIRECTED graph the matrix is SYMMETRIC (adj[i][j] == adj[j][i]),
 * so every edge is stored twice.
 *
 *   Graph:  0 - 1,  0 - 2,  1 - 2,  2 - 3          Matrix:
 *                                                      0 1 2 3
 *        0 --- 1                                    0 [0 1 1 0]
 *         \   /                                     1 [1 0 1 0]
 *           2 --- 3                                 2 [1 1 0 1]
 *                                                   3 [0 0 1 0]
 *
 *   Row sum of vertex i       = out-degree (degree for undirected)
 *   Column sum of vertex i    = in-degree
 *
 * ALGORITHMS
 * ----------
 *  add edge (u,v,w)  : adj[u][v] = w  (and adj[v][u] = w if undirected)
 *  remove edge       : set the cell(s) to 0
 *  edge exists?      : look at adj[u][v]                      -> O(1)
 *  neighbours of u   : scan row u                             -> O(V)
 *  degree of u       : sum of row u (and column u)            -> O(V)
 *  count edges       : scan the whole matrix                  -> O(V^2)
 *
 * COMPLEXITY / WHEN TO USE
 * ------------------------
 *  Space : O(V^2) always, even if there are very few edges.
 *  Best for DENSE graphs and when you need fast "is there an edge u-v?" tests.
 *  Wasteful for SPARSE graphs (use the adjacency list - file 14).
 *
 * COMPILE & RUN
 * -------------
 *  gcc 13_graph_adjacency_matrix.c -o graph_matrix && ./graph_matrix
 *  (vertices are numbered 0, 1, 2, ... V-1)
 *
 * SAMPLE OUTPUT
 * -------------
 *   Number of vertices (1-20): 5
 *   Directed graph?  (1 = yes, 0 = no): 0
 *   Weighted graph?  (1 = yes, 0 = no): 0
 *
 *   ===== GRAPH - ADJACENCY MATRIX MENU =====
 *    1. Add edge           2. Remove edge        3. Add vertex
 *    4. Display matrix     5. Does edge u-v exist?  6. Neighbours of a vertex
 *    7. Degrees            8. Graph properties summary
 *    0. Exit
 *   Enter choice: 1
 *   Enter u v: 0
 *   1
 *   Edge 0-1 added (weight 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 0
 *   2
 *   Edge 0-2 added (weight 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 1
 *   2
 *   Edge 1-2 added (weight 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 2
 *   3
 *   Edge 2-3 added (weight 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 4
 *   Adjacency matrix (undirected):
 *          0  1  2  3  4
 *     0 |  0  1  1  0  0
 *     1 |  1  0  1  0  0
 *     2 |  1  1  0  1  0
 *     3 |  0  0  1  0  0
 *     4 |  0  0  0  0  0
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 7
 *   Vertex  Degree
 *      0        2
 *      1        2
 *      2        3
 *      3        1
 *      4        0
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   ---- Graph properties ----
 *   Type              : undirected, unweighted
 *   Vertices |V|      : 5
 *   Edges    |E|      : 4
 *   Sum of degrees    : 8  (= 2*|E| = 8, handshaking lemma holds)
 *   Self-loops        : no (0)
 *   Simple graph      : yes (matrix cannot hold parallel edges)
 *   Matrix symmetric  : yes
 *   Complete graph    : no
 *   Regular graph     : no
 *   Isolated vertices : 4
 *   Density           : 0.40  (4 of 10 possible edges)
 *   Matrix memory     : 5 x 5 = 25 cells (independent of |E|)
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 5
 *   Enter u v: 0
 *   1
 *   Yes, edge 0-1 exists (checked in O(1)).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 6
 *   Enter vertex: 2
 *   Neighbours of 2: 0 1 3
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 2
 *   Enter u v: 1
 *   2
 *   Edge removed.
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 1
 *   Enter u v: 3
 *   3
 *   Edge 3-3 added (weight 1).
 *
 *   ----- (menu shown again - same options) -----
 *   Enter choice: 8
 *   ---- Graph properties ----
 *   Type              : undirected, unweighted
 *   Vertices |V|      : 5
 *   Edges    |E|      : 4
 *   Sum of degrees    : 8  (= 2*|E| = 8, handshaking lemma holds)
 *   Self-loops        : yes (1)
 *   Simple graph      : no (has self-loops)
 *   Matrix symmetric  : yes
 *   Complete graph    : no
 *   Regular graph     : no
 *   Isolated vertices : 4
 *   Density           : 0.30  (3 of 10 possible edges)
 *   Matrix memory     : 5 x 5 = 25 cells (independent of |E|)
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

#define MAXV 20

int adj[MAXV][MAXV];        /* the adjacency matrix                          */
int V = 0;                  /* current number of vertices                    */
int directed = 0;           /* 1 = directed graph, 0 = undirected            */
int weighted = 0;           /* 1 = weighted,       0 = unweighted            */

int validVertex(int v) { return v >= 0 && v < V; }

/* ------------------------------ ADD EDGE -------------------------------- */
void addEdge(int u, int v, int w)
{
    if (!validVertex(u) || !validVertex(v)) { printf("Invalid vertex (use 0..%d).\n", V - 1); return; }
    if (w <= 0) { printf("Weight must be positive.\n"); return; }
    if (adj[u][v]) printf("Edge %d%s%d already existed; weight updated.\n", u, directed ? "->" : "-", v);
    adj[u][v] = w;
    if (!directed) adj[v][u] = w;                       /* keep it symmetric  */
    printf("Edge %d%s%d added (weight %d).\n", u, directed ? "->" : "-", v, w);
}

/* ---------------------------- REMOVE EDGE ------------------------------- */
void removeEdge(int u, int v)
{
    if (!validVertex(u) || !validVertex(v)) { printf("Invalid vertex.\n"); return; }
    if (!adj[u][v]) { printf("No such edge.\n"); return; }
    adj[u][v] = 0;
    if (!directed) adj[v][u] = 0;
    printf("Edge removed.\n");
}

/* ----------------------------- ADD VERTEX ------------------------------- */
/* The new row and column are already 0 (global array), so just V++.         */
void addVertex(void)
{
    if (V == MAXV) { printf("Maximum of %d vertices reached.\n", MAXV); return; }
    V++;
    printf("Vertex %d added.\n", V - 1);
}

/* ------------------------------- DISPLAY -------------------------------- */
void displayMatrix(void)
{
    printf("Adjacency matrix (%s%s):\n", directed ? "directed" : "undirected",
           weighted ? ", weighted" : "");
    printf("     ");
    for (int j = 0; j < V; j++) printf("%3d", j);
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%3d |", i);
        for (int j = 0; j < V; j++) printf("%3d", adj[i][j]);
        printf("\n");
    }
}

/* ---------------------------- DEGREE helpers ---------------------------- */
int outDegree(int v) { int d = 0; for (int j = 0; j < V; j++) if (adj[v][j]) d++; return d; }
int inDegree(int v)  { int d = 0; for (int i = 0; i < V; i++) if (adj[i][v]) d++; return d; }
/* undirected degree: self-loop counts twice                                  */
int degree(int v)    { return outDegree(v) + (adj[v][v] ? 1 : 0); }

void showNeighbours(int u)
{
    if (!validVertex(u)) { printf("Invalid vertex.\n"); return; }
    printf("Neighbours of %d%s: ", u, directed ? " (out-going)" : "");
    int any = 0;
    for (int j = 0; j < V; j++)
        if (adj[u][j]) {
            if (weighted) printf("%d(w=%d) ", j, adj[u][j]); else printf("%d ", j);
            any = 1;
        }
    if (!any) printf("none");
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
    int edges = 0, loops = 0, sumDeg = 0, nonLoopEdges = 0;

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            if (adj[i][j]) {
                if (i == j) loops++;
                else nonLoopEdges++;                    /* counted per cell   */
            }
    /* undirected: each non-loop edge occupies two cells                      */
    edges = directed ? nonLoopEdges + loops : nonLoopEdges / 2 + loops;

    printf("---- Graph properties ----\n");
    printf("Type              : %s%s\n", directed ? "directed" : "undirected", weighted ? ", weighted" : ", unweighted");
    printf("Vertices |V|      : %d\n", V);
    printf("Edges    |E|      : %d\n", edges);

    /* Handshaking lemma                                                       */
    if (directed) {
        int si = 0, so = 0;
        for (int v = 0; v < V; v++) { si += inDegree(v); so += outDegree(v); }
        printf("Sum in-degrees    : %d,  sum out-degrees : %d  (both must equal |E| = %d)\n", si, so, edges);
    } else {
        for (int v = 0; v < V; v++) sumDeg += degree(v);
        printf("Sum of degrees    : %d  (= 2*|E| = %d, handshaking lemma %s)\n",
               sumDeg, 2 * edges, sumDeg == 2 * edges ? "holds" : "FAILS");
    }

    printf("Self-loops        : %s (%d)\n", loops ? "yes" : "no", loops);
    printf("Simple graph      : %s\n", loops ? "no (has self-loops)" : "yes (matrix cannot hold parallel edges)");

    /* Symmetric check */
    int sym = 1;
    for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) if (adj[i][j] != adj[j][i]) sym = 0;
    printf("Matrix symmetric  : %s\n", sym ? "yes" : "no");

    /* Complete? every ordered/unordered pair of distinct vertices joined      */
    int complete = 1;
    for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) if (i != j && !adj[i][j]) complete = 0;
    printf("Complete graph    : %s\n", (complete && V > 0) ? "yes" : "no");

    /* Regular? all degrees equal                                              */
    int regular = 1, d0 = V ? (directed ? outDegree(0) : degree(0)) : 0;
    for (int v = 0; v < V; v++) {
        if (directed) { if (outDegree(v) != d0 || inDegree(v) != d0) regular = 0; }
        else if (degree(v) != d0) regular = 0;
    }
    printf("Regular graph     : %s", (regular && V > 0) ? "yes" : "no");
    if (regular && V > 0) printf(" (every vertex has degree %d)", d0);
    printf("\n");

    /* Isolated vertices                                                       */
    printf("Isolated vertices : ");
    int iso = 0;
    for (int v = 0; v < V; v++)
        if ((directed ? inDegree(v) + outDegree(v) : degree(v)) == 0) { printf("%d ", v); iso = 1; }
    if (!iso) printf("none");
    printf("\n");

    /* Density (self-loops ignored)                                            */
    int maxEdges = directed ? V * (V - 1) : V * (V - 1) / 2;
    if (maxEdges > 0)
        printf("Density           : %.2f  (%d of %d possible edges)\n",
               (double)nonLoopEdges / (directed ? 1 : 2) / maxEdges, nonLoopEdges / (directed ? 1 : 2), maxEdges);
    printf("Matrix memory     : %d x %d = %d cells (independent of |E|)\n", V, V, V * V);
}

/* ------------------------------ MAIN MENU ------------------------------- */
int main(void)
{
    int choice, u, v, w;

    printf("Number of vertices (1-%d): ", MAXV);
    if (checked_scanf("%d", &V) != 1 || V < 1 || V > MAXV) { printf("Invalid.\n"); return 1; }
    printf("Directed graph?  (1 = yes, 0 = no): ");  checked_scanf("%d", &directed);
    printf("Weighted graph?  (1 = yes, 0 = no): ");  checked_scanf("%d", &weighted);

    while (1) {
        printf("\n===== GRAPH - ADJACENCY MATRIX MENU =====\n");
        printf(" 1. Add edge           2. Remove edge        3. Add vertex\n");
        printf(" 4. Display matrix     5. Does edge u-v exist?  6. Neighbours of a vertex\n");
        printf(" 7. Degrees            8. Graph properties summary\n");
        printf(" 0. Exit\nEnter choice: ");
        if (checked_scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter u v%s: ", weighted ? " weight" : ""); w = 1;
            if (weighted) checked_scanf("%d %d %d", &u, &v, &w); else checked_scanf("%d %d", &u, &v);
            addEdge(u, v, w);
            break;
        case 2: printf("Enter u v: "); checked_scanf("%d %d", &u, &v); removeEdge(u, v); break;
        case 3: addVertex(); break;
        case 4: displayMatrix(); break;
        case 5:
            printf("Enter u v: "); checked_scanf("%d %d", &u, &v);
            if (!validVertex(u) || !validVertex(v)) printf("Invalid vertex.\n");
            else printf(adj[u][v] ? "Yes, edge %d%s%d exists (checked in O(1)).\n"
                                  : "No edge between them.\n", u, directed ? "->" : "-", v);
            break;
        case 6: printf("Enter vertex: "); checked_scanf("%d", &u); showNeighbours(u); break;
        case 7: showDegrees(); break;
        case 8: showProperties(); break;
        case 0: printf("Bye!\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
