#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"

void superblock_ntoh(superblock_entry_t* sb) {
    sb->num_blocks = ntohl(sb->num_blocks);
    sb->block_size = ntohs(sb->block_size);
    sb->fat_start = ntohl(sb->fat_start);
    sb->fat_blocks = ntohl(sb->fat_blocks);
    sb->dir_start = ntohl(sb->dir_start);
    sb->dir_blocks = ntohl(sb->dir_blocks);
}
char *month_to_string(short m) {
    switch(m) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "?!?";
    }
}


void unpack_datetime(unsigned char *time, short *year, short *month, 
    short *day, short *hour, short *minute, short *second)
{
    assert(time != NULL);

    memcpy(year, time, 2);
    *year = htons(*year);

    *month = (unsigned short)(time[2]);
    *day = (unsigned short)(time[3]);
    *hour = (unsigned short)(time[4]);
    *minute = (unsigned short)(time[5]);
    *second = (unsigned short)(time[6]);
}


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE *f;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: ls360fs --image <imagename>\n");
        exit(1);
    }
    
    f = fopen(imagename, "r");
    if (f == NULL) {
        printf("ls360fs: image not found\n");
        exit(1);
    }
    
    fread(&sb, sizeof(superblock_entry_t),1,f);
    superblock_ntoh(&sb);
    int num_entries = sb.block_size/SIZE_DIR_ENTRY * sb.dir_blocks;
    fseek(f, sb.dir_start*sb.block_size,SEEK_SET);
    for(int i = 0; i < num_entries;i++) {
        directory_entry_t directory_entry;
        fread(&directory_entry, sizeof(directory_entry_t), 1, f);
        if ((directory_entry.status & 1) == 0) continue;
            short modify_year;
            short modify_month;
            short modify_day;
            short modify_hour;
            short modify_minute;
            short modify_second;

            unpack_datetime(directory_entry.modify_time, &modify_year, &modify_month, &modify_day, &modify_hour, &modify_minute, &modify_second);

            printf("%8d %4d-%3s-%2d %2d:%2d:%2d %s\n", ntohl(directory_entry.file_size), modify_year, month_to_string(modify_month), modify_day, modify_hour, modify_minute, modify_second, directory_entry.filename);
        
    }
    return 0; 
}

