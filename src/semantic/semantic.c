#include	"semantic.h"
#include	"symbol_table.h"
#include	"Declaration.h"
#include	"DeclarationList.h"
#include	<stdarg.h>
#include	<stdio.h>
#include	<string.h>

void
semantic_error( const char * fmt, ... )
{
	va_list		args;

	va_start( args, fmt );
	fprintf( stderr, "\x1b[2;31mSemantic Error:  " );
	vfprintf( stderr, fmt, args );
	fprintf( stderr, "\x1b[0m" );
	va_end( args );
}

void
get_multiplier( TypeInfo * ltype, TypeInfo * rtype, char * multiplier )
{
	if ( rtype->tag != tag_scalar )
	{
		semantic_error( "subscript type not a scalar." );
	}
	else if ( rtype->scalar_type != type_int )
	{
		semantic_error( "subscript type not integer." );
	}
	else if ( ltype->modifier_count < 1 )
	{
		semantic_error( "Use of subscript with non-array type." );
	}
	else if ( ltype->modifier[0].tag != mt_array )
	{
		semantic_error( "Use of subscript with non-array type." );
	}
	else
	{
		sprintf( multiplier, "$%d", ltype->modifier[0].multiplier );
	}
}

Declaration *
ensure_declaration_exists( char * identifier, char * filename, int linenum )
{
	Declaration *		d;

        if ( (d = lookup_declaration( identifier )) == 0 )
	{
		semantic_error( "Undeclared identifier %s file %s line %d\n", identifier, filename, linenum );
		return 0;
	}
	else
	{
		return d;
	}
}

void
ensure_array_type( const Result * left_opnd, char * filename, int linenum, char * multiplier )
{
	TypeInfo *		ltype = left_opnd->type;

	if ( ltype->modifier_count < 1 )
	{
		semantic_error( "Use of subscript with non-array type. %s line %d", filename, linenum );
	}
	else if ( ltype->modifier[0].tag != mt_array )
	{
		semantic_error( "Use of subscript with non-array type. %s line %d", filename, linenum );
	}
	else
	{
		sprintf( multiplier, "$%d", ltype->modifier[0].multiplier );
	}
}

void
ensure_subscript_type( const Result * right_opnd, char * filename, int linenum )
{
	TypeInfo *		rtype = right_opnd->type;

	if ( rtype->tag != tag_scalar )
	{
		semantic_error( "subscript type not a scalar. %s line %d\n", filename, linenum );
	}
	else if ( rtype->scalar_type == type_int )
	{
		return;
	}
	else if ( rtype->scalar_type == type_char )
	{
		return;
	}
	else
	{
		semantic_error( "Invlid subscript type. %s line %d", filename, linenum );
	}
}

void
ensure_struct_type( const Result * left_opnd, char * filename, int linenum )
{
	TypeInfo *		ltype = left_opnd->type;

	if ( ltype->tag != tag_struct )
	{
		semantic_error( "Use of member selection with non-struct. %s line %d", filename, linenum );
	}
}

void
ensure_struct_exists( const char * identifier, Type * stype, char * filename, int linenum )
{
	Declaration *		declaration;

	if ( (declaration = lookup_struct( identifier )) == 0 )
	{
		semantic_error( "%s is not a struct. %s line %d", identifier, filename, linenum );
	}
	else
	{
		stype->identifier = (char *)identifier;
		make_struct_type( identifier, stype );
	}
}

void
ensure_member_exists( const Result * left_opnd, char * identifier, char * filename, int linenum, char * location )
{
	TypeInfo *		ltype = left_opnd->type;
	Declaration *		declaration;

	if ( ltype->tag != tag_struct )
	{
		semantic_error( "Use of member selection with non-struct. %s line %d", filename, linenum );
	}
	else if ( (declaration = lookup_decl( ltype->struct_type->strct.member_list, identifier )) == 0 )
	{
		semantic_error( "struct %s does not contain member %s. %s line %d", ltype->struct_type->strct.identifier, identifier, filename, linenum );
	}
	else
	{
		strcpy( location, declaration->member.location );
		return;
	}
}

void
ensure_lvalue( const Result * left_opnd, char * filename, int linenum )
{
	if ( (left_opnd->flags & IS_LVALUE) == 0 )
	{
		semantic_error( "expression is not an L-value. %s line %d", filename, linenum );
	}
}
