#!/usr/bin/env python3
"""
Helper script to convert between binary, hex and decimal.
"""

import sys

if len(sys.argv) != 2:
    print("Invalid arguments.\n\tpython convert.py [NUM]")
    print("\twhere NUM is a number in binary (indicated by 0b prefix),")
    print("\thexadecimal (indicated by 0x prefix) or decimal (no prefix).")
    sys.exit(1)

if sys.argv[1][:2] == "0b":
    num = int(sys.argv[1], 2)
elif sys.argv[1][:2] == "0x":
    num = int(sys.argv[1], 16)
else:
    num = int(sys.argv[1])

print("Binary = {:08b}, Hexadecimal = {:02x}, Decimal = {:d}".format(num, num, num))
