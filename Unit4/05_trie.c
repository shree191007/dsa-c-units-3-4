/* Unit 4 - Trie (prefix tree), lowercase a-z
   insert, search, starts-with, delete, count words, list all words (lexicographic),
   autocomplete, longest common prefix, iterative traversal using explicit stack */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 26
typedef struct trie { struct trie *child[ALPHA]; int end; int pass; } trie_t;

trie_t *t_new(void) { trie_t *n = calloc(1, sizeof *n); if (!n) exit(1); return n; }

void t_insert(trie_t *root, const char *w) {
    trie_t *c = root;
    for (; *w; w++) {
        int i = *w - 'a';
        if (!c->child[i]) c->child[i] = t_new();
        c = c->child[i]; c->pass++;
    }
    c->end = 1;
}
static trie_t *walk(trie_t *root, const char *p) {
    for (; *p && root; p++) root = root->child[*p - 'a'];
    return root;
}
int t_search(trie_t *root, const char *w) { trie_t *n = walk(root, w); return n && n->end; }
int t_starts_with(trie_t *root, const char *p) { return walk(root, p) != NULL; }

static int has_children(trie_t *n) { for (int i = 0; i < ALPHA; i++) if (n->child[i]) return 1; return 0; }
static int del_rec(trie_t *n, const char *w) {                    /* returns 1 if n can be freed */
    if (!*w) {
        if (!n->end) return 0;
        n->end = 0; return !has_children(n);
    }
    int i = *w - 'a';
    if (!n->child[i]) return 0;
    if (del_rec(n->child[i], w + 1)) { free(n->child[i]); n->child[i] = NULL; return !n->end && !has_children(n); }
    return 0;
}
int t_delete(trie_t *root, const char *w) {
    if (!t_search(root, w)) return 0;
    /* fix pass counts */
    trie_t *c = root; for (const char *p = w; *p; p++) { c = c->child[*p - 'a']; c->pass--; }
    del_rec(root, w); return 1;
}
int t_count_words(trie_t *n) { int c = n->end; for (int i = 0; i < ALPHA; i++) if (n->child[i]) c += t_count_words(n->child[i]); return c; }
int t_count_prefix(trie_t *root, const char *p) { trie_t *n = walk(root, p); return n ? t_count_words(n) : 0; }

static void collect(trie_t *n, char *buf, int len) {
    if (n->end) { buf[len] = 0; printf("%s ", buf); }
    for (int i = 0; i < ALPHA; i++) if (n->child[i]) { buf[len] = 'a' + i; collect(n->child[i], buf, len + 1); }
}
void t_print_all(trie_t *root) { char buf[128]; collect(root, buf, 0); puts(""); }
void t_autocomplete(trie_t *root, const char *p) {
    trie_t *n = walk(root, p); char buf[128];
    if (!n) { puts("(none)"); return; }
    strcpy(buf, p); collect(n, buf, (int)strlen(p)); puts("");
}
/* longest common prefix of all stored words */
void t_lcp(trie_t *root) {
    char buf[128]; int len = 0; trie_t *c = root;
    for (;;) {
        int cnt = 0, idx = -1;
        for (int i = 0; i < ALPHA; i++) if (c->child[i]) { cnt++; idx = i; }
        if (cnt != 1 || c->end) break;
        buf[len++] = 'a' + idx; c = c->child[idx];
    }
    buf[len] = 0; printf("\"%s\"\n", buf);
}
/* iterative preorder walk (explicit stack) printing every word in order */
void t_print_iter(trie_t *root) {
    typedef struct { trie_t *n; int next; } fr_t;
    fr_t st[128]; char buf[128]; int top = 0;
    st[top++] = (fr_t){root, 0};
    while (top) {
        fr_t *f = &st[top - 1];
        if (f->next == 0 && f->n->end) { buf[top - 1] = 0; printf("%s ", buf); }
        int i = f->next;
        while (i < ALPHA && !f->n->child[i]) i++;
        if (i == ALPHA) { top--; continue; }
        f->next = i + 1; buf[top - 1] = 'a' + i;
        st[top++] = (fr_t){f->n->child[i], 0};
    }
    puts("");
}
void t_free(trie_t *n) { if (!n) return; for (int i = 0; i < ALPHA; i++) t_free(n->child[i]); free(n); }

int main(void) {
    trie_t *root = t_new();
    const char *w[] = {"apple", "app", "apply", "apt", "bat", "bath", "batman", "cat"};
    for (int i = 0; i < 8; i++) t_insert(root, w[i]);
    printf("words stored: %d\n", t_count_words(root));
    printf("search app:%d appl:%d bath:%d dog:%d\n", t_search(root, "app"), t_search(root, "appl"), t_search(root, "bath"), t_search(root, "dog"));
    printf("starts_with 'ba':%d 'ca':%d 'x':%d\n", t_starts_with(root, "ba"), t_starts_with(root, "ca"), t_starts_with(root, "x"));
    printf("all words (recursive): "); t_print_all(root);
    printf("all words (iterative): "); t_print_iter(root);
    printf("autocomplete 'ap'  : "); t_autocomplete(root, "ap");
    printf("autocomplete 'bat' : "); t_autocomplete(root, "bat");
    printf("words with prefix 'app': %d\n", t_count_prefix(root, "app"));
    t_delete(root, "app"); t_delete(root, "batman");
    printf("after deleting app, batman: "); t_print_all(root);
    printf("search app:%d apple:%d\n", t_search(root, "app"), t_search(root, "apple"));
    trie_t *r2 = t_new(); t_insert(r2, "flower"); t_insert(r2, "flow"); t_insert(r2, "flight");
    printf("LCP of flower/flow/flight: "); t_lcp(r2);
    t_free(root); t_free(r2);
    return 0;
}
