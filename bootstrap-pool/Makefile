include ./Makefile.inc

.PHONY: all clean

all: clean $(BINDIR)/storeb

clean:
	@rm -rf $(BINDIR)

$(BINDIR)/storeb: $(SRCDIR)/pool/command/Store.pool $(BINDIR) $(POOLSC)
	@echo "build storeb (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ pool::command::Store
	@chmod a+x $@
