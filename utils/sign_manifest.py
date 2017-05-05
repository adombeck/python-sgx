#!/usr/bin/env python3

import glob
import os
import subprocess
import sys
import argparse

DIR = os.path.dirname(__file__)
GRAPHENE_DIR = os.path.abspath(os.path.join(DIR, "../graphene/"))
SGX_SIGN = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/pal-sgx-sign")
SGX_GET_TOKEN = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/pal-sgx-get-token")
LIBPAL = os.path.join(GRAPHENE_DIR, "Pal/src/libpal-enclave.so")
KEY = os.path.join(GRAPHENE_DIR, "Pal/src/host/Linux-SGX/signer/enclave-key.pem")


def execute(command):
    print("executing %r" % " ".join(command))
    subprocess.check_call(command)


def sign_manifest(manifest):
    output = manifest + ".sgx"
    command = [SGX_SIGN,
               "-libpal", LIBPAL,
               "-key", KEY,
               "-output", output,
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


def get_manifests(path):
    manifests = glob.glob(path + "/*.manifest")
    if not manifests:
        sys.exit("Cannot find manifest in directory %r" % path)
    return manifests


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("PATH", nargs='?', default=".", help="Path to the manifest file or the directory containing the manifest files")
    return parser.parse_args()


def main():
    args = parse_args()
    path = os.path.abspath(args.PATH)

    if path.endswith(".manifest"):
        if not os.path.isfile(path):
            sys.exit("Cannot find file %r" % path)
        manifests = [path]
    else:
        manifests = get_manifests(path)

    for manifest in manifests:
        sign_manifest(manifest)
        create_token(manifest)


if __name__ == "__main__":
    main()
