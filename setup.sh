#!/bin/bash

set -e

echo "Building python-sgx"
make clean && make

echo "Installing python-sgx"
python3.5 ./setup.py install
