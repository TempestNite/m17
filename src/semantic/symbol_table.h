#ifndef symbol_table_h
#define symbol_table_h

#include	"TypeInfo.h"
#include	"Declaration.h"

int		setup_symbol_table();
Declaration *	enter_scope();
Declaration *	exit_scope();
Declaration *	enter_struct( Declaration * );
Declaration *	exit_struct();
Declaration *	add_struct( const char * identifier );
Declaration *	add_function( char * identifier, const Type * );
void		add_variable( char * identifier, const Type *, char * );
void		add_parameter( char * identifier, const Type *, char * );
void		add_member( char * identifier, const Type *, char * );
Declaration *	enter_function( Declaration * );
int		exit_function();
void		set_scope_space( int );
void		set_parm_space( int );
void		set_struct_space( int );

Declaration *	lookup_declaration( const char * identifier );
Declaration *	lookup_struct( const char * identifier );

void		semantic_error( const char * fmt, ... );

void		print_scope_stack( char *, int );

#endif
