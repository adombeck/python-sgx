import platform
import sys
from os import path

PROJECT_GIT_DIR = path.dirname(sys.modules[__name__].__file__)
GRAPHENE_DIR = path.realpath(path.join(PROJECT_GIT_DIR, "graphene"))
HOME_DIR = path.expanduser("~/.sgx")


class UnsupportedSystemException(Exception):
    def __init__(self):
        message = "Unsupported OS. Currently this package can only be used on Ubuntu 14.04 and Ubuntu 16.04."
        super(Exception, self).__init__(message)

distribution, ubuntu_version = platform.linux_distribution()[:2]
if distribution != "Ubuntu":
    raise UnsupportedSystemException()

if ubuntu_version == "14.04":
#    PYTHON_VERSION = "3.4"
    LIBPROTOBUF_VERSION = "8"
elif ubuntu_version == "16.04":
#    PYTHON_VERSION = "3.5"
    LIBPROTOBUF_VERSION = "9"
else:
    raise UnsupportedSystemException()

# XXX: Currently we require Python 3.5. Install it with `apt install python3.5` on Ubuntu 14.04.
PYTHON_VERSION = "3.5"

OPENSSL_ELLIPTIC_CURVE = "secp384r1"  # Use NIST curve P-384
