#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void check_file(const char *path)
{
    printf("Real UID: %lu; Effective UID: %lu\n",
           (unsigned long)getuid(), (unsigned long)geteuid());
    fflush(stdout);
    FILE *file = fopen(path, "r+");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    puts("File opened for reading and writing");
    if (fclose(file) == EOF)
        perror("fclose");
}

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "file";
    puts("Before setuid:");
    check_file(path);
    /* Set the effective identity to the identity of the caller. */
    if (setuid(getuid()) == -1) {
        perror("setuid");
        return EXIT_FAILURE;
    }
    puts("After setuid:");
    check_file(path);
    return EXIT_SUCCESS;
}
