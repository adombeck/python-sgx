import socket
import logging
import json

from sgx.exceptions import ConnectionClosedError

logger = logging.getLogger(__name__)

MSG_TERMINATOR = b"\0"


def receive_msg(sock: socket.socket):
    json_bytes = bytes()
    while True:
        data = sock.recv(4096).strip()
        if data.endswith(MSG_TERMINATOR):
            data = data[:-1]
            json_bytes += data
            break
        else:
            json_bytes += data

    if not json_bytes:
        raise ConnectionClosedError()

    logger.debug("Received (size: %s): %s", len(json_bytes), json_bytes)

    json_string = json_bytes.decode('utf8')
    msg = json.loads(json_string)
    return msg


def send_msg(sock: socket.socket, msg):
    json_string = json.dumps(msg)
    json_bytes = json_string.encode()
    sock.sendall(json_bytes + MSG_TERMINATOR)
