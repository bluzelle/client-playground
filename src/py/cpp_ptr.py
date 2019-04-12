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

# print("Own?", response_ptr.thisown)
# response_ptr.thisown = True
#


db = libdb.DB()
print(db.getResponseSharedPtrCount()) # 1
response_ptr = db.getResponseSharedPtr()
print(db.getResponseSharedPtrCount()) # 2
# object_methods = [method_name for method_name in dir(response_ptr)
#                   if callable(getattr(response_ptr, method_name))]
# print(object_methods)
response_ptr = None
print(db.getResponseSharedPtrCount()) # SHOULD BE 1

