from contextlib import contextmanager

from sgx.trusted.attestation import attestation_swig
from sgx.trusted.attestation.attestation_swig import *


def initialize_remote_attestation(attester_public_key, use_pse=False):
    return attestation_swig.initialize_remote_attestation(attester_public_key, 1 if use_pse else 0)


@contextmanager
def remote_attestation_context(attester_public_key, use_pse=False):
    ra_context = initialize_remote_attestation(attester_public_key, use_pse)
    yield ra_context
    close_remote_attestation(ra_context)
