// Copied from linux-sgx/sdk/trts/linux/trts_pic.S

// Modified to always return that the address is inside the enclave, because for processes run inside the enclaved library OS, the whole address space is inside the enclave

int sgx_is_within_enclave(const void *addr, unsigned int size)
{
		return 1;
}

int sgx_is_outside_enclave(const void *addr, unsigned int size)
{
		return 0;
}
