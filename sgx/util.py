import os

from sgx.exceptions import UnexpectedLengthError

from sgx.config import CONFIG_DIR


def get_attester_public_key():
    pubkey_path = os.path.join(CONFIG_DIR, "attester_public.key")
    with open(pubkey_path) as pubkey_file:
        return bytes.fromhex(pubkey_file.read())


def check_json(data, type_, length=None):
    if not isinstance(data, type_):
        raise TypeError("Received JSON is of type %r, expected type %r" % (type(data), type_))

    if length and len(data) != length:
        raise UnexpectedLengthError("Received JSON %r has length %r, expected length %r" % (data, len(data), length))
