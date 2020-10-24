include ./Makefile.inc

.PHONY: all clean regenerate-parsers tests clear-tests

all: clean $(BINDIR)/storeb $(BINDIR)/pbasm $(BINDIR)/poolc $(BINDIR)/parsergen
tests: clear-tests regenerate-test-parsers $(BINDIR)/pooltest
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
	
$(BINDIR)/parsergen: $(SRCDIR)/pool/command/ParserGen.pool $(BINDIR) $(POOLSC)
	@echo "build parsergen (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ pool::command::ParserGen
	@chmod a+x $@
	
$(BINDIR)/poolc: $(SRCDIR)/pool/command/PoolCompiler.pool $(BINDIR) $(POOLSC)
	@echo "build poolc (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ pool::command::PoolCompiler
	@chmod a+x $@
	
$(BINDIR)/pooltest: $(TESTDIR)/test/Command.pool $(BINDIR) $(POOLSC)
	@echo "build pooltest (pool)"
	@$(POOLSC) --classpath $(TESTDIR) $(PSC_FLAGS) --output $@ test::Command
	@chmod a+x $@

clear-tests:
	@rm -rf $(TESTDIR)/pool/generator/sample/Parser.pool $(TESTDIR)/pool/generator/sample/Tokenizer.pool
	@rm -rf $(BINDIR)/pooltest

regenerate-test-parsers: $(PARSERGEN)
	@$(PARSERGEN) --classpath $(TESTDIR) $(TESTDIR)/pool/generator/sample/Parser.gram

regenerate-parsers: $(PARSERGEN)
	@$(PARSERGEN) --classpath $(SRCDIR) $(SRCDIR)/pool/generator/regex/Parser.gram
	@$(PARSERGEN) --classpath $(SRCDIR) $(SRCDIR)/pool/generator/parser/grammar/Parser.gram
	@$(PARSERGEN) --classpath $(SRCDIR) $(SRCDIR)/pool/asm/Parser.gram
	@$(PARSERGEN) --classpath $(SRCDIR) $(SRCDIR)/pool/compiler/parser/Parser.gram

