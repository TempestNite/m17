#ifndef Token_h
#define Token_h
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

typedef struct Token
{
    char *lexeme;
    int id;
    int lineNumber;
    int tokLen;
} Token;

void addLex (Token *, char);
void printToken (Token *);
int getToken (Token *);
void printErr (Token *, char );
void clearToken (Token *);
#endif
