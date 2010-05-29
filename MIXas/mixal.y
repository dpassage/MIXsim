%{
#define YYDEBUG 1
#include <stdio.h>
#include "mixparse.h"
int yylex (void);
void yyerror (char const *);
%}
%union {
    int val;
    char *symbol;
}

%token MIXAL_WHITESPACE;
%token <symbol> MIXAL_SYMBOL;
%token <val> MIXAL_NUMBER;
%token MIXAL_COMMENT;
%type <val> address;
%type <val> expression;
%type <val> atomicexpression;

%%

program:  /* empty */
    | program line
;

line:  MIXAL_COMMENT  { fprintf(stderr, "Comment!\n"); mixparse_comment(); }
    | instruction
;

instruction: loc MIXAL_SYMBOL address '\n' { fprintf(stderr, "Symbol is %s\n", $2); }
;

loc: MIXAL_WHITESPACE
    | MIXAL_SYMBOL MIXAL_WHITESPACE
;

address: /* empty */ 
    | MIXAL_WHITESPACE
    | MIXAL_WHITESPACE expression  { $$ = $2; mixparse_address($$); }
    | MIXAL_WHITESPACE expression MIXAL_WHITESPACE { $$ = $2; mixparse_address($$); }
;

expression: atomicexpression
    | '-' atomicexpression { $$ = -$2; } 
;

atomicexpression: MIXAL_NUMBER
    | MIXAL_SYMBOL
;

%%


void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
    return;
}
