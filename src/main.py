import sys
import os
import asyncio

sys.path.extend([os.getcwd()])

from lib import libdb
db = libdb.DB()

async def slowGet():
    await asyncio.sleep(1)

async def main():
    await slowGet()


asyncio.run(main(), debug=True)


class PyCallback(libdb.Callback):

    def __init__(self):
        libdb.Callback.__init__(self)

    def run(self):
        print ("PyCallback.run()")

db.setCallback(PyCallback().__disown__())
db.call()
db.delCallback()