/* Unit 4 - Graph using ADJACENCY MATRIX
   BFS, DFS (recursive + iterative), connected components, path finding (BFS shortest, DFS all paths),
   degree, cycle detection (undirected & directed), transpose */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXV 20

typedef struct { int n, directed; int m[MAXV][MAXV]; } graph_t;

void g_init(graph_t *g, int n, int directed) { g->n = n; g->directed = directed; memset(g->m, 0, sizeof g->m); }
void g_add_edge(graph_t *g, int u, int v) { g->m[u][v] = 1; if (!g->directed) g->m[v][u] = 1; }
void g_remove_edge(graph_t *g, int u, int v) { g->m[u][v] = 0; if (!g->directed) g->m[v][u] = 0; }
void g_print(graph_t *g) {
    printf("    "); for (int j = 0; j < g->n; j++) printf("%2d ", j); puts("");
    for (int i = 0; i < g->n; i++) { printf("%2d: ", i); for (int j = 0; j < g->n; j++) printf("%2d ", g->m[i][j]); puts(""); }
}
int out_degree(graph_t *g, int v) { int d = 0; for (int j = 0; j < g->n; j++) d += g->m[v][j]; return d; }
int in_degree(graph_t *g, int v)  { int d = 0; for (int i = 0; i < g->n; i++) d += g->m[i][v]; return d; }

/* ---------- BFS ---------- */
void bfs_from(graph_t *g, int s, int *vis) {
    int q[MAXV], f = 0, b = 0;
    q[b++] = s; vis[s] = 1;
    while (f < b) {
        int u = q[f++]; printf("%d ", u);
        for (int v = 0; v < g->n; v++) if (g->m[u][v] && !vis[v]) { vis[v] = 1; q[b++] = v; }
    }
}
void bfs(graph_t *g, int s) { int vis[MAXV] = {0}; bfs_from(g, s, vis); puts(""); }

/* ---------- DFS ---------- */
void dfs_rec(graph_t *g, int u, int *vis) {
    vis[u] = 1; printf("%d ", u);
    for (int v = 0; v < g->n; v++) if (g->m[u][v] && !vis[v]) dfs_rec(g, v, vis);
}
void dfs(graph_t *g, int s) { int vis[MAXV] = {0}; dfs_rec(g, s, vis); puts(""); }
void dfs_iter(graph_t *g, int s) {
    int vis[MAXV] = {0}, st[MAXV * MAXV], top = 0;
    st[top++] = s;
    while (top) {
        int u = st[--top];
        if (vis[u]) continue;
        vis[u] = 1; printf("%d ", u);
        for (int v = g->n - 1; v >= 0; v--) if (g->m[u][v] && !vis[v]) st[top++] = v;
    }
    puts("");
}

/* ---------- connected components (undirected) ---------- */
int components(graph_t *g, int print) {
    int vis[MAXV] = {0}, c = 0;
    for (int i = 0; i < g->n; i++) if (!vis[i]) {
        c++; if (print) printf("  component %d: ", c);
        if (print) bfs_from(g, i, vis); else { int tmp[MAXV]; (void)tmp; dfs_rec(g, i, vis); }
        if (print) puts("");
    }
    return c;
}

