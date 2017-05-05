# Python SGX

A Python wrapper around Intel's SGX Linux SDK, to be used inside an SGX enclave.

Currently, this project does not provide functions to *create* an enclave. Instead, it is meant to be used by Python modules which run inside an *otherwise* created enclave. It was developed and tested exclusively for usage with the Graphene Library OS, which runs applications inside an SGX enclave.


## Requirements
1. [Intel SGX Linux Driver](https://github.com/01org/linux-sgx-driver)


## Submodules

1. [Patched Intel SGX Linux SDK](https://github.com/adombeck/linux-sgx)

2. [Patched Graphene Library OS](https://github.com/adombeck/graphene)


## Installation

1. Initialize git submodules:

        git submodule update --init

2. Run the submodules' setup scripts:

        cd linux-sgx && ./setup.sh && cd ..
        cd graphene && ./setup.sh && cd ..

3. Adjust the paths in `sgx/config.py` (or keep the default paths)
4. Run the setup script:

        ./setup.sh


## Uninstallation

1. Run the uninstallation script:

        sudo ./uninstall.sh


## Run tests
    python3-sgx tests/test_seal.py seal
    python3-sgx tests/test_seal.py unseal

## Test remote attestation
Note: You need to install the [python-sgx-attester](https://github.com/adombeck/python-sgx-attester) to connect to the remote attestation manager

1. Run the untrusted remote attestation manager:

        untrusted-ra-manager

2. Run the trusted remote attestation manager:

        trusted-ra-manager

3. Connect to the `trusted-ra-manager` with the `sgx-attester`

        sgx-attester -c 127.0.0.1 6789

## Notes
In order to use the remote attestation, you have to acquire a copy of the attester's public key through a trusted channel and store it in `/etc/python-sgx/attester_public.key`. The `Python SGX Attester` package contains a script `generate_key_pair.py` which creates the key in the required format.
