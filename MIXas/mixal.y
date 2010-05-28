%{
#define YYSTYPE char *
#include <stdio.h>
int yylex (void);
void yyerror (char const *);
%}

%token MIXAL_WHITESPACE;
%token MIXAL_SYMBOL;
%token MIXAL_NUMBER;
%token MIXAL_COMMENT;

%%

program:  /* empty */
    | program card
;

card:  MIXAL_COMMENT  { fprintf(stderr, "Comment!\n"); }
    | line
;

line: loc MIXAL_SYMBOL address '\n' { fprintf(stderr, "Line o' code!\n"); }
;

loc: MIXAL_WHITESPACE
    | MIXAL_SYMBOL MIXAL_WHITESPACE
;

address: /* empty */ 
    | MIXAL_WHITESPACE
    | MIXAL_WHITESPACE MIXAL_NUMBER
    | MIXAL_WHITESPACE MIXAL_NUMBER MIXAL_WHITESPACE
;

%%


void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
    return;
}
