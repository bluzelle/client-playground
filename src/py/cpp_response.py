# https://github.com/aio-libs - more libs here
import sys
import os
import asyncio
import concurrent.futures
import time
import uvloop
import gc

uvloop.install()

sys.path.extend([os.getcwd()])

from build import libdb

async def get_via_socket():
    db = libdb.DB()
    response = db.makeResponseSharedPtr()
    db.setResponseSharedPtr(response)
    print(db.getResponseSharedPtrCount())  # 2

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


