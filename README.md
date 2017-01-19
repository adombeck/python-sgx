# Python SGX

A Python wrapper around Intel's SGX Linux SDK, to be used inside an SGX enclave.

Currently, this project does not provide functions to *create* an enclave. Instead, it is meant to be used by Python modules which run inside an *otherwise* created enclave. It was developed and tested exclusively for usage with the Graphene Library OS, which runs applications inside an SGX enclave.

## Requirements

1. [Intel SGX Linux SDK](https://github.com/01org/linux-sgx)
 
2. [Graphene Library OS](https://github.com/oscarlab/graphene)
 
## Installation

1. Adjust the paths in `python-sgx/config.py`
2. Run `make` (or `make clean && make` if you built the project previously)
3. Run `sudo ./setup.py install`

## Run tests
    cd launcher
    graphene-pal python3.manifest.sgx tests/test_seal.py seal
    graphene-pal python3.manifest.sgx tests/test_seal.py unseal
