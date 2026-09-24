#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "hash_table.h"

int main(void)
{
    uint16_t i = 0;
    pid_t parent = getpid();
    printf("Parent: %d\n", getpid());

    while (i < 100)
    {
        pid_t child = fork();
        if (getpid() == parent)
            i++;
        else
        {
            int local_var = 10;
            printf("Child: %d, Local Variable's Value: %d, Local Variable's Address: %p\n", getpid(), local_var, &local_var);
            local_var = 15;
            break;
        }
    }

    return 0;
}