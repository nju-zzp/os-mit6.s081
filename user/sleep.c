//
// Created by zzp on 11/14/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(2, "Usage: Sleep to pause for a number of ticks...\n");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit(0);
}