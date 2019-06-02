.PHONY: all clean

all clean:
	@$(MAKE) -sC bootstrap-cpp $@
	@$(MAKE) -sC bootstrap-pool $@
