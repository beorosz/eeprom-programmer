import os

length = 32768
rom = bytearray(b'\xea' * length)

rom = os.urandom(length)

with open("testrom.bin",  "wb") as out_file:
  out_file.write(rom);
