#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "Input.h"

void addLex (Token *token, char ch)
{
    token -> lexeme = (char *) realloc (token -> lexeme,
                                            sizeof(char) * (token->tokLen+1));
    *( (token -> lexeme) + token -> tokLen ) = ch;
    ++token -> tokLen;
}

void printToken (Token *token)
{
    if (strcmp (token -> lexeme, "") != 0 )
    {
        int lineLoc = token -> lineNumber;
        // printf ("Token ID: %d\n", token -> id);
        // printf ("Token Lexeme: \'%s\'\n\n", (token -> lexeme));
        // printf ("Token Lexeme: \'%s\'\t\t%d\n\n", (token -> lexeme), token->lineNumber);
        printf ("%d\t%-30s%10d\n", token->id, (token -> lexeme), token->lineNumber);
        // memset (token -> lexeme, 0x00, sizeof( *(token -> lexeme) ));
        memset (token, 0, sizeof(*token));

        free(token -> lexeme);
        token -> id = -1;
        token -> tokLen = 0;
        token -> lineNumber = lineLoc;
    }
}

void clearToken (Token *token)
{
    int lineLoc = token -> lineNumber;
    memset (token, 0, sizeof(*token));

    // free(token -> lexeme);

    token -> lineNumber = lineLoc;
    token -> tokLen = 0;
}

void printErr (Token *token, char ch)
{
    int lineLoc = token -> lineNumber;
    if (token->lexeme != 0)
    {
        fprintf(stderr, KRED
                    "Line %d: \'%c\' is an invalid character after \'%s\'\n\n",
                    token -> lineNumber, ch, token -> lexeme);
    }
    else fprintf(stderr, KRED "Line %d: \'%c\' is an invalid character\n\n",
                        token -> lineNumber, ch);
    fprintf(stderr, KNRM "");

    memset (token->lexeme, 0, token -> tokLen);

    token -> tokLen = 0;
    token -> lineNumber = lineLoc;
}
