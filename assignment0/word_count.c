#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char *argv[]) {
    FILE *fp;
    char file_name[1024];
    int count = 0;
    char character;
    char prev = ' ';
    
    strncpy(file_name, argv[1], strlen(argv[1]) + 1);
    fp = fopen(file_name,"r");

    while ((character = fgetc(fp))!= EOF) {
        if (isspace(character)) {
            if (!isspace(prev)) {
                count++;
            }
        }
        prev = character;
    }
        

             
              
    fclose(fp);
    if (!isspace(prev)) {
                count++;
            }
    printf("%i %s \n", count,file_name);

    return 0;
}