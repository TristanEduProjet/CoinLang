CC = gcc
CPPFLAGS = -Wall -Wextra -MD -MP
LEX = flex
LEX.l = $(LEX) $(LFLAGS)
YACC = bison
YFLAGS = --warnings=all -d
#LD=gcc


all: minicoin

%.d: %.c
	$(CC) -MM -MD -o $@ $<

%.o: %.c %.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.parser.c: %.y
	$(YACC.y) $(OUTPUT_OPTION) $<

%.y.o: %.parser.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.scanner.c: %.l
	$(LEX.l) -o$@ $<

%.l.o: %.scanner.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

minicoin : minicoin.l.o minicoin.y.o minicoin_tree.o minicoin_eval.o
	$(CXX) $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION) $^

#désactivation de la suppression des fichiers intermediaires
.SECONDARY:
#.PRECIOUS: %.c %.o

# pull in dependency info for *existing* .o/.d files
-include $(wildcard *.d)

.PHONY: all clean distclean

#supprime les fichiers temporaires de la compilation
clean :
	$(RM) *.o
	$(RM) *.scanner.*
	$(RM) *.parser.*
	$(RM) *.d

#supprime tout
distclean : clean
	$(RM) minicoin
	$(RM) *~
