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
#define ZERO_OUT 0

struct WordCount{
    size_t count;
    char  *word;
};

static int InsertWordIntoHashTable(char* , struct WordCount*, size_t*, size_t);
static int CountOccurancesOfWordsInFile(FILE*);
static FILE *OpenFileSpecified(char const* const , char []);
static bool IsInputNotValid(int, char* []);
static int ComputeHashOfWord(char*);

/********************************************************************************************************
*                                           PUBLIC FUNCTIONS                                            *
********************************************************************************************************/

int main (int argc, char *argv[]) {

    if(IsInputNotValid(argc, argv)){
        return 1;
    }

    FILE *file_to_count_words_from = OpenFileSpecified(argv[1], "r");

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
*/
static int CountOccurancesOfWordsInFile(FILE *file_to_count_words_from){
    char line_from_file[MAX_LINE_LENGTH];
    char *tokenized_string;

    size_t current_word_byte_size = MAX_WORD_LENGTH * sizeof(char);
    char *current_word = (char*)calloc(MAX_WORD_LENGTH ,current_word_byte_size);

    if (NULL == current_word){
        printf("failed to allocate memory for current word pointer\n");
        return 1;
    }

    size_t word_count_current_count = 0;
    size_t word_count_hash_table_current_length = HASH_TABLE_MIN_SIZE;
    size_t word_count_hash_table_byte_size = HASH_TABLE_MIN_SIZE * sizeof(struct WordCount);
    struct WordCount *word_count_hash_table = (struct WordCount*)malloc(word_count_hash_table_byte_size);
    
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
                word_count_hash_table_current_length = word_count_hash_table_current_length << 1;
                word_count_hash_table_byte_size = word_count_hash_table_byte_size << 1;
                printf("size of byte malloc call %zul\n", word_count_hash_table_byte_size);
                struct WordCount *realloc_word_count_hash_table =
                     (struct WordCount*)realloc(word_count_hash_table, word_count_hash_table_byte_size);

                if (NULL == realloc_word_count_hash_table){
                    printf("failed to reallocate more memory once table was full already.\n");
                    printf("count of table was at %zul", word_count_current_count);
                    
                    free(current_word);
                    current_word = NULL;
                    
                    return 1;
                }

                word_count_hash_table = realloc_word_count_hash_table;
                realloc_word_count_hash_table = NULL;
            }

            int insertion_result =
                InsertWordIntoHashTable(current_word, word_count_hash_table, &word_count_current_count,
                                         word_count_hash_table_current_length);

            if (0 != insertion_result){
                free(current_word);
                current_word = NULL;
                
                free(word_count_hash_table);
                word_count_hash_table = NULL;
                return 1;
            }
            
            memset(current_word, ZERO_OUT, current_word_byte_size);
            tokenized_string = strtok(NULL, TOKENIZER_DELIMITTER);
        }
    }

    free(current_word);
    current_word = NULL;
    
    free(word_count_hash_table);
    word_count_hash_table = NULL;

    return 0;
}

// TODO add proper hash_table implementation
static int InsertWordIntoHashTable(char *word_to_add, struct WordCount *word_count_hash_table,
                                    size_t *word_count_current_count, size_t word_count_hash_table_current_length){

    int hash_of_word = ComputeHashOfWord(word_to_add);
    if ( hash_of_word < 0){
        // printf("current word that resulted in bad hash: %s\n", word_to_add);
        // printf("hash of bad value: %d\n", hash_of_word);
        // printf("bad hash , or bad word found\n");
        // non ascii charecter, fine to return 0 in testing.
        return 0;
    }


    int size_of_word_to_add = strnlen(word_to_add, MAX_WORD_LENGTH);

    if (MAX_WORD_LENGTH < size_of_word_to_add){
        printf("error in finding word length, shouldn't have been able to get here but we did :c \n");
        return 1;
    }

    size_t size_of_bytes_of_word_to_add = size_of_word_to_add * sizeof(char);

    struct WordCount to_add_word_struct;
    to_add_word_struct.count = 1;
    to_add_word_struct.word = (char *)calloc(size_of_word_to_add, size_of_bytes_of_word_to_add);
    strncpy(to_add_word_struct.word, word_to_add, size_of_word_to_add);

    int i = 0;
    while(i < word_count_hash_table_current_length){
        if (0 == word_count_hash_table[hash_of_word].count){
            word_count_hash_table[i] = to_add_word_struct;
            *word_count_current_count += 1;
            return 0;
        }

        if (hash_of_word + 1 >= word_count_hash_table_current_length){
            hash_of_word = -1;
        }
        
        i++;
        hash_of_word++;
    }

    if ( i >= word_count_hash_table_current_length){
        printf("there was an error in insertion on the table of words\n");
        return 1;
    }
    
    return 0;
}

static FILE *OpenFileSpecified(char const *const file_name, char mode[]){
    return fopen(file_name, mode);
}

// trivial ascii addition
// TODO support emojis?
/*
    [nix-shell:~/Code/c/CProgrammingBook/TheCProgrammingLanguage/word_count_ranking]$ ./result/bin/word_count_rank nixpkgs_contributing.txt
    current word that resulted in bad hash: ✔️
    hash of bad value: -56
    current word that resulted in bad hash: ❌
    hash of bad value: -117
    current word that resulted in bad hash: ⚠️
    hash of bad value: -4
    current word that resulted in bad hash: —
    hash of bad value: -10
*/
static int ComputeHashOfWord(char *word){
    int hash_value = 0;
    char *current_charecter = word;
    
    while('\0' != *current_charecter){
        hash_value += (int)*current_charecter++;
    }

    return hash_value%HASH_TABLE_MIN_SIZE;
}

static bool IsInputNotValid(int argc, char *argv[]){
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
