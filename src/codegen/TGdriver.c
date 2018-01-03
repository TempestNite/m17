#include	<stdio.h>
#include	<stdlib.h>
#include	"TempMgr.h"

// Test driver for temp_generator

int
main()
{
	TempMgr *	tempMgr;
	int		offset;
	char		location[40];

	tempMgr = malloc( sizeof( TempMgr ) );
	setupTempMgr( tempMgr, 24000 );
	offset = push_temp( tempMgr, 20, location );
	printf( "offset is %d\n", offset );
	offset = push_temp( tempMgr, 20, location );
	printf( "offset is %d\n", offset );
	offset = push_temp( tempMgr, 20, location );
	printf( "offset is %d\n", offset );
	offset = push_temp( tempMgr, 20, location );
	printf( "offset is %d\n", offset );

	offset = pop_temp( tempMgr );
	printf( "offset after pop is %d\n", offset );
	offset = pop_temp( tempMgr );
	printf( "offset after pop is %d\n", offset );
	offset = pop_temp( tempMgr );
	printf( "offset after pop is %d\n", offset );
	offset = pop_temp( tempMgr );
	printf( "offset after pop is %d\n", offset );
	offset = pop_temp( tempMgr );
	printf( "offset after pop is %d\n", offset );


//	offset = push_temp( tempMgr, 10 );
//	printf( "offset is %d\n", offset );
	destroyTempMgr( tempMgr );
	return 0;
}
