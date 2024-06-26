/* getstats.c 
 *
 * CSC 360, Summer 2023
 *
 * - If run without an argument, dumps information about the PC to STDOUT.
 *
 * - If run with a process number created by the current user, 
 *   dumps information about that process to STDOUT.
 *
 * Please change the following before submission:
 *
 * Author: Ethan Lam
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Note: You are permitted, and even encouraged, to add other
 * support functions in order to reduce duplication of code, or
 * to increase the clarity of your solution, or both.
 */

void print_process_info(char * process_num) {
    char file_header[255];
    char num[100];
    FILE *f;
    char str[255];
    char nonvoluntary[255];
    char header[255];
    char original_header[255];
    strcpy(file_header, "/proc/");
    strcpy(num,process_num);
    strcat(file_header,num);
    strcpy(original_header,file_header);
    
    f = fopen(file_header,"r");
    if (f == NULL) {
        printf("Process number %s not found.\n", process_num);
        
    } else {
        printf("Process number: %s\n",process_num);
        fclose(f);
        strcpy(header,"/status");
        strcat(file_header,header);
        
        f = fopen(file_header,"r");
        while (fgets(str,255,f)!=NULL) {
            if(strncmp("Name:", str, strlen("Name:")) ==0) {
                break;
            }
        }
        printf("%s",str);
        fclose(f);
        strcpy(header,"/cmdline");
        strcpy(file_header,original_header);
        strcat(file_header,header);
        
        f = fopen(file_header,"r");
        fgets(str, 255, f);
        printf("Filename (if any): %s\n",str);
        fclose(f);
        strcpy(header,"/status");
        strcpy(file_header,original_header);
        strcat(file_header,header);
        f = fopen(file_header,"r");
        while (fgets(str,255,f)!=NULL) {
            if(strncmp("Threads:", str, strlen("Threads:")) ==0) {
                break;
            }
        }
        printf("%s",str);
        fclose(f);
        f = fopen(file_header,"r");
        while (fgets(str,255,f)!=NULL) {
            if(strncmp("voluntary_ctxt_switches:", str, strlen("voluntary_ctxt_switches:")) ==0) {
                break;
            }
        }
        while (fgets(nonvoluntary,255,f)!=NULL) {
            if(strncmp("nonvoluntary_ctxt_switches:", nonvoluntary, strlen("nonvoluntary_ctxt_switches:")) ==0) {
                break;
            }
        }
        char * token = strtok(str, ":");
        token = strtok(NULL, ":");
        int voluntary = atoi(token);
        token = strtok(nonvoluntary, ":");
        token = strtok(NULL,":");
        int notvoluntary = atoi(token);
        int total = voluntary + notvoluntary;
        printf("Total context switches: %i\n", total);
        
        fclose(f);
        
        
    }
} 


void print_full_info() {
    FILE *f;
    char str[255];
    f = fopen("/proc/cpuinfo","r");
    while (fgets (str, 255, f) != NULL) {
        if (strncmp("model name", str, strlen("model name")) == 0) {
            break;
        }
    }
    printf("%s", str);
    fclose(f);
    f = fopen("/proc/cpuinfo","r");
    while (fgets (str, 255, f) != NULL) {
        if (strncmp("cpu cores", str, strlen("cpu cores")) == 0) {
            break;
        }
    }
    printf("%s", str);
    fclose(f);
    
    f = fopen("/proc/version","r");
    while (fgets (str, 255, f) != NULL) {
        if (strncmp("Linux version", str, strlen("Linux version")) == 0) {
            break;
        }
    }
    printf("%s", str);
    fclose(f);
    
    f = fopen("/proc/meminfo","r");
    while (fgets (str, 255, f) != NULL) {
        if (strncmp("MemTotal", str, strlen("MemTotal")) == 0) {
            break;
        }
    }
    printf("%s", str);
    fclose(f);
    
    f = fopen("/proc/uptime","r");
    fgets(str, 255, f);
    char * token = strtok(str, " ");
    int time = atoi(token);
    int days = time/86400;
    int hours = time%86400/3600;
    int minutes = time%86400%3600/60;
    int seconds = time%86400%3600%60;
    
    printf("Uptime: %i days, %i hours, %i minutes, %i seconds\n", days, hours, minutes, seconds);
    fclose(f);

}


int main(int argc, char ** argv) {  
    if (argc == 1) {
        print_full_info();
    } else {
        print_process_info(argv[1]);
    }
}
