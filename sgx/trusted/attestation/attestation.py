from sgx.trusted.attestation import attestation_swig
from sgx.trusted.attestation.attestation_swig import *


def initialize_remote_attestation(attester_public_key, use_pse):
    return attestation_swig.initialize_remote_attestation(attester_public_key, use_pse)
