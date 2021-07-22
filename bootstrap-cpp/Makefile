include ./Makefile.inc

POOLBCLIBS  = linux.a poolc.a sys.a
POOLBSCLIBS = linux.a poolc.a pasm.a sys.a
PBASMLIBS   = linux.a pasm.a sys.a
STOREBLIBS  = linux.a sys.a

.PHONY: all install clean

all: clean $(BINDIR)/pbasm $(BINDIR)/poolbc $(BINDIR)/poolbsc $(BINDIR)/storeb

install: $(BINDIR)/poolbsc
	@cp $(BINDIR)/poolbsc $(INSTALLDIR)/poolbsc

clean:
	@rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR) $(SRCDIR)/poolc/parser/Parserbase.h $(SRCDIR)/poolc/parser/parse.cc $(SRCDIR)/poolc/parser/PoolParser.cc $(SRCDIR)/pasm/i386/PasmParser.cpp

$(BINDIR)/pbasm: $(SRCDIR)/commands/pbasm.cpp $(PBASMLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build pbasm (cpp)"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(PBASMLIBS:%=$(LIBDIR)/%)

$(BINDIR)/poolbc: $(SRCDIR)/commands/poolbc.cpp $(POOLBCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbc (cpp)"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBCLIBS:%=$(LIBDIR)/%)

$(BINDIR)/poolbsc: $(SRCDIR)/commands/poolbsc.cpp $(POOLBSCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build poolbsc (cpp)"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBSCLIBS:%=$(LIBDIR)/%)

$(BINDIR)/storeb: $(SRCDIR)/commands/store.cpp $(STOREBLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build storeb (cpp)"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(STOREBLIBS:%=$(LIBDIR)/%)
