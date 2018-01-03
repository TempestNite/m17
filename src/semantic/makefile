CC	= gcc
LEXICAL	= ../lexical
PARSER	= ../parser
CODEGEN	= ../codegen
FLAGS	= -g -Wall -I$(LEXICAL) -I$(PARSER) -I$(CODEGEN) -I.
COMPILE	= $(CC) $(FLAGS)

all: 	TIdriver analyze

TypeInfo.o:	TypeInfo.c TypeInfo.h
	$(COMPILE) -c TypeInfo.c

TIdriver:	TIdriver.o TypeInfo.o symbol_table.o Declaration.o DeclarationList.o	\
		semantic.o
	$(COMPILE) -o TIdriver TIdriver.c TypeInfo.o symbol_table.o Declaration.o DeclarationList.o semantic.o

DeclarationList.o:	DeclarationList.c DeclarationList.h
	$(COMPILE) -c DeclarationList.c

Input.o:	$(LEXICAL)/Input.c $(LEXICAL)/Input.h
	$(COMPILE) -c $(LEXICAL)/Input.c

StringMaker.o:	$(LEXICAL)/StringMaker.c $(LEXICAL)/StringMaker.h
	$(COMPILE) -c $(LEXICAL)/StringMaker.c

Result.o:	Result.c Result.h
	$(COMPILE) -c Result.c

semantic.o:	semantic.c semantic.h
	$(COMPILE) -c semantic.c

lexical.o:	$(LEXICAL)/lexical.c $(LEXICAL)/lexical.h
	$(COMPILE) -c $(LEXICAL)/lexical.c

parser.o:	parser.c parser.h $(LEXICAL)/lexical.h
	$(COMPILE) -c parser.c

analyze:	analyze.c		\
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
	$(COMPILE) -o analyze analyze.c lexical.o Input.o StringMaker.o parser.o	\
		symbol_table.o TypeInfo.o Declaration.o DeclarationList.o		\
		LocGen.o semantic.o TempMgr.o int_stack.o generate.o Result.o

Declaration.o:	Declaration.c Declaration.h
	$(COMPILE) -c Declaration.c

symbol_table.o:	symbol_table.c symbol_table.h
	$(COMPILE) -c symbol_table.c

LocGen.o:	$(CODEGEN)/LocGen.c $(CODEGEN)/LocGen.h
	$(COMPILE) -c $(CODEGEN)/LocGen.c

TempMgr.o:	$(CODEGEN)/TempMgr.c $(CODEGEN)/TempMgr.h
	$(COMPILE) -c $(CODEGEN)/TempMgr.c

int_stack.o:	$(CODEGEN)/int_stack.c $(CODEGEN)/int_stack.h
	$(COMPILE) -c $(CODEGEN)/int_stack.c

generate.o:	$(CODEGEN)/generate.c $(CODEGEN)/generate.h
	$(COMPILE) -c $(CODEGEN)/generate.c

clean:
	rm -rf *.o TIdriver analyze a.out test10 test12 *.s test11 test13
