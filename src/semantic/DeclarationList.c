#include	"DeclarationList.h"
#include	"Declaration.h"
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

DeclarationList *
add_decl( DeclarationList * dl, Declaration * declaration )
{
	int			size;

	if ( dl == 0 )
	{
		size = sizeof( DeclarationList ) + sizeof( Declaration );
		if ( (dl = malloc( size )) == 0 )
		{
			return 0;
		}
		else
		{
			dl->list[0] = *declaration;
			dl->count = 1;
//			printDeclarationList( dl, 0 );
			return dl;
		}
	}
	else
	{
		size = sizeof( DeclarationList ) + (dl->count + 1) * sizeof( Declaration );
		if ( (dl = realloc( dl, size )) == 0 )
		{
			return 0;
		}
		else
		{
			dl->list[dl->count] = *declaration;
			dl->count += 1;
//			printDeclarationList( dl, 0 );
			return dl;
		}
	}
}

Declaration *
lookup_decl( DeclarationList * dl, const char * identifier )
{
	int			i;

	if ( dl == 0 )
	{
		return 0;
	}
	else
	{
		for ( i = 0 ; i < dl->count ; i++ )
		{
			if ( strcmp( identifier, get_identifier( &dl->list[i] ) ) == 0 )
			{
				return dl->list + i;
			}
		}
		return 0;
	}
}

DeclarationList *
destroy_decl_list( DeclarationList * dl )
{
	int			i;

	if ( dl == 0 )
	{
		return 0;
	}
	else
	{
		for ( i = 0 ; i < dl->count ; i++ )
		{
			free_identifier( &dl->list[i] );
		}
		free( dl );
		return 0;
	}
}

static void
indent( int lim )
{
	int			i;

	for ( i = 0 ; i < lim ; i++ )
		fprintf( stderr, "\t" );
}

void
printDeclarationList( DeclarationList * dl, int ind )
{
	int			i;

	if ( dl == 0 )
	{
		indent( ind );	fprintf( stderr, "DeclarationList %lx is empty\n", (unsigned long)dl );
	}
	else
	{
		fprintf( stderr, "Start DeclarationList count is %d\n", dl->count );
		for ( i = 0 ; i < dl->count ; i++ )
		{
//			fprintf( stderr, " %s", get_identifier( dl->list+i ) );
			printDeclaration( dl->list+i, ind + 1 );
		}
		fprintf( stderr, "End DeclarationList count is %d\n", dl->count );
	}
}
