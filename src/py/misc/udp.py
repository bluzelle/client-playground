import asyncio
import socket

class UDPClient():

    def __init__(self, host, port, loop=None):
        self._loop = asyncio.get_event_loop() if loop is None else loop
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sock.setblocking(False)
        self._addr = (host, port)
        self._future = None
        self._data = None

    def sendto(self, data):
        self._future = asyncio.Future(loop=self._loop)
        self.data = data if isinstance(data, bytes) else str(data).encode('utf-8')
        loop.add_writer(self._sock.fileno(), self._try_to_send)
        return self._future

    def _try_to_send(self):
        try:
            self._sock.sendto(self.data, self._addr)
        except (BlockingIOError, InterruptedError):
            return
        except Exception as exc:
            self.abort(exc)
        else:
            self.close()
            self._future.set_result(True)

    def abort(self, exc):
        self.close()
        self._future.set_exception(exc)

    def close(self):
        self._loop.remove_writer(self._sock.fileno())
        self._sock.close()

if __name__ == '__main__':
    # @asyncio.coroutine
    # def test():
    #     yield from UDPClient('127.0.0.1', 1234).sendto('ok')

    # or 3.5+ syntax
    async def test():
        await UDPClient('127.0.0.1', 1246).sendto('ok')
    loop = asyncio.get_event_loop()
    loop.run_until_complete(test())