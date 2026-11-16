/* Unit 3 - Expression trees
   Build from postfix, prefix and infix (via shunting-yard -> postfix),
   traverse (infix/prefix/postfix), evaluate. Single-digit/multi-digit ints, + - * / ^ and parentheses */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct enode { char op; double val; struct enode *l, *r; } enode_t;   /* op==0 => operand */

static enode_t *mk(char op, double v, enode_t *l, enode_t *r) {
    enode_t *n = malloc(sizeof *n);
    if (!n) exit(1);
    n->op = op; n->val = v; n->l = l; n->r = r; return n;
}
static int is_op(char c) { return strchr("+-*/^", c) != NULL; }
static int prec(char c) { return c == '^' ? 3 : (c == '*' || c == '/') ? 2 : (c == '+' || c == '-') ? 1 : 0; }

/* ---- build from postfix: tokens separated by spaces ---- */
enode_t *from_postfix(const char *s) {
    enode_t *st[256]; int top = 0;
    char buf[256]; strncpy(buf, s, 255); buf[255] = 0;
    for (char *t = strtok(buf, " "); t; t = strtok(NULL, " ")) {
        if (strlen(t) == 1 && is_op(t[0])) {
            if (top < 2) { puts("bad postfix"); exit(1); }
            enode_t *r = st[--top], *l = st[--top];
            st[top++] = mk(t[0], 0, l, r);
        } else st[top++] = mk(0, atof(t), NULL, NULL);
    }
    return top == 1 ? st[0] : NULL;
}
/* ---- build from prefix: tokens separated by spaces (recursive descent) ---- */
static enode_t *prefix_rec(char **tok, int *i, int n) {
    if (*i >= n) return NULL;
    char *t = tok[(*i)++];
    if (strlen(t) == 1 && is_op(t[0])) {
        enode_t *l = prefix_rec(tok, i, n), *r = prefix_rec(tok, i, n);
        return mk(t[0], 0, l, r);
    }
    return mk(0, atof(t), NULL, NULL);
}
enode_t *from_prefix(const char *s) {
    char buf[256], *tok[128]; int n = 0, i = 0;
    strncpy(buf, s, 255); buf[255] = 0;
    for (char *t = strtok(buf, " "); t; t = strtok(NULL, " ")) tok[n++] = t;
    return prefix_rec(tok, &i, n);
}
/* ---- infix -> postfix (shunting yard) ; operands are integers ---- */
void infix_to_postfix(const char *in, char *out) {
    char st[256]; int top = 0, o = 0;
    for (int i = 0; in[i]; ) {
        char c = in[i];
        if (isspace((unsigned char)c)) { i++; continue; }
        if (isdigit((unsigned char)c) || c == '.') {
            while (isdigit((unsigned char)in[i]) || in[i] == '.') out[o++] = in[i++];
            out[o++] = ' ';
        } else if (c == '(') { st[top++] = c; i++; }
        else if (c == ')') {
            while (top && st[top - 1] != '(') { out[o++] = st[--top]; out[o++] = ' '; }
            if (top) top--;
            i++;
        } else if (is_op(c)) {
            while (top && is_op(st[top - 1]) &&
                   (prec(st[top - 1]) > prec(c) || (prec(st[top - 1]) == prec(c) && c != '^'))) {
                out[o++] = st[--top]; out[o++] = ' ';
            }
            st[top++] = c; i++;
        } else i++;
    }
    while (top) { out[o++] = st[--top]; out[o++] = ' '; }
    out[o] = 0;
}
enode_t *from_infix(const char *s) { char pf[512]; infix_to_postfix(s, pf); return from_postfix(pf); }

void print_val(enode_t *n) { if (n->val == (long)n->val) printf("%ld", (long)n->val); else printf("%g", n->val); }
void infix(enode_t *n) {
    if (!n) return;
    if (!n->op) { print_val(n); return; }
    printf("("); infix(n->l); printf(" %c ", n->op); infix(n->r); printf(")");
}
void prefix(enode_t *n)  { if (!n) return; if (n->op) printf("%c ", n->op); else { print_val(n); printf(" "); } prefix(n->l); prefix(n->r); }
void postfix(enode_t *n) { if (!n) return; postfix(n->l); postfix(n->r); if (n->op) printf("%c ", n->op); else { print_val(n); printf(" "); } }

double eval(enode_t *n) {
    if (!n->op) return n->val;
    double a = eval(n->l), b = eval(n->r);
    switch (n->op) {
        case '+': return a + b; case '-': return a - b;
        case '*': return a * b; case '/': return a / b;
        default:  return pow(a, b);
    }
}
int height(enode_t *n) { if (!n) return -1; int a = height(n->l), b = height(n->r); return 1 + (a > b ? a : b); }
void destroy(enode_t *n) { if (n) { destroy(n->l); destroy(n->r); free(n); } }

static void show(const char *label, enode_t *t) {
    printf("%s\n  infix  : ", label); infix(t);
    printf("\n  prefix : "); prefix(t);
    printf("\n  postfix: "); postfix(t);
    printf("\n  value  = %g, height = %d\n", eval(t), height(t));
}
int main(void) {
    enode_t *t1 = from_postfix("2 3 + 4 5 * -");  show("From postfix '2 3 + 4 5 * -'", t1);
    enode_t *t2 = from_prefix("* + 2 3 - 7 4");    show("From prefix '* + 2 3 - 7 4'", t2);
    enode_t *t3 = from_infix("(12 + 3) * 4 - 20 / 5 ^ 1"); show("From infix '(12 + 3) * 4 - 20 / 5 ^ 1'", t3);
    destroy(t1); destroy(t2); destroy(t3);
    return 0;
}
