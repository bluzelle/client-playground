import asyncio
from lib.lib import lib

async def get():
    await asyncio.sleep(1)

async def main():
    print('Hello ...')
    await get()
    lib().m2()
    print('... World!')



asyncio.run(main())