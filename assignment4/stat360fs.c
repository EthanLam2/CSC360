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
int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE  *f;
    
    int available_blocks = 0;
    int reserved_blocks = 0;
    int allocated_blocks = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: stat360fs --image <imagename>\n");
        exit(1);
    }
    
    f = fopen(imagename, "r");
    if (f == NULL) {
        printf("stat360fs:image not found\n");
        exit(1);
    }
    fread(&sb, sizeof(superblock_entry_t),1,f);
    superblock_ntoh(&sb);
    int fat_entries = sb.num_blocks;
    int FAT[fat_entries];
    fseek(f,sb.fat_start * sb.block_size, SEEK_SET);
    fread(FAT,sizeof(FAT),1,f);
    for (int i = 0; i < fat_entries; i++) {
        int entry = ntohl(FAT[i]);
        if (entry == FAT_AVAILABLE) {
            available_blocks++;
        } else if (entry == FAT_RESERVED) {
            reserved_blocks++;
        } else {
            allocated_blocks++;
        }
    }

    char* trunc_dir = strrchr(imagename, (int)'/');
    char* file_name;
    if (trunc_dir == NULL) {
        file_name = imagename;
    } else {
        file_name = trunc_dir + 1;
    }
    
    printf("%s (%s)\n\n", sb.magic, file_name);
    printf("-------------------------------------------------\n");
    printf("%5s %6s %6s %6s %6s %6s\n", "Bsz", "Bcnt", "FATst", "FATcnt", "DIRst", "DIRcnt");
    printf("%5i %6i %6i %6i %6i %6i\n\n", sb.block_size, sb.num_blocks, sb.fat_start, sb.fat_blocks, sb.dir_start, sb.dir_blocks);
    printf("-------------------------------------------------\n");
    printf("%5s %6s %6s\n", "Free", "Resv", "Alloc");
    printf("%5i %6i %6i\n", available_blocks, reserved_blocks, allocated_blocks);
    return 0; 
}
