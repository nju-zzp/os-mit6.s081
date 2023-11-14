//
// Created by zzp on 11/14/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char* argv[]) {
    int pipeToChildEnds[2];
    int pipetoParentEnds[2];
    pipe(pipeToChildEnds);
    pipe(pipetoParentEnds);
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "Error in ping pong fork()");
        exit(1);
    } else if (pid == 0) {
        char buffer[16];
        close(pipeToChildEnds[1]);
        int n = read(pipeToChildEnds[0], buffer, 1);
        close(pipeToChildEnds[0]);
        if(n < 1) {
            fprintf(2, "Error thrown by child process read ping");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        close(pipetoParentEnds[0]);
        write(pipetoParentEnds[1], "z", 1);
        close(pipetoParentEnds[1]);
        exit(0);
    } else {
        close(pipeToChildEnds[0]);
        write(pipeToChildEnds[1], "z", 1);
        close(pipeToChildEnds[1]);

        char buffer[16];
        close(pipetoParentEnds[1]);
        int n = read(pipetoParentEnds[0], buffer, 1);
        close(pipetoParentEnds[0]);
        if(n < 1) {
            fprintf(2, "Error thrown by parent process read pong");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        exit(0);
    }

}