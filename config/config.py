CONFIG_DIR = "/etc/python-sgx/"
DATA_DIR = "/var/lib/python-sgx/"
SEALED_DIR = DATA_DIR + "sealed/"

GROUP = "sgx"

# Currently only Ubuntu 16.04 is supported
DISTRIBUTION = "Ubuntu"
UBUNTU_VERSION = "16.04"
LIBPROTOBUF_VERSION = "9"

# Currently we require Python 3.5.
PYTHON_VERSION = "3.5"

# Use NIST curve P-384 (required by Intel's SGX SDK)
OPENSSL_ELLIPTIC_CURVE = "secp384r1"

