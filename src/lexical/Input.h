#ifndef Input_h
#define Input_h

// Buffered input mechanism to provide unbounded input one character at a time.  The readChar() function
// returns an int value:  -1 means end-of-file and any other value has to be cast to a char to be useful.

typedef struct Input {
	int	fd;
	int	length;
	int	used;
	char	buffer[2048];
} Input;

int	setupInput( Input *, int );
int	readChar( Input * );
void	closeInput( Input * );

#endif
