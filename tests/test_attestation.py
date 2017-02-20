import sys
import traceback

try:
    import sgx.attestation
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()


def main():
    sgx.attestation.create_pse_session()

if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()
