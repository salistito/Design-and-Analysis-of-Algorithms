#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr; // creating file pointer to work with files
    int n=1000000;
    unsigned int max_32bits = 4294967295;
    unsigned int random_uint;
  
    // opening file in writing mode
    fptr = fopen("program.txt", "w");
  
    if (fptr != NULL) {
        printf("File created successfully!\n");
    }
    else {
        printf("Failed to create the file.\n");
        // exit status for OS that an error occurred
        exit(1);
    }
    for(int i=0;i<1000000;i++){
        random_uint = rand() % max_32bits;
        fprintf(fptr, " %llu", random_uint);
    }
  
    // close connection
    fclose(fptr);
    return 0;
}