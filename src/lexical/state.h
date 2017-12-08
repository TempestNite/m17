#ifndef state_h
#define state_h

#include "Input.h"
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Token.h"

#define K_LEN       16
#define MIN_KTOK    10
#define MAX_STRLEN  256


/* 0 identifiers
** type 1-6 for alphanum
** 7 for strings
** 10-25 keywords
** 30-54 symbols
*/

enum type { ID_M, alpha, deci, oct, hexa, bi, flt, STRING_M };
enum symbol { lparenth=30, rparenth, lsbrace, rsbrace, lcbrace, rcbrace,
                semicolon, period, comma, and, or, equal, not,
                notequal, gt, lt, ge, le, plus, minus, ast, slash,
                percent, assgn, bang, bitand, bitor, bitnot };

enum reserved { CHAR_M=10, INT_M, FLOAT_M, STRUCT_M, VOID_M, WHILE_M, DO_M,
                    FOR_M, IF_M, ELSE_M, RETURN_M, READ_M, WRITE_M, ALLOC_M,
                    DEALLOC_M, FUNC_M };

Token* token;
Input input;
char ch;

/*****************************************************************************/
/* Prototypes   														      /
/*****************************************************************************/

int setupScanner(int fd);
int getToken(Token*);
#endif