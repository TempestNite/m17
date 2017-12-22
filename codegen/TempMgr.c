#include	"TempMgr.h"
#include	<stdio.h>

int
setupTempMgr( TempMgr * this, int autosize )
{
	this->offset = autosize;
	printf( "\x1b[31;36mBKR setupTempMgr() autosize is %d\n\x1b[0m", autosize );
	setup_int_stack( &this->stack );
	return 1;
}

int
push_temp( TempMgr * this, int size, char * location )
{
	int		temp;

	push_int( &this->stack, size );
	temp = - this->offset;
	this->offset += size;
	printf( "\x1b[31;36mBKR push_temp( %d ) returns temp offset %d\n\x1b[0m", size, temp );
	sprintf( location, "%d(%%rbp)", temp );
	return temp;
}

int
pop_temp( TempMgr * this )
{
	int		size;

	if ( (size = pop_int( &this->stack )) == -1 )
	{
		return -1;
	}
	else
	{
		this->offset -= size;
	printf( "\x1b[31;36mBKR pop_temp() popped size %d offset is now %d \n\x1b[0m", size, this->offset );
		return this->offset;
	}
}

void
destroyTempMgr( TempMgr * this )
{
	destroy_int_stack( & this->stack );
}
