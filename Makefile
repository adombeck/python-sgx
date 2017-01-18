targets = all clean install

include Makefile.config

.PHONY: $(targets)
$(targets):
	$(MAKE) -C sgx $@
	$(MAKE) -C launcher $@

