#include	"TypeInfo.h"
#include	"Declaration.h"
#include	"symbol_table.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

static int	byte_size[] = { 0, 0, sizeof(int), sizeof(char), sizeof(float) };

static int
get_memory_size( TypeInfo * t )
{
	int			i;
	int			memory_size;

	memory_size = t->basic_size;
	for( i = 0 ; i < t->modifier_count ; i += 1 )
	{
		if ( t->modifier[i].tag == mt_pointer )
		{
			memory_size = sizeof( void * );
		}
		else if ( t->modifier[i].tag == mt_array )
		{
			memory_size *= t->modifier[i].array_bound;
		}
		else
		{
			printf( "internal error %s line %d\n", __FILE__, __LINE__ );
		}
	}
	return memory_size;
}

static void
set_array_modifiers( TypeInfo * t )
{
	int			i;
	int			multiplier;

	multiplier = t->basic_size;
	for( i = t->modifier_count - 1 ; i >= 0 ; i -= 1 )
	{
		if ( t->modifier[i].tag == mt_array )
		{
			t->modifier[i].multiplier = multiplier;
			multiplier *= t->modifier[i].array_bound;
		}
		else if ( t->modifier[i].tag == mt_pointer )
		{
			multiplier = sizeof( void * );
		}
		else
		{
			printf( "internal error weird tag %d %s line %d\n", t->modifier[i].tag, __FILE__, __LINE__ );
		}
	}
}

TypeInfo *
make_builtin_type( ScalarType scalar_type, Type * btype )
{
	TypeInfo *		t;

	if ( (t = malloc( sizeof( TypeInfo ) )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31minitialize_declaration() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else
	{
		t->tag = tag_scalar;
		t->scalar_type = scalar_type;
		t->struct_type = 0;
		t->basic_size = byte_size[scalar_type];
		t->memory_size = byte_size[scalar_type];
		t->modifier_count = 0;
		btype->typeinfo = t;
		return t;
	}
}

TypeInfo *
make_struct_type( const char * identifier, Type * stype )
{
	TypeInfo *		t;
	Declaration *		declaration;

	if ( (declaration = lookup_struct( identifier )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31mAttempt to make TypeInfo for nonexistent struct type.  file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else if ( declaration->tag != dt_struct )
	{
		fprintf( stderr, "\x1b[2;31mAttempt to make TypeInfo for somehtng that is not a struct.  file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else if ( (t = malloc( sizeof( TypeInfo ) )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31mmalloc() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else
	{
		t->tag = tag_struct;
		t->scalar_type = type_none;
		t->struct_type = declaration;
		t->basic_size = declaration->strct.memory_size;
		t->memory_size = declaration->strct.memory_size;
		t->modifier_count = 0;
		stype->typeinfo = t;
		return t;
	}
}

TypeInfo *
add_array( Type * atype, unsigned int bound )
{
	int			sub;
	TypeInfo *		t = atype->typeinfo;

	if ( (t = realloc( t, sizeof( TypeInfo ) + (t->modifier_count + 1) * sizeof(Modifier) )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31mrealloc() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else
	{
		sub = t->modifier_count;
		t->modifier[sub].tag = mt_array;
		t->modifier[sub].array_bound = bound;
		t->modifier[sub].multiplier = t->basic_size;	// BKR
		t->modifier_count += 1;
		atype->typeinfo = t;
		return t;
	}
}

TypeInfo *
add_pointer( Type * atype )
{
	int			sub;
	TypeInfo *		t = atype->typeinfo;

	if ( (t = realloc( t, sizeof( TypeInfo ) + (t->modifier_count + 1) * sizeof(Modifier) )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31mrealloc() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else
	{
		sub = t->modifier_count;
		t->modifier[sub].tag = mt_pointer;
		t->modifier[sub].array_bound = 0;
		t->modifier[sub].multiplier = sizeof( void * );
		t->modifier_count += 1;
		atype->typeinfo = t;
		return t;
	}
}

TypeInfo *
end_type( TypeInfo * t )
{
	t->memory_size = get_memory_size( t );
	set_array_modifiers( t );
	return t;
}

TypeInfo *
remove_array( TypeInfo * t )			// Remove leftmost modifier
{
	TypeInfo *		tnew;
	int			size = sizeof( TypeInfo ) + (t->modifier_count * sizeof( Modifier ));
	int			i, limit;

	if ( (tnew = malloc( size )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31minitialize_declaration() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else if ( t->modifier_count == 0 )	// Don't change anything
	{
		memcpy( tnew, t, size );
		return tnew;
	}
	else
	{
		memcpy( tnew, t, size );
		limit = tnew->modifier_count - 1;
		for ( i = 0 ; i < limit ; i++ )
		{
			tnew->modifier[i] = tnew->modifier[i+1];
		}
		tnew->modifier_count -= 1;
		return end_type( tnew );
	}
}

TypeInfo *
remove_pointer( TypeInfo * t )			// Remove rightmost modifier
{
	TypeInfo *		tnew;
	int			size = sizeof( TypeInfo ) + (t->modifier_count * sizeof( Modifier ));

	if ( (tnew = malloc( size )) == 0 )
	{
		fprintf( stderr, "\x1b[2;31minitialize_declaration() failure file %s line %d.\x1b[0m\n",
		 	__FILE__, __LINE__ );
		return 0;
	}
	else if ( t->modifier_count == 0 )	// Don't change anything
	{
		memcpy( tnew, t, size );
		return tnew;
	}
	else
	{
		memcpy( tnew, t, size );
		tnew->modifier_count -= 1;
		return end_type( tnew );
	}
}

static void
print_modifier_list( const TypeInfo * t )
{
	int		i, limit;

	limit = t->modifier_count;
	for ( i = 0 ; i < limit ; i++ )
	{
		switch ( t->modifier[i].tag )
		{
			case mt_array:
				printf( "[%d] multiplier %d", t->modifier[i].array_bound, t->modifier[i].multiplier );
				break;
			case mt_pointer:
				printf( "*" );
				break;
			default:
				printf( "unknown modifier tag file %s line %d\n", __FILE__, __LINE__ );
				break;
		}
	}
}

static char *	scalar_name[] = { "NONE", "void", "int", "char", "float" };

void
print_typeinfo( const TypeInfo * t, int ind )
{
	switch( t->tag )
	{
		case tag_scalar:
			printf( "Type: %s ", scalar_name[t->scalar_type] );
			print_modifier_list( t );
			break;
		case tag_struct:
			printf( "BKR struct ype info not implemented yet.\n" );
			break;
		default:
			printf( "unknown modifier tag file %s line %d\n", __FILE__, __LINE__ );
			break;
	}
}

int
get_size( const TypeInfo * t )
{
	return t->memory_size;
}
