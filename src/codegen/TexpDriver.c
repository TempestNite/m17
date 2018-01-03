#include	<stdio.h>
#include	<stdlib.h>
#include	"TempMgr.h"

// Test driver for TempMgr

int
main()
{
	TempMgr *	tg;
	int		offset;

	tg = malloc( sizeof( TempMgr ) );
	setupTempMgr( tg, 16 );
	offset = push_temp( tg, 20 );
	printf( "offset is %d\n", offset );
	offset = pop_temp( tg );
	printf( "offset after pop is %d\n", offset );
	offset = push_temp( tg, 10 );
	printf( "offset is %d\n", offset );
	return 0;
}
