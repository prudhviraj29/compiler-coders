/* main.c - driver for the Bcs24 compiler front end
 *
 * usage: bcs24 [-v] [-d] <program-file>
 *   -v   print error details (line + message) to stderr
 *   -d   dump the AST to stderr after a successful parse
 *
 * stdout is always exactly "Parsing Successful" or "Syntax Error".
 */
#include <stdio.h>
#include <string.h>
#include "ast.h"

extern FILE *yyin;
extern int   yyparse(void);
extern Node *ast_root;
extern int   verbose_errors;

int main(int argc, char **argv)
{
    int dump_ast = 0;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-v")) verbose_errors = 1;
        else if (!strcmp(argv[i], "-d")) dump_ast = 1;
        else                             path = argv[i];
    }
    if (!path) {
        fprintf(stderr, "usage: %s [-v] [-d] <program-file>\n", argv[0]);
        return 2;
    }

    yyin = fopen(path, "r");
    if (!yyin) {
        perror(path);
        return 2;
    }

    int rc = yyparse();
    fclose(yyin);

    if (rc != 0) {
        printf("Syntax Error\n");
        return 1;
    }

    printf("Parsing Successful\n");
    if (dump_ast) ast_dump(ast_root, stderr);

    /* ---- Stage 2 goes here ----
     *   semantic_check(ast_root);              // optional: declared-before-use, types
     *   gen_tac(ast_root, "output.txt");       // 3-address code for assignments
     */

    ast_free(ast_root);
    return 0;
}
