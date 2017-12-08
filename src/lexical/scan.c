#include "state.h"
#include "Token.h"
#include "Input.h"

int main (int argc, char *argv[])
{
    token = malloc ( sizeof(*token) );
    token -> lineNumber = 1;
    // printf("\nTOKEN START LINE: %d\n\n\n", token->lineNumber);
    // token -> tokLen = 0;
    int state = 0, cic;

    // should sanitize args
    int fd = open(argv[1], O_RDONLY);
    setupScanner(fd);

    // reading one char until EOF
    while ( ( cic = readChar (&input) ) != -1)
    {
        // printf("TOKENLIN: %d\n", token->lineNumber);
        ch = (char) cic;
        // printf("ch %c\tint %d\n", ch, ch);
        // token -> tokLen = 0;
        getToken (token);
        if ((int) ch != -1) printToken (token);
    }

    free (token);

    return 0;
}