#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *text;
    struct Node *next;
} Node;

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head->text);
        free(head);
        head = next;
    }
}

int main(void)
{
    char buffer[4096];
    Node *head = NULL;
    Node *tail = NULL;

    puts("Enter lines (up to 4094 characters each).");
    puts("To finish, enter a line beginning with a dot.");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '.')
            break;

        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[--length] = '\0';
        } else if (length == sizeof(buffer) - 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard the rest of an overly long line. */
            }
            fputs("Line too long; it was skipped.\n", stderr);
            continue;
        }

        Node *node = malloc(sizeof(*node));
        if (node == NULL) {
            fputs("Cannot allocate memory for a list node.\n", stderr);
            free_list(head);
            return EXIT_FAILURE;
        }
        node->text = malloc(length + 1);
        if (node->text == NULL) {
            fputs("Cannot allocate memory for a line.\n", stderr);
            free(node);
            free_list(head);
            return EXIT_FAILURE;
        }
        memcpy(node->text, buffer, length + 1);
        node->next = NULL;

        if (tail == NULL)
            head = node;
        else
            tail->next = node;
        tail = node;
    }

    if (ferror(stdin)) {
        fputs("Input error.\n", stderr);
        free_list(head);
        return EXIT_FAILURE;
    }

    puts("Saved lines:");
    for (Node *node = head; node != NULL; node = node->next)
        puts(node->text);

    free_list(head);
    return EXIT_SUCCESS;
}
