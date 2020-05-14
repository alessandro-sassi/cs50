// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a new node to store the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // Insert the word into the node
        strcpy(new_node->word, word);

        //hash the node
        int h = hash(word);

        //insert the node into the hashtable
        new_node->next = hashtable[h];
        hashtable[h] = new_node;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
        // Set counter for words
        int words = 0;

        // Iterate over every bucket of the hashtable
        for (int h = 0; h < N; h++)
        {
            // Point the cursor to the head of the linked list at each bucket
            node *cursor = hashtable[h];

            // Travers the list counting each word until NULL is reached
            while (cursor != NULL)
            {
                words++;
                cursor = cursor->next;
            }
        }

    return words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // set the cursor to point the head of the hash bucket
    node *cursor = hashtable[hash(word)];

    // Iterate over every element of the llist
    while (cursor != NULL)
    {
        // Check if the word is the same, if not, go to the next element in the llist
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        else
        {
            cursor = cursor->next;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate over every list of the hashtable and recursively delete every node of the lists
    for (int h = 0; h < N; h++)
    {
        node *cursor = hashtable[h];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
