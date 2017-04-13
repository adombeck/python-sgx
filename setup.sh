#! /bin/bash

set -e

echo "Installing submodules"

DIR=$PWD

echo "Installing linux-sgx"
cd $DIR/linux-sgx
./setup.sh

echo "Installing graphene"
cd $DIR/graphene
./setup.sh

echo "Building python-sgx"
cd $DIR
make clean && make

echo "Installing python-sgx"
python3.5 ./setup.py install
