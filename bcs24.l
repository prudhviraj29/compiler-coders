%{
#include "bcs24.tab.h"
%}

%option noyywrap

letter [a-zA-Z]
digit  [0-9]

%%

[ \t\r\n]+        ;

"BcsMain"         return BCSMAIN;
"if"              return IF;
"else"            return ELSE;
"while"           return WHILE;
"int"             return INT;
"bool"            return BOOL;

{letter}({letter}|{digit})*    return ID;
{digit}+                       return NUM;

"<="              return LE;
">="              return GE;
"=="              return EQ;
"!="              return NE;
"<"               return LT;
">"               return GT;

"="               return '=';
"+"               return '+';
"*"               return '*';

"("               return '(';
")"               return ')';
"{"               return '{';
"}"               return '}';
";"               return ';';

.                 return INVALID;

%%
