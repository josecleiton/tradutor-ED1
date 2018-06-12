#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ERRO exit (314159)

#define ARQ_DICT "dicionario.txt"

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
Tree* treeSearch (Tree* root, char needle[]);
void treeKeyPush (Tree** root, char pt[]);
void treeOnFile (Tree** root);
void translate (Tree** root);
void treeWrite (Tree* root);
Tree* treeFree (Tree* root);

/*
**  FUNÇÕES AUXILIARES
*/
int fileCheck (void);
void strToLower (char w[]);
int strIsAlpha (char w[]);
char* strPunct (void);
int strCountPunct (char w[]);
void* MALLOC (size_t tam);
void clearScreen (void);
