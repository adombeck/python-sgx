import sys
import traceback

try:
    import sgx.sealing
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()

file_name = "tests/test.sealed"


def seal():
    with open(file_name, "bw+") as f:
        data = sgx.sealing.seal(b"this is some secret text with a \0 (null byte)", b"this is additional plaintext with a \0 (null byte)")
        print("data: %r" % data)
        print("data_len: %r" % len(data))
        f.write(data)


def unseal():
    with open(file_name, "br") as f:
        data = f.read()
    print("data: %r" % data)
    secret, plain_text = sgx.sealing.unseal(data)
    print("secret: %r" % secret)
    print("plain text: %r" % plain_text)


def main():
    if sys.argv[1] == "seal":
        seal()
    elif sys.argv[1] == "unseal":
        unseal()
    else:
        print("Usage: %s (seal|unseal)")

if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()


