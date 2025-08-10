#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char *args[] = {"/bin/ls", NULL, NULL};

    if (execv(args[0], args) == -1)
    {
        perror("execv failed");
        return (1);
    }

    return (0);
}
