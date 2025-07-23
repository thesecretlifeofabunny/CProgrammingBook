/*
Exercise 6-4. Write a program that prints the distrinct words in its input
sorted into descreasing order of frequency of occurance.
precede each word by its count.

TODO:

- Hash Table of Structs
    - Default size should be sane, unsure if 2^5 or 2^6 starting or even 2^4?
    - malloc() (don't need to calloc) for first iteration
    - realloc() on currentsize <= HashTable.size - 1

    - insert on word hash using ComputeHashOfWord
    - simplest naive is first fit, so I will do that

- can I pass strtok char* to COmputeHashOfWord? Need to investigate!
*/

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define EXPECTED_AMOUNT_OF_ARGUMENTS 2
#define HASH_TABLE_MIN_SIZE 128
// Linux VFS Standard
#define MAX_FILE_NAME_LENGTH 255
#define MAX_LINE_LENGTH 1024
#define TOKENIZER_DELIMITTER " "

struct WordCount{
    int count;
    char* word[];
};

static void CountOccurancesOfWordsInFile(FILE*);
static FILE* OpenFileSpecified(char const* const , char []);
static bool IsInputNotValid(int, char* []);
static int ComputeHashOfWord(char*);

/********************************************************************************************************
*                                           PUBLIC FUNCTIONS                                            *
********************************************************************************************************/

int main (int argc, char* argv[]) {

    if(IsInputNotValid(argc, argv)){
        return 1;
    }

    FILE* file_to_count_words_from = OpenFileSpecified(argv[1], "r");

    if (file_to_count_words_from == NULL){
        printf("Unable to open the file, sowwy :c\n");
        return 1;
    }

    CountOccurancesOfWordsInFile(file_to_count_words_from);
    
    return 0;
}


/********************************************************************************************************
*                                           PRIVATE FUNCTIONS                                           *
********************************************************************************************************/
static void CountOccurancesOfWordsInFile(FILE* file_to_count_words_from){
    char line_from_file[MAX_LINE_LENGTH];
    char* tokenized_string;
    
    while(fgets(line_from_file, sizeof(line_from_file), file_to_count_words_from)){
        tokenized_string = strtok(line_from_file, TOKENIZER_DELIMITTER);
        while(tokenized_string != NULL){
            
            tokenized_string = strtok(NULL, TOKENIZER_DELIMITTER);
        }
    }
}

static FILE* OpenFileSpecified(char const* const file_name, char mode[]){
    return fopen(file_name, mode);
}

// trivial ascii addition
static int ComputeHashOfWord(char* word){
    int hash_value = 0;
    char* current_charecter = word;
    
    while(*current_charecter != '\0'){
        hash_value += (int)*current_charecter++;
    }

    return hash_value%HASH_TABLE_MIN_SIZE;
}

static bool IsInputNotValid(int argc, char* argv[]){
    if(argc != EXPECTED_AMOUNT_OF_ARGUMENTS) {
        printf("This program takes in at most %d argument(s) in the form of a file\n", EXPECTED_AMOUNT_OF_ARGUMENTS);
        return true;
    }
    
    if(MAX_FILE_NAME_LENGTH == strnlen(argv[1], MAX_FILE_NAME_LENGTH)){
        printf("The file name is over the limit, the limit of a file name length is %d\n", MAX_FILE_NAME_LENGTH);
        return true;
    }

    return false;
}
