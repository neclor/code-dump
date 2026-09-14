import os
from itertools import product

from dotenv import load_dotenv
from pykeepass import PyKeePass

load_dotenv()

KDBX = os.environ["KDBX"]
PREFIX = os.environ["PREFIX"]
SUFFIX = os.environ["SUFFIX"]

symbols = " -+_,.;:/|\\~=!@#$%^&*[]{}|<>?`"
alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*-_=+[]{}|;:,.<>?/`~"

def bruteforce(try_swapped: bool = False) -> None:
    orders = [(PREFIX, SUFFIX)]
    if try_swapped:
        orders.append((SUFFIX, PREFIX))

    for chars in product(symbols, repeat=1):
        middle = "".join(chars)

        for prefix, suffix in orders:
            password = prefix + middle + suffix

            print("Trying:", password)
            try:
                PyKeePass(KDBX, password=password)
                print("FOUND:", password)
                return
            except Exception:
                pass
    else:
        print("Password not found")

def main() -> None:
    bruteforce(try_swapped=True)

if __name__ == "__main__": main()
