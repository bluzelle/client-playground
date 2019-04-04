# https://github.com/aio-libs - more libs here
from build import libdb
import sys
import os
import asyncio
import concurrent.futures
import time
import uvloop

uvloop.install()

sys.path.extend([os.getcwd()])


per_process_main_db_singleton = None


def init(db_config):
    global per_process_main_db_singleton
    per_process_main_db_singleton = libdb.DB()


def slowDBCommand(cpp_timeout):
    try:
        # *** c++ lib call ***
        return per_process_main_db_singleton.slowGet(cpp_timeout * 1000)
    except Exception as e:
        print(e)


async def get(cpp_timeout):
    loop = asyncio.get_event_loop()
    result = await loop.run_in_executor(executor, slowDBCommand, cpp_timeout)
    print("db_call_result = ", result)
    return result


async def main(tasks):
    await asyncio.gather(*tasks, return_exceptions=True)


max_workers = 4
executor = concurrent.futures.ProcessPoolExecutor(
    max_workers=max_workers, initializer=lambda: init("per_process_db_config"))

tic = time.time()
timeout_in_cpp = [5, 3]
tasks = [get(cpp_timeout=i) for i in timeout_in_cpp]
asyncio.run(main(tasks))
print("thread pool python -> blocking cpp async call took:", time.time() - tic)


async def get_via_socket():
    loop = asyncio.get_event_loop()
    reader, writer = await asyncio.open_connection('127.0.0.1', 1234,
                                                   loop=loop)

    writer.write("sample text from Python".encode())
    request = (await reader.read(255)).decode('utf8')
    print("Response from c++ socket == ", request)

    print('Closing the python socket ... ')
    writer.close()


tasks = [get_via_socket(), get(cpp_timeout=5)]


async def socket_main(tasks):
    await asyncio.gather(*tasks, return_exceptions=True)


# initializes the socket thread
libdb.DB().listen_many()
tic = time.time()
asyncio.run(socket_main(tasks))
print("Two calls with 5 sec blocks took", time.time() - tic)
