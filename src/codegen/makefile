CC	= gcc
LEXICAL	= ../lexical
PARSER	= ../parser
SEMANTIC	= ../semantic
FLAGS	= -g -Wall -I$(PARSER) -I$(LEXICAL) -I$(SEMANTIC)
COMPILE	= $(CC) $(FLAGS)

#LFLAGS = -lfl

all: 	OGdriver TGdriver compiler

int_stack.o:	int_stack.c int_stack.h
	$(COMPILE) -c int_stack.c

TempMgr.o:	TempMgr.c TempMgr.h
	$(COMPILE) -c TempMgr.c

LocGen.o:	LocGen.c LocGen.h
	$(COMPILE) -c LocGen.c

OGdriver:		OGdriver.c LocGen.o
	$(COMPILE) -o OGdriver OGdriver.c LocGen.o

TGdriver:		TGdriver.c TempMgr.o int_stack.o
	$(COMPILE) -o TGdriver TGdriver.c TempMgr.o int_stack.o

StringMaker.o:	$(LEXICAL)/StringMaker.c $(LEXICAL)/StringMaker.h
	$(COMPILE) -c $(LEXICAL)/StringMaker.c

lexical.o:	$(LEXICAL)/lexical.c $(LEXICAL)/lexical.h $(LEXICAL)/Input.h $(LEXICAL)/StringMaker.h
	$(COMPILE) -c $(LEXICAL)/lexical.c

TypeInfo.o:	$(SEMANTIC)/TypeInfo.c $(SEMANTIC)/TypeInfo.h
	$(COMPILE) -c $(SEMANTIC)/TypeInfo.c

Input.o:	$(LEXICAL)/Input.c $(LEXICAL)/Input.h
	$(COMPILE) -c $(LEXICAL)/Input.c

generate.o:	generate.c generate.h
	$(COMPILE) -c generate.c

jump.o:		jump.c jump.h
	$(COMPILE) -c jump.c

semantic.o:	$(SEMANTIC)/semantic.c $(SEMANTIC)/semantic.h
	$(COMPILE) -c $(SEMANTIC)/semantic.c

symbol_table.o:	$(SEMANTIC)/symbol_table.c $(SEMANTIC)/symbol_table.h
	$(COMPILE) -c $(SEMANTIC)/symbol_table.c

DeclarationList.o:	$(SEMANTIC)/DeclarationList.c	\
			$(SEMANTIC)/DeclarationList.h	\
			$(SEMANTIC)/Declaration.h
	$(COMPILE) -c $(SEMANTIC)/DeclarationList.c

Result.o:	$(SEMANTIC)/Result.c $(SEMANTIC)/Result.h
	$(COMPILE) -c $(SEMANTIC)/Result.c


Declaration.o:	$(SEMANTIC)/Declaration.c $(SEMANTIC)/Declaration.h
	$(COMPILE) -c $(SEMANTIC)/Declaration.c

parser.o:	parser.c $(PARSER)/parser.h $(LEXICAL)/lexical.h $(SEMANTIC)/Result.h
	$(COMPILE) -c parser.c

compiler:	compiler.c		\
		Input.o			\
		parser.o		\
		StringMaker.o		\
		DeclarationList.o	\
		Declaration.o		\
		symbol_table.o		\
		TypeInfo.o		\
		semantic.o		\
		Result.o		\
		TempMgr.o		\
		LocGen.o		\
		int_stack.o		\
		generate.o		\
		lexical.o	
	$(COMPILE) -o compiler compiler.c lexical.o Input.o StringMaker.o parser.o TypeInfo.o generate.o TempMgr.o int_stack.o semantic.o DeclarationList.o Declaration.o Result.o symbol_table.o

clean:
	rm -rf *.o parserdriver OGdriver TGdriver compiler
