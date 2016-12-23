#!/bin/bash

set -e

./pal sealing

cd /home/adrian/projects/graphene/LibOS/shim/test/apps/python3
./python3.manifest.sgx scripts/test_seal.py seal
./python3.manifest.sgx scripts/test_seal.py unseal
