import logging
import os
from contextlib import contextmanager

from sgx.trusted import sealing


class PersistentFileNotFoundError(Exception):
    pass


# XXX: Implement replay protection

@contextmanager
def handle_sealing(manifest_path):
    """Unseals the files specified in the sealing manifest.
    Requires an ephemeral filesystem in the enclave, to protect against the host OS
    (which Graphene does currently not provide)."""
    _unseal(manifest_path)
    yield
    _seal(manifest_path)


def _get_manifest_entries(manifest_path):
    with open(manifest_path) as manifest:
        lines = manifest.readlines()
        for line in lines:
            if line.startswith("#"):
                # It's a comment
                continue
            yield line.split()


def _unseal(manifest_path):
    for type_, persistent_path, ephemeral_path in _get_manifest_entries(manifest_path):
        if type_ == "d" and not os.path.isdir(persistent_path):
            os.mkdir(persistent_path)

        if type_ == "d":
            _unseal_directory(ephemeral_path, persistent_path)
        else:
            try:
                _unseal_file(ephemeral_path, persistent_path)
            except PersistentFileNotFoundError:
                _create_empty(persistent_path)
                _create_empty(ephemeral_path)


def _create_empty(path):
    open(path, 'bw').close()


def _unseal_directory(ephemeral_path, persistent_path):
    try:
        os.mkdir(ephemeral_path)
    except FileExistsError:
        pass
    for entry in os.listdir(persistent_path):
        new_persistent_path = os.path.join(persistent_path, entry)
        new_ephemeral_path = os.path.join(ephemeral_path, entry)
        _unseal_path(new_ephemeral_path, new_persistent_path)


def _unseal_path(ephemeral_path, persistent_path):
    if os.path.isdir(persistent_path):
        _unseal_directory(ephemeral_path, persistent_path)
    else:
        _unseal_file(ephemeral_path, persistent_path)


def _unseal_file(ephemeral_path, persistent_path):
    logging.debug("Unsealing file %r to %r", persistent_path, ephemeral_path)
    try:
        with open(persistent_path, 'br') as source:
            data = source.read()
    except FileNotFoundError:
        raise PersistentFileNotFoundError

    if data:
        secret, plain_text = sealing.unseal(data)
    else:
        secret = bytes()

    with open(ephemeral_path, 'bw') as dest:
        dest.write(secret)


def _seal(manifest_path):
    for type_, persistent_path, ephemeral_path in _get_manifest_entries(manifest_path):
        _seal_mapping(persistent_path, ephemeral_path)
        _check_if_deleted(persistent_path, ephemeral_path)


def _seal_mapping(persistent_path, ephemeral_path):
    if os.path.isdir(ephemeral_path):
        _seal_directory(ephemeral_path, persistent_path)
    else:
        _seal_file(ephemeral_path, persistent_path)


def _seal_directory(ephemeral_path, persistent_path):
    try:
        os.mkdir(persistent_path)
    except FileExistsError:
        pass
    for entry in os.listdir(ephemeral_path):
        new_persistent_path = os.path.join(persistent_path, entry)
        new_ephemeral_path = os.path.join(ephemeral_path, entry)

        # In Graphene chroots, for some reason files that were deleted are still being listed by os.listdir
        if not os.path.exists(new_ephemeral_path):
            continue

        _seal_mapping(new_persistent_path, new_ephemeral_path)

    for entry in os.listdir(persistent_path):
        new_persistent_path = os.path.join(persistent_path, entry)
        new_ephemeral_path = os.path.join(ephemeral_path, entry)
        _check_if_deleted(new_persistent_path, new_ephemeral_path)


def _seal_file(ephemeral_path, persistent_path):
    logging.debug("Sealing file %r to %r", ephemeral_path, persistent_path)
    with open(ephemeral_path, 'br') as source:
        secret = source.read()

        logging.debug("secret: %r", secret)

        if secret:
            data = sealing.seal(secret)
        else:
            data = bytes()

        with open(persistent_path, 'bw') as dest:
            dest.write(data)


def _check_if_deleted(persistent_path, ephemeral_path):
    if not os.path.exists(ephemeral_path):
        os.remove(persistent_path)
