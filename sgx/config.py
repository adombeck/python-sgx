import sys
from os import path

PROJECT_GIT_DIR = path.dirname(sys.modules[__name__].__file__)
GRAPHENE_DIR = path.realpath(path.join(PROJECT_GIT_DIR, "graphene"))
KEY_DIR = "/etc/python-sgx/"
DATA_DIR = "/var/lib/python-sgx/"

DISTRIBUTION = "Ubuntu"
UBUNTU_VERSION = "16.04"
LIBPROTOBUF_VERSION = "8" if UBUNTU_VERSION == "14.04" else "9"

# XXX: Currently we require Python 3.5. Install it with `apt install python3.5` on Ubuntu 14.04.
PYTHON_VERSION = "3.5"

OPENSSL_ELLIPTIC_CURVE = "secp384r1"  # Use NIST curve P-384
