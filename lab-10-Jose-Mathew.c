#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26


// Trie node structure
struct TrieNode 
{
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // Counts the number of times word is inserted
};


struct Trie 
{
    struct TrieNode *root;
};


// Create a new trie node
struct TrieNode *createTrieNode() 
{
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (pNode) 
    {
        pNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
    return pNode;
}


// Initialize trie structure
struct Trie *createTrie() 
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        pTrie->root = createTrieNode(); // Set root to new node
    }
    return pTrie;
}


// Insert words into trie
void insert(struct Trie *pTrie, char *word) 
{
    struct TrieNode *trail = pTrie->root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) 
    {
        int index = word[i] - 'a'; // Convert char to index ranging from 0 to 25
        if (!trail->children[index])
            trail->children[index] = createTrieNode(); // Create new node if it does not exist
        trail = trail->children[index];
    }
    trail->count++; // Increment count of word at end node
}


// Count number of occurrences of a word
int numberOfOccurances(struct Trie *pTrie, char *word) 
{
    struct TrieNode *trail = pTrie->root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) 
    {
        int index = word[i] - 'a';
        if (!trail->children[index])
            return 0; // If node does not exist, then word does not exist either
        trail = trail->children[index];
    }
    return trail->count; // Return count of word
}


// Deallocate nodes of trie
struct TrieNode *deallocateTrieNode(struct TrieNode *node) 
{
    for (int i = 0; i < ALPHABET_SIZE; i++) 
    {
        if (node->children[i]) 
        {
            deallocateTrieNode(node->children[i]);
        }
    }
    free(node);
    return NULL;
}


// Deallocate trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) 
{
    if (pTrie) 
    {
        pTrie->root = deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}


// Read words from file and store in array
int readDictionary(char *filename, char **pInWords) 
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return 0;

    int numWords;
    fscanf(file, "%d\n", &numWords); // Read number of words
    for (int i = 0; i < numWords; i++) 
    {
        pInWords[i] = malloc(100); // Allocate memory for each word
        fscanf(file, "%s\n", pInWords[i]); // Read each word
    }

    fclose(file);
    return numWords;
}


int main(void) 
{
    char *inWords[256];

    // read the number of the words in the dictionary 
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) 
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) 
    {
        insert(pTrie, inWords[i]);
    }

    // parse lineby line and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) 
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    for (int i = 0; i < numWords; i++) 
    {
        free(inWords[i]); // Free allocated memory for words
    }

    return 0;
}
