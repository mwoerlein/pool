include ./Makefile.inc

.PHONY: all clean regenerate-parsers tests clear-tests install

all: clean $(BINDIR)/storeb $(BINDIR)/pbasm $(BINDIR)/pool $(BINDIR)/poolbc $(BINDIR)/poolbsc $(BINDIR)/parsergen
tests: clear-tests regenerate-test-parsers $(BINDIR)/pooltest
	@$(BINDIR)/pooltest

install: $(BINDIR)/poolbsc
	@cp $(BINDIR)/poolbsc $(INSTALLDIR)/poolbsc

clean:
	@rm -rf $(BINDIR)

$(BINDIR)/screentest: $(SRCDIR)/sys/command/ScreenTest.pool $(BINDIR) $(POOLSC)
	@echo "build screentest (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::ScreenTest
	@chmod a+x $@
	
$(BINDIR)/storeb: $(SRCDIR)/pool/command/Store.pool $(BINDIR) $(POOLSC)
	@echo "build storeb (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::Store
	@chmod a+x $@
	
$(BINDIR)/pbasm: $(SRCDIR)/pool/command/Pbasm.pool $(BINDIR) $(POOLSC)
	@echo "build pbasm (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::Pbasm
	@chmod a+x $@
	
$(BINDIR)/parsergen: $(SRCDIR)/pool/command/ParserGen.pool $(BINDIR) $(POOLSC)
	@echo "build parsergen (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::ParserGen
	@chmod a+x $@
	
$(BINDIR)/poolbc: $(SRCDIR)/pool/command/PoolCompiler.pool $(BINDIR) $(POOLSC)
	@echo "build poolbc (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::PoolCompiler
	@chmod a+x $@
	
$(BINDIR)/poolbsc: $(SRCDIR)/linux/command/PoolStaticCompiler.pool $(BINDIR) $(POOLSC)
	@echo "build poolbsc (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::PoolStaticCompiler
	@chmod a+x $@
	
$(BINDIR)/pool: $(SRCDIR)/pool/command/PoolRuntime.pool $(BINDIR) $(POOLSC)
	@echo "build pool (pool)"
	@$(POOLSC) $(PSC_FLAGS) --output $@ linux::command::PoolRuntime
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

