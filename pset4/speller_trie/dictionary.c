// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

//prototypes for size function and for function that frees nodes
int check_node(node *cursor);
bool free_node(node *arrow);


// Represents a trie
node *root;
node *cursor;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        cursor = root;

        for (int i = 0, n, l = strlen(word); i < l; i++)
        {
            // if word[i] is an apostrophe
            if (word[i] == 39)
            {
                n = 26;
            }
            else
            {
                n = word[i] - 97;
            }

            // if there isn't already a char, create a new node and make cursor pointing to it
            if (cursor->children[n] == NULL)
            {
                new_node(n);
                cursor = cursor->children[n];
            }
            else
            //progress trough the trie with the cursor
            {
                cursor = cursor->children[n];
            }
        }

        // reached end of the word
        cursor->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return check_node(root);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    cursor = root;

    for (int i = 0, n, l = strlen(word); i < l; i++)
    {
        if (word[i] == 39)
        {
            n = 26;
        }
        else
        {
            n = tolower(word[i]) - 97;
        }

        if (cursor->children[n] == NULL)
        {
            return false;
        }
        else
        {
            cursor = cursor->children[n];
        }
    }

    if (cursor->is_word == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return free_node(root);
}

// create a new node
bool new_node(int n)
{
    cursor->children[n] = malloc(sizeof(node));
    if (cursor->children[n] == NULL)
    {
        return false;
    }
    cursor->children[n]->is_word = false;
    for (int i = 0; i < N; i++)
    {
        cursor->children[n]->children[i] = NULL;
    }

    return true;
}

//function that count words in dictionary
int check_node(node *curs)
{
    int w = 0;

    if (curs->is_word == true)
    {
        w++;
    }

    for (int i = 0; i < N; i++)
    {
        if (curs->children[i] != NULL)
        {
            w += check_node(curs->children[i]);
        }
    }

    return w;

}

//functions that free the nodes for unloading
bool free_node(node *arrow)
{
    if (root == NULL)
    {
        return true;
    }
    for (int i = 0; i < N; i++)
    {
        if (arrow->children[i] != NULL && free_node(arrow->children[i]) == true)
        {

        }
    }

    //base case
    free(arrow);
    return true;
}