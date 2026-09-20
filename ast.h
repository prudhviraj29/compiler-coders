/* ast.h - Abstract syntax tree for Bcs24.
 *
 * Stage 1 only builds the tree. Stage 2 (semantic analysis + 3-address code)
 * walks it, so nothing in the lexer/parser needs to change for Stage 2.
 */
#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef enum { T_INT, T_BOOL } VarType;

typedef enum {
    N_PROGRAM,  /* lhs = declaration list, rhs = statement list            */
    N_DECL,     /* type, text = variable name                              */
    N_ASSIGN,   /* text = target variable, lhs = expression                */
    N_IF,       /* lhs = condition, rhs = then-list, extra = else-list     */
    N_WHILE,    /* lhs = condition, rhs = body list                        */
    N_BINOP,    /* op, lhs, rhs                                            */
    N_ID,       /* text = variable name                                    */
    N_NUM       /* text = literal lexeme                                   */
} NodeKind;

typedef enum {
    OP_ADD, OP_MUL,
    OP_LT, OP_GT, OP_LE, OP_GE, OP_EQ, OP_NE
} OpKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    int      line;
    OpKind   op;
    VarType  type;
    char    *text;
    Node    *lhs, *rhs, *extra;
    Node    *next;      /* sibling link: declarations / statements are linked lists */

    /* ---- Stage 2 hooks (unused in Stage 1) ----
     * e.g. char *place;   name of the temp / variable holding this node's value
     *      VarType vtype; result type computed during semantic analysis        */
};

/* constructors (text is copied) */
Node *ast_program(Node *decls, Node *stmts, int line);
Node *ast_decl(VarType t, const char *name, int line);
Node *ast_assign(const char *name, Node *expr, int line);
Node *ast_if(Node *cond, Node *then_l, Node *else_l, int line);
Node *ast_while(Node *cond, Node *body, int line);
Node *ast_binop(OpKind op, Node *l, Node *r, int line);
Node *ast_id(const char *name, int line);
Node *ast_num(const char *lexeme, int line);

Node *ast_list_append(Node *head, Node *n);   /* returns head */
void  ast_free(Node *n);
void  ast_dump(const Node *n, FILE *out);     /* debug: prints the tree */
const char *op_str(OpKind op);

#endif
