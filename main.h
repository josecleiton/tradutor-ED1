#define INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ERRO exit (314159)

#define ARQ_DICT "dicionario.txt"
#define INPUT "test.txt"

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
**  PAUSE
**  limpeza do buffer de entrada e pause universal (independe do sistema)
*/
#define PAUSE scanf ("%*c")

typedef struct tree
{
    char pt[50];
    char en[50];
    struct tree *left, *right;
} Tree;

typedef struct treeprint
{
    Tree* info;
    struct treeprint *prox;
} TreePrint;

typedef struct queuePnt
{
    char info;
    struct queuePnt *prox;
} QPnt;

typedef short int SU;
typedef unsigned char BOOL;

int treeFPush (Tree** root);
Tree* treePush (Tree* root, char pt[], char en[]);
Tree* treeSearch (Tree* root, char needle[]);
Tree* treeKeyPush (Tree** root, char pt[]);
void treeOnFile (Tree** root);
int translate (Tree** root, char** h);
int pega_token (QPnt** queue);
QPnt* QPntPush (QPnt* queue, char info);
char catDel (QPnt** q);
void treeWrite (Tree* root);
Tree* treeFree (Tree* root);
void treePrintLetter (Tree* root, char del);
void treeChange (Tree** root);
void treePrint (Tree* root);
TreePrint* printPush (TreePrint* q, Tree* info);
TreePrint* printPop (TreePrint* q);

/*
**  FUNÇÕES AUXILIARES
*/
void fileTranslate (void);
int fileCheck (void);
int max (int a, int b);
int treeHeight (Tree* root);
int treeDistance (Tree* root, char key[]);
void strToLower (char w[]);
int strIsAlpha (char w[]);
void* MALLOC (size_t tam);
void clearScreen (void);