targets = all clean install

.PHONY: $(targets)
$(targets):
	$(MAKE) -C sealing $@
	$(MAKE) -C attestation $@

