/*****************************************************************************/
/* CS521 Compiler Construction											     */
/* PA1: Lexical Analyzer												     */
/* Author: Shane Dasta													     */
/*****************************************************************************/
#include "state.h"

/*****************************************************************************/
/* Program         														      /
/*****************************************************************************/

char* keywords [16] = {"char", "int", "float", "struct", "void",
                            "while", "do", "for", "if", "else",
                            "return", "read", "write", "allocate",
                            "deallocate", "func" };



int advance (char cic)
{
    cic = readChar (&input);
    int eof = (int) cic;
    if (eof == -1) token->lineNumber--;
    return cic;
}

int state0 ()
{
    if ( isspace(ch) )
    {
        // printf("TOKENLIN: %d\n", token->lineNumber);
        if ( ch == '\n' ) ++(token -> lineNumber);
        // printf("TOKENLIN: %d\n", token->lineNumber);
        ch = (char) advance(ch);
        // printf("ch: %c", ch);
        return 0;
    }

    else if (ch == '/') return 20;
    else if ( isalpha(ch) ) return 1;
    else if (ch == '0') return 2;
    else if ( isdigit(ch) ) return 11;
    else if (ch == '\"') { addLex(token, ch); return 17; }
    else if (ch == '{') { token -> id = lcbrace; addLex(token, ch); return 99; }
    else if (ch == '}') { token -> id = rcbrace; addLex(token, ch); return 99; }
    else if (ch == '(') { token -> id = lparenth; addLex(token, ch); return 99; }
    else if (ch == ')') { token -> id = rparenth; addLex(token, ch); return 99; }
    else if (ch == '[') { token -> id = lsbrace; addLex(token, ch); return 99; }
    else if (ch == ']') { token -> id = rsbrace; addLex(token, ch); return 99; }
    else if (ch == ';') { token -> id = semicolon; addLex(token, ch); return 99; }
    else if (ch == '.') { token -> id = period; addLex(token, ch); return 99; }
    else if (ch == ',') { token -> id = comma; addLex(token, ch);return 99; }
    else if (ch == '+') { token -> id = plus; addLex(token, ch); return 99; }
    else if (ch == '-') { token -> id = minus; addLex(token, ch); return 99; }
    else if (ch == '*') { token -> id = ast; addLex(token, ch); return 99; }
    else if (ch == '%') { token -> id = percent; addLex(token, ch); return 99; }
    else if (ch == '~') { token -> id = bitnot; addLex(token, ch); return 99; }
    else if (ch == '!') { token -> id = bang; addLex(token, ch); return 99; }
    else if (ch == '=') return 24;
    else if (ch == '>') return 25;
    else if (ch == '<') return 26;
    else if (ch == '!') return 27;
    else if (ch == '&') return 28;
    else if (ch == '|') return 29;
    else { return 9; }
}

// word state
int state1 ()
{
    // printf("HERE2\n");
    token -> id = 0;
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( isalnum(ch) )
    {
        return 1;
    }

     else return 99;
}

// Non-digit state
int state2 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( ch == '.' )
    {
        token -> id = flt;
        return 12;
    }

    else if (ch == 'x' || ch == 'X')
    {
        token -> id = hexa;
        return 3;
    }

    else if ( isdigit(ch) )
    {
        token -> id = oct;
        return 5;
    }

    else if (ch == 'b' || ch == 'B')
    {
        token -> id = bi;
        return 7;
    }

    else { token -> id = deci; return 99; }
}

int isHex(char ch)
{
    int asc = (int) ch;
    if ( isdigit(ch) || (asc > 64 && asc < 71) || (asc > 96 && asc < 103) )
    {
        return 1;
    }

    return 0;
}

int isOct(char ch)
{
    int asc = (int) ch;
    if ( asc > 47 && asc < 56 )
    {
        return 1;
    }

    return 0;
}

// hex x state; add the x
// check if next is hex
// move forward or complain
int state3 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( isHex(ch) ) return 4;
    else return 9;
}

// in a hex char state
// return to state if next still hex
int state4 ()
{
    addLex(token, ch);
    // printf("\nTOKEN LINE: %d\n\n\n", token->lineNumber);

    ch = (char) advance(ch);

    if ( isHex(ch) ) return 4;

    else if ( isalpha(ch) )
    {
        printErr (token, ch);
        // exit (4);
        return 9;
    }
    else return 99;
}

// oct char state, add the char
// check if next is oct char
// move forward or complain
int state5 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( isOct(ch) ) return 6;
    else { printErr(token, ch); return 9; }
}

// in a oct char state
// return to state if next still oct
int state6 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( isOct(ch) ) return 6;
    else if ( isalpha(ch) )
    {
        printErr (token, ch);
        return 9;
    }
    else return 99;
}

