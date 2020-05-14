from cs50 import get_int
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break

space = height - 1
for block in range(1, height + 1):
    print(" " * space , end = "")
    print("#" * block + "  " + "#" * block, end = "")
    print()
    space -= 1

