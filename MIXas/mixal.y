%{
#define YYSTYPE char *
#include <stdio.h>
int yylex (void);
void yyerror (char const *);
%}

%token SYMBOL
%token OPCODE 
%token NUMBER
%token COMMENT

%%

program:  /* empty */
    | program card
;

card:   COMMENT  { fprintf(stderr, "Comment!\n"); }
    | line
;

line: loc OPCODE address '\n'
;

loc: /* empty */
    | SYMBOL
;

address: NUMBER
;

%%


void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
    return;
}
