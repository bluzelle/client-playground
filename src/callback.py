import sys
import os

sys.path.extend([os.getcwd()])

from lib import libdb

class PyCallback(libdb.Callback):

    def __init__(self):
        libdb.Callback.__init__(self)

    def run(self):
        print ("PyCallback.run()")

db = libdb.DB()

db.setCallback(PyCallback().__disown__())
db.call()
db.delCallback()