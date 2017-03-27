//
// Created by ori on 22/03/17.
//

#include "SPConfig.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {
    SP_CONFIG_MSG a;
    SPConfig config = spConfigCreate("/home/ori/ClionProjects/final_project/conf.config", &a);
    spConfigDestroy(config);
    return 0;
}