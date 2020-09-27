POOLSC=../bootstrap-cpp/bin/poolbsc
PSC_FLAGS=--binding linux::elf -t 0x10000 --classpath $(SRCDIR)
PARSERGEN=./bin/parsergen

SRCDIR=./src
TESTDIR=./tests
BINDIR=./bin

$(BINDIR):
	@mkdir -p $@
