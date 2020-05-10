// Implements a dictionary's functionality
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dictionary.h"
#include <stdint.h>

int words = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
// Number of buckets in hash table
const unsigned long N = 184567 ;
long long hash_algo(const char *str);
// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char *word2 = calloc((strlen(word) + 1), sizeof(char));
    strcpy(word2, word);
    for (int j = 0; j < strlen(word2); j++)
        word2[j] = tolower(word2[j]);
    for (node *tmp = table[hash_algo(word2)]; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->word, word2) == 0)
        {
            free(word2);
            return true;
        }
    }
    free(word2);
    return false;
}
// Hashes word to a number
long long hash_algo(const char *str)
{
    unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) ^ c;
        return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    node *n;
    char *word = calloc(46, sizeof(char));
    while (fscanf(file, "%s", word) != EOF)
    {
        n = calloc(1, sizeof(node));
        if(table[hash_algo(word)] != NULL){
            strcpy(n->word, word);
            n->next = table[hash_algo(n->word)]->next;
            table[hash_algo(n->word)]->next = n;
            words++;
            continue;
        }
        table[hash_algo(word)] = n;
        n = calloc(1, sizeof(node));
        strcpy(n->word, word);
        n->next = table[hash_algo(n->word)]->next;
        table[hash_algo(n->word)]->next = n;
        words++;
    }
    fclose(file);
    free(word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp;
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