// bin b state; add the b
// check if next is bin
// move forward or complain
int state7 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( ch == '0' || ch == '1' ) return 8;
    else { printErr(token, ch); return 9; }
}

// in a bin char state
// return to state if next still bin
int state8 ()
{
    addLex(token, ch);

    ch = (char) advance(ch);

    if ( ch == '0' || ch == '1' ) return 8;
    else if ( safeNeighbor() )
    {
        printErr (token, ch);
        return 9;
    }
    else return 99;
}

// error state
int state9 ()
{
    ch = (char) advance(ch);
    if ((int) ch  == -1) return -1;
    if (!isspace(ch)) return 9;
    else { return 0; }
}

// unlimited digit state
int state11 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( isdigit(ch) ) return 11;
    else if (ch == '.') return 12;
    else if ( safeNeighbor() ) { token -> id = deci; return 99; }
    else
    {
        printErr(token, ch);
        return 9;
    }
}

// decimal dot state
int state12 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( isdigit(ch) ) return 13;
    else
    {
        printErr(token, ch);
        return 9;
    }
}

// decimal place state
int state13 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( isdigit(ch) ) return 13;
    else if ( ch == 'e' || ch == 'E' ) return 14;
    else if ( safeNeighbor() ) { token -> id = flt; return 99; }
    else
    {
        printErr(token, ch);
        return 9;
    }
}

// floating point state + or -
int state14 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( ch == '+' || ch == '-' ) return 15;
    else
    {
        printErr(token, ch);
        return 9;
    }
}

int safeNeighbor ()
{
    int asc = (int) ch;
    if ( (asc > 32 && asc < 48) || (asc > 57 && asc < 63) || ( isspace(ch) ) 
            || asc == 93 )
        return 1;
    else return 0;
}

// post E+ e- state, check for integer
int state15 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( isdigit(ch) ) return 16;
    else
    {
        printErr(token, ch);
        return 9;
    }
}

int state16 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);
    if ( isdigit(ch) ) return 16;
    else if ( safeNeighbor() ) { token -> id = flt; return 99; }
    else
    {
        printErr(token, ch);
        return 9;
    }
}

int state17 ()
{
    ch = (char) advance(ch);
    addLex(token, ch);
    // printf("CHAR NEXT %c\t%d\t%c\t%d\n", ch, ch,
    //            input.buffer[input.used], input.buffer[input.used]);
    if (ch == '\n')
    {
        addLex(token, '\0');
        int lineLoc = token -> lineNumber;
        fprintf(stderr, KRED "COULD NOT FIND CLOSING QUOTE LINE %d: "
                    "%s\n", token-> lineNumber, token -> lexeme);
        fprintf(stderr, KNRM "");
        memset (token->lexeme, 0, token -> tokLen);

        token -> tokLen = 0;
        token -> lineNumber = lineLoc;
        input.used--;
        return 9;
    }

    if (ch == '\"')
    {
        token -> id = 7;
        addLex(token, '\0');
        return 99;
    }

    else if (ch == '\\')
    {
        return 18;
    }

    else return 17;
}

// escape seq
int state18 ()
{
    // char will be an alpha
    ch = (char) advance(ch);
    if (ch == 'a')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, '7');
        return 17;
    }
    else if (ch == 'b')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, '8');
        return 17;
    }
    else if (ch == 'f')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, 'C');
        return 17;
    }
    else if (ch == 'n')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, 'A');
        return 17;
    }
    else if (ch == 'r')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, 'D');
        return 17;
    }
    else if (ch == 't')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, '9');
        return 17;
    }
    else if (ch == 'v')
    {
        addLex(token, 'x');
        addLex(token, '0');
        addLex(token, 'B');
        return 17;
    }
    else if (ch == '\\')
    {
        addLex(token, 'x');
        addLex(token, '5');
        addLex(token, 'C');
        return 17;
    }
    else if (ch == '\'')
    {
        addLex(token, 'x');
        addLex(token, '2');
        addLex(token, '7');
        return 17;
    }
    else if (ch == '\"')
    {
        addLex(token, 'x');
        addLex(token, '2');
        addLex(token, '2');
        return 17;
    }
    else if (ch == '?')
    {
        addLex(token, 'x');
        addLex(token, '3');
        addLex(token, 'F');
        return 17;
    }
    else if (ch == 'x' || ch == 'X')
    {
        addLex(token, ch);
        ch = (char) advance(ch);
        return 19;
    }

    else
    {
        printErr(token, ch); return 9;
    }
}

