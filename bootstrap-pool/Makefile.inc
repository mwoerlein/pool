POOLSC=../bootstrap-cpp/bin/poolbsc
PSC_FLAGS=--binding linux::elf -t 0x10000 --classpath $(SRCDIR)

SRCDIR=./src
BINDIR=./bin

$(BINDIR):
	@mkdir -p $@
