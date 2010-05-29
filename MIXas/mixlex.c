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

static int mixlex_file_getnextchar(void) {
    return fgetc(inputfile);
}

static int mixlex_file_ungetchar(int ch) {
    return ungetc(ch, inputfile);
}

static int cur = 0;
static char const *inputstring;
static char ungetbuffer[10];
static int ungetcur = 0;

static int string_nextchar(void) {
    if (ungetcur > 0) {
        return ungetbuffer[--ungetcur];
    }
    if (inputstring[cur] == '\0') {
        return -1;
    }
    return inputstring[cur++];
}

static int string_ungetchar(int ch) {
    if (ungetcur == 10) {
        return -1;
    }
    ungetbuffer[ungetcur] = ch;
    ungetcur++;
    return ch;
}

static int lex_column = 0; /* column of character just read */
static int (*getnextchar)(void);
static int (*ungetchar)(int);
static char tokenbuffer[11];

void mixlex_input(FILE *f) {
    inputfile = f;
    getnextchar = mixlex_file_getnextchar;
    ungetchar = mixlex_file_ungetchar;
}

void mixlex_input_string(const char *s) {
    inputstring = s;
    cur = 0;
    ungetcur = 0;
    getnextchar = string_nextchar;
    ungetchar = string_ungetchar;
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
    inputfile = NULL;
    inputstring = NULL;
    getnextchar = NULL;
    ungetchar = NULL;
    lexstate = NOTOKEN;
}

    

int yylex (void) {
    int ch;
    char *buf = &tokenbuffer[0];
    
    while (1) {
        ch = (getnextchar)();
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
                    case -1:
                        return 0;
                        break;
                    case ' ':
                    case '\t':
                        lexstate = INSPACE;
                        break;
                    case '\n':
                        lex_column = 0;
                        /* fallthrough */
                    case '-':
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
