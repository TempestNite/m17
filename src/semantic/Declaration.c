#include	"Declaration.h"
#include	"DeclarationList.h"
#include	<stdlib.h>
#include	<stdio.h>

char *
get_identifier( const Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_variable:	return decl->variable.identifier;
		case dt_parameter:	return decl->parameter.identifier;
		case dt_struct:		return decl->strct.identifier;
		case dt_member:		return decl->member.identifier;
		case dt_function:	return decl->function.identifier;
		case dt_scope:		return "";
		default:		return "UNKNOWN TAG";
	}
}

void
free_identifier( Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_variable:	free( decl->variable.identifier );	break;
		case dt_parameter:	free( decl->parameter.identifier );	break;
		case dt_struct:		free( decl->strct.identifier );		break;
		case dt_member:		free( decl->member.identifier );	break;
		case dt_function:	free( decl->function.identifier );	break;
		case dt_scope:
		default:		return;
	}
}

struct DeclarationList *
get_decl_list( const Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_struct:		return decl->strct.member_list;
		case dt_function:	return decl->function.param_list;
		case dt_scope:		return decl->scope.decl_list;
		default:		return 0;
	}
}

Declaration *
get_enclosing( const Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_struct:		return decl->strct.enclosing;
		case dt_function:	return decl->function.enclosing;
		case dt_scope:		return decl->scope.enclosing;
		default:		return 0;
	}
}

static void
indent( int lim )
{
	int		i;

	for ( i = 0 ; i < lim ; i++ )
		printf( "\t" );
}

void
printDeclaration( const Declaration * decl, int ind )
{
	switch ( decl->tag )
	{
		case dt_variable:
			indent( ind );	printf( "variable:  %s ", decl->variable.identifier );
			indent( ind );	print_typeinfo( decl->variable.type, ind + 1 );
			indent( ind );	printf( "location:  %s ", decl->variable.location );
			printf( "\n" );
			break;
		case dt_parameter:
			indent( ind );	printf( "parameter:  %s ", decl->parameter.identifier );
			indent( ind );	print_typeinfo( decl->parameter.type, ind + 1 );
			indent( ind );	printf( "location:  %s ", decl->parameter.location );
			printf( "\n" );
			break;
		case dt_struct:
			indent( ind );	printf( "struct:  %s ", decl->strct.identifier );
			printDeclarationList( decl->strct.member_list, ind + 1 );
			indent( ind );	printf( "memory_size:  %d ", decl->strct.memory_size );
			printf( "\n" );
			break;
		case dt_member:
			indent( ind );	printf( "member:  %s ", decl->member.identifier );
			indent( ind );	print_typeinfo( decl->member.type, ind + 1 );
			indent( ind );	printf( "location:  %s ", decl->member.location );
			printf( "\n" );
			break;
		case dt_function:	
			indent( ind );	printf( "function:  %s ", decl->function.identifier );
			print_typeinfo( decl->function.return_type, ind + 1 );
			printf( "\n" );
			printDeclarationList( decl->function.param_list, ind );
			break;
		case dt_scope:
			indent( ind );	printf( "SCOPE " );
			printDeclarationList( decl->scope.decl_list, ind + 1 );
			printf( "\n" );
			break;
		default:
			printf( "UNKNOWN TAG file %s line %d\n", __FILE__, __LINE__ );
			break;
	}
}

TypeInfo *
get_type_info( const Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_variable:	return decl->variable.type;
		case dt_parameter:	return decl->parameter.type;
		case dt_member:		return decl->member.type;
		case dt_function:	return decl->function.return_type;
		case dt_struct:		return 0;
		case dt_scope:		return 0;
		default:		printf( "UNKNOWN TAG file %s line %d\n", __FILE__, __LINE__ );	return 0;
	}
}

char *
get_decl_location( Declaration * decl )
{
	switch ( decl->tag )
	{
		case dt_variable:	return decl->variable.location;
		case dt_parameter:	return decl->parameter.location;
		case dt_member:		return decl->member.location;
		case dt_function:	return decl->function.identifier;
		case dt_struct:		return 0;
		case dt_scope:		return 0;
		default:		printf( "UNKNOWN TAG file %s line %d\n", __FILE__, __LINE__ );	return 0;
	}
}
