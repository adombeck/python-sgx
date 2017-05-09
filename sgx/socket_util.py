import socket
import logging

from sgx.exceptions import ConnectionClosedError

logger = logging.getLogger(__name__)


def receive_bytes(sock: socket.socket, max_size=4096):
    received_raw = sock.recv(max_size).strip()
    if not received_raw:
        raise ConnectionClosedError()

    logger.debug("Received (size: %s): %s", len(received_raw), received_raw)
    return received_raw
