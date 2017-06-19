CC = gcc
CPPFLAGS = -Wall -Wextra -MD -MP
ARFLAGS = rvcs
LIBS=$(patsubst %/,%,$(wildcard */))

#Libs with makefile
LIBSMF = $(patsubst %/,%,$(dir $(wildcard */Makefile)))
clean_LIBSMF += $(addprefix clean_,$(LIBSMF))

#libs without makefile
LIBSWMF=$(filter-out $(LIBSMF),$(LIBS))
clean_LIBSWMF += $(addprefix clean_,$(LIBSWMF))


all: $(LIBS) argtable3

clean: $(clean_LIBSMF) $(clean_LIBSWMF)

distclean: clean
	rm -f $(addsuffix *.so,$(LIBS)) $(addsuffix *.a,$(LIBS))

.PHONY: $(LIBS) $(clean_LIBSMF) argtable3

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

$(.LIBPATTERNS): %.o
	$(AR) $(ARFLAGS) $@ $<
