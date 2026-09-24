%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);

extern FILE *yyin;
%}

/* Tokens coming from Flex */

%token BCSMAIN
%token IF ELSE WHILE
%token INT BOOL

%token ID NUM

%token LT GT LE GE EQ NE

%token INVALID

%start program

%%

program
    : BCSMAIN '{' declist stmtlist '}'
    ;

declist
    : declist decl
    | decl
    ;

decl
    : type ID ';'
    ;

type
    : INT
    | BOOL
    ;

stmtlist
    : stmtlist ';' stmt
    | stmt
    ;

stmt
    : ID '=' aexpr
    | IF '(' expr ')' '{' stmtlist '}' ELSE '{' stmtlist '}'
    | WHILE '(' expr ')' '{' stmtlist '}'
    ;

expr
    : aexpr LT aexpr
    | aexpr GT aexpr
    | aexpr LE aexpr
    | aexpr GE aexpr
    | aexpr EQ aexpr
    | aexpr NE aexpr
    | aexpr
    ;

aexpr
    : aexpr '+' term
    | term
    ;

term
    : term '*' factor
    | factor
    ;

factor
    : ID
    | NUM
    ;

%%

void yyerror(const char *s)
{
    /* We don't need detailed error messages for Stage 1. */
}
