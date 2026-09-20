%{
/* bcs24.y - Parser for Bcs24 (bison)
 *
 * Builds an AST in `ast_root`. Stage 1 only needs to know whether parsing
 * succeeded; Stage 2 will walk `ast_root`.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int  yylex(void);
void yyerror(const char *msg);

Node *ast_root = NULL;     /* set when the whole program has been parsed */
int   verbose_errors = 0;  /* -v flag: print details to stderr           */
%}

%locations
%define parse.error verbose

%union {
    char    *str;
    OpKind   op;
    VarType  type;
    Node    *node;
}

%token BCSMAIN IF ELSE WHILE INT BOOL
%token LEXERR                      /* illegal character (never used in a rule) */
%token <str> ID NUM
%token <op>  RELOP

%type <type> type
%type <node> program declist decl stmtlist stmt expr aexpr term factor

%destructor { free($$); }  <str>     /* token text left on the stack after an error */

%%

program
    : BCSMAIN '{' declist stmtlist '}'
        { ast_root = $$ = ast_program($3, $4, @1.first_line); }
    ;

declist
    : declist decl          { $$ = ast_list_append($1, $2); }
    | decl                  { $$ = $1; }
    ;

decl
    : type ID ';'           { $$ = ast_decl($1, $2, @2.first_line); free($2); }
    ;

type
    : INT                   { $$ = T_INT; }
    | BOOL                  { $$ = T_BOOL; }
    ;

stmtlist
    : stmtlist ';' stmt     { $$ = ast_list_append($1, $3); }
    | stmt                  { $$ = $1; }
    ;

stmt
    : ID '=' aexpr
        { $$ = ast_assign($1, $3, @1.first_line); free($1); }
    | IF '(' expr ')' '{' stmtlist '}' ELSE '{' stmtlist '}'
        { $$ = ast_if($3, $6, $10, @1.first_line); }
    | WHILE '(' expr ')' '{' stmtlist '}'
        { $$ = ast_while($3, $6, @1.first_line); }
    ;

expr
    : aexpr RELOP aexpr     { $$ = ast_binop($2, $1, $3, @2.first_line); }
    | aexpr                 { $$ = $1; }
    ;

/* aexpr -> aexpr + aexpr | term  is ambiguous; this is the equivalent
 * left-associative form (same language, no conflicts). */
aexpr
    : aexpr '+' term        { $$ = ast_binop(OP_ADD, $1, $3, @2.first_line); }
    | term                  { $$ = $1; }
    ;

term
    : term '*' factor       { $$ = ast_binop(OP_MUL, $1, $3, @2.first_line); }
    | factor                { $$ = $1; }
    ;

factor
    : ID                    { $$ = ast_id($1, @1.first_line);  free($1); }
    | NUM                   { $$ = ast_num($1, @1.first_line); free($1); }
    ;

%%

void yyerror(const char *msg)
{
    if (verbose_errors)
        fprintf(stderr, "line %d: %s\n", yylloc.first_line, msg);
}
