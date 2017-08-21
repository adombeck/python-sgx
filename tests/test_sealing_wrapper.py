#!/usr/bin/env python3-sgx

import logging

MANIFEST_PATH = "/python-sgx/sealing"

# noinspection PyBroadException
try:
    import sgx.trusted.sealing.wrapper as sealing_wrapper
except:
    logging.exception('')
    exit()


def test_append_to_file():
    path = '/tests/unsealed'

    try:
        with open(path) as f:
            i = int(f.readlines()[-1])
    except FileNotFoundError:
        logging.info("Creating file %r" % path)
        create_empty(path)
        i = 0
    except IndexError:
        i = 0

    logging.info("Appending %r to file %r", i + 1, path)
    with open(path, 'a') as f:
        f.write("%s\n" % (i+1))


def create_empty(path):
    open(path, 'bw').close()


def main():
    logging.basicConfig(level=logging.DEBUG)

    with sealing_wrapper.handle_sealing(MANIFEST_PATH):
        test_append_to_file()

        # XXX: For some reason the file modifications by the child are not synced with the parent,
        #      therefore the sealing doesn't work with child processes.
        #
        # import sys
        # exit_code = os.spawnvp(os.P_WAIT, sys.argv[1], sys.argv[1:])
        # if exit_code != 0:
        #     logging.error("Command %r exited with code %r", " ".join(sys.argv[1:]), exit_code)


if __name__ == '__main__':
    # noinspection PyBroadException
    try:
        main()
    except:
        # Without this graphene's pal does not return on exceptions
        logging.exception('')
        exit()
