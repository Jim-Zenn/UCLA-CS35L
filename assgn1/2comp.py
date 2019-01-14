from bitstring import Bits

NBITS = int(input("How many bits?\n"))

total = 0
count = 0

for a in range(-2**(NBITS - 1), 2**(NBITS - 1)):
    for b in range(-2**(NBITS - 1), 2**(NBITS - 1)):
        total += 1
        try:
            Bits(int=a * b, length=NBITS)
        except Exception as e:
            count += 1

print(total, count, count / total)
