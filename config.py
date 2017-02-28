import platform
from os import path

GRAPHENE_DIR = path.expanduser("~/projects/graphene")


class UnsupportedSystemException(Exception):
    def __init__(self):
        message = "Unsupported OS. Currently this package can only be used on Ubuntu 14.04 and Ubuntu 16.04."
        super(Exception, self).__init__(message)

distribution, ubuntu_version = platform.linux_distribution()[:2]
if distribution != "Ubuntu":
    raise UnsupportedSystemException()

if ubuntu_version == "14.04":
    PYTHON_VERSION = "3.4"
    LIBPROTOBUF_VERSION = "8"
elif ubuntu_version == "16.04":
    PYTHON_VERSION = "3.5"
    LIBPROTOBUF_VERSION = "9"
else:
    raise UnsupportedSystemException()