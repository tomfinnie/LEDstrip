include TESTickle/TESTickle.mk

CFLAGS += -I TESTickle -I ..

ifeq ($(OS),Windows_NT)
	RM := cmd /C del
	DOTEXE := .exe
else
	RM := rm -f
	DOTEXE :=
endif

.PHONY: test clean

ledlib.o: ../ledlib.c
	$(CC) $(CFLAGS) -o $@ -c $<

ledlib_battery.test$(DOTEXE): $(call test_files_for,ledlib_battery.test.c) ledlib.o
	$(CC) $(CFLAGS) -o $@ $^

ledlib_dispatch.test$(DOTEXE): $(call test_files_for,ledlib_dispatch.test.c) ledlib.o
	$(CC) $(CFLAGS) -o $@ $^

test: ledlib_battery.test$(DOTEXE) ledlib_dispatch.test$(DOTEXE)
	prove -f -e "" $(addprefix ./,$^)

clean:
	$(RM) $(TESTICKLE_BUILT_ITEMS)
	$(RM) *.o
	$(RM) -r *.dSYM
