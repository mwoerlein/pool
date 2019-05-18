include ./Makefile.inc

POOLBCLIBS = linux.a poolc.a sys.a
PBASMLIBS = linux.a pasm.a sys.a

.PHONY: all clean

all: clean $(BINDIR)/pbasm $(BINDIR)/poolbc

clean:
	@rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) $(SRCDIR)/poolc/parser/parse.cc $(SRCDIR)/poolc/parser/PoolParser.cc

$(BINDIR)/pbasm: $(SRCDIR)/commands/pbasm.cpp $(PBASMLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build pbasm"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(PBASMLIBS:%=$(LIBDIR)/%)

$(BINDIR)/poolbc: $(SRCDIR)/commands/poolbc.cpp $(POOLBCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbc"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBCLIBS:%=$(LIBDIR)/%)
