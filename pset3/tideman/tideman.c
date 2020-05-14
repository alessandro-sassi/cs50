#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int source;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int c);
bool check_source(int c);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);


        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (strcmp(name, candidates[c]) == 0)
        {
            ranks[rank] = c;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[(ranks[i])][(ranks[j])]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i_strongest = 0, swap = 0;
    pair temp;

    // repeat sorting of pairs until no more swaps needed
    while (swap < pair_count)
    {
        for (int i = swap, strongest = 0; i < pair_count; i++)
        {
            // check for strenght of pairs
            if (preferences[pairs[i].winner][pairs[i].loser] > strongest)
            {
                // record the strongest victory and the index of where that pair is located in the array
                strongest = preferences[pairs[i].winner][pairs[i].loser];
                i_strongest = i;
            }
        }

        // Swap the first pair in the array with the pair with the strongest winner
        temp = pairs[i_strongest];
        pairs[i_strongest] = pairs[swap];
        pairs[swap] = temp;
        swap++;
    }
    for (int i = 0; i < pair_count; i++)
    {
        printf("Pair %s-%s strenght: %i\n", candidates[pairs[i].winner], candidates[pairs[i].loser],
               preferences[pairs[i].winner][pairs[i].loser]);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        source = pairs[i].winner;

        if (check_cycle(pairs[i].loser) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
    return;
}

// Check if no cycles is going to be created
bool check_cycle(int c)
{
    if (locked[c][source] == true)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[c][i] == true)
            {
                return check_cycle(i);
            }
        }
        return false;
    }
}


// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (check_source(i) == true)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

// Check if a candidate has no arrow pointing to him
bool check_source(int c)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[j][c] == true)
        {
            return false;
        }
        else if (j == candidate_count - 1)
        {
            break;
        }
    }
    return true;
}
