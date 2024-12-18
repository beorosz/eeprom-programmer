rom = bytearray(b'\xea' * 32768)
# also works: rom = bytearray([0xea] * 32768)

rom[0x7ffc] = 0x00
rom[0x7ffd] = 0x80

# rom[0] = 0xa9 # LDA
# rom[1] = 0x42 # 42h

# rom[2] = 0x8d # STA
# rom[3] = 0x00 # address LSB
# rom[4] = 0x60 # address MSB

with open("rom.bin",  "wb") as out_file:
  out_file.write(rom);
