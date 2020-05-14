from sys import argv, exit
import csv

# MAIN FUNCTION
def main(dict_list = [], strs = [], str_dict = {}):
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Create a list with all the elements of the csv file ordered in a dict
    with open(argv[1], 'r') as database:
        dict_reader = csv.DictReader(database)
        for row in dict_reader:
            dict_list.append(dict(row))

    # Create a list with every STRs of the csv file
    with open(argv[1], 'r') as database:
        reader = csv.reader(database)
        for row in reader:
            for e in row[1:]:
                strs.append(e)
            break

    # Save the sequence as a string
    with open(argv[2], 'r') as dna:
        dna = dna.readline()

    # Create a dictionary with the consecutives recurrences of every STRs
    for STR in strs:
        str_dict[STR] = str(str_count(STR, dna))

    # Return the name if the STRs match correctly in the dna sequence
    for DICT in range(len(dict_list)):
        str_check = 0
        for STR in str_dict:
            if str_dict[STR] != dict_list[DICT][STR]:
                break
            str_check += 1
            if str_check == len(dict_list[DICT]) - 1:
                return dict_list[DICT]['name']
    return "No match"


def str_count(STR, dna, count = 0, best = 0, i = 0):
    """ Returns how many consecutives times the STR appear in the dna sequence file """
    while i < len(dna):
        if dna[i : len(STR) + i] == STR:
            i += len(STR)
            count += 1
        else:
            if count > best:
                best = count
            count = 0
            i += 1
    return best

print(main())