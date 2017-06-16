#!/usr/bin/env python3-sgx

import sys
import traceback

try:
    import sgx.trusted.sealing
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()

file_name = "tests/test.sealed"


def seal():
    with open(file_name, "bw+") as f:
        data = sgx.trusted.sealing.seal(secret=b"this is some secret text with a \0 (null byte)",
                                        plain_text=b"this is additional plaintext with a \0 (null byte)")
        print("data: %r" % data)
        print("data_len: %r" % len(data))
        f.write(data)


def unseal():
    with open(file_name, "br") as f:
        data = f.read()
    print("data: %r" % data)
    secret, plain_text = sgx.trusted.sealing.unseal(data)
    print("secret: %r" % secret)
    print("plain text: %r" % plain_text)


def print_usage():
    print("Usage: %s (seal|unseal)" % sys.argv[0])


def main():
    if len(sys.argv) < 2:
        print_usage()
        return

    if sys.argv[1] == "seal":
        seal()
    elif sys.argv[1] == "unseal":
        unseal()
    else:
        print_usage()

if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()
