include ./Makefile.inc

POOLBCLIBS = linux.a poolc.a sys.a

.PHONY: all clean

all: clean $(BINDIR)/poolbc 

clean:
	@rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) $(SRCDIR)/poolc/parser/parse.cc $(SRCDIR)/poolc/parser/PoolParser.cc

$(BINDIR)/poolbc: $(SRCDIR)/commands/poolbc.cpp $(POOLBCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbc"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBCLIBS:%=$(LIBDIR)/%)
