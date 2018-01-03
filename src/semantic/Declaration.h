#ifndef Declaration_h
#define Declaration_h

#include	"TypeInfo.h"

struct DeclarationList;

typedef enum DeclTag { dt_none, dt_variable, dt_parameter, dt_struct, dt_member, dt_function, dt_scope } DeclTag;

typedef struct Variable {
	char *				identifier;
	TypeInfo *			type;
	char				location[20];		// from code generator
} Variable;

typedef struct Parameter {
	char *				identifier;
	TypeInfo *			type;
	char				location[20];		// from code generator
} Parameter;

typedef struct Struct {
	char *				identifier;
	struct DeclarationList *	member_list;
	unsigned int			memory_size;
	unsigned int			alignment;
	struct Declaration *		enclosing;
	struct Declaration *		next;
} Struct;

typedef struct Member {
	char *				identifier;
	TypeInfo *			type;
	char				location[20];		// from code generator
} Member;

typedef struct Scope {
	struct DeclarationList *	decl_list;
	struct Declaration *		enclosing;
	int				space_used;
} Scope;

typedef struct Function {
	char *				identifier;
	TypeInfo *			return_type;
	struct DeclarationList *	param_list;
	int				parm_space;
	struct Declaration *		enclosing;
} Function;

typedef struct Declaration {
	DeclTag			tag;
	union {
		Variable	variable;
		Parameter	parameter;
		Struct		strct;
		Member		member;
		Scope		scope;
		Function	function;
	};
} Declaration;

char *				get_identifier( const Declaration * );
void				free_identifier( Declaration * );
struct DeclarationList *	get_decl_list( const Declaration * );
Declaration *			get_enclosing( const Declaration * );
TypeInfo *			get_type_info( const Declaration * );
char *				get_decl_location( Declaration * );

void				printDeclaration( const Declaration *, int );

#endif
