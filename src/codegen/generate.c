#include	"generate.h"
#include	<stdio.h>
#include	"Result.h"
#include	"TypeInfo.h"
#include	<string.h>

int
generate( const char * label, const char * mnemonic, const char * left_opnd, const char * right_opnd, const char * comment )
{
	if ( label != 0 )	printf( "%s:\t", label );
	else			printf( "\t" );

	if ( mnemonic != 0 )	printf( "%s\t", mnemonic );
	else			printf( "\t" );

	if ( left_opnd != 0 && right_opnd != 0 )	printf( "%s, %s\t", left_opnd, right_opnd );
	else if ( left_opnd != 0 )			printf( "%s\t", left_opnd );
	else if ( right_opnd != 0 )			printf( "%s\t", right_opnd );

	if ( comment != 0 )	printf( "\t# %s\n", comment );
	else			printf( "\n" );

	return 1;
}

void
generate_formats()
{
	generate( 0, ".section", ".rodata", 0, 0 );
	generate( ".fmt_d", ".string", "\"%d\"", 0, 0 );
	generate( ".fmt_s", ".string", "\"%s\"", 0, 0 );
	generate( ".fmt_c", ".string", "\"%c\"", 0, 0 );
	generate( ".fmt_l", ".string", "\"%lx\"", 0, 0 );
	generate( ".fmt_f", ".string", "\"%g\"", 0, 0 );
}

void
generate_label( char * label )
{
	static int			counter = 0;

	sprintf( label, ".L%d", counter++ );
}

static void
generate_cond_instr( int flags, char * jumpstring )
{
	if ( flags & IS_EQUAL )			strcpy( jumpstring, "je" );
	else if ( flags & IS_NOTEQUAL )		strcpy( jumpstring, "jne" );
	else if ( flags & IS_LESSTHAN )		strcpy( jumpstring, "jl" );
	else if ( flags & IS_LESSEQUAL )	strcpy( jumpstring, "jle" );
	else if ( flags & IS_GREATERTHAN )	strcpy( jumpstring, "jg" );
	else if ( flags & IS_GREATEREQUAL )	strcpy( jumpstring, "jge" );
	else strcpy( jumpstring, "Bogus jump mnemonic" );
}

static void
generate_anticond_instr( int flags, char * jumpstring )
{
	if ( flags & IS_EQUAL )			strcpy( jumpstring, "jne" );
	else if ( flags & IS_NOTEQUAL )		strcpy( jumpstring, "je" );
	else if ( flags & IS_LESSTHAN )		strcpy( jumpstring, "jge" );
	else if ( flags & IS_LESSEQUAL )	strcpy( jumpstring, "jg" );
	else if ( flags & IS_GREATERTHAN )	strcpy( jumpstring, "jle" );
	else if ( flags & IS_GREATEREQUAL )	strcpy( jumpstring, "jl" );
	else strcpy( jumpstring, "Bogus antijump mnemonic" );
}

void
generate_set( int flags, char * jumpstring )
{
	if ( flags & IS_EQUAL )			strcpy( jumpstring, "sete" );
	else if ( flags & IS_NOTEQUAL )		strcpy( jumpstring, "setne" );
	else if ( flags & IS_LESSTHAN )		strcpy( jumpstring, "setl" );
	else if ( flags & IS_LESSEQUAL )	strcpy( jumpstring, "setle" );
	else if ( flags & IS_GREATERTHAN )	strcpy( jumpstring, "setg" );
	else if ( flags & IS_GREATEREQUAL )	strcpy( jumpstring, "setge" );
	else strcpy( jumpstring, "Bogus set mnemonic" );
}

void
generate_anticond_jump( const Result * expr, char * falseLabel )
{
	char			cond_jump[40];

	if ( expr->flags & IS_CONDITION )
	{
		generate_anticond_instr( expr->flags, cond_jump );
		generate( 0, cond_jump, falseLabel, 0, "opnd was condition" );
	}
	else
	{
		generate( 0, "cmpl", "$0", expr->location, "opnd was not a condition" );
		generate( 0, "je", falseLabel, 0, 0 );
	}
}

void
generate_cond_jump( const Result * expr, char * trueLabel )
{
	char			cond_jump[40];

	if ( expr->flags & IS_CONDITION )
	{
		generate_cond_instr( expr->flags, cond_jump );
		generate( 0, cond_jump, trueLabel, 0, "opnd was condition" );
	}
	else
	{
		generate( 0, "cmpl", "$0", expr->location, "opnd was not a condition" );
		generate( 0, "je", trueLabel, 0, 0 );
	}
}

void
adjust_rsp_entry( int autospace )
{
	char			string[20];

	if ( autospace > 0 )
	{
		sprintf( string, "$%ld", autospace + sizeof( void * ) );
		generate( 0, "subq", string, "%rsp", 0 );
	}
}

void
adjust_rsp_exit( int autospace )
{
	char			string[20];

	if ( autospace > 0 )
	{
		sprintf( string, "$%ld", autospace + sizeof( void * ) );
		generate( 0, "addq", string, "%rsp", 0 );
	}
}

const char *
get_format( const Result * expr )
{
	TypeInfo *		t = expr->type;

	if ( is_char_pointer( t ) )	return "$.fmt_s";
	else if ( is_char_array( t ) )	return "$.fmt_s";
	else if ( is_pointer( t ) )	return "$.fmt_l";
	else switch( getScalarType( t ) ) 
	{
		case type_char:		return "$.fmt_c";
		case type_int:		return "$.fmt_d";
		case type_float:	return "$.fmt_f";
		default:		return "NO FORMAT";
	}
}

