//
// Created by zzp on 11/14/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
//
//#include "../kernel/types.h"
//#include "../kernel/stat.h"
//#include "../kernel/fs.h"
//#include "user.h"

void findFileRecursive(char* pathName, char* fileName);

static char thisDirName[2] = {'.', '\0'};
static char previousDirName[3] = {'.', '.', '\0'};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "Error: arguments amount Wrong...\n");
        exit(1);
    }
    if (strlen(argv[2]) > DIRSIZ) {
        fprintf(2, "Error: Over the max length of file name...\n");
        exit(1);
    }
    char* directoryPath = argv[1];
    char* fileSimpleName = argv[2];
    findFileRecursive(directoryPath, fileSimpleName);
    exit(0);
}

void findFileRecursive(char* pathName, char* fileName) {
    int fd = open(pathName, 0);
    if (fd < 0) {
        fprintf(2, "Error: Wrong path...\n");
        exit(1);
    }
    struct stat direSt;
    struct stat entrySt;
    struct dirent de;

    if (fstat(fd, &direSt) < 0) {
        fprintf(2, "Error: cannot stat %s\n", pathName);
        exit(1);
    }
    if (direSt.type != T_DIR) {
        fprintf(2, "Error: %s is NOT a directory\n", pathName);
        exit(1);
    }

    char buffer[512];
    memset(buffer, 0, 512);
    int length = strlen(pathName);
    memcpy(buffer, pathName, length);
    buffer[length] = '/';

    while ((read(fd, &de, sizeof(de))) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, thisDirName) == 0 || strcmp(de.name, previousDirName) == 0) {
            continue;
        }
        memcpy(buffer + length + 1, de.name, DIRSIZ);
        if (stat(buffer, &entrySt) < 0) {
            fprintf(2, "Error: %s stat error\n", buffer);
            exit(1);
        }
        if (entrySt.type == T_FILE && strcmp(buffer + length + 1, fileName) == 0) {
            printf("%s\n", buffer);
        } else if (entrySt.type == T_DIR) {
            findFileRecursive(buffer, fileName);
        }
    }

    close(fd);
}