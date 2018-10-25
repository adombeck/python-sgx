# Python SGX

A Python interface to the SGX SDK for Linux. Uses [Graphene-SGX](https://github.com/oscarlab/graphene) to execute Python in an enclave.

## Project Status

**Not actively maintained as of August 2017.**

**Probably doesn't even build anymore.**

**You don't want to use this if you are not prepared to maintain it yourself.**


## Submodules

**The submodules are outdated. Don't file bug reports to the original authors if you use these forks.**

1. [Intel SGX Linux Driver](https://github.com/adombeck/linux-sgx-driver)

1. [Patched Intel SGX Linux SDK](https://github.com/adombeck/linux-sgx)

1. [Patched Graphene Library OS](https://github.com/adombeck/graphene)


## Installation

1. Initialize the submodules:

        git submodule update --init

2. Run the submodules' setup scripts:

        cd linux-sgx-driver && ./setup.sh && cd ..
        cd linux-sgx && ./setup.sh && cd ..
        cd graphene && ./setup.sh && cd ..

3. Adjust the paths in `config/config.py` (or keep the default paths)
4. Copy the public key of the [SGX Remote Attestation Challenger](https://github.com/adombeck/sgx-ra-challenger) to `/etc/python-sgx/challenger_public.key`, or delete this line from the `config/python3.manifest.template` if you don't want to use the remote attestation:

	sgx.trusted_files.challenger_public_key = file:$(CONFIG_DIR)/challenger_public.key

5. Run the setup script:

        ./setup.sh


6. Add your user to the `sgx` group:

        sudo usermod -a -G sgx $USER

	  Note that this will only take effect after a new login.


## Uninstallation

1. Run the uninstallation script:

        sudo ./uninstall.sh


## Test sealing

    python3-sgx tests/test_seal.py seal
    python3-sgx tests/test_seal.py unseal
    python3-sgx tests/test_sealing_wrapper.py

Note: You have to be in the repository's base directory to be able to run the tests. 

## Test remote attestation
Note: You need to install the [challenger package](https://github.com/adombeck/sgx-ra-challenger) on the challenging machine and store a copy of the challenger's public key in `/etc/python-sgx/challenger_public.key`. The [SGX Remote Attestation Challenger package](https://github.com/adombeck/sgx-ra-challenger) contains a script `generate_key_pair.py` which creates a key in the required format.

1. Run the Quoting Manager (handles communication with the Quoting Enclave):

        quoting-manager
        
2. Run the Remote Attestation Manager (handles communication with the challenger):

        sgx-ra-manager

3. Use the [SGX Remote Attestation Challenger](https://github.com/adombeck/sgx-ra-challenger) to connect to the Remote Attestation Manager:

        sgx-ra-challenger -c 127.0.0.1 6789

