import sys
import traceback

try:
    import sgx.trusted.attestation
    from sgx.trusted.attestation import initialize_remote_attestation, get_new_public_key
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()


def main():
    sgx.trusted.attestation.main()

    context = initialize_remote_attestation(use_pse=0)
    print("context: %r" % context)

    context = initialize_remote_attestation(use_pse=0)
    print("context: %r" % context)

    public_key = get_new_public_key(context)
    print("public_key: %r" % public_key)


if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()
