#include	<stdio.h>
#include	"parser.h"
#include	<fcntl.h>
#include	"Result.h"
#include	<unistd.h>
#include	<stdlib.h>

int
main( int argc, char ** argv )
{
	int			fd;

	if ( argc < 2 )
	{
		fprintf( stderr, "\x1b[1;31mMust specify source file name on command line.\x1b[0m\n" );
		return 1;
	}
	else if ( (fd = open( argv[1], O_RDONLY )) == -1 )
	{
		fprintf( stderr, "\x1b[1;31mCould not open file %s .\x1b[0m\n", argv[1] );
		return 1;
	}
	else
	{
		setup_parser( fd, argv[1] );
//		while ( stmt( 0 ) )
		if ( program() )
		{
			printf( "parse successful\n" );
		}
		printf( "program parsing complete\n" );
		close( fd );
		return 0;
	}
}
