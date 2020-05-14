from cs50 import get_string
import string

text = get_string("Text: ")
words, sentences, letters = 1, 0, 0
alpha = string.ascii_letters

for letter in text:
    if letter == ' ':
        words += 1
    if letter in ['.', '?', '!']:
        sentences += 1
    if letter in alpha:
        letters += 1

L = (letters / words) * 100
S = (sentences / words) * 100
index = round(0.0588 * L - 0.296 * S - 15.8)
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")





