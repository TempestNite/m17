#ifndef generate_h
#define generate_h

struct Result;

int	generate(	const char * label,
			const char * mnemonic,
			const char * left_opnd,
			const char * right_opnd,
			const char * comment );

void	generate_label( char * );
void	generate_cond_jump( const struct Result *, char * );
void	generate_anticond_jump( const struct Result *, char * );
void	generate_set( int, char * );

#endif
