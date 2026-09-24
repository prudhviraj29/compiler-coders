#include <stdio.h>

extern FILE *yyin;
int yyparse(void);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if (yyin == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    if (yyparse() == 0)
    {
        printf("Parsing Successful\n");
    }
    else
    {
        printf("Syntax Error\n");
    }

    fclose(yyin);

    return 0;
}
