include ./Makefile.inc

POOLBCLIBS  = linux.a poolc.a sys.a
POOLBSCLIBS = linux.a poolc.a pasm.a sys.a
PBASMLIBS   = linux.a pasm.a sys.a
STOREBLIBS  = linux.a sys.a

.PHONY: all clean

all: clean $(BINDIR)/pbasm $(BINDIR)/poolbc $(BINDIR)/poolbsc $(BINDIR)/storeb

clean:
	@rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) $(SRCDIR)/poolc/parser/parse.cc $(SRCDIR)/poolc/parser/PoolParser.cc $(SRCDIR)/pasm/i386/PasmParser.cpp

$(BINDIR)/pbasm: $(SRCDIR)/commands/pbasm.cpp $(PBASMLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build pbasm"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(PBASMLIBS:%=$(LIBDIR)/%)

$(BINDIR)/poolbc: $(SRCDIR)/commands/poolbc.cpp $(POOLBCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbc"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBCLIBS:%=$(LIBDIR)/%)

$(BINDIR)/poolbsc: $(SRCDIR)/commands/poolbsc.cpp $(POOLBSCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbsc"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBSCLIBS:%=$(LIBDIR)/%)

$(BINDIR)/storeb: $(SRCDIR)/commands/store.cpp $(STOREBLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build storeb"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(STOREBLIBS:%=$(LIBDIR)/%)
