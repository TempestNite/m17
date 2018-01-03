#include	"LocGen.h"
#include	<stdio.h>

int
setupLocGen( LocGen * this, OGtype ogtype )
{
	switch( ogtype )
	{
		case ot_auto:
			this->ogtype = ogtype;
			this->space_used = 0;
			this->alignment = 1;
			this->multiplier = -1;
			break;
		case ot_parm:
			this->ogtype = ogtype;
			this->space_used = 2 * sizeof( void * );
			this->alignment = 1;
			this->multiplier = 1;
			break;
		case ot_member:
			this->ogtype = ogtype;
			this->space_used = 0;
			this->alignment = 1;
			this->multiplier = 1;
			break;
		default:
			fprintf( stderr, "\x1b[2;31mBogus ogtype %d file %s line %d.\x1b[0m", this->ogtype, __FILE__, __LINE__ );
	}
	return 1;
}

int
get_location( LocGen * this, int size, int alignment, char * location )
{
	int		offset;

	if ( (alignment & (alignment - 1)) != 0 )
	{
		fprintf( stderr, "alignment arg is not a power of 2 file %s line %d\n", __FILE__, __LINE__ );
		return 0;
	}
	else switch( this->ogtype )
	{
		case ot_auto:
			offset = ((this->space_used + alignment - 1) & ~(alignment - 1)) + size;
			this->space_used = offset;
			sprintf( location, "%d(%%rbp)", -offset );
			break;
		case ot_parm:
			offset = ((this->space_used + alignment - 1) & ~(alignment - 1));
			this->space_used = offset + size;
			sprintf( location, "%d(%%rbp)", offset );
			break;
		case ot_member:
			offset = ((this->space_used + alignment - 1) & ~(alignment - 1));
			this->space_used = offset + size;
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

void
update_alignment( LocGen * locGen, int alignment )
{
	if ( alignment > locGen->alignment )
	{
		locGen->alignment = alignment;
	}
}
