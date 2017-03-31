#!/usr/bin/env python3

import glob
import os
import subprocess

from config import GRAPHENE_DIR

SGX_SIGN = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/pal-sgx-sign")
SGX_GET_TOKEN = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/pal-sgx-get-token")
LIBPAL = os.path.join(GRAPHENE_DIR, "Pal/src/libpal-enclave.so")
KEY = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/enclave-key.pem")

manifests = glob.glob("*.manifest")


def execute(command):
    print("executing %r" % " ".join(command))
    subprocess.check_call(command)


def sign_manifest(manifest):
    output = manifest + ".sgx"
    exec_ = manifest[:-9]
    command = [SGX_SIGN,
               "-libpal", LIBPAL,
               "-key", KEY,
               "-output", output,
               "-exec", exec_,
               "-manifest", manifest
               ]
    execute(command)


def create_token(manifest):
    token = manifest[:-9] + ".token"
    sig = manifest[:-9] + ".sig"
    command = [SGX_GET_TOKEN,
               "-output", token,
               "-sig", sig]
    execute(command)


def main():
    for manifest in manifests:
        sign_manifest(manifest)
        create_token(manifest)


if __name__ == "__main__":
    main()
