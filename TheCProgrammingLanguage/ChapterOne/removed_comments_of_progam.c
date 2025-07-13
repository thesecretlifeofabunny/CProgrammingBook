










#include <stdio.h>
#include <stdbool.h>

#define MIN_ARGUMENTS 2
#define MAX_LINE_LENGTH 1024

#define COMMENT_OPENER_AND_CLOSER '"'
#define COMMENT_BLOCK_OPENER "/*"
#define COMMENT_BLOCK_CLOSER "*/"


#define OUT_FILE "removed_comments_of_progam.c"

bool RemoveCommentsOfFile(FILE*);
void PrintLinesOfFile(FILE*);
FILE* OpenFileSpecified(char const* const, char []);
bool NotEnoughArgumentsProvided(int);

int main(int argc, char* argv[]){
    if (NotEnoughArgumentsProvided(argc)) return 1;   

    FILE* file_to_remove_comments = OpenFileSpecified(argv[1], "r");



    
    if (file_to_remove_comments == NULL){
        printf("failed to open file\n");
        return 1;
    }

    
    bool result_of_removal = RemoveCommentsOfFile(file_to_remove_comments);

    if (!result_of_removal){
        printf("failed to remove comments sowwy :(\n");
        return 1;
    }
    
    fclose(file_to_remove_comments); 
    return 0; 
}

bool RemoveCommentsOfFile(FILE* file_to_remove_comments_from){
    char line_from_file[MAX_LINE_LENGTH]; 
    char line_to_new_file[MAX_LINE_LENGTH]; 

    FILE* file_to_write_to = OpenFileSpecified(OUT_FILE, "w");

    if (file_to_write_to == NULL){
        printf("Failed to open file to write to");
        return false;
    }

    bool is_multi_line_comment = false;
    bool was_multi_line_comment_in_this_line = false;

    while(fgets(line_from_file, sizeof(line_from_file), file_to_remove_comments_from)){
        
        
        int j = 0;
        if (!is_multi_line_comment){
            was_multi_line_comment_in_this_line = false;
        }

        bool is_string = false;

        for (int i = 0 ; i < MAX_LINE_LENGTH ; i++){
            char current_char = line_from_file[i];               
            bool is_line_end = current_char == '\0' || current_char == '\n';
            bool is_single_line_comment = false;
            bool at_multi_line_comment_end = false;

            if (current_char == '"'){
                is_string = !is_string;
            }
            if (!is_line_end && !is_multi_line_comment && !is_string){
                is_single_line_comment = current_char == '/' && i+1 < MAX_LINE_LENGTH && line_from_file[i+1] == '/';
            }

            if(!is_line_end && !is_single_line_comment && !is_multi_line_comment && !is_string){
                is_multi_line_comment = current_char =='/' && i+1 < MAX_LINE_LENGTH && line_from_file[i+1] == '*';
                if (is_multi_line_comment){
                    was_multi_line_comment_in_this_line = true;
                    j = i;
                }
            }
            else if (is_multi_line_comment){
                at_multi_line_comment_end = current_char == '*' && i+1 < MAX_LINE_LENGTH && line_from_file[i+1] == '/';
            }
                       
            if (!is_line_end &&  !is_single_line_comment && !is_multi_line_comment && !was_multi_line_comment_in_this_line){
                line_to_new_file[i] = current_char;
            }
            else if (!is_line_end && !is_single_line_comment && !is_multi_line_comment && was_multi_line_comment_in_this_line){
                line_to_new_file[j] = current_char;                
                j++;
            }
            else if (is_single_line_comment && !was_multi_line_comment_in_this_line){
                line_to_new_file[i] = '\n';
                line_to_new_file[i+1] = '\0';
            }
            else if (is_single_line_comment && was_multi_line_comment_in_this_line){
                line_from_file[j] = '\n';
                line_to_new_file[j+1] = '\0';
            }
            else if (is_multi_line_comment && !at_multi_line_comment_end && !is_line_end){
                continue;
            }
            else if (is_multi_line_comment && is_line_end){
                line_to_new_file[j] = '\n';
                line_to_new_file[j+1] = '\0';
            }
            else if (is_multi_line_comment && at_multi_line_comment_end){
                is_multi_line_comment = false;
                at_multi_line_comment_end = false;
                was_multi_line_comment_in_this_line = true;
                i++;
                continue;
            }
            else {
                line_to_new_file[i] = '\n';
                line_to_new_file[i+1] = '\0';
            }
                        
            if (is_line_end || is_single_line_comment) {
                printf("%s", line_to_new_file);
                fprintf(file_to_write_to, "%s", line_to_new_file);
                break;
            }
        }
    }

    fclose(file_to_write_to);
  
    return true;
}


void PrintLinesOfFile(FILE* file_to_print){
    
    char line[MAX_LINE_LENGTH];

    while(fgets(line, sizeof(line), file_to_print)){
        printf("%s", line);
    }
}

FILE* OpenFileSpecified(char const* const file_name, char mode[]){
    char line[MAX_LINE_LENGTH];
    return fopen(file_name, mode);
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
