POOLSC=../bin/poolbsc
PSC_FLAGS=--warning --binding linux::elf -t 0x10000 --classpath $(SRCDIR)
PARSERGEN=./bin/parsergen

SRCDIR=./src
TESTDIR=./tests
BINDIR=./bin
INSTALLDIR=../bin

$(BINDIR):
	@mkdir -p $@
