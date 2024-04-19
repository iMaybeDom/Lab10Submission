#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

/*

 * Dominick Amaral
 * COP 3502C
 * Lab Assignment 10
 * 4/18/24

 */

struct TrieNode{
    struct TrieNode *CC[ALPHABET_SIZE];
    int count;
};

struct Trie{
    struct TrieNode *root;
};

struct TrieNode *createTrieNode(){

    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pNode->count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++){
        pNode->CC[i] = NULL;
    }
    return pNode;
}

struct Trie *createTrie(){

    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

void insert(struct Trie *pTrie, char *word){

    struct TrieNode *crawl = pTrie->root;

    while (*word){
        int index = *word - 'a';

        if (!crawl->CC[index]){
            crawl->CC[index] = createTrieNode();
        }
        crawl = crawl->CC[index];
        word++;
    }
    crawl->count++;
}

int numberOfOccurances(struct Trie *pTrie, char *word){

    struct TrieNode *crawl = pTrie->root;

    while (*word){
        int index = *word - 'a';

        if (!crawl->CC[index]){
            return 0;
        }
        crawl = crawl->CC[index];
        word++;
    }
    return crawl->count;
}

void deallocateTrieNode(struct TrieNode *node){

    if (node != NULL){

        for (int i = 0; i < ALPHABET_SIZE; i++){

            if (node->CC[i] != NULL){
                deallocateTrieNode(node->CC[i]);
            }
        }
        free(node);
    }
}

struct Trie *deallocateTrie(struct Trie *pTrie){

    if (pTrie != NULL){
        deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

int readDictionary(char *filename, char **pInWords){

    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    int numWords = 0;
    char word[100];

    while (fgets(word, sizeof(word), file)){
        word[strcspn(word, "\n")] = '\0';
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void){
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);

    for (int i = 0; i < numWords; ++i){
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();

    for (int i = 0; i < numWords; i++){
        insert(pTrie, inWords[i]);
    }
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};

    for (int i = 0; i < 5; i++){
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);

    if (pTrie != NULL)
        printf("There is an error in this program\n");

    for (int i = 0; i < numWords; i++){
        free(inWords[i]);
    }
    return 0;
}