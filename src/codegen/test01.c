#include	<stdio.h>

int
main()
{
	int		offset;
	char		base[20];
	char		location[20] = "-40(%rbp)";

	sscanf( location, "%d%s", &offset, base );
	printf( "offset is %d base is %s\n", offset, base );
	offset += 10;
	sprintf( location, "%d%s", offset, base );
	printf( "%s\n", location );
	return 0;
}
