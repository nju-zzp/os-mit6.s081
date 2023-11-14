//
// Created by zzp on 11/14/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
//
//#include "../kernel/types.h"
//#include "../kernel/stat.h"
//#include "../kernel/param.h"
//#include "user.h"

int main(int argc, char *argv[]) {
    char* programName = argv[1];
    char* innerArgsArray[MAXARG];
    innerArgsArray[0] = programName;

    char** innerArgvPointer = innerArgsArray + 1;
    char** outerArgvPointer = argv + 2;
    while (*outerArgvPointer != 0) {
        *(innerArgvPointer++) = *(outerArgvPointer++);
    }

    char buffer[512];
    memset(buffer, 0, 512);
    while (*(gets(buffer, 512)) > 0) {
        char** p = innerArgvPointer;
        buffer[strlen(buffer) - 1] = 0;
        *(p++) = buffer;
        *p = 0;
        int pid = fork();
        if (pid == 0) {
            exec(programName, innerArgsArray);
        } else {
            wait(0);
            continue;
        }
    }
    exit(0);
}