# GCC compiler
CC = gcc

# Compiler flags
CFLAGS = -std=gnu89

# The build SCANNER PARSER executable:
SCANNER = scan
PARSER	= m17
INPUT = Input
STATE = state
TOKEN = Token

all: $(INPUT) $(STATE) $(TOKEN) $(SCANNER)

$(INPUT):
	$(CC) $(CFLAGS) -c $(INPUT).c $(INPUT).h

$(TOKEN): $(INPUT).o
	$(CC) $(CFLAGS) -c $(TOKEN).c $(TOKEN).h

$(STATE):
	$(CC) $(CFLAGS) -c $(STATE).c $(STATE).h

$(SCANNER): $(INPUT).o $(TOKEN).o $(STATE).o
	$(CC) $(CFLAGS) -o $(SCANNER) $(SCANNER).c $(INPUT).o $(TOKEN).o $(STATE).o


clean:
		$(RM) $(SCANNER) $(PARSER) $(INPUT).o $(TOKEN).o $(STATE).o $(STATE).h.gch $(INPUT).h.gch $(TOKEN).h.gch $(STATE).h.gch
