/*
 *  mixlex.c
 *  MIXsim
 *
 *  Created by David Paschich on 5/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <ctype.h>

#include "mixlex.h"

#include "y.tab.h"

static FILE *inputfile;
static int mixlex_default_getnextchar(void) {
    return fgetc(inputfile);
}

static int lex_column = 0; /* column of character just read */
static int (*getnextchar)(void) = mixlex_default_getnextchar;
static char tokenbuffer[11];

void mixlex_input(FILE *f) {
    inputfile = f;
}
void mixlex_set_getchar(int (*nextchar)(void)) {
    getnextchar = nextchar;
}
char *mixlex_get_token() {
    return &tokenbuffer[0];
}

static enum lexstate {
    LINESTART,
    LOC_INTOKEN,
    LOC_AFTERTOKEN,
    OPCODE_INTOKEN,
    OPCODE_AFTERTOKEN
} lexstate = LINESTART;

void mixlex_reset(void) {
    lex_column = 0;
    getnextchar = mixlex_default_getnextchar;
    lexstate = LINESTART;
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
            return COMMENT;
        }
        if (lex_column == 11) {
            switch (ch) {
                case '\n':
                    lex_column = 0;
                    lexstate = LINESTART;
                    /* fallthrough */
                case ' ':
                    if (lexstate == LOC_INTOKEN) {
                        *buf = '\0';
                        buf = &tokenbuffer[0];
                        return SYMBOL;
                    }
                    lexstate = OPCODE_INTOKEN;
                    continue;
                    break;
                default:
                    return -1;
                    break;
            }
        }
        switch (lexstate) {
            case LINESTART:
                switch (ch) {
                    case ' ':
                        lexstate = LOC_AFTERTOKEN;
                        break;
                    case '\n':
                        lexstate = LINESTART;
                        lex_column = 0;
                    default:
                        if (isupper(ch) || isdigit(ch)) {
                            *buf = ch;
                            buf++;
                        } else {
                            return -1;
                        }
                        break;
                }
                break;
            case LOC_INTOKEN:
                switch (ch) {
                    case '\n':
                        lexstate = LINESTART;
                        lex_column = 0;
                        *buf = '\0';
                        buf = &tokenbuffer[0];
                        return SYMBOL;
                    case ' ':
                        lexstate = LOC_AFTERTOKEN;
                        *buf = '\0';
                        buf = &tokenbuffer[0];
                        return SYMBOL;                        
                    default:
                        if (isupper(ch) || isdigit(ch)) {
                            *buf = ch;
                            buf++;
                        }
                        break;
                }
            case OPCODE_INTOKEN:
                    switch (ch) {
                        case '\n':
                            lexstate = LINESTART;
                            lex_column = 0;
                            *buf = '\0';
                            buf = &tokenbuffer[0];
                            return OPCODE;
                            break;
                        case ' ':
                            lexstate = OPCODE_AFTERTOKEN;
                            *buf = '\0';
                            buf = &tokenbuffer[0];
                            return OPCODE;
                            break;                            
                        default:
                            if (isupper(ch) || isdigit(ch)) {
                                *buf = ch;
                                buf++;
                            }
                            break;
                    }
                    break;

            default:
                if (ch == '\n') {
                    lexstate = LINESTART;
                    lex_column = 0;
                }
                break;
        }
    }
    return 0;
}
