%{
/* bcs24.l - Lexical analyzer for Bcs24 (flex) */
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "bcs24.tab.h"

/* keep bison's location info in sync with flex's line counter */
#define YY_USER_ACTION  yylloc.first_line = yylloc.last_line = yylineno;

static char *copy_text(void)
{
    char *s = strdup(yytext);
    if (!s) { perror("strdup"); exit(2); }
    return s;
}
%}

%option noyywrap nounput noinput yylineno

letter  [a-zA-Z]
digit   [0-9]

%%

[ \t\r\n]+          { /* skip whitespace */ }

"BcsMain"           { return BCSMAIN; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"int"               { return INT; }
"bool"              { return BOOL; }

{letter}({letter}|{digit})*   { yylval.str = copy_text(); return ID; }
{digit}+                      { yylval.str = copy_text(); return NUM; }

"<="                { yylval.op = OP_LE; return RELOP; }
">="                { yylval.op = OP_GE; return RELOP; }
"=="                { yylval.op = OP_EQ; return RELOP; }
"!="                { yylval.op = OP_NE; return RELOP; }
"<"                 { yylval.op = OP_LT; return RELOP; }
">"                 { yylval.op = OP_GT; return RELOP; }

"="                 { return '='; }
"+"                 { return '+'; }
"*"                 { return '*'; }
";"                 { return ';'; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }

.                   { return LEXERR; /* illegal character -> parser reports Syntax Error */ }

%%
