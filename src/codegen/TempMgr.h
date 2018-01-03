#ifndef TempMgr_h
#define TempMgr_h

// Offset generator for temp space

#include	"int_stack.h"

typedef struct TempMgr {
	unsigned int	offset;		// in bytes
	int_stack	stack;
} TempMgr;

int	setupTempMgr( TempMgr *, int autosize );

int	push_temp( TempMgr *, int size, char * );
int	pop_temp();

void	destroyTempMgr( TempMgr * );


#endif
