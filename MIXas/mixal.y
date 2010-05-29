%{
#define YYDEBUG 1
#include <stdio.h>
#include <string.h>
    
#include "mixparse.h"

int yylex (void);
void yyerror (char const *);

%}
%union {
    int val;
    char *string;
}

%token <string> MIXAL_WHITESPACE;
%token <string> MIXAL_SYMBOL;
%token <val> MIXAL_NUMBER;
%token MIXAL_COMMENT;
%type <val> address;
%type <val> expression;
%type <val> atomicexpression;
%type <val> index;
%type <val> field;
%left ':' '+' '*' '/' '-'

%%

program:  /* empty */
    | program line
;

line:  MIXAL_COMMENT  { fprintf(stderr, "Comment!\n"); mixparse_comment(); }
    | instruction
;

instruction: loc MIXAL_SYMBOL address index field '\n' 
    { 
        fprintf(stderr, "Symbol is %s\n", $2);
        mixparse_instruction($2, $3, $4, $5);
    }
;

loc: MIXAL_WHITESPACE 
        { 
            if (strlen($1) != 11) {
                yyerror("improper whitespace at start of line");
                YYERROR;
            }                      
        }
    | MIXAL_SYMBOL MIXAL_WHITESPACE
        { 
            if ((strlen($1) + strlen($2)) != 11) {
                yyerror("improper whitespace at start of line");
                YYERROR;
            }                      
        }
;

address: /* empty */ { $$ = 0; mixparse_address($$); }
    | MIXAL_WHITESPACE { $$ = 0; mixparse_address($$); }
    | MIXAL_WHITESPACE expression  { $$ = $2; mixparse_address($$); }
    | MIXAL_WHITESPACE expression MIXAL_WHITESPACE { $$ = $2; mixparse_address($$); }
;

index: /* empty */ { $$ = 0; }
    | ',' expression { $$ = $2; }
;

field: /* empty */ { $$ = 0; }
    | '(' expression ')' { $$ = $2; }
;

expression: atomicexpression
    | '-' atomicexpression { $$ = -$2; } 
    | expression ':' expression { $$ = ($1 * 8) + $3; }
    | expression '+' expression { $$ = $1 + $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { $$ = $1 / $3; }
    | expression '-' expression { $$ = $1 - $3; }
;

atomicexpression: MIXAL_NUMBER
    | MIXAL_SYMBOL { $$ = -1; }
;

%%


void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
    return;
}
