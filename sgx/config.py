CONFIG_DIR = "/etc/python-sgx/"
DATA_DIR = "/var/lib/python-sgx/"
SEALED_DIR = DATA_DIR + "sealed/"

GROUP = "sgx"

# Currently only Ubuntu 16.04 and 14.04 are supported
DISTRIBUTION = "Ubuntu"
UBUNTU_VERSION = "16.04"
LIBPROTOBUF_VERSION = "8" if UBUNTU_VERSION == "14.04" else "9"

# Currently we require Python 3.5. Install it with `apt install python3.5` on Ubuntu 14.04.
PYTHON_VERSION = "3.5"

# Use NIST curve P-384 (required by Intel's SGX SDK)
OPENSSL_ELLIPTIC_CURVE = "secp384r1"

