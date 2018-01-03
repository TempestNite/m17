#include	<stdio.h>

int
main( int argc, char ** argv )
{
	unsigned int		i, j;

	j = 119;
	i = j / 13;
	for ( i = 1 ; i < 100 ; i = i + 1 )
	{
		if ( i % 2 )
			printf( "%d %s", i, " is even number\n" );
		else if ( i % 3 )
			printf( "%d %s", i, " is odd multiple of three\n" );
		else if ( i % 5 )
			printf( "%d %s", i, " is odd multiple of five\n" );
	}
	return 0;
}
