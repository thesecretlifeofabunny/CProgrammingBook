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
#include <stdlib.h>
#include <string.h>

#define EXPECTED_AMOUNT_OF_ARGUMENTS 2
#define HASH_TABLE_MIN_SIZE 128
// Linux VFS Standard
#define MAX_FILE_NAME_LENGTH 255
#define MAX_LINE_LENGTH 1024
#define TOKENIZER_DELIMITTER " "
// more than reasonable max for this program https://en.wikipedia.org/wiki/Longest_word_in_English
#define MAX_WORD_LENGTH 28

struct WordCount{
    size_t count;
    char*  word[];
};

static int InsertWordIntoHashTable(char* , struct WordCount*, size_t, size_t);
static int CountOccurancesOfWordsInFile(FILE*);
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

    if (NULL == file_to_count_words_from){
        printf("Unable to open the file, sowwy :c\n");
        return 1;
    }

    return CountOccurancesOfWordsInFile(file_to_count_words_from);
}


/********************************************************************************************************
*                                           PRIVATE FUNCTIONS                                           *
********************************************************************************************************/
/* TODO:
  - create hashtable array and insert and update as needed
  - sort array after
  - Fix seg fault :sob:
   
[nix-shell:~/Code/c/CProgrammingBook/TheCProgrammingLanguage/word_count_ranking]$ gcc -ggdb -o word_count_rank word_count_rank.c -fsanitize=address -fno-omit-frame-pointer

[nix-shell:~/Code/c/CProgrammingBook/TheCProgrammingLanguage/word_count_ranking]$ ./word_count_rank nixpkgs_contributing.txt | error.txt
bash: error.txt: command not found
AddressSanitizer:DEADLYSIGNAL
=================================================================
==27938==ERROR: AddressSanitizer: SEGV on unknown address 0x7f1024c42000 (pc 0x7f1024193d0c bp 0x7ffc8a9f9380 sp 0x7ffc8a9f8e68 T0)
==27938==The signal is caused by a WRITE memory access.
AddressSanitizer:DEADLYSIGNAL
AddressSanitizer: nested bug in the same thread, aborting.
*/
static int CountOccurancesOfWordsInFile(FILE* file_to_count_words_from){
    char line_from_file[MAX_LINE_LENGTH];
    char* tokenized_string;
    char* current_word;

    size_t word_count_current_count = 0;
    size_t word_count_hash_table_current_length = HASH_TABLE_MIN_SIZE;
    size_t word_count_hash_table_bit_size = HASH_TABLE_MIN_SIZE * sizeof(struct WordCount);
    struct WordCount* word_count_hash_table = (struct WordCount*)malloc(word_count_hash_table_bit_size);
    
    if(NULL == word_count_hash_table){
        printf("malloc failed to initialize the array\n");
        return 1;
    }
  
    while(NULL != fgets(line_from_file, sizeof(line_from_file), file_to_count_words_from)){
        tokenized_string = strtok(line_from_file, TOKENIZER_DELIMITTER);
        while(NULL != tokenized_string){
            // seg faults here on strncpy, unsure why as the while loop is a null check.
            // Wonder if its dirty bits or max Length exceeding.
            strncpy(current_word, tokenized_string, MAX_WORD_LENGTH);
            
            if (word_count_current_count + 1 >= word_count_hash_table_current_length){
                word_count_hash_table_bit_size = word_count_hash_table_bit_size << 1;
                struct WordCount* realloc_word_count_hash_table =
                     (struct WordCount*)realloc(word_count_hash_table, word_count_hash_table_bit_size);

                if (NULL == realloc_word_count_hash_table){
                    printf("failed to reallocate more memory once table was full already.\n");
                    return 1;
                }

                word_count_hash_table = realloc_word_count_hash_table;
                realloc_word_count_hash_table = NULL;
            }

            int insertion_result =
                InsertWordIntoHashTable(current_word, word_count_hash_table, word_count_current_count,
                                         word_count_hash_table_current_length);

            if (0 != insertion_result){
                free(word_count_hash_table);
                word_count_hash_table = NULL;
                return 1;
            }
            
            tokenized_string = strtok(NULL, TOKENIZER_DELIMITTER);
        }
    }

    
    free(word_count_hash_table);
    word_count_hash_table = NULL;
    return 0;
}

static int InsertWordIntoHashTable(char* word_to_add, struct WordCount* word_count_hash_table,
                                    size_t word_count_current_count, size_t word_count_hash_table_current_length){

    int hash_of_word = ComputeHashOfWord(word_to_add);
    if ( hash_of_word < 0){
        printf("bad hash , or bad word found");
        return 1;
    }

    int i = 0;
    while(i < word_count_hash_table_current_length){
        

        if (hash_of_word + 1 >= word_count_hash_table_current_length){
            hash_of_word = -1;
        }
        
        i++;
        hash_of_word++;
    }
    
    return 0;
}

static FILE* OpenFileSpecified(char const* const file_name, char mode[]){
    return fopen(file_name, mode);
}

// trivial ascii addition
static int ComputeHashOfWord(char* word){
    int hash_value = 0;
    char* current_charecter = word;
    
    while('\0' != *current_charecter){
        hash_value += (int)*current_charecter++;
    }

    return hash_value%HASH_TABLE_MIN_SIZE;
}

static bool IsInputNotValid(int argc, char* argv[]){
    if(EXPECTED_AMOUNT_OF_ARGUMENTS != argc) {
        printf("This program takes in at most %d argument(s) in the form of a file\n", EXPECTED_AMOUNT_OF_ARGUMENTS);
        return true;
    }
    
    if(MAX_FILE_NAME_LENGTH == strnlen(argv[1], MAX_FILE_NAME_LENGTH)){
        printf("The file name is over the limit, the limit of a file name length is %d\n", MAX_FILE_NAME_LENGTH);
        return true;
    }

    return false;
}
