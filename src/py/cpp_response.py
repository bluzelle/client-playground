# https://github.com/aio-libs - more libs here
from pprint import pprint
import sys
import os
import asyncio
import concurrent.futures
import time
import uvloop
import gc
from socket import *

import asyncio
import warnings

from udp_support import *

#uvloop.install()

sys.path.extend([os.getcwd()])

from build import libdb

async def get_via_socket():
    my_port = 1246
    # Create a local UDP enpoint
    local = await open_local_endpoint('127.0.0.1', my_port)

    db = libdb.DB()
    test = db.newTest()
    response = test.makeResponseSharedPtr()
    test.setResponseSharedPtr(response)
    cpp_port = response.get_signal_id(my_port)
    print("response.result.is_ready? ",response.is_ready())
    print("CPP process_request()..... ")
    test.process_request(timeout=1)
    loop = asyncio.get_event_loop()

    # Create a remote UDP enpoint, pointing to the first one
    #remote = await open_remote_endpoint(*local.address)

    # The remote endpoint sends a datagram
    # remote.send(b'Hey Hey, My My')

    # The local endpoint receives the datagram, along with the address
    print("Python: waiting for UDP signal..... ")

    data, address = await local.receive()

    # Print: Got 'Hey Hey, My My' from 127.0.0.1 port 50603
    print(f"Got {data!r} from {address[0]} port {address[1]}")

    print("CPP result ready? ", response.is_ready())
    print("CPP response => ", response.get_result())

asyncio.run(get_via_socket())


# simple udp send command
# echo -n "b" | /usr/bin/nc -4u -w0 localhost 1246