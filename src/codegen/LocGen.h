#ifndef LocGen_h
#define LocGen_h

// Location generator for auto variables, formal parameters and structure members

typedef enum	OGtype { ot_none, ot_member, ot_auto, ot_parm } OGtype;

typedef struct LocGen {
	OGtype		ogtype;
	int		space_used;
	int		alignment;
	int		multiplier;
} LocGen;

int	setupLocGen( LocGen *, OGtype );
int	get_location( LocGen *, int size, int alignment, char * location );
int	get_space_used( LocGen * );
void	restore_space_used( LocGen *, int );
void	update_alignment( LocGen *, int );

#endif
