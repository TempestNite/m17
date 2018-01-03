#include	"Result.h"
#include	"semantic.h"

TypeInfo *
get_result_type( const Result * left_opnd, const Result * right_opnd, const char * filename, int linenum )
{
	ScalarType			ltype, rtype;
	Type				unused;

	if ( is_array( left_opnd->type ) )
	{
		semantic_error( "Invalid use of array type on left side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else if ( is_pointer( left_opnd->type ) )
	{
		semantic_error( "Invalid use of pointer type on left side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else if ( is_struct( left_opnd->type ) )
	{
		semantic_error( "Invalid use of struct type on left side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else if ( is_array( right_opnd->type ) )
	{
		semantic_error( "Invalid use of array type right side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else if ( is_pointer( right_opnd->type ) )
	{
		semantic_error( "Invalid use of ppointer type right side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else if ( is_struct( right_opnd->type ) )
	{
		semantic_error( "Invalid use of struct type right side of arithmetic operator. %s : %d\n", filename, linenum );
		return 0;
	}
	else
	{
		ltype = getScalarType( left_opnd->type );
		rtype = getScalarType( right_opnd->type );
		return make_builtin_type( ltype > rtype ? ltype : rtype, &unused );
	}
}
