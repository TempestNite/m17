#include	"Input.h"
#include	<unistd.h>

int
setupInput( Input * this, int fdesc )
{
	this->fd = fdesc;
	this->used = 0;
	this->length = 0;
}

void
closeInput( Input * this )
{
	close( this->fd );
}

int
readChar( Input * this )
{
	char		c;

	if ( this->used < this->length )
	{
		c = this->buffer[this->used++];
		return c;
	}
	else if ( (this->length = read( this->fd, this->buffer, sizeof(this->buffer) )) < 1 )
	{
		return -1;
	}
	else
	{
		c = this->buffer[0];
		this->used = 1;
		return c;
	}
}
