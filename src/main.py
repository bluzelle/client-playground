import sys
import os
import asyncio

sys.path.extend([os.getcwd()])

from lib import libdb
db = libdb.DB()

GET_TIMEOUT = 2 * 1000

async def slowGet():
    await db.slowGet(GET_TIMEOUT)

async def main():
    await slowGet()


#asyncio.run(main(), debug=True)
db.slowGet(GET_TIMEOUT)

class PyCallback(libdb.Callback):

    def __init__(self):
        libdb.Callback.__init__(self)

    def run(self):
        print ("PyCallback.run()")

db.setCallback(PyCallback().__disown__())
db.call()
db.delCallback()