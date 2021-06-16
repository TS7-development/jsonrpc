#! /usr/bin/env python3

import socket
import argparse


def receive_all(sock: socket.socket, max_length: int = 0, max_timeouts: int = 1):
    if not sock:
        return

    response = b''
    bytes_received: int = 0
    timeouts: int = 0

    while max_length == 0 or (max_length > 0 and bytes_received < max_length):
        try:
            response += s.recv(1)
            bytes_received += 1
        except socket.timeout:
            timeouts += 1
            if timeouts >= max_timeouts:
                return response

    return response


parser = argparse.ArgumentParser(description="Echo client")
parser.add_argument("--server", "-s", help="Address of the server", default="localhost")
parser.add_argument("--port", "-p", help="Port of the server", type=int, default=9300)
parser.add_argument("--tcp", "-t", help="Connect via TCP", action="store_true")
parser.add_argument("--udp", "-u", help="Connect via UDP", action="store_true")
parser.add_argument("--timeout", "-T", help="Defines the receive timeout (Default: 0.1)", type=float, default=0.1)
parser.add_argument("--length", "-l", help="Max length of the response (Default: 1024)", type=int, default=1024)
parser.add_argument("--quit", help="Send the quit message after all other messages", action="store_true")
parser.add_argument("message", help="Message that shall be send", nargs="+")
args = parser.parse_args()
print(args)

if not args.tcp and not args.udp:
    print("Specify a connection type with --tcp or --udp")
    exit(-1)

if args.tcp:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((args.server, args.port))
        s.settimeout(args.timeout)
        for entry in args.message:
            data = entry.encode()
            print(f"-> {data}")
            s.sendall(data)
            data = receive_all(s, max_length=args.length)
            print(f"<- {repr(data)}")

        if args.quit:
            s.sendall(":quit".encode())

        s.close()
        exit(0)

if args.udp:
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.connect((args.server, args.port))
        s.settimeout(args.timeout)
        for entry in args.message:
            data = entry.encode()
            print(f"-> {data}")
            s.sendall(data)
            data = receive_all(s, max_length=args.length)
            print(f"<- {repr(data)}")

        if args.quit:
            s.sendall(":quit".encode())

        s.close()
        exit(0)
