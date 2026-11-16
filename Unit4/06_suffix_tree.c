/* Unit 4 - Suffix trie and compressed suffix tree
   Text is terminated by '$'. Compressed tree stores edge labels as [start,end] indices into the text
   (built by inserting each suffix with edge splitting, O(n^2)).
   Operations: substring search, count occurrences, list occurrence positions,
   longest repeated substring, count distinct substrings, print tree. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 128
typedef struct snode {
    struct snode *child[MAXC];
    int start, end;              /* edge label text[start..end] leading INTO this node */
    int suffix_index;            /* >=0 for leaves */
} snode_t;

static const char *text; static int n;

static snode_t *new_node(int s, int e) {
    snode_t *x = calloc(1, sizeof *x); if (!x) exit(1);
    x->start = s; x->end = e; x->suffix_index = -1; return x;
}
static int edge_len(snode_t *x) { return x->end - x->start + 1; }

snode_t *build(const char *t) {                              /* t must end with '$' */
    text = t; n = (int)strlen(t);
    snode_t *root = new_node(-1, -1);
    for (int i = 0; i < n; i++) {                            /* insert suffix t[i..] */
        snode_t *cur = root; int pos = i;
        while (pos < n) {
            unsigned char c = (unsigned char)text[pos];
            snode_t *nx = cur->child[c];
            if (!nx) { snode_t *leaf = new_node(pos, n - 1); leaf->suffix_index = i; cur->child[c] = leaf; break; }
            int len = edge_len(nx), k = 0;
            while (k < len && pos + k < n && text[nx->start + k] == text[pos + k]) k++;
            if (k == len) { cur = nx; pos += len; continue; }
            /* split edge at k */
            snode_t *mid = new_node(nx->start, nx->start + k - 1);
            cur->child[c] = mid;
            nx->start += k;
            mid->child[(unsigned char)text[nx->start]] = nx;
            snode_t *leaf = new_node(pos + k, n - 1); leaf->suffix_index = i;
            mid->child[(unsigned char)text[pos + k]] = leaf;
            break;
        }
    }
    return root;
}
/* walk pattern; returns node whose subtree contains all matches, or NULL */
static snode_t *find_pattern(snode_t *root, const char *p) {
    snode_t *cur = root; int pl = (int)strlen(p), i = 0;
    while (i < pl) {
        snode_t *nx = cur->child[(unsigned char)p[i]];
        if (!nx) return NULL;
        int len = edge_len(nx), k = 0;
        while (k < len && i < pl) { if (text[nx->start + k] != p[i]) return NULL; k++; i++; }
        cur = nx;
    }
    return cur;
}
int st_contains(snode_t *root, const char *p) { return find_pattern(root, p) != NULL; }
static void leaves(snode_t *x, int *out, int *cnt) {
    if (x->suffix_index >= 0) { out[(*cnt)++] = x->suffix_index; return; }
    for (int c = 0; c < MAXC; c++) if (x->child[c]) leaves(x->child[c], out, cnt);
}
int st_occurrences(snode_t *root, const char *p, int *pos) {
    snode_t *x = find_pattern(root, p); int cnt = 0;
    if (x) leaves(x, pos, &cnt);
    return cnt;
}
static void lrs(snode_t *x, int depth, int *best, int *where) {   /* deepest internal node = longest repeated substring */
    if (x->suffix_index >= 0) return;
    int d = depth + (x->start >= 0 ? edge_len(x) : 0);
    if (x->start >= 0 && d > *best) {
        *best = d;
        snode_t *y = x; while (y->suffix_index < 0) { int c; for (c = 0; c < MAXC && !y->child[c]; c++) { } y = y->child[c]; }
        *where = y->suffix_index;
    }
    for (int c = 0; c < MAXC; c++) if (x->child[c]) lrs(x->child[c], d, best, where);
}
void st_longest_repeated(snode_t *root) {
    int best = 0, where = 0; lrs(root, 0, &best, &where);
    if (!best) puts("(none)"); else printf("\"%.*s\"\n", best, text + where);
}
static int distinct(snode_t *x) {                            /* number of distinct substrings (excluding the '$' ones) */
    int total = 0;
    if (x->start >= 0) { int len = edge_len(x); if (x->suffix_index >= 0 && text[x->end] == '$') len--; total += len; }
    for (int c = 0; c < MAXC; c++) if (x->child[c]) total += distinct(x->child[c]);
    return total;
}
int st_distinct_substrings(snode_t *root) { return distinct(root); }
static void show(snode_t *x, int d) {
    for (int c = 0; c < MAXC; c++) if (x->child[c]) {
        snode_t *y = x->child[c];
        printf("%*s%.*s", d * 3, "", edge_len(y), text + y->start);
        if (y->suffix_index >= 0) printf("  [suffix %d]", y->suffix_index);
        puts(""); show(y, d + 1);
    }
}
void st_print(snode_t *root) { show(root, 0); }
void st_free(snode_t *x) { if (!x) return; for (int c = 0; c < MAXC; c++) st_free(x->child[c]); free(x); }

/* ----- plain (uncompressed) suffix trie, for comparison ----- */
typedef struct strie { struct strie *c[MAXC]; } strie_t;
strie_t *suffix_trie(const char *t) {
    strie_t *root = calloc(1, sizeof *root);
    for (int i = 0; t[i]; i++) {
        strie_t *cur = root;
        for (int j = i; t[j]; j++) { unsigned char ch = (unsigned char)t[j]; if (!cur->c[ch]) cur->c[ch] = calloc(1, sizeof *cur); cur = cur->c[ch]; }
    }
    return root;
}
int trie_has(strie_t *r, const char *p) { for (; *p && r; p++) r = r->c[(unsigned char)*p]; return r != NULL; }
void trie_free(strie_t *r) { if (!r) return; for (int i = 0; i < MAXC; i++) trie_free(r->c[i]); free(r); }

int main(void) {
    const char *s = "banana$";
    snode_t *root = build(s);
    puts("Suffix tree of banana$:"); st_print(root);
    const char *pats[] = {"ana", "nan", "ban", "nab", "a", "na"};
    for (int i = 0; i < 6; i++) {
        int pos[64], k = st_occurrences(root, pats[i], pos);
        printf("pattern %-4s: %s, %d occurrence(s)", pats[i], st_contains(root, pats[i]) ? "found" : "absent", k);
        for (int j = 0; j < k; j++) for (int a = j + 1; a < k; a++) if (pos[a] < pos[j]) { int t = pos[a]; pos[a] = pos[j]; pos[j] = t; }
        if (k) { printf(" at "); for (int j = 0; j < k; j++) printf("%d ", pos[j]); }
        puts("");
    }
    printf("Longest repeated substring: "); st_longest_repeated(root);
    printf("Distinct substrings of banana: %d\n", st_distinct_substrings(root));

    strie_t *tr = suffix_trie("banana");
    printf("Suffix trie contains 'nana':%d 'nab':%d\n", trie_has(tr, "nana"), trie_has(tr, "nab"));
    trie_free(tr); st_free(root);

    snode_t *r2 = build("mississippi$");
    printf("mississippi: LRS = "); st_longest_repeated(r2);
    int pp[64];
    printf("mississippi: 'issi' occurs %d times\n", st_occurrences(r2, "issi", pp));
    st_free(r2);
    return 0;
}
