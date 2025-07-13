/*
- This is a solution to 1-23
- " Write a program to remove all comments from a C program.
    Don't forget to handle quoted strings and charecter constants properly. C com-
    ments do not nest."

- took pointers from this answer to get started, added enough formative work to not need to ref imho but no harm in contributing code sourcing.
- https://stackoverflow.com/a/9206332

*/

#include <stdio.h>
#include <stdbool.h>

#define MIN_ARGUMENTS 2
#define MAX_LINE_LENGTH 1024

#define COMMENT_OPENER_AND_CLOSER '"'
#define COMMENT_BLOCK_OPENER "/*"
#define COMMENT_BLOCK_CLOSER "*/"

// todo generalize the out_file
#define OUT_FILE "removed_comments_of_progam.c"

bool RemoveCommentsOfFile(FILE*);
void PrintLinesOfFile(FILE*);
FILE* OpenFileSpecified(char const* const);
bool NotEnoughArgumentsProvided(int);

int main(int argc, char* argv[]){
    if (NotEnoughArgumentsProvided(argc)) return 1;   

    FILE* file_to_remove_comments = OpenFileSpecified(argv[1]);
    
    if (file_to_remove_comments == NULL){
        printf("failed to open file\n");
        return 1;
    }

    // PrintLinesOfFile(file_to_remove_comments);
    bool result_of_removal = RemoveCommentsOfFile(file_to_remove_comments);

    if (!result_of_removal){
        printf("failed to remove comments sowwy :(\n");
    }
    
    fclose(file_to_remove_comments);
    return 0;
}

bool RemoveComemntsOfFile(FILE* file_to_remove_comments_from){
    
    char line[MAX_LINE_LENGTH];

    while(fgets(line, sizeof(line), file_to_remove_comments_from)){
        printf("%s", line);
    }

    return true;
}

void PrintLinesOfFile(FILE* file_to_print){
    // debugging function
    char line[MAX_LINE_LENGTH];

    while(fgets(line, sizeof(line), file_to_print)){
        printf("%s", line);
    }
}

FILE* OpenFileSpecified(char const* const file_name){
    printf("removing comments in %s", file_name);

    char line[MAX_LINE_LENGTH];
    return fopen(file_name, "r");
}

bool NotEnoughArgumentsProvided(int argc){
    if (argc < MIN_ARGUMENTS){
        printf("This program requires one argument in the form of an absolute file path\n");
        printf("./remove_comments.c hello.c\n");
        printf("This will remove the comments in hello.c and save the file as the new hello.c");
        return true;
    }
    return false;
}
