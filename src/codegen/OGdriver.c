#include	<stdio.h>
#include	<stdlib.h>
#include	"LocGen.h"

// Test driver for LocGen

int
main()
{
	LocGen *	og;
	char		location[40];

	og = malloc( sizeof( LocGen ) );
	setupLocGen( og, ot_auto );
	get_location( og, 20, 4, location );
	printf( "location is %s\n", location );
	get_location( og, 4, 4,  location );
	printf( "location is %s\n", location );
	get_location( og, 4, 4, location );
	printf( "location is %s\n", location );
	return 0;
}
