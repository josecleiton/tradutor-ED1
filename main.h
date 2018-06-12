#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ERRO exit (314159)

#define ARQ_DICT "lib/dicionario.txt"

/*
**  ABERTURA DE ARQUIVO
*/
#define OPENFILE(ptr_file, file_name, type) { \
    ptr_file = fopen (file_name, type); \
    if (! ptr_file) { \
        fprintf (stderr, "Arquivo %s nao encontrado.\n", file_name); \
        exit (2718); \
    } \
}

/*
**  CLRBUF
**  limpeza do buffer de entrada
*/
#define CLRBUF scanf ("%*c")

typedef struct tree
{
    char pt[50];
    char en[50];
    struct tree *left, *right;
} Tree;

int treeFPush (Tree** root);
Tree* treePush (Tree* root, char pt[], char en[]);
char* translate (Tree** root);



void* MALLOC (size_t tam);
void clearScreen (void);