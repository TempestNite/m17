#ifndef Result_h
#define Result_h

#include	"TypeInfo.h"

// typedef enum ResultTag { rt_none, rt_temp, rt_constant, rt_variable } ResultTag;

#define IS_TEMP		0x01
#define IS_CONDITION	0x02
#define IS_LVALUE	0x04
#define IS_EQUAL	0x08
#define IS_NOTEQUAL	0x10
#define IS_LESSTHAN	0x20
#define IS_LESSEQUAL	0x40
#define IS_GREATERTHAN	0x80
#define IS_GREATEREQUAL	0x100
#define IS_ERROR	0x20000

typedef struct Result {
	int		flags;
	TypeInfo *	type;
	char		location[40];
} Result;

#endif