/* ---------- paths ---------- */
int shortest_path(graph_t *g, int s, int t, int *path) {      /* BFS; returns #edges or -1 */
    int par[MAXV], vis[MAXV] = {0}, q[MAXV], f = 0, b = 0;
    for (int i = 0; i < g->n; i++) par[i] = -1;
    q[b++] = s; vis[s] = 1;
    while (f < b) {
        int u = q[f++];
        if (u == t) break;
        for (int v = 0; v < g->n; v++) if (g->m[u][v] && !vis[v]) { vis[v] = 1; par[v] = u; q[b++] = v; }
    }
    if (!vis[t]) return -1;
    int tmp[MAXV], k = 0;
    for (int x = t; x != -1; x = par[x]) tmp[k++] = x;
    for (int i = 0; i < k; i++) path[i] = tmp[k - 1 - i];
    return k - 1;
}
static void all_paths_rec(graph_t *g, int u, int t, int *vis, int *path, int len, int *count) {
    vis[u] = 1; path[len++] = u;
    if (u == t) { (*count)++; printf("  "); for (int i = 0; i < len; i++) printf("%d%s", path[i], i + 1 < len ? "->" : "\n"); }
    else for (int v = 0; v < g->n; v++) if (g->m[u][v] && !vis[v]) all_paths_rec(g, v, t, vis, path, len, count);
    vis[u] = 0;
}
int all_paths(graph_t *g, int s, int t) {
    int vis[MAXV] = {0}, path[MAXV], count = 0;
    all_paths_rec(g, s, t, vis, path, 0, &count); return count;
}
int has_path(graph_t *g, int s, int t) { int vis[MAXV] = {0}; int sink = 0; (void)sink;
    int q[MAXV], f = 0, b = 0; q[b++] = s; vis[s] = 1;
    while (f < b) { int u = q[f++]; if (u == t) return 1; for (int v = 0; v < g->n; v++) if (g->m[u][v] && !vis[v]) { vis[v] = 1; q[b++] = v; } }
    return 0; }

/* ---------- cycle detection ---------- */
static int cyc_und(graph_t *g, int u, int p, int *vis) {
    vis[u] = 1;
    for (int v = 0; v < g->n; v++) if (g->m[u][v]) {
        if (!vis[v]) { if (cyc_und(g, v, u, vis)) return 1; }
        else if (v != p) return 1;
    }
    return 0;
}
static int cyc_dir(graph_t *g, int u, int *color) {            /* 0 white 1 gray 2 black */
    color[u] = 1;
    for (int v = 0; v < g->n; v++) if (g->m[u][v]) {
        if (color[v] == 1) return 1;
        if (color[v] == 0 && cyc_dir(g, v, color)) return 1;
    }
    color[u] = 2; return 0;
}
int has_cycle(graph_t *g) {
    int vis[MAXV] = {0};
    for (int i = 0; i < g->n; i++) if (!vis[i]) {
        if (g->directed ? cyc_dir(g, i, vis) : cyc_und(g, i, -1, vis)) return 1;
    }
    return 0;
}
void transpose(graph_t *g, graph_t *t) { g_init(t, g->n, g->directed); for (int i = 0; i < g->n; i++) for (int j = 0; j < g->n; j++) t->m[j][i] = g->m[i][j]; }

int main(void) {
    graph_t g; g_init(&g, 8, 0);
    int e[][2] = {{0,1},{0,2},{1,3},{2,3},{3,4},{5,6}};      /* vertex 7 isolated, {5,6} separate */
    for (int i = 0; i < 6; i++) g_add_edge(&g, e[i][0], e[i][1]);
    puts("Adjacency matrix:"); g_print(&g);
    printf("BFS from 0       : "); bfs(&g, 0);
    printf("DFS (recursive)  : "); dfs(&g, 0);
    printf("DFS (iterative)  : "); dfs_iter(&g, 0);
    printf("Degree of 3: %d\n", out_degree(&g, 3));
    printf("Connected components = %d\n", components(&g, 1));
    int p[MAXV], d = shortest_path(&g, 0, 4, p);
    printf("Shortest path 0->4 (%d edges): ", d); for (int i = 0; i <= d; i++) printf("%d ", p[i]); puts("");
    printf("Path 0->6 exists? %s\n", has_path(&g, 0, 6) ? "yes" : "no");
    printf("All paths 0->4:\n"); printf("  total = %d\n", all_paths(&g, 0, 4));
    printf("Undirected graph has cycle? %s\n", has_cycle(&g) ? "yes" : "no");

    graph_t dg; g_init(&dg, 5, 1);
    g_add_edge(&dg, 0, 1); g_add_edge(&dg, 1, 2); g_add_edge(&dg, 2, 3); g_add_edge(&dg, 3, 4);
    printf("Directed chain has cycle? %s\n", has_cycle(&dg) ? "yes" : "no");
    g_add_edge(&dg, 4, 1);
    printf("After adding 4->1 cycle? %s | in/out degree of 1 = %d/%d\n", has_cycle(&dg) ? "yes" : "no", in_degree(&dg, 1), out_degree(&dg, 1));
    return 0;
}
