CC=gcc
CFLAGS=-fms-extensions -std=c99 -Wall -Wextra -Werror -g
BIN=bin
OBJ=obj
PERL=perl

ifeq ($(OS),Windows_NT)
	RM := cmd /C del
	MKDIR := cmd /C md
	OBJECTS := $(OBJ)\*.o
	BINARIES := $(BIN)\*.exe
	REMOVEDSYM := 
	DOTEXE := .exe
else
	RM := rm -f
	MKDIR := mkdir -p
	OBJECTS := $(OBJ)/*.o
	BINARIES := $(BIN)/*
	REMOVEDSYM := $(RM) -r $(BIN)/*.dSYM
	DOTEXE := 
endif

.PHONY: create_build_dirs test clean
.IGNORE: create_build_dirs

all: create_build_dirs $(BIN)/test$(DOTEXE)

create_build_dirs:
	$(MKDIR) $(OBJ) $(BIN)
	
$(OBJ)/ledlib.o: ../ledlib.c ../ledlib.h
	$(CC) $(CFLAGS) -o $@ -c ../ledlib.c

$(OBJ)/test.o: test.c TESTickle.h
	$(CC) $(CFLAGS) -o $@ -c test.c

%_runner.c: %.c
	$(PERL) test_generator.pl $< > $@

$(BIN)/test$(DOTEXE): test_runner.c $(OBJ)/test.o $(OBJ)/ledlib.o
	$(CC) $(CFLAGS) -o $@ $^

test:
	prove -f -e "" bin/*$(DOTEXE)

clean:
	$(RM) *_runner.c
	$(RM) $(OBJECTS)
	$(REMOVEDSYM)
	$(RM) $(BINARIES)
