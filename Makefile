CC = gcc
CPPFLAGS = -Wall -Wextra -MD -MP
CXXFLAGS = -std=c++11
LEX = flex
LEX.l = $(LEX) $(LFLAGS)
YACC = bison
YFLAGS = --warnings=all -d
#LD=gcc
DBGFLAGS = -g3 -ggdb3 -Og -DDEBUG -D_DEBUG
RLSFLAGS = -O3 -DNDEBUG -D_NDEBUG


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

minicoin : minicoin.y.o minicoin.l.o minicoin_tree.o minicoin_eval.o unorderedmap.o
	$(CXX) $(LOADLIBES) $(LDLIBS) $(OUTPUT_OPTION) $^

debug: CFLAGS += $(DBGFLAGS)
debug: CXXFLAGS += $(DBGFLAGS)
debug: all

release: CFLAGS += $(RLSFLAGS)
release: CXXFLAGS += $(RLSFLAGS)
release: all

#désactivation de la suppression des fichiers intermediaires
.SECONDARY:
#.PRECIOUS: %.c %.o

# pull in dependency info for *existing* .o/.d files
-include $(wildcard *.d)

.PHONY: all debug release clean distclean

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
