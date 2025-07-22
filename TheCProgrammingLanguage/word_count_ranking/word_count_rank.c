/*
Exercise 6-4. Write a program that prints the distrinct words in its input
sorted into descreasing order of frequency of occurance.
precede each word by its count.

TODO:

- I think hashtable
    - Hashtable will either be hashtable of the below struct first fit, or linked list on each fit and append to list at hash spot.
- Need to do a hash function
- struct of word + count attributes
- for each word add word to map update count if hash hit + word match
- seperate intake by punctation !?,. and spaces
- print out function
*/

#include <stdio.h>
#include <stdbool.h>

#define EXPECTED_AMOUNT_OF_ARGUMENTS 2
#define HASH_TABLE_MIN_SIZE 32

struct WordCount{
    int count;
    char* word[];
};

static bool is_input_not_valid(int, char* []);
static int compute_hash_of_word(char*);

/********************************************************************************************************
*                                           PUBLIC FUNCTIONS                                            *
********************************************************************************************************/

int main (int argc, char* argv[]) {

    if(is_input_not_valid(argc, argv)){
        printf("This program takes in at most %d argument(s) in the form of a file", EXPECTED_AMOUNT_OF_ARGUMENTS);
        return 1;
    }

    // naive before bed testing
    int hash_of_word = compute_hash_of_word(argv[1]);
    printf("hash of input is %d", hash_of_word);

    return 0;
}


/********************************************************************************************************
*                                           PRIVATE FUNCTIONS                                           *
********************************************************************************************************/

// trivial ascii addition
static int compute_hash_of_word(char* word){
    int hash_value = 0;
    char* current_charecter = word;
    
    while(*current_charecter != '\0'){
        hash_value += (int)*current_charecter++;
    }

    return hash_value%HASH_TABLE_MIN_SIZE;
}

// TODO include checking on type of input, do I support stream of charects or just a file or what?
static bool is_input_not_valid(int argc, char* argv[]){
    return argc != EXPECTED_AMOUNT_OF_ARGUMENTS;
}
