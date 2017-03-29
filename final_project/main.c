//
// Created by ori on 22/03/17.
//

#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    SP_CONFIG_MSG a;
    if ((argc == 3 && strcmp(argv[1], "-c")) || (argc != 1 && argc != 3)){
        printf("Invalid command line : use -c <config_filename>");
        return 1;
    }
    char *filename = argc == 1 ? "spcbir.config" : argv[2];
    SPConfig config = spConfigCreate(filename, &a);
    spConfigDestroy(config);
    return 0;
}