/* Unit 4 - Weighted graph applications: Dijkstra, Prim MST, Kruskal MST (union-find), Floyd-Warshall, Bellman-Ford */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 6
#define INF 1000000000

typedef struct { int u, v, w; } edge_t;

static int w_mat[V][V];
static void build(const edge_t *e, int ne, int directed) {
    for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) w_mat[i][j] = (i == j) ? 0 : INF;
    for (int i = 0; i < ne; i++) { w_mat[e[i].u][e[i].v] = e[i].w; if (!directed) w_mat[e[i].v][e[i].u] = e[i].w; }
}

void dijkstra(int src) {
    int dist[V], par[V], done[V] = {0};
    for (int i = 0; i < V; i++) { dist[i] = INF; par[i] = -1; }
    dist[src] = 0;
    for (int it = 0; it < V; it++) {
        int u = -1;
        for (int i = 0; i < V; i++) if (!done[i] && (u < 0 || dist[i] < dist[u])) u = i;
        if (dist[u] == INF) break;
        done[u] = 1;
        for (int v = 0; v < V; v++) if (w_mat[u][v] != INF && w_mat[u][v] > 0 && dist[u] + w_mat[u][v] < dist[v]) { dist[v] = dist[u] + w_mat[u][v]; par[v] = u; }
    }
    printf("Dijkstra from %d:\n", src);
    for (int i = 0; i < V; i++) {
        printf("  to %d: dist=%d path=", i, dist[i]);
        int stk[V], k = 0; for (int x = i; x != -1; x = par[x]) stk[k++] = x;
        for (int j = k - 1; j >= 0; j--) printf("%d%s", stk[j], j ? "->" : "\n");
    }
}

int prim(void) {
    int key[V], par[V], in[V] = {0}, total = 0;
    for (int i = 0; i < V; i++) { key[i] = INF; par[i] = -1; }
    key[0] = 0;
    for (int it = 0; it < V; it++) {
        int u = -1;
        for (int i = 0; i < V; i++) if (!in[i] && (u < 0 || key[i] < key[u])) u = i;
        in[u] = 1; total += key[u];
        for (int v = 0; v < V; v++) if (!in[v] && w_mat[u][v] != INF && w_mat[u][v] < key[v]) { key[v] = w_mat[u][v]; par[v] = u; }
    }
    puts("Prim MST edges:");
    for (int i = 1; i < V; i++) printf("  %d - %d (w=%d)\n", par[i], i, w_mat[par[i]][i]);
    return total;
}

static int parent[V], rnk[V];
static int find(int x) { return parent[x] == x ? x : (parent[x] = find(parent[x])); }
static int unite(int a, int b) {
    a = find(a); b = find(b); if (a == b) return 0;
    if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
    parent[b] = a; if (rnk[a] == rnk[b]) rnk[a]++; return 1;
}
static int cmp_edge(const void *a, const void *b) { return ((const edge_t *)a)->w - ((const edge_t *)b)->w; }
int kruskal(edge_t *e, int ne) {
    for (int i = 0; i < V; i++) { parent[i] = i; rnk[i] = 0; }
    qsort(e, ne, sizeof *e, cmp_edge);
    int total = 0; puts("Kruskal MST edges:");
    for (int i = 0; i < ne; i++) if (unite(e[i].u, e[i].v)) { printf("  %d - %d (w=%d)\n", e[i].u, e[i].v, e[i].w); total += e[i].w; }
    return total;
}

void floyd(void) {
    int d[V][V];
    for (int i = 0; i < V; i++) for (int j = 0; j < V; j++) d[i][j] = w_mat[i][j];
    for (int k = 0; k < V; k++) for (int i = 0; i < V; i++) for (int j = 0; j < V; j++)
        if (d[i][k] != INF && d[k][j] != INF && d[i][k] + d[k][j] < d[i][j]) d[i][j] = d[i][k] + d[k][j];
    puts("Floyd-Warshall all-pairs distances:");
    for (int i = 0; i < V; i++) { for (int j = 0; j < V; j++) { if (d[i][j] == INF) printf("  INF"); else printf("%5d", d[i][j]); } puts(""); }
}

int bellman_ford(const edge_t *e, int ne, int src) {            /* handles negative weights; returns 0 on negative cycle */
    int dist[V];
    for (int i = 0; i < V; i++) dist[i] = INF;
    dist[src] = 0;
    for (int i = 0; i < V - 1; i++) for (int j = 0; j < ne; j++)
        if (dist[e[j].u] != INF && dist[e[j].u] + e[j].w < dist[e[j].v]) dist[e[j].v] = dist[e[j].u] + e[j].w;
    for (int j = 0; j < ne; j++) if (dist[e[j].u] != INF && dist[e[j].u] + e[j].w < dist[e[j].v]) { puts("Bellman-Ford: negative cycle"); return 0; }
    printf("Bellman-Ford from %d:", src); for (int i = 0; i < V; i++) printf(" %d", dist[i]); puts("");
    return 1;
}

int main(void) {
    edge_t e[] = {{0,1,7},{0,2,9},{0,5,14},{1,2,10},{1,3,15},{2,3,11},{2,5,2},{3,4,6},{4,5,9}};
    int ne = sizeof e / sizeof *e;
    build(e, ne, 0);
    dijkstra(0);
    printf("Prim total weight = %d\n", prim());
    printf("Kruskal total weight = %d\n", kruskal(e, ne));
    floyd();
    edge_t neg[] = {{0,1,4},{0,2,5},{1,3,-3},{2,3,2},{3,4,2},{4,5,1}};
    bellman_ford(neg, 6, 0);
    return 0;
}
