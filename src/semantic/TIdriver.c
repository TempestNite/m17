#include	<stdio.h>
#include	<stdlib.h>
#include	"TypeInfo.h"

void
print_modifier_list( TypeInfo * t )
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
	printf( "\n" );
}

static char *	scalar_name[] = { "NONE", "void", "int", "char", "float" };

void
print_type_info( TypeInfo * t )
{
	switch( t->tag )
	{
		case tag_scalar:
			printf( "%s ", scalar_name[t->scalar_type] );
			print_modifier_list( t );
			printf( "memory_size %d\n", t->memory_size );
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
main()
{
	TypeInfo *	type_info;
	TypeInfo *	t2;
	Type		type;

	type_info = make_builtin_type( type_int, &type );
	print_type_info( type_info );
	add_array( &type, 10 );
	add_array( &type, 20 );
	add_array( &type, 30 );
	add_pointer( &type );
	end_type( type_info );
	print_type_info( type_info );
	t2 = remove_array( type_info );
	print_type_info( t2 );
	t2 = remove_pointer( t2 );
	print_type_info( t2 );
	free( type_info );
	return 0;
}
