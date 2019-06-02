.PHONY: all clean

all clean:
	@$(MAKE) -sC bootstrap-cpp $@
	@$(MAKE) -sC bootstrap-pool $@

POOLSC=./bootstrap-cpp/bin/poolbsc
PSC_FLAGS=--binding linux::elf -t 0x10000 --classpath ./src --classpath ./bootstrap-pool/src --classpath ../gridos/src
