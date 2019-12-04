//
// Created by russell on 12/3/19.
//

#include <stdio.h>
#include <string.h>

extern
int create_and_write_shared_memory(char *mmap_key, char *value, unsigned int size);

int main(int argc, char **argv)
{
    char *key = "/introduction";
    char *intro = "Hello, I am George and I like to flex";
    create_and_write_shared_memory(key, intro, strlen(intro));

    return 0;
}