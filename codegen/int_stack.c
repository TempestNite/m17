#include	"int_stack.h"
#include	<stdio.h>
#include	<stdlib.h>

#define MINGROWTH	sizeof( int )

void
setup_int_stack( int_stack * this )
{
	this->ptr = 0;
	this->sub = 0;
	this->memsize = 0;
	this->space_used = 0;
}

void
push_int( int_stack * this, int x )
{
	if ( (this->memsize - this->space_used) >= sizeof( int ) )
	{
		this->ptr[this->sub] = x;
		this->sub += 1;
		this->space_used += sizeof( int );
	}
	else if ( (this->ptr = realloc( this->ptr, this->memsize + MINGROWTH )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31realloc() failed in file %s line %d.\x1b[0m", __FILE__, __LINE__ );
		this->space_used = 0;
		this->memsize = 0;
	}
	else
	{
		this->memsize += MINGROWTH;
		this->ptr[this->sub] = x;
		this->sub += 1;
		this->space_used += sizeof( int );
	}
}

int
pop_int( int_stack * this )
{
	int		top;

	if ( this->sub == 0 )
	{
		return -1;
	}
	else
	{
		this->sub -= 1;
		top = this->ptr[this->sub];
		return top;
	}
}

void
destroy_int_stack( int_stack * this )
{
	if ( this->ptr != 0 ) free( this->ptr );
}
