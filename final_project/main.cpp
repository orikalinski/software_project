#include <string.h>
#include <stdio.h>

#include "main_aux.h"

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    SP_CONFIG_MSG msg;
    if ((argc == 3 && strcmp(argv[1], "-c")) || (argc != 1 && argc != 3)) {
        printf("Invalid command line : use -c <config_filename>");
        return 1;
    }
    const char *filename = argc == 1 ? "spcbir.config" : argv[2];
    const SPConfig config = spConfigCreate(filename, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    int numberOfImages = spConfigGetNumOfImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    if (createLogger(config) != 0) return 1;
    sp::ImageProc imageProc(config);
    KDTreeNode *root;
    if ((root = getKDTree(config, imageProc, numberOfImages)) == NULL) return 1;
    if (handleQueryImages(config, root, imageProc, numberOfImages) != 0) return 1;
    return 0;
}