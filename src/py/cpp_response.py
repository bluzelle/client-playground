# https://github.com/aio-libs - more libs here
import sys
import os
import asyncio
import concurrent.futures
import time
import uvloop
import gc
from pprint import pprint
uvloop.install()

sys.path.extend([os.getcwd()])

from build import libdb

async def get_via_socket():
    db = libdb.DB()
    test = db.newTest()
    response = test.makeResponseSharedPtr()
    test.setResponseSharedPtr(response)
    test.run_timer(2)
    await asyncio.sleep(4)
    print("Ready? ",response.is_ready())
    print("Response? ",response.get_result())

    loop = asyncio.get_event_loop()

    # reader, writer = await asyncio.open_connection('127.0.0.1', 1234,
    #                                                loop=loop)
    #
    #
    # writer.write("sample text from Python".encode())
    # request = (await reader.read(255)).decode('utf8')
    # print("Response from c++ socket == ", request)
    #
    # print('Closing the python socket ... ')
    # writer.close()


asyncio.run(get_via_socket())


