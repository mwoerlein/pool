include ./Makefile.inc

.PHONY: all clean tests clear-tests

all: clean $(BINDIR)/storeb $(BINDIR)/pbasm
tests: clear-tests $(BINDIR)/pooltest
	@$(BINDIR)/pooltest

clean:
	@rm -rf $(BINDIR)

$(BINDIR)/storeb: $(SRCDIR)/pool/command/Store.pool $(BINDIR) $(POOLSC)
	@echo "build storeb (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ pool::command::Store
	@chmod a+x $@
	
$(BINDIR)/pbasm: $(SRCDIR)/pool/command/Pbasm.pool $(BINDIR) $(POOLSC)
	@echo "build pbasm (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ pool::command::Pbasm
	@chmod a+x $@
	
$(BINDIR)/pooltest: $(TESTDIR)/test/Command.pool $(BINDIR) $(POOLSC)
	@echo "build pooltest (pool)"
	@$(POOLSC) --classpath $(TESTDIR) $(PSC_FLAGS) --output $@ test::Command
	@chmod a+x $@

clear-tests:
	@rm -rf $(BINDIR)/pooltest
