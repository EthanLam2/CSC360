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

int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    char *filename  = NULL;
    FILE *f;
    int block = -123;
    int remaining_file_size = -1;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            filename = argv[i+1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL) {
        fprintf(stderr, "usage: cat360fs --image <imagename> " \
            "--file <filename in image>");
        exit(1);
    }
    f = fopen(imagename, "r");
    if (f == NULL) {
        printf("cat360fs: image not found\n");
        exit(1);
    }
    fread(&sb, sizeof(superblock_entry_t),1,f);
    superblock_ntoh(&sb);
    int fat_entries = sb.num_blocks;
    int FAT[fat_entries];
    fseek(f,sb.fat_start * sb.block_size, SEEK_SET);
    fread(FAT,sizeof(FAT),1,f);
    int num_entries = sb.block_size/SIZE_DIR_ENTRY * sb.dir_blocks;
    fseek(f, sb.dir_start*sb.block_size,SEEK_SET);
    for(int i = 0; i < num_entries;i++) {
        directory_entry_t directory_entry;
        fread(&directory_entry, sizeof(directory_entry_t), 1, f);
        if (strcmp(filename, directory_entry.filename) == 0) {
            block = htonl(directory_entry.start_block);
            remaining_file_size = ntohl(directory_entry.file_size);
            break;
        }
    }
    
    if (block == -123) {
        printf("cat360fs: file not found\n");
        exit(1);
    }
    char read_data_block[sb.block_size];
    while (block != FAT_LASTBLOCK) {
        fseek(f, block * sb.block_size, SEEK_SET);
        fread(read_data_block, sizeof(char), sb.block_size, f);
    
        int num_write_bytes;
        if (remaining_file_size < sb.block_size) {
            num_write_bytes = remaining_file_size;
        } else {
            num_write_bytes = sb.block_size;
        }
    
   
        fwrite(read_data_block, sizeof(char), num_write_bytes, stdout);

        remaining_file_size -= sb.block_size;
        block = ntohl(FAT[block]);
    }

    return 0; 
}