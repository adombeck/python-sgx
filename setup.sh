#!/bin/bash

set -e

# Install required packages
dpkg -s python3-dev > /dev/null || sudo apt install python3-dev
dpkg -s python3-pip > /dev/null || sudo apt install python3-pip

# We need swig version 3.0.10 or higher. Install from zesty or newer.
dpkg -s swig > /dev/null || sudo apt install swig/zesty

# Install sh module via pip (version in xenial repositories is not recent enough)
pip3 -q show sh || sudo pip3 install sh
pip3 -q show setuptools || sudo pip3 install setuptools

echo "Building python-sgx"
make clean && make

echo "Installing python-sgx"
sudo ./setup.py install
