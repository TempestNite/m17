#include	<stdarg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	"symbol_table.h"
#include	"DeclarationList.h"

typedef struct SymbolTable {
	Declaration *		nearest;		// LIFO list of nested scopes
	Declaration *		type_list;		// List of struct types
} SymbolTable;

static SymbolTable	symbol_table;

static void
internal_error( const char * fmt, ... )
{
	va_list		args;

	va_start( args, fmt );
	fprintf( stderr, "\x1b[2;31mInternal Error:  " );
	vfprintf( stderr, fmt, args );
	fprintf( stderr, "\x1b[0m" );
	va_end( args );
}

int
setup_symbol_table()
{
	symbol_table.nearest = 0;
	symbol_table.type_list = 0;
	return 1;
}

Declaration *
enter_scope()
{
	Declaration *		scope;

	printf( "\x1b[1;32mBKR enter_scope() invoked.\x1b[0m\n" );
	if ( (scope = malloc( sizeof(Declaration) )) == 0 )
	{
		internal_error( "malloc() failure file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
		scope->tag = dt_scope;
		scope->scope.decl_list = 0;
		scope->scope.enclosing = symbol_table.nearest;
		symbol_table.nearest = scope;
		printf( "BKR enter_scope() scope is %lx enclosing scope is %lx\n", (unsigned long)scope, (unsigned long)(scope->scope.enclosing) );
		print_scope_stack( __FILE__, __LINE__ );
		return scope;
	}
}

Declaration *
exit_scope()
{
	Declaration *		next;

	if ( symbol_table.nearest == 0 )
	{
		internal_error( "No scope to exit. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else if ( symbol_table.nearest->tag != dt_scope )
	{
		internal_error( "Attempt to exit something that is not a scope. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
		destroy_decl_list( symbol_table.nearest->scope.decl_list );
		next = symbol_table.nearest->scope.enclosing;
		free( symbol_table.nearest );
		symbol_table.nearest = next;
		printf( "\x1b[1;32mBKR exit_scope() file %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		print_scope_stack( __FILE__, __LINE__ );
		return next;
	}
}

Declaration *
enter_struct( Declaration * declaration )
{
	if ( declaration->tag != dt_struct )
	{
		internal_error( "Attempt to enter struct for something that is not a struct. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
		printf( "\x1b[1;32mBKR enter_struct() file %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		declaration->strct.enclosing = symbol_table.nearest;
		symbol_table.nearest = declaration;
		return declaration;
	}
}

Declaration *
exit_struct()
{
	Declaration *		declaration = symbol_table.nearest;

	if ( declaration->tag != dt_struct )
	{
		internal_error( "Attempt to exit struct for something that is not a struct. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
		symbol_table.nearest = symbol_table.nearest->strct.enclosing;
		printf( "\x1b[1;32mBKR exit_struct() file %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		return declaration;
	}
}

Declaration *
enter_function( Declaration * declaration )
{
	if ( declaration->tag != dt_function )
	{
		internal_error( "Attempt to enter function for something that is not a function. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
//		printf( "\x1b[1;32mBKR enter_function() file %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		declaration->function.enclosing = symbol_table.nearest;
		symbol_table.nearest = declaration;
		printf( "BKR enter_function() scope is %lx enclosing scope is %lx\n", (unsigned long)declaration, (unsigned long)(declaration->function.enclosing) );
		return declaration;
	}
}

int
exit_function()
{
	Declaration *		declaration = symbol_table.nearest;

	if ( declaration->tag != dt_function )
	{
		internal_error( "Attempt to exit function for something that is not a function. file %s line %d.", __FILE__, __LINE__ );
		return 1;
	}
	else
	{
		symbol_table.nearest = symbol_table.nearest->function.enclosing;
		printf( "\x1b[1;32mBKR exit_function() file %s line %d.\x1b[0m\n", __FILE__, __LINE__ );
		print_scope_stack( __FILE__, __LINE__ );
		return 1;
	}
}

void
add_variable( char * identifier, const Type * type, char * location )
{
	Declaration		declaration;
	DeclarationList *	decl_list;

	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_scope )
	{
		internal_error( "symbol_table.nearest is not a scope. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( lookup_decl( symbol_table.nearest->scope.decl_list, identifier ) != 0 )
	{
		semantic_error( "duplicate variable declaration %s", identifier );
	}
	else
	{
		declaration.tag = dt_variable;
		declaration.variable.identifier = identifier;
		declaration.variable.type = type->typeinfo;
		strcpy( declaration.variable.location, location );
		decl_list = add_decl( symbol_table.nearest->scope.decl_list, &declaration );
		symbol_table.nearest->scope.decl_list = decl_list;
	}
}

void
add_parameter( char * identifier, const Type * ptype, char * location )
{
	Declaration		declaration;
	DeclarationList *	decl_list;

	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_function )
	{
		internal_error( "symbol_table.nearest is not a function. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( lookup_decl( symbol_table.nearest->function.param_list, identifier ) != 0 )
	{
		semantic_error( "duplicate formal parameter declaration %s", identifier );
	}
	else
	{
		declaration.tag = dt_parameter;
		declaration.parameter.identifier = identifier;
		declaration.parameter.type = ptype->typeinfo;
		strcpy( declaration.parameter.location, location );
		decl_list = add_decl( symbol_table.nearest->function.param_list, &declaration );
		symbol_table.nearest->function.param_list = decl_list;
	}
}

void
add_member( char * identifier, const Type * mtype, char * location )
{
	Declaration		declaration;
	DeclarationList *	decl_list;

	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_struct )
	{
		internal_error( "symbol_table.nearest is not a struct. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( lookup_decl( symbol_table.nearest->strct.member_list, identifier ) != 0 )
	{
		semantic_error( "duplicate formal parameter declaration %s", identifier );
	}
	else
	{
		declaration.tag = dt_member;
		declaration.member.identifier = identifier;
		declaration.member.type = mtype->typeinfo;
		strcpy( declaration.member.location, location );
		decl_list = add_decl( symbol_table.nearest->strct.member_list, &declaration );
		symbol_table.nearest->strct.member_list = decl_list;
	}
}

Declaration *
add_function( char * identifier, const Type * rtype )
{
	Declaration		declaration;
	DeclarationList *	decl_list;

	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else if ( symbol_table.nearest->tag != dt_scope )
	{
		internal_error( "symbol_table.nearest is not a scope. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else if ( lookup_decl( symbol_table.nearest->scope.decl_list, identifier ) != 0 )
	{
		semantic_error( "duplicate variable declaration %s", identifier );
		return 0;
	}
	else
	{
		declaration.tag = dt_function;
		declaration.function.identifier = identifier;
		declaration.function.return_type = rtype->typeinfo;
		declaration.function.param_list = 0;
		declaration.function.enclosing = 0;
		decl_list = add_decl( symbol_table.nearest->scope.decl_list, &declaration );
		symbol_table.nearest->scope.decl_list = decl_list;
		return & decl_list->list[decl_list->count - 1];
	}
}

void
set_scope_space( int size )
{
	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_scope )
	{
		internal_error( "symbol_table.nearest is not a scope. file %s line %d.", __FILE__, __LINE__ );
	}
	else
	{
		symbol_table.nearest->scope.space_used = size;
	}
}

void
set_parm_space( int size )
{
	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_function )
	{
		internal_error( "symbol_table.nearest is not a function. file %s line %d.", __FILE__, __LINE__ );
	}
	else
	{
		symbol_table.nearest->function.parm_space = size;
	}
}

void
set_struct_space( int size )
{
	if ( symbol_table.nearest == 0 )
	{
		internal_error( "symbol_table.nearest is zero. file %s line %d.", __FILE__, __LINE__ );
	}
	else if ( symbol_table.nearest->tag != dt_struct )
	{
		internal_error( "symbol_table.nearest is not a function. file %s line %d.", __FILE__, __LINE__ );
	}
	else
	{
		symbol_table.nearest->strct.memory_size = size;
	}
}

Declaration *
lookup_struct( const char * identifier )
{
	Declaration *		p;

	p = symbol_table.type_list;
	while ( p != 0 )
	{
		if ( strcmp( identifier, p->strct.identifier ) == 0 )
		{
			return p;
		}
		else
		{
			p = p->strct.next;
		}
	}
	return 0;
}

Declaration *
add_struct( const char * identifier )
{
	Declaration *		declaration;

	if ( lookup_struct( identifier ) != 0 )
	{
		semantic_error( "duplicate struct declaration %s", identifier );
		return 0;
	}
	else if ( (declaration = malloc( sizeof( Declaration ) )) == 0 )
	{
		internal_error( "malloc() faliure. file %s line %d.", __FILE__, __LINE__ );
		return 0;
	}
	else
	{
		declaration->tag = dt_struct;
		declaration->strct.identifier = (char *)identifier;
		declaration->strct.member_list = 0;
		declaration->strct.memory_size = 0;
		declaration->strct.enclosing = 0;
		declaration->strct.next = symbol_table.type_list;
		symbol_table.type_list = declaration;
		return declaration;
	}
}

Declaration *
lookup_declaration( const char * identifier )
{
	Declaration *		scope;
	Declaration *		declaration;
	DeclarationList *	decl_list;

	for ( scope = symbol_table.nearest ; scope != 0 ; scope = get_enclosing( scope ) )
	{
		if ( (decl_list = get_decl_list( scope )) == 0 )
		{
			continue;
		}
		else if ( (declaration = lookup_decl( decl_list, identifier )) != 0 )
		{
			return declaration;
		}
		else
		{
			continue;
		}
	}
	return 0;
}

void
print_scope_stack( char * file, int line )
{
	Declaration *		scope;
	DeclarationList *	decl_list;
	int			i;

	for ( scope = symbol_table.nearest ; scope != 0 ; scope = get_enclosing( scope ) )
	{
		if ( (decl_list = get_decl_list( scope )) == 0 )
		{
			printf( "Scope %lx has no declarations\n", (unsigned long)scope );
		}
		else
		{
			printf( "Scope %lx contains:\n", (unsigned long)scope );
			for ( i = 0 ; i < decl_list->count ; i++ )
			{
//				printf( " %s", get_identifier( decl_list->list + i ) );
				printDeclaration( decl_list->list + i, 1 );
			}
			printf( "\n" );
		}
	}
	printf( "END\n" );
}
