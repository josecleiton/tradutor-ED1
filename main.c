#include "main.h"

FILE* dict;
char frase[500];

int main (void)
{
    Tree *root;
    char op;
    OPENFILE (dict, ARQ_DICT, "at");
    treeFInsere (&root);
    puts ("\n\t\tTRADUTOR RUDIMENTAR DE PORTUGUES PARA INGLES\n");
    getchar ();
    while (1)
    {
        clearScreen ();
        puts ("MENU:\n t= Traduzir frase do teclado\n e= Sair\n\nopcao = ");
        scanf ("%c", &op);
        switch (op)
        {
            case 't':
                clearScreen ();
                CLRBUF;
                puts ("Digite uma frase: ");
                scanf ("%[^\n]", frase);
                putchar ('\n');
                printf (translate(&root));
                getchar (); CLRBUF;
                break;
            case 'e': return 0;
            default: 
                CLRBUF;
                puts ("Opcao invalida.\n");
                getchar (); CLRBUF;
        }
    }

    return 0;
}

int treeFPush (Tree** root)
{
    int i = 0;
    char pt[50], en[50];
    while (!feof (dict))
    {
        fscanf (dict, "%[^,],%[^\n]", pt, en);
        if (*root == NULL) *root = treePush (*root, pt, en);
        treePush (*root, pt, en);
        i++;
    }
    return i;
}

Tree* treePush (Tree* root, char pt[], char en[])
{
    if (!root)
    {
        root = (Tree*) MALLOC (sizeof(Tree));
        strcpy (root->pt, pt);
        strcpy (root->en, en);
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (strcmp (root->pt, pt) > 0)
        root->left = treePush (root->left, pt, en);
    else if (strcmp (root->pt, pt) < 0)
        root->right = treePush (root->right, pt, en);
}

Tree* treeSearch (Tree* root, char pt[])
{
    if (!root) return NULL;
    treeSearch (root->left, pt);
    if (!strcmp(root->pt, pt)) return root;
    treeSearch (root->right, pt);
}

char* translate (Tree** root)
{
    char* main = (char*) MALLOC (strlen(frase)*2);
    *main = '\0';
    char* token = strtok (frase, " ,.;");
    while (token)
    {
        Tree* search = treeSearch (*root, token);
        if (!search)
            treeKeyPush(root, token);
        else
        {
            int k = strlen (main);
            if (k)
            {
                main[k++] = ' ';
                main[k++] = '\0';
            }
            strcat (main, search->en);
            k+= strlen (search->en);
            main[k] = '\0';
        }
        token = strtok (NULL, " ,.;");
    }
    strcpy (frase, main);
    free (main);
}

void treeKeyPush (Tree** root, char pt[])
{
    char handle[50];
    puts ("Palavra nao encontrada no dicionario, por favor, insira sua traducao: ");
    scanf ("%[^\n]", handle);
    if (!(*root)) *root = treePush (*root, pt, handle);
    treePush (*root, pt, handle);
}

void* MALLOC (size_t tam)
{
    void* ptr = malloc (tam);
    if (! ptr)
    {
        fprintf (stderr, "Memoria insuficiente.");
        ERRO;
    }
    return ptr;
}

void clearScreen (void)
{
    int n;
    for (n = 0; n < 10; n++)
      printf ("\n\n\n\n\n\n\n\n\n\n");
}