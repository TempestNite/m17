#ifndef TypeInfo_h
#define TypeInfo_h

struct Declaration;

typedef enum ModTag { mt_none, mt_array, mt_pointer } ModTag;

typedef struct Modifier {
	ModTag			tag;
	unsigned int		array_bound;
	unsigned int		multiplier;
} Modifier;

typedef enum Tag { tag_none, tag_scalar, tag_struct } Tag;

typedef enum ScalarType { type_none, type_void, type_int, type_char, type_float } ScalarType;

typedef struct TypeInfo {
	Tag			tag;
	ScalarType		scalar_type;
	struct Declaration *	struct_type;
	unsigned int		basic_size;
	unsigned int		memory_size;
	unsigned int		modifier_count;
	Modifier		modifier[0];
} TypeInfo;

typedef struct Type {
	char *		identifier;
	TypeInfo *	typeinfo;
} Type;

TypeInfo *	make_builtin_type( ScalarType, Type * );
TypeInfo *	make_struct_type( const char *, Type * );
TypeInfo *	add_array( Type *, unsigned int );
TypeInfo *	add_pointer( Type * );
TypeInfo *	end_type( TypeInfo * );			// call after all modifiers
TypeInfo *	remove_array( TypeInfo * );
TypeInfo *	remove_pointer( TypeInfo * );
void		print_typeinfo( const TypeInfo *, int );
int		get_size( const TypeInfo * );

#endif
