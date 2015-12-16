CC=gcc
CFLAGS=-fms-extensions -std=c99 -Wall -Wextra -Werror -g
BIN=bin
OBJ=obj
PERL=perl

# Note that the OBJECTS and BINARIES defined here are not dependent on the
# *.test.c files in the main directory, hence all objects and binaries will
# be cleaned.
ifeq ($(OS),Windows_NT)
	RM := cmd /C del
	MKDIR := cmd /C md
	OBJECTS := $(OBJ)\\*.o
	BINARIES := $(BIN)\\*.exe
	REMOVEDSYM :=
	DOTEXE := .exe
else
	RM := rm -f
	MKDIR := mkdir -p
	OBJECTS := $(OBJ)/*.o
	BINARIES := $(BIN)/*
	REMOVEDSYM := $(RM) -rf $(BIN)/*.dSYM
	DOTEXE :=
endif

# Dynamically create a list of objects and binaries that must be compiled,
# baesd on the files ending with `.test.c` in this directory.
SRC_FILES=$(wildcard *.test.c)

OBJ_NAMES=$(patsubst %.test.c,%.o,$(SRC_FILES))
OBJ_PATTERN=$(OBJ)/%
OBJ_PATHS=$(patsubst %,$(OBJ_PATTERN),$(OBJ_NAMES))

BIN_NAMES=$(patsubst %.test.c,%,$(SRC_FILES))
BIN_PATTERN=$(BIN)/%$(DOTEXE)
BIN_PATHS=$(patsubst %,$(BIN_PATTERN),$(BIN_NAMES))

.PHONY: all create_build_dirs test clean
.IGNORE: create_build_dirs

all: create_build_dirs $(BIN_PATHS)

create_build_dirs:
	$(MKDIR) $(OBJ) $(BIN)

$(OBJ)/%.o: %.test.c TESTickle.h
	$(CC) $(CFLAGS) -o $@ -c $<

# added manually
$(OBJ)/ledlib.o: ../ledlib.c ../ledlib.h
	$(CC) $(CFLAGS) -o $@ -c $<

%_runner.c: %.test.c
	$(PERL) test_generator.pl $< > $@

# ledlib.o added manually
$(BIN)/%$(DOTEXE): %_runner.c $(OBJ)/%.o $(OBJ)/ledlib.o
	$(CC) $(CFLAGS) -o $@ $^

test:
	prove -f -e "" bin/*$(DOTEXE)

clean:
	$(RM) *_runner.c
	$(RM) $(OBJECTS)
	$(REMOVEDSYM)
	$(RM) $(BINARIES)
