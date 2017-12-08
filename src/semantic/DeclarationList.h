#ifndef DeclarationList_h
#define DeclarationList_h

#include	"TypeInfo.h"
#include	"Declaration.h"

typedef struct DeclarationList {
	unsigned int		count;
	Declaration		list[0];
} DeclarationList;

DeclarationList *	add_decl( DeclarationList *, Declaration * );
Declaration *		lookup_decl( DeclarationList *, const char * );
DeclarationList *	destroy_decl_list( DeclarationList * );
void			printDeclarationList( DeclarationList *, int );

#endif
