import sys
import traceback

try:
    import sgx.attestation
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()


def main():
    print("in main()")
#    sgx.attestation.create_pse_session()
    print("after create_pse_session()")
    id = sgx.attestation.get_extended_epid_group_id()
    print("id: %r" % id)


if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()
