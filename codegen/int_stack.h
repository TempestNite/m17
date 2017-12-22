#ifndef int_stack_h
#define int_stack_h

typedef struct int_stack {
	int *		ptr;
	unsigned int	sub;
	unsigned int	memsize;
	unsigned int	space_used;
} int_stack;

void	setup_int_stack( int_stack * );
void	push_int( int_stack *, int );
int	pop_int( int_stack * );
void	destroy_int_stack( int_stack * );

#endif
