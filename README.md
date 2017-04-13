# Python SGX

A Python wrapper around Intel's SGX Linux SDK, to be used inside an SGX enclave.

Currently, this project does not provide functions to *create* an enclave. Instead, it is meant to be used by Python modules which run inside an *otherwise* created enclave. It was developed and tested exclusively for usage with the Graphene Library OS, which runs applications inside an SGX enclave.

## Submodules

1. [Patched Intel SGX Linux SDK](https://github.com/adombeck/linux-sgx)

2. [Patched Graphene Library OS](https://github.com/adombeck/graphene)

3. [PyCrypto v2.7a1](https://github.com/dlitz/pycrypto)
    - Required for AES-CMAC

## Installation

1. Initialize git submodules: `git submodule update --init`
2. Adjust the paths in `python-sgx/config.py`
3. Run the setup script: `sudo ./setup.sh`

## Run tests
    cd launcher
    graphene-pal python3.manifest.sgx tests/test_seal.py seal
    graphene-pal python3.manifest.sgx tests/test_seal.py unseal

## Notes
In order to use the remote attestation, you have to acquire a copy of the attester's public key through a trusted channel and store it in `/etc/python-sgx/attester_public.key`. The `Python SGX Attester` package contains a script `generate_key_pair.py` which creates the key in the required format.
