# Python SGX

A Python wrapper around Intel's SGX Linux SDK, to be used inside an SGX enclave.

## Requirements

1. [Intel SGX Linux SDK](https://github.com/01org/linux-sgx)
 
2. [Graphene Library OS](https://github.com/oscarlab/graphene) - Currently, this project does not provide functions to create an enclave. Instead, it is meant to be used by Python modules which run inside an otherwise created enclave. It was developed and tested exclusively for usage with the Graphene Library OS.
 
## Installation

1. Adjust the paths in `python-sgx/config.py`
2. Run `sudo ./setup.py install`