from sgx.sealing import sealing_swig


def seal(secret, plain_text=bytes()):
    return sealing_swig.seal(secret, len(secret), plain_text, len(plain_text))

unseal = sealing_swig.unseal
