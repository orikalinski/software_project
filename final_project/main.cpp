#include <iostream>
#include <string.h>

int main() {
    std::cout << "Hello, World!" << std::endl;
    char *a = "bla1=bla2";
    char *b = strtok(, "=");
    printf("%s", b);
    return 0;
}