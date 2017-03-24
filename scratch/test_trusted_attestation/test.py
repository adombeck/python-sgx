import sys
import traceback

try:
    print("Importing attestation_swig")
    import attestation_swig
except ImportError as e:
    print(traceback.format_exc(), file=sys.stderr)
    exit()


def main():
    attestation_swig.main()
    print("Successfully executed attestation_swig.main()")


if __name__ == "__main__":
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        print(traceback.format_exc(), file=sys.stderr)
        exit()
