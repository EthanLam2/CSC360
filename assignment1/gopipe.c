/* gopipe.c
 *
 * CSC 360, Summer 2023
 *
 * Execute up to four instructions, piping the output of each into the
 * input of the next.
 *
 * Please change the following before submission:
 *
 * Author: Ethan Lam
 */


/* Note: The following are the **ONLY** header files you are
 * permitted to use for this assignment! */


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>


int main() {
    char buff[80];
    int num_commands = 0;
    char commands[4][80];
    int i;
    char *t;
    int pid_1,pid_2,pid_3,pid_4;
    int status;
    int fd_1[2];
    int fd_2[2];
    int fd_3[2];
    
    for (i = 0;i<4;i++) {
        int bytes_read = read(0, buff, 80);
        if (bytes_read<=1) {
            break;
       }
        strncpy(commands[i], buff, bytes_read);
        if (commands[i][bytes_read - 1] == '\n') {
            commands[i][bytes_read - 1] = '\0';
            bytes_read--;
        }
        num_commands++;
            
    }
    if (num_commands == 0) {
        return 0;
    }
    if (num_commands >= 1) {
        
        int num_tokens_1 = 0;
        char *token_1[10] = {0,0,0,0,0,0,0,0,0,0};
        t = strtok(commands[0], " ");
        while (t != NULL && num_tokens_1 < 8) {
            token_1[num_tokens_1] = t;
            num_tokens_1++;
            t = strtok(NULL, " ");
        }        
        if(num_commands != 1) {
            pipe(fd_1);
        }
        if ((pid_1 = fork()) == 0) {
     
            if (num_commands != 1) {

              
                dup2(fd_1[1],1);
                close(fd_1[0]);
            }
        execve(token_1[0], token_1, 0);
        
    }
    }
    if (num_commands != 2) {
        pipe(fd_2);
    }
    if (num_commands >=2) {
        int num_tokens_2 = 0;
        char *token_2[10] = {0,0,0,0,0,0,0,0,0,0};
        t = strtok(commands[1], " ");
        while (t != NULL && num_tokens_2 < 8) {
            token_2[num_tokens_2] = t;
            num_tokens_2++;
            t = strtok(NULL, " ");
        }
        
        if ((pid_2 = fork()) == 0) {
      
        if (num_commands != 2) {
            
            dup2(fd_2[1],1);
            close(fd_2[0]);
            
        }
     
        dup2(fd_1[0], 0);
        close(fd_1[1]);
        execve(token_2[0], token_2, 0);
       
    }

       
    }
    if (num_commands != 3) {
        pipe(fd_3);
    }
    if (num_commands >=3) {
        int num_tokens_3 = 0;
        char *token_3[10] = {0,0,0,0,0,0,0,0,0,0};
        t = strtok(commands[2], " ");
        while (t != NULL && num_tokens_3 < 8) {
            token_3[num_tokens_3] = t;
            num_tokens_3++;
            t = strtok(NULL, " ");
        }
        if ((pid_3 = fork()) == 0) {
       
            if (num_commands != 3) {

                dup2(fd_3[1],1);
                close(fd_3[0]);
            }
            dup2(fd_2[0], 0);
            close(fd_1[0]);
            close(fd_1[1]);
            close(fd_2[1]);
            execve(token_3[0], token_3, 0);

        }
    }
    if (num_commands ==4) {
        int num_tokens_4 = 0;
        char *token_4[10] = {0,0,0,0,0,0,0,0,0,0};
        t = strtok(commands[3], " ");
        while (t != NULL && num_tokens_4 < 8) {
            token_4[num_tokens_4] = t;
            num_tokens_4++;
            t = strtok(NULL, " ");
        }
        if ((pid_4 = fork()) == 0) {
            dup2(fd_3[0], 0);
            close(fd_3[1]);
            close(fd_1[0]);
            close(fd_1[1]);
            close(fd_2[1]);
            close(fd_2[0]);
            execve(token_4[0], token_4, 0);
        }
    }
    close(fd_1[0]);
    close(fd_1[1]);
    close(fd_2[0]);
    close(fd_2[1]);
    close(fd_3[0]);
    close(fd_3[1]);
    
   
    if (num_commands >=1) {
        waitpid(pid_1, &status, 0);
        if (num_commands >=2) { 
            waitpid(pid_2, &status, 0); 
            if (num_commands >=3) {
                waitpid(pid_3, &status, 0);
                if (num_commands >=4) {
                    waitpid(pid_4, &status, 0);
               
                }
            }
        }
    }
   
    
}
