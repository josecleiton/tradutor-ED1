#define INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define ERRO exit (314159)

#define ARQ_DICT "dicionario.txt"
#define INPUT "entrada.txt"

#define TAM 50

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

typedef short int SU;

int mainOutput (Tree** root, char** txt);
int treeFPush (Tree** root);
Tree* treePush (Tree* root, char pt[], char en[]);
Tree* treeSearch (Tree* root, char needle[]);
Tree* treeKeyPush (Tree** root, char pt[]);
void treeOnFile (Tree** root);
int aux_transl (Tree** root, char** txt);
void transl (Tree** root, char tk[]);
void treeWrite (Tree* root);
Tree* treeFree (Tree* root);
void treePrintLetter (Tree* root, char del);
void treeChange (Tree** root);
void treePrintO (Tree* root);
void treePrint (Tree* root);
TreePrint* printPush (TreePrint* q, Tree* info);
TreePrint* printPop (TreePrint* q);
int countNode (Tree* root);
int countLeaf (Tree* root);

/*
**  FUNÇÕES AUXILIARES
*/
void fileTranslate (char** texto);
int fileCheck (void);
int max (int a, int b);
int treeHeight (Tree* root);
int treeDistance (Tree* root, char key[]);
void strToLower (char w[]);
bool strIsAlpha (char w[]);
bool strIsTxt (char* txt);
void* MALLOC (size_t tam);
void clearScreen (void);