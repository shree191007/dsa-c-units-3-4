/* Unit 4 - Graph using ADJACENCY LIST
   BFS, DFS (recursive + iterative), connected components, shortest path (BFS), all paths,
   cycle detection, topological sort (Kahn + DFS), bipartite check, strongly connected components (Kosaraju) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct adj { int v; struct adj *next; } adj_t;
typedef struct { int n, directed; adj_t **head; } graph_t;

void g_init(graph_t *g, int n, int directed) { g->n = n; g->directed = directed; g->head = calloc(n, sizeof(adj_t *)); }
static void push_edge(graph_t *g, int u, int v) {          /* append at tail to keep insertion order */
    adj_t *a = malloc(sizeof *a); a->v = v; a->next = NULL;
    adj_t **p = &g->head[u]; while (*p) p = &(*p)->next; *p = a;
}
void g_add_edge(graph_t *g, int u, int v) { push_edge(g, u, v); if (!g->directed) push_edge(g, v, u); }
void g_free(graph_t *g) {
    for (int i = 0; i < g->n; i++) for (adj_t *a = g->head[i], *t; a; a = t) { t = a->next; free(a); }
    free(g->head);
}
void g_print(graph_t *g) {
    for (int i = 0; i < g->n; i++) { printf("%d: ", i); for (adj_t *a = g->head[i]; a; a = a->next) printf("%d -> ", a->v); puts("NULL"); }
}

/* ---------- BFS ---------- */
void bfs_from(graph_t *g, int s, int *vis) {
    int *q = malloc(g->n * sizeof(int)), f = 0, b = 0;
    q[b++] = s; vis[s] = 1;
    while (f < b) {
        int u = q[f++]; printf("%d ", u);
        for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) { vis[a->v] = 1; q[b++] = a->v; }
    }
    free(q);
}
void bfs(graph_t *g, int s) { int *vis = calloc(g->n, sizeof(int)); bfs_from(g, s, vis); puts(""); free(vis); }

/* ---------- DFS ---------- */
void dfs_rec(graph_t *g, int u, int *vis) {
    vis[u] = 1; printf("%d ", u);
    for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) dfs_rec(g, a->v, vis);
}
void dfs(graph_t *g, int s) { int *vis = calloc(g->n, sizeof(int)); dfs_rec(g, s, vis); puts(""); free(vis); }
void dfs_iter(graph_t *g, int s) {
    int *vis = calloc(g->n, sizeof(int)), *st = malloc((g->n * g->n + 1) * sizeof(int)), top = 0;
    st[top++] = s;
    while (top) {
        int u = st[--top];
        if (vis[u]) continue;
        vis[u] = 1; printf("%d ", u);
        /* push neighbours in reverse so first neighbour is visited first */
        int tmp[256], k = 0;
        for (adj_t *a = g->head[u]; a; a = a->next) tmp[k++] = a->v;
        for (int i = k - 1; i >= 0; i--) if (!vis[tmp[i]]) st[top++] = tmp[i];
    }
    puts(""); free(vis); free(st);
}

/* ---------- connected components ---------- */
int components(graph_t *g, int print) {
    int *vis = calloc(g->n, sizeof(int)), c = 0;
    for (int i = 0; i < g->n; i++) if (!vis[i]) {
        c++; if (print) printf("  component %d: ", c);
        bfs_from(g, i, vis); if (print) puts("");
    }
    free(vis); return c;
}

/* ---------- paths ---------- */
int shortest_path(graph_t *g, int s, int t, int *path) {
    int *par = malloc(g->n * sizeof(int)), *vis = calloc(g->n, sizeof(int)), *q = malloc(g->n * sizeof(int)), f = 0, b = 0, res = -1;
    for (int i = 0; i < g->n; i++) par[i] = -1;
    q[b++] = s; vis[s] = 1;
    while (f < b) { int u = q[f++]; for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) { vis[a->v] = 1; par[a->v] = u; q[b++] = a->v; } }
    if (vis[t]) {
        int *tmp = malloc(g->n * sizeof(int)), k = 0;
        for (int x = t; x != -1; x = par[x]) tmp[k++] = x;
        for (int i = 0; i < k; i++) path[i] = tmp[k - 1 - i];
        res = k - 1; free(tmp);
    }
    free(par); free(vis); free(q); return res;
}
static void all_rec(graph_t *g, int u, int t, int *vis, int *path, int len, int *cnt) {
    vis[u] = 1; path[len++] = u;
    if (u == t) { (*cnt)++; printf("  "); for (int i = 0; i < len; i++) printf("%d%s", path[i], i + 1 < len ? "->" : "\n"); }
    else for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) all_rec(g, a->v, t, vis, path, len, cnt);
    vis[u] = 0;
}
int all_paths(graph_t *g, int s, int t) {
    int *vis = calloc(g->n, sizeof(int)), *path = malloc(g->n * sizeof(int)), cnt = 0;
    all_rec(g, s, t, vis, path, 0, &cnt); free(vis); free(path); return cnt;
}

/* ---------- cycle detection ---------- */
static int cyc_und(graph_t *g, int u, int p, int *vis) {
    vis[u] = 1;
    for (adj_t *a = g->head[u]; a; a = a->next) {
        if (!vis[a->v]) { if (cyc_und(g, a->v, u, vis)) return 1; }
        else if (a->v != p) return 1;
    }
    return 0;
}
static int cyc_dir(graph_t *g, int u, int *color) {
    color[u] = 1;
    for (adj_t *a = g->head[u]; a; a = a->next) {
        if (color[a->v] == 1) return 1;
        if (color[a->v] == 0 && cyc_dir(g, a->v, color)) return 1;
    }
    color[u] = 2; return 0;
}
int has_cycle(graph_t *g) {
    int *c = calloc(g->n, sizeof(int)), r = 0;
    for (int i = 0; i < g->n && !r; i++) if (!c[i]) r = g->directed ? cyc_dir(g, i, c) : cyc_und(g, i, -1, c);
    free(c); return r;
}

