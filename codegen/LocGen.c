#include	"LocGen.h"
#include	<stdio.h>

int
setupLocGen( LocGen * this, OGtype ogtype )
{
	switch( ogtype )
	{
		case ot_auto:
			this->ogtype = ogtype;
			this->space_used = sizeof( void * );;
			this->multiplier = -1;
			break;
		case ot_parm:
			this->ogtype = ogtype;
			this->space_used = 2 * sizeof( void * );;
			this->multiplier = 1;
			break;
		case ot_member:
			this->ogtype = ogtype;
			this->space_used = 0;
			this->multiplier = 1;
			break;
		default:
			fprintf( stderr, "\x1b[2;31mBogus ogtype %d file %s line %d.\x1b[0m", this->ogtype, __FILE__, __LINE__ );
	}
	return 1;
}

static int
get_offset( LocGen * this, int size )
{
        this->space_used += size;
        return this->space_used * this->multiplier;
}

int
get_location( LocGen * this, int size, char * location )
{
	int		offset;

	switch( this->ogtype )
	{
		case ot_auto:
			offset = get_offset( this, size );
			sprintf( location, "%d(%%rbp)", offset );
			break;
		case ot_parm:
			offset = get_offset( this, size );
			sprintf( location, "%d(%%rbp)", offset );
			break;
		case ot_member:
			offset = get_offset( this, size );
			sprintf( location, "%d", offset );
			break;
		default:
			fprintf( stderr, "\x1b[2;31Bogus ogtype %d file %s line %d.\x1b[0m", this->ogtype, __FILE__, __LINE__ );
	}
	return 1;
}

int
get_space_used( LocGen * this )
{
	return this->space_used;
}

void
restore_space_used( LocGen * this, int oldspace )
{
	this->space_used = oldspace;
}
