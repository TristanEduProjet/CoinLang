CC = gcc
CPPFLAGS = -w -MD -MP
ARFLAGS = rvcs
LIBS=$(patsubst %/,%,$(wildcard */))

#Libs with makefile
LIBSMF = $(patsubst %/,%,$(dir $(wildcard */Makefile)))
clean_LIBSMF += $(addprefix clean_,$(LIBSMF))

#libs without makefile
LIBSWMF=$(filter-out $(LIBSMF),$(LIBS))
clean_LIBSWMF += $(addprefix clean_,$(LIBSWMF))


all: $(LIBS) argtable3 klib

clean: $(clean_LIBSMF) $(clean_LIBSWMF) clean_argtable3 clean_klib

distclean: clean
	rm -f $(addsuffix *.so,$(LIBS)) $(addsuffix *.a,$(LIBS))

.PHONY: $(LIBS) $(clean_LIBSMF) argtable3 clean_argtable3 klib clean_klib

$(LIBSMF):
	echo make -C $@

$(clean_LIBSMF):
	echo make -C $(patsubst clean_%,%,$@) clean

#$(LIBSWMF):
#	echo manual $@

#$(clean_LIBSWMF):
#	echo manual $(patsubst clean_%,%,$@) clean

#
# TODO: rendre générique les libs sans makefile
#

argtable3: argtable3/libargtable3.a
#argtable3/libargtable3.a : argtable3/argtable3.o
#argtable3/argtable3.o : argtable3/argtable3.c

clean_argtable3:
	rm -f argtable3/libargtable3.a
	rm -f argtable3/argtable3.d

klib: klib/libklib.a

KLIB_FILES = kexpr khmm knetfile knhx kson kstring
# bgzf ksw kurl
# kthread #uint8_t not recognize
# kmath #M_SQRT2 problem
KLIB_OBJ = $(patsubst %,klib/%,$(KLIB_FILES:=.o) ksa.o)
# kopen.o
klib/libklib.a: $(patsubst %,klib/%,$(KLIB_FILES:=.h) kbit.h kbtree.h kdq.h kgraph.h khash.h klist.h kseq.h ksort.h kvec.h)
klib/libklib.a: $(KLIB_OBJ)
	$(AR) $(ARFLAGS) $@ $(KLIB_OBJ)

clean_klib:
	rm -f klib/*.o
	rm -f klib/*.d
	rm -f klib/klib/libklib.a

clean_sglib: ;

$(.LIBPATTERNS): %.o
	$(AR) $(ARFLAGS) $@ $<

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