/* ---------- topological sort ---------- */
int topo_kahn(graph_t *g, int *order) {
    int *indeg = calloc(g->n, sizeof(int)), *q = malloc(g->n * sizeof(int)), f = 0, b = 0, k = 0;
    for (int u = 0; u < g->n; u++) for (adj_t *a = g->head[u]; a; a = a->next) indeg[a->v]++;
    for (int i = 0; i < g->n; i++) if (!indeg[i]) q[b++] = i;
    while (f < b) { int u = q[f++]; order[k++] = u; for (adj_t *a = g->head[u]; a; a = a->next) if (--indeg[a->v] == 0) q[b++] = a->v; }
    free(indeg); free(q); return k == g->n;            /* 0 => cycle */
}
static void topo_dfs_rec(graph_t *g, int u, int *vis, int *st, int *top) {
    vis[u] = 1;
    for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) topo_dfs_rec(g, a->v, vis, st, top);
    st[(*top)++] = u;
}
void topo_dfs(graph_t *g, int *order) {
    int *vis = calloc(g->n, sizeof(int)), *st = malloc(g->n * sizeof(int)), top = 0;
    for (int i = 0; i < g->n; i++) if (!vis[i]) topo_dfs_rec(g, i, vis, st, &top);
    for (int i = 0; i < g->n; i++) order[i] = st[g->n - 1 - i];
    free(vis); free(st);
}

/* ---------- bipartite ---------- */
int is_bipartite(graph_t *g) {
    int *col = malloc(g->n * sizeof(int)), *q = malloc(g->n * sizeof(int)), ok = 1;
    for (int i = 0; i < g->n; i++) col[i] = -1;
    for (int s = 0; s < g->n && ok; s++) if (col[s] < 0) {
        int f = 0, b = 0; q[b++] = s; col[s] = 0;
        while (f < b && ok) { int u = q[f++]; for (adj_t *a = g->head[u]; a; a = a->next) {
            if (col[a->v] < 0) { col[a->v] = 1 - col[u]; q[b++] = a->v; } else if (col[a->v] == col[u]) { ok = 0; break; } } }
    }
    free(col); free(q); return ok;
}

/* ---------- strongly connected components (Kosaraju, directed) ---------- */
static void fill_order(graph_t *g, int u, int *vis, int *st, int *top) {
    vis[u] = 1; for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) fill_order(g, a->v, vis, st, top); st[(*top)++] = u;
}
static void print_comp(graph_t *g, int u, int *vis) { vis[u] = 1; printf("%d ", u); for (adj_t *a = g->head[u]; a; a = a->next) if (!vis[a->v]) print_comp(g, a->v, vis); }
int scc(graph_t *g) {
    int *vis = calloc(g->n, sizeof(int)), *st = malloc(g->n * sizeof(int)), top = 0, c = 0;
    for (int i = 0; i < g->n; i++) if (!vis[i]) fill_order(g, i, vis, st, &top);
    graph_t t; g_init(&t, g->n, 1);
    for (int u = 0; u < g->n; u++) for (adj_t *a = g->head[u]; a; a = a->next) push_edge(&t, a->v, u);
    memset(vis, 0, g->n * sizeof(int));
    while (top) { int u = st[--top]; if (!vis[u]) { printf("  SCC %d: ", ++c); print_comp(&t, u, vis); puts(""); } }
    g_free(&t); free(vis); free(st); return c;
}

int main(void) {
    graph_t g; g_init(&g, 8, 0);
    int e[][2] = {{0,1},{0,2},{1,3},{2,3},{3,4},{5,6}};
    for (int i = 0; i < 6; i++) g_add_edge(&g, e[i][0], e[i][1]);
    puts("Adjacency list:"); g_print(&g);
    printf("BFS from 0      : "); bfs(&g, 0);
    printf("DFS recursive   : "); dfs(&g, 0);
    printf("DFS iterative   : "); dfs_iter(&g, 0);
    printf("Components = %d\n", components(&g, 1));
    int p[8], d = shortest_path(&g, 0, 4, p);
    printf("Shortest 0->4 (%d edges): ", d); for (int i = 0; i <= d; i++) printf("%d ", p[i]); puts("");
    printf("All paths 0->4:\n"); printf("  total %d\n", all_paths(&g, 0, 4));
    printf("Cycle? %s | Bipartite? %s\n", has_cycle(&g) ? "yes" : "no", is_bipartite(&g) ? "yes" : "no");

    graph_t dag; g_init(&dag, 6, 1);
    int de[][2] = {{5,2},{5,0},{4,0},{4,1},{2,3},{3,1}};
    for (int i = 0; i < 6; i++) g_add_edge(&dag, de[i][0], de[i][1]);
    int ord[6];
    printf("DAG cycle? %s\nTopological (Kahn): ", has_cycle(&dag) ? "yes" : "no");
    if (topo_kahn(&dag, ord)) for (int i = 0; i < 6; i++) printf("%d ", ord[i]);
    printf("\nTopological (DFS) : "); topo_dfs(&dag, ord); for (int i = 0; i < 6; i++) printf("%d ", ord[i]); puts("");

    graph_t sc; g_init(&sc, 5, 1);
    g_add_edge(&sc, 0, 1); g_add_edge(&sc, 1, 2); g_add_edge(&sc, 2, 0); g_add_edge(&sc, 1, 3); g_add_edge(&sc, 3, 4);
    printf("Strongly connected components:\n  total %d\n", scc(&sc));
    g_free(&g); g_free(&dag); g_free(&sc);
    return 0;
}
