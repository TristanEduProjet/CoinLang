EXTLIBS = $(sort $(dir $(wildcard extsub/*/)))
CC = gcc
GIT = git
CPPFLAGS = -Wall -Wextra -MD -MP -DVERSION=\"$(shell $(GIT) describe --abbrev=4 --dirty --always --tags)\" $(patsubst %,-I%,$(EXTLIBS))
CXXFLAGS = -std=c++11
ifeq ($(CC), gcc)
	CFLAGS += -fms-extensions -std=gnu99
else
	CFLAGS += -std=c99
	ifeq ($(CC), clang)
		CFLAGS += -fms-extensions -Wno-microsoft
		#-Wmicrosoft -Wc++11-extensions -Wno-long-long
	endif
endif
LEX = flex
LEX.l = $(LEX) $(LFLAGS)
YACC = bison
YFLAGS = --warnings=all --graph -d
#LD=gcc
LDLIBS = -largtable3
LDFLAGS = $(patsubst %,-L%,$(EXTLIBS))
#LDFLAGS = -Lextsub/argtable3
DBGFLAGS = -g3 -ggdb3 -Og -DDEBUG -D_DEBUG -DYYDEBUG=1 -save-temps
RLSFLAGS = -O3 -DNDEBUG -D_NDEBUG
XSLT = xsltproc
DOT = dot


all: libraries minicoin

%.d: %.c
	$(CC) -MM -MD -o $@ $<

%.o: %.c %.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.parser.c %.parser.h %.parser.output %.parser.dot : %.y
	$(YACC.y) $(OUTPUT_OPTION) $<

%.y.o: %.parser.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.scanner.c %.scanner.h: %.l
	$(LEX.l) -o$@ $<

%.l.o: %.scanner.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

minicoin : libraries main.o minicoin.y.o minicoin.l.o minicoin_inst.o unorderedmap.o
	$(CXX) $(OUTPUT_OPTION) $(TARGET_ARCH) $(filter-out $<,$^) $(LDFLAGS) $(LOADLIBES) $(LDLIBS)

libraries:
	$(MAKE) -C extsub

#%.html: %.y.o
#	ifeq ($(firstword $(MAKECMDGOALS)), debug)
#		#$(XSLT) $$($(YACC) --print-datadir)/xslt/xml2xhtml.xsl $(basename $(notdir $<)).xml > $(basename $(notdir $<)).xhtml
#	endif

#%.png: %.parser.dot
#	$(DOT) -Tpng -O $<
#$(DOT) -Tpng -O $(($<):.c=.dot)
## hack of makefile deps
%.png: %.parser.c
	$(DOT) -Tpng $(OUTPUT_OPTION) $(<:.c=.dot)

graph: minicoin.png

debug: override CFLAGS += $(DBGFLAGS)
debug: override CXXFLAGS += $(DBGFLAGS)
debug: override YFLAGS += --report=solved -x $(if $(filter $(YACC), bison), --debug, -t)
debug: all

release: CFLAGS += $(RLSFLAGS)
release: CXXFLAGS += $(RLSFLAGS)
release: all

#désactivation de la suppression des fichiers intermediaires
.SECONDARY:
#.PRECIOUS: %.c %.o

# pull in dependency info for *existing* .o/.d files
-include $(wildcard *.d)

.PHONY: all libraries debug release graph clean distclean

#supprime les fichiers temporaires de la compilation
clean :
	$(RM) *.i
	$(RM) *.ii
	$(RM) *.s
	$(RM) *.o
	$(RM) *.d
	$(RM) *.scanner.*
	$(RM) *.parser.*
	$(MAKE) -C extsub clean

#supprime tout
distclean : clean
	$(RM) minicoin
	$(RM) *.png
	$(RM) *.xhtml
	$(RM) *~
	$(MAKE) -C extsub distclean
