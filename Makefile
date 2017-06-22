EXTLIBS = $(sort $(dir $(wildcard extsub/*/)))
CC = gcc
GIT=git
CPPFLAGS = -Wall -Wextra -MD -MP -DVERSION=\"$(shell $(GIT) describe --abbrev=4 --dirty --always --tags)\" $(patsubst %,-I%,$(EXTLIBS))
CXXFLAGS = -std=c++11
LEX = flex
LEX.l = $(LEX) $(LFLAGS)
YACC = bison
YFLAGS = --warnings=all -d
#LD=gcc
LDLIBS = -largtable3
LDFLAGS = $(patsubst %,-L%,$(EXTLIBS))
#LDFLAGS = -Lextsub/argtable3
DBGFLAGS = -g3 -ggdb3 -Og -DDEBUG -D_DEBUG
RLSFLAGS = -O3 -DNDEBUG -D_NDEBUG


all: libraries minicoin

%.d: %.c
	$(CC) -MM -MD -o $@ $<

%.o: %.c %.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.parser.c: %.y
	$(YACC.y) $(OUTPUT_OPTION) $<

%.y.o: %.parser.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.scanner.c %.scanner.h: %.l
	$(LEX.l) -o$@ $<

%.l.o: %.scanner.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

minicoin : libraries main.o minicoin.y.o minicoin.l.o minicoin_tree.o minicoin_eval.o unorderedmap.o
	$(CXX) $(OUTPUT_OPTION) $(TARGET_ARCH) $(filter-out $<,$^) $(LDFLAGS) $(LOADLIBES) $(LDLIBS)

libraries:
	$(MAKE) -C extsub

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

.PHONY: all libraries debug release clean distclean

#supprime les fichiers temporaires de la compilation
clean :
	$(RM) *.o
	$(RM) *.scanner.*
	$(RM) *.parser.*
	$(RM) *.d
	$(MAKE) -C extsub clean

#supprime tout
distclean : clean
	$(RM) minicoin
	$(RM) *~
	$(MAKE) -C extsub distclean
