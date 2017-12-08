#ifndef semantic_h
#define semantic_h

#include	"TypeInfo.h"
#include	"Result.h"

struct Declaration;

void			semantic_error( const char * fmt, ... );
struct Declaration *	ensure_declaration_exists( char *, char *, int );
void			ensure_array_type( const Result *, char *, int, char * );
void			ensure_subscript_type( const Result *, char *, int );
void			ensure_struct_type( const Result *, char *, int );
void			ensure_struct_exists( const char *, Type *, char *, int );
void			ensure_member_exists( const Result *, char *, char *, int, char * );
void			ensure_lvalue( const Result *, char *, int );

#endif
