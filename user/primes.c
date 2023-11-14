//
// Created by zzp on 11/14/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

void childProcess(int* parentPipeEnds);

int main(int argc, char* argv[]) {
    int pipeEnds[2];
    pipe(pipeEnds);

    int num = 0;
    int childPid = 0;
    for (int i = 2; i <= 35; ++i) {
        if (num == 0) {
            num = i;
            printf("prime %d\n", num);
        } else if (i % num != 0) {
            // send to next process
            if (childPid == 0) {
                childPid = fork();
                if (childPid > 0) {
                    close(pipeEnds[0]);
                }
            }
            if (childPid == 0) {
                // child process
                // read -> processing -> writing -> wait -> exit;
                childProcess(pipeEnds);
            } else {
                // parent process
                // sending number and continue
                char *numPtr = (char *) &i;
                write(pipeEnds[1], numPtr, 4);
                continue;
            }
        }
    }
    close(pipeEnds[1]);
    wait(0);
    exit(0);
}

void childProcess(int* parentPipeEnds) {
    close(parentPipeEnds[1]);

    int toChildPipeEnds[2];
    pipe(toChildPipeEnds);

    int childPid = 0;

    int num = 0;
    char buffer[8];
    while (read(parentPipeEnds[0], buffer, 4) == 4) {
        int* receivedNumPtr = (int *) buffer;
        int receivedNum = *receivedNumPtr;
        if (num == 0) {
            num = receivedNum;
            printf("prime %d\n", num);
        } else if (receivedNum % num != 0) {
            if (childPid == 0) {
                childPid = fork();
                if (childPid > 0) {
                    close(toChildPipeEnds[0]);
                }
            }
            if (childPid == 0) {
                // child
                childProcess(toChildPipeEnds);
            } else {
                // me write
                char *ptr = (char *) &receivedNum;
                write(toChildPipeEnds[1], ptr, 4);
                continue;
            }
        }
    }
    close(toChildPipeEnds[1]);
    wait(0);
    exit(0);
}

