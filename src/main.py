# https://github.com/aio-libs - more libs here
import sys
import os
import asyncio
import concurrent.futures
import time
import uvloop

uvloop.install()


sys.path.extend([os.getcwd()])

from lib import libdb

per_process_main_db_singleton = None

def init(db_config):
    global per_process_main_db_singleton
    per_process_main_db_singleton = libdb.DB()

def slowDBCommand(cpp_timeout):
    try:
        return per_process_main_db_singleton.slowGet(cpp_timeout* 1000)
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
executor = concurrent.futures.ProcessPoolExecutor(max_workers=max_workers, initializer = lambda: init("per_process_db_config"))

tic = time.time()
tasks = [get(cpp_timeout=i) for i in [3, 6, 9]]
asyncio.run(main(tasks))
print("async call spend:", time.time() - tic)