// string hex digits
int state19 ()
{
    if ( isHex(ch) )
    {
        addLex(token, ch);
        
        if (isHex(input.buffer[input.used]) ) 
        {
            ch = (char) advance(ch);
            return 19;
        }
        else return 17;
    }

}

// Comment state
int state20 ()
{
    // addLex(token, ch);
    ch = (char) advance(ch);
    if (ch == '/')
    {
        // token -> id = 40;
        // printf("SINGLE\n");
        return 22;
    }
    
    else if (ch == '*') return 21;

    else
    {
        token -> id = slash;
        addLex(token, '/');
        input.used--;
        return 99;
    }
}

// second forward slash state
int state21 ()
{
    // printf("STATE21\n");
    ch = (char) advance(ch);
    if (ch == '*') return 23;
    else if (ch == '\n') { token->lineNumber++; }
    return 21;
}

// comment content state up until newline
int state22 ()
{
    // printf("/HERE \'%c\'\n", ch);
    ch = (char) advance(ch);
    // printf("/HERE %d\n", ch);
    if (ch == '\n')
    {
        // token -> lineNumber++;
        return 0;
    }
    else return 22;
}

int state23 ()
{
    ch = (char) advance(ch);
    if (ch == '/') { ch = (char) advance(ch); return 0; }
    else return 21;
}

int state24 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '=') { addLex(token, ch); token->id = assgn; return 99; }
    else { token->id = equal; input.used--; return 99; }
}

int state25 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '=') { addLex(token, ch); token->id = ge; return 99; }
    else { token->id = gt; input.used--; return 99; }
}

int state26 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '=') { addLex(token, ch); token->id = le; return 99; }
    else { token->id = lt; input.used--; return 99; }
}

int state27 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '=') { addLex(token, ch); token->id = notequal; return 99; }
    else { token->id = not; input.used--; return 99; }
}

int state28 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '&') { addLex(token, ch); token->id = and; return 99; }
    else if (isspace(ch) ) { token->id = bitand; return 99; }
    else { printErr(token, ch); return 9; }
}

int state29 ()
{
    addLex(token, ch);
    ch = (char) advance(ch);

    if (ch == '|') { addLex(token, ch); token->id = and; return 99; }
    else if (isspace(ch) ) { token->id = bitor; return 99; }
    else { printErr(token, ch); return 9; }
}

// absorb state
int state99 ()
{
    int i, cic;

    if (token -> id == 0)
    {
        for (i = 0; i < K_LEN; ++i)
            if ( strcmp(keywords[i], token -> lexeme) == 0 )
            {
                token -> id = i + MIN_KTOK;
                break;
            }
    }

    cic = (int) ch;
    // printf("PRECIC %c\t%d\n", ch, cic);
    if ( ( (token->id != 7) && (token->id < 30) ) || (token-> id == bitand) ||
                (token->id == bitor) ) {input.used--; }
    return -1;
}

int startcic()
{
    int cic;
    // printf("HERE\n");
    if ( ( cic = readChar (&input) ) != -1)
    {
        // printf("TOKENLIN: %d\n", token->lineNumber);
        ch = (char) cic;
        // printf("ch %c\tint %d\n", ch, ch);
    }
}

int setupScanner(int fd)
{
    setupInput (&input, fd);
    return 0;
}

int getToken (Token* token)
{
    
    // token -> lineNumber = 1;
    // printf("\nTOKEN START LINE: %d\n\n\n", token->lineNumber);
    token -> tokLen = 0;
    int state = 0;

    // ch = (char) readChar(&input);
    if (ch == -1) { token->id = -1; return -1; }

    do
    {
        switch (state)
        {
            case 0: state = state0(); break;
            case 1: state = state1(); break;
            case 2: state = state2(); break;
            case 3: state = state3(); break;
            case 4: state = state4(); break;
            case 5: state = state5(); break;
            case 6: state = state6(); break;
            case 7: state = state7(); break;
            case 8: state = state8(); break;
            case 9: state = state9(); break;

            case 11: state = state11(); break;
            case 12: state = state12(); break;
            case 13: state = state13(); break;
            case 14: state = state14(); break;
            case 15: state = state15(); break;
            case 16: state = state16(); break;
            case 17: state = state17(); break;
            case 18: state = state18(); break;
            case 19: state = state19(); break;
            case 20: state = state20(); break;
            case 21: state = state21(); break;
            case 22: state = state22(); break;
            case 23: state = state23(); break;
            case 24: state = state24(); break;
            case 25: state = state25(); break;
            case 26: state = state26(); break;
            case 27: state = state27(); break;
            case 28: state = state28(); break;
            case 29: state = state29(); break;

            case 99: state = state99(); break;

            default: printf("Something went wrong.\n"); exit(99);
        }
    } while (state != -1);

    // printToken(token);

    return state;
}


