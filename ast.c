#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static Node *new_node(NodeKind k, int line)
{
    Node *n = calloc(1, sizeof *n);
    if (!n) { perror("calloc"); exit(2); }
    n->kind = k;
    n->line = line;
    return n;
}

static char *dupstr(const char *s)
{
    char *p = strdup(s);
    if (!p) { perror("strdup"); exit(2); }
    return p;
}

Node *ast_program(Node *decls, Node *stmts, int line)
{
    Node *n = new_node(N_PROGRAM, line);
    n->lhs = decls;
    n->rhs = stmts;
    return n;
}

Node *ast_decl(VarType t, const char *name, int line)
{
    Node *n = new_node(N_DECL, line);
    n->type = t;
    n->text = dupstr(name);
    return n;
}

Node *ast_assign(const char *name, Node *expr, int line)
{
    Node *n = new_node(N_ASSIGN, line);
    n->text = dupstr(name);
    n->lhs = expr;
    return n;
}

Node *ast_if(Node *cond, Node *then_l, Node *else_l, int line)
{
    Node *n = new_node(N_IF, line);
    n->lhs = cond;
    n->rhs = then_l;
    n->extra = else_l;
    return n;
}

Node *ast_while(Node *cond, Node *body, int line)
{
    Node *n = new_node(N_WHILE, line);
    n->lhs = cond;
    n->rhs = body;
    return n;
}

Node *ast_binop(OpKind op, Node *l, Node *r, int line)
{
    Node *n = new_node(N_BINOP, line);
    n->op = op;
    n->lhs = l;
    n->rhs = r;
    return n;
}

Node *ast_id(const char *name, int line)
{
    Node *n = new_node(N_ID, line);
    n->text = dupstr(name);
    return n;
}

Node *ast_num(const char *lexeme, int line)
{
    Node *n = new_node(N_NUM, line);
    n->text = dupstr(lexeme);
    return n;
}

Node *ast_list_append(Node *head, Node *n)
{
    Node *p = head;
    while (p->next) p = p->next;
    p->next = n;
    return head;
}

void ast_free(Node *n)
{
    while (n) {
        Node *next = n->next;
        ast_free(n->lhs);
        ast_free(n->rhs);
        ast_free(n->extra);
        free(n->text);
        free(n);
        n = next;
    }
}

const char *op_str(OpKind op)
{
    switch (op) {
    case OP_ADD: return "+";
    case OP_MUL: return "*";
    case OP_LT:  return "<";
    case OP_GT:  return ">";
    case OP_LE:  return "<=";
    case OP_GE:  return ">=";
    case OP_EQ:  return "==";
    case OP_NE:  return "!=";
    }
    return "?";
}

/* ---------- debug dump ---------- */

static void indent(FILE *o, int d) { fprintf(o, "%*s", d * 2, ""); }

static void dump(const Node *n, FILE *o, int d)
{
    for (; n; n = n->next) {
        indent(o, d);
        switch (n->kind) {
        case N_PROGRAM:
            fprintf(o, "Program\n");
            dump(n->lhs, o, d + 1);
            dump(n->rhs, o, d + 1);
            break;
        case N_DECL:
            fprintf(o, "Decl %s %s\n", n->type == T_INT ? "int" : "bool", n->text);
            break;
        case N_ASSIGN:
            fprintf(o, "Assign %s =\n", n->text);
            dump(n->lhs, o, d + 1);
            break;
        case N_IF:
            fprintf(o, "If\n");
            indent(o, d + 1); fprintf(o, "cond:\n");  dump(n->lhs, o, d + 2);
            indent(o, d + 1); fprintf(o, "then:\n");  dump(n->rhs, o, d + 2);
            indent(o, d + 1); fprintf(o, "else:\n");  dump(n->extra, o, d + 2);
            break;
        case N_WHILE:
            fprintf(o, "While\n");
            indent(o, d + 1); fprintf(o, "cond:\n");  dump(n->lhs, o, d + 2);
            indent(o, d + 1); fprintf(o, "body:\n");  dump(n->rhs, o, d + 2);
            break;
        case N_BINOP:
            fprintf(o, "BinOp %s\n", op_str(n->op));
            dump(n->lhs, o, d + 1);
            dump(n->rhs, o, d + 1);
            break;
        case N_ID:
            fprintf(o, "Id %s\n", n->text);
            break;
        case N_NUM:
            fprintf(o, "Num %s\n", n->text);
            break;
        }
    }
}

void ast_dump(const Node *n, FILE *out) { dump(n, out, 0); }
