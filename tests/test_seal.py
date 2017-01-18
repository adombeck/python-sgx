try:
    import sgx.sealing
except ImportError as e:
    print(e.msg)
    exit()

import sys

file_name = "tests/test.sealed"

def seal():
    with open(file_name, "bw+") as f:
        data = sgx.sealing.seal(b"this is some secret text", 24, b"this is additional plaintext", 28)
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
    if sys.argv[1] == "unseal":
        unseal()    

if __name__ == "__main__":
    main()
