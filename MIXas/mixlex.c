/*
 *  mixlex.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "mixlex.h"

#include "y.tab.h"

static FILE *inputfile;
static int mixlex_default_getnextchar(void) {
    return fgetc(inputfile);
}

static int mixlex_default_ungetchar(int ch) {
    return ungetc(ch, inputfile);
}

static int lex_column = 0; /* column of character just read */
static int (*getnextchar)(void) = mixlex_default_getnextchar;
static int (*ungetchar)(int) = mixlex_default_ungetchar;
static char tokenbuffer[11];

void mixlex_input(FILE *f) {
    inputfile = f;
}
void mixlex_set_getchar(int (*nextchar)(void)) {
    getnextchar = nextchar;
}
void mixlex_set_ungetchar(int (*ungetcharfn)(int)) {
    ungetchar = ungetcharfn;
}

char *mixlex_get_token() {
    return &tokenbuffer[0];
}

static enum lexstate {
    INSPACE,
    INSYMBOL,
    INNUMBER,
    NOTOKEN
} lexstate = NOTOKEN;

void mixlex_reset(void) {
    lex_column = 0;
    getnextchar = mixlex_default_getnextchar;
    ungetchar = mixlex_default_ungetchar;
    lexstate = NOTOKEN;
}

    

int yylex (void) {
    int ch;
    char *buf = &tokenbuffer[0];
    
    while ((ch = (getnextchar)()) != EOF) {
        lex_column++;
        if (lex_column == 1 && ch == '*') {
            /* this is a comment line */
            do {
                ch = (getnextchar)();
                if (ch == -1) {
                    return -1;
                }
            } while (ch != '\n');
            lex_column = 0;
            return MIXAL_COMMENT;
        }
        switch (lexstate) {
            case NOTOKEN:
                switch (ch) {
                    case ' ':
                    case '\t':
                        lexstate = INSPACE;
                        break;
                    case '\n':
                        lex_column = 0;
                        return ch;
                        break;
                    default:
                        if (isupper(ch)) {
                            lexstate = INSYMBOL;
                            *buf = ch;
                            buf++;
                        } else if (isdigit(ch)) {
                            lexstate = INNUMBER;
                            *buf = ch;
                            buf++;
                        } else {
                            return -1;
                        }
                }
                break;
            case INSPACE:
                switch (ch) {
                    case ' ':
                    case '\t':
                        break;
                    default:
                        (ungetchar)(ch);
                        lex_column--;
                        lexstate = NOTOKEN;
                        return MIXAL_WHITESPACE;
                        break;
                }
                break;
            case INSYMBOL:
                if (isupper(ch) || isdigit(ch)) {
                    *buf = ch;
                    buf++;
                } else {
                    *buf = '\0';
                    buf = &tokenbuffer[0];
                    lexstate = NOTOKEN;
                    (ungetchar)(ch);
                    lex_column--;
                    yylval.symbol = strdup(buf);
                    return MIXAL_SYMBOL;
                }
                break;
            case INNUMBER:
                if (isupper(ch)) {
                    *buf = ch;
                    buf++;
                    lexstate = INSYMBOL;
                } else if (isdigit(ch)) {
                    *buf = ch;
                    buf++;
                } else {
                    *buf = '\0';
                    buf = &tokenbuffer[0];
                    lexstate = NOTOKEN;
                    (ungetchar)(ch);
                    lex_column--;
                    yylval.val = atoi(buf);
                    return MIXAL_NUMBER;                    
                }

                break;

        }
    }
    return 0;
}
