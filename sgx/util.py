import os

from sgx.config import KEY_DIR


def get_attester_public_key():
    pubkey_path = os.path.join(KEY_DIR, "attester_public.key")
    with open(pubkey_path) as pubkey_file:
        return bytes.fromhex(pubkey_file.read())
