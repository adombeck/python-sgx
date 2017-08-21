# Python SGX

A Python wrapper around Intel's SGX Linux SDK, to be used inside an SGX enclave. Uses [Graphene-SGX](https://github.com/oscarlab/graphene) to execute Python in an enclave.

## Requirements
1. [Intel SGX Linux Driver](https://github.com/01org/linux-sgx-driver)


## Submodules

1. [Patched Intel SGX Linux SDK](https://github.com/adombeck/linux-sgx)

2. [Patched Graphene Library OS](https://github.com/adombeck/graphene)


## Installation

1. Initialize the submodules:

        git submodule update --init

2. Run the submodules' setup scripts:

        cd linux-sgx && ./setup.sh && cd ..
        cd graphene && ./setup.sh && cd ..

3. Adjust the paths in `sgx/config.py` (or keep the default paths)
4. Run the setup script:

        ./setup.sh


5. Add your user to the `sgx` group:

        sudo usermod -a -G sgx $USER

	  Note that this will only take effect after a new login.


## Uninstallation

1. Run the uninstallation script:

        sudo ./uninstall.sh


## Test sealing

    python3-sgx tests/test_seal.py seal
    python3-sgx tests/test_seal.py unseal

Note: You have to be in the repository's base directory to be able to run the tests. 

## Test remote attestation
Note: You need to install the [challenger package](https://github.com/adombeck/sgx-ra-challenger) on the challenging machine and store a copy of the challenger's public key in `/etc/python-sgx/challenger_public.key`. The [SGX Remote Attestation Challenger package](https://github.com/adombeck/sgx-ra-challenger) contains a script `generate_key_pair.py` which creates a key in the required format.

1. Run the Quoting Manager (handles communication with the Quoting Enclave):

        quoting-manager
        
2. Run the Remote Attestation Manager (handles communication with the challenger):

        sgx-ra-manager

3. Use the [SGX Remote Attestation Challenger](https://github.com/adombeck/sgx-ra-challenger) to connect to the Remote Attestation Manager:

        sgx-ra-challenger -c 127.0.0.1 6789

