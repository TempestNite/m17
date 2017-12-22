#include	"generate.h"
#include	<stdio.h>
#include	"Result.h"
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
		generate( 0, "cmpl", "$0", expr->location, "opnd waa not a condition" );
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
		generate( 0, "cmpl", "$0", expr->location, "opnd waa not a condition" );
		generate( 0, "je", trueLabel, 0, 0 );
	}
}
