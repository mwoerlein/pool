include ./Makefile.inc

POOLBCLIBS = linux.a sys.a

.PHONY: all clean

all: clean $(BINDIR)/poolbc 

clean:
	@rm -rf $(BINDIR) $(OBJDIR) $(LIBDIR)

$(BINDIR)/poolbc: $(SRCDIR)/commands/poolbc.cpp $(POOLBCLIBS:%=$(LIBDIR)/%) $(BINDIR)
	@echo "build store"
	@$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< $(POOLBCLIBS:%=$(LIBDIR)/%)
