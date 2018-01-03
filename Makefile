# GCC compiler
CC = gcc

# Compiler flags
CFLAGS = -std=gnu89 -I./src/lexical -I./src/parser \
-I./src/semantic -I./src/codegen

# The build SCANNER PARSER_SRC executable:
DRIVER_BIN		= ./bin/m17
DRIVER_SRC		= ./src/driver/driver
PARSER_SRC		= ./src/parser/parser
INPUT_SRC		= ./src/lexical/Input
STATE_SRC 		= ./src/lexical/state
TOKEN_SRC 		= ./src/lexical/Token
DECLLIST_SRC 	= ./src/semantic/DeclarationList
DECL_SRC 		= ./src/semantic/Declaration
TYPE_SRC		= ./src/semantic/TypeInfo
SYM_SRC			= ./src/semantic/symbol_table
SEM_SRC			= ./src/semantic/semantic
LOC_SRC			= ./src/codegen/LocGen

PARSER_OBJ 		= parser
INPUT_OBJ		= Input
STATE_OBJ		= state
TOKEN_OBJ		= Token
DECL_OBJ		= Declaration
DECLLIST_OBJ	= DeclarationList
TYPE_OBJ		= TypeInfo
SYM_OBJ			= symbol_table
SEM_OBJ			= semantic
LOC_OBJ			= LocGen

all: $(INPUT_SRC) $(STATE_SRC) $(TOKEN_SRC) \
	$(PARSER_SRC) $(DECLLIST_SRC) $(DECL_SRC) \
	$(TYPE_SRC) $(SYM_SRC) $(SEM_SRC) $(LOC_SRC) \
	$(DRIVER_SRC)
	

$(INPUT_SRC): $(INPUT_SRC).c $(INPUT_SRC).h
	$(CC) $(CFLAGS) -c $(INPUT_SRC).c

$(TOKEN_SRC): $(TOKEN_SRC).c $(TOKEN_SRC).h
	$(CC) $(CFLAGS) -c $(TOKEN_SRC).c

$(DECLLIST_SRC): $(DECLLIST_SRC).c $(DECLLIST_SRC).h
	$(CC) $(CFLAGS) -c $(DECLLIST_SRC).c

$(DECL_SRC): $(DECL_SRC).c $(DECL_SRC).h
	$(CC) $(CFLAGS) -c $(DECL_SRC).c

$(SYM_SRC): $(SYM_SRC).c $(SYM_SRC).h
	$(CC) $(CFLAGS) -c $(SYM_SRC).c

$(LOC_SRC): $(LOC_SRC).c $(LOC_SRC).h
	$(CC) $(CFLAGS) -c $(LOC_SRC).c

$(SEM_SRC): $(SEM_SRC).c $(SEM_SRC).h
	$(CC) $(CFLAGS) -c $(SEM_SRC).c

$(TYPE_SRC): $(TYPE_SRC).c $(TYPE_SRC).h ./src/semantic/Result.h
	$(CC) $(CFLAGS) -c $(TYPE_SRC).c

$(STATE_SRC): $(STATE_SRC).c $(STATE_SRC).h
	$(CC) $(CFLAGS) -c $(STATE_SRC).c

$(PARSER_SRC): $(PARSER_SRC).c $(PARSER_SRC).h
	$(CC) $(CFLAGS) -c $(PARSER_SRC).c

$(DRIVER_SRC): $(STATE_OBJ).o $(PARSER_OBJ).o \
	$(INPUT_OBJ).o $(TOKEN_OBJ).o $(LOC_OBJ).o

	$(CC) $(CFLAGS) -o $(DRIVER_BIN) $(DRIVER_SRC).c \
	$(STATE_OBJ).o $(PARSER_OBJ).o $(INPUT_OBJ).o $(TOKEN_OBJ).o \
	$(DECL_OBJ).o $(DECLLIST_OBJ).o $(TYPE_OBJ).o $(SYM_OBJ).o \
	$(SEM_OBJ).o $(LOC_OBJ).o

clean:
	$(RM) $(DRIVER_BIN) *.o *.h.gch
