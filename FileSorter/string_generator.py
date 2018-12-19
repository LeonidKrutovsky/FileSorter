#!/usr/bin/python3

import sys
import random
import string
import asyncio
from concurrent.futures import ProcessPoolExecutor

executor = ProcessPoolExecutor(max_workers=8)
loop = asyncio.get_event_loop()

def main():
    if len(sys.argv) < 2:
        strings_number = 1024 * 1024
    else:
        strings_number = int(sys.argv[1])

    f = open('input.txt', 'w')

    tasks = [asyncio.ensure_future(get_random_strings(strings_number//8)) for _ in range(8)]
    for l in loop.run_until_complete(asyncio.gather(*tasks)):
        for s in l:
            f.write(s)

    f.close()

def worker(num: int):
    return [''.join(random.choice(
                    string.ascii_uppercase + string.digits + string.ascii_lowercase
                   ) for _ in range(random.randint(5,100))) + '\n' for _ in range(num)]

async def get_random_strings(num: int):
    return await loop.run_in_executor(executor, worker, num)

if __name__ == '__main__':
    main()


