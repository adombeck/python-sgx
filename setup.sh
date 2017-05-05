#!/bin/bash

set -e

echo "Building python-sgx"
make clean && make

echo "Installing python-sgx"
sudo ./setup.py install
