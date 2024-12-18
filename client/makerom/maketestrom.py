length = 32768
rom = bytearray(b'\xea' * length)

for index in range(0, length):
  rom[index] = index % 256

with open("testrom.bin",  "wb") as out_file:
  out_file.write(rom);
