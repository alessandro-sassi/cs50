from cs50 import get_int

# prompt user for credit card number
while True:
    card = get_int("Credit Card number: ")
    if card > 0:
        break

# Luhn algorithm to check validity of the number 4003600000000014
card = str(card)
sum = 0

# multiply every other digit, and then sum them together according to the algorithm
for digit in card[-2::-2]:
    digit = str(int(digit) * 2)
    if int(digit) < 10:
        sum += int(digit)
    else:
        sum += int(digit[0]) + int(digit[1])

# add the other digits not multiplied to the sum
for digit in card[::-2]:
    sum += int(digit)

# Print the type of credit card if valid
if sum % 10 == 0:
    if (len(card) == 15) and (card[:2] in ['34', '37']):
        print("AMEX")
    elif (len(card) == 16) and (card[:2] in ['51', '52', '53', '54', '55']):
        print("MASTERCARD")
    elif (len(card) in [13, 16]) and (card[0] == '4'):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
















