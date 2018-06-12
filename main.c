#include "main.h"

FILE* dict;
char texto[1024];

int main (void)
{
    Tree *root = NULL;
    char op;
    OPENFILE (dict, ARQ_DICT, "rt");
    treeFPush (&root);
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
                scanf ("%[^\n]", texto);
                putchar ('\n');
                if (!translate (&root)) break;
                printf ("A traducao é: %s\n", texto);
                getchar (); CLRBUF;
                break;
            case 'e': 
                treeOnFile (&root);
                return 0;
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
    if (fileCheck ())
    {
        while (!feof (dict))
        {
            if (fscanf (dict, "%[^=]=%[^\n]%*c", pt, en) > 0)
            {
                if (*root == NULL) *root = treePush (*root, pt, en);
                else treePush (*root, pt, en);
                i++;
            }
        }
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
    if (strcmp (pt, root->pt) < 0)
        root->left = treePush (root->left, pt, en);
    else if (strcmp (pt, root->pt) > 0)
        root->right = treePush (root->right, pt, en);
}

Tree* treeSearch (Tree* root, char needle[])
{
    if (!root) return NULL;
    else
    {
        if (! strcmp (needle, root->pt)) return root;
        else
        {
            if (strcmp (needle, root->pt) < 0) return treeSearch (root->left, needle);
            else return treeSearch (root->right, needle);
        }
    }
}

int translate (Tree** root)
{
    char* eng = (char*) MALLOC (strlen(texto)*2);
    int k = 0;
    *eng = '\0';
    Del* punct = strPunct ();
    char* token = strtok (texto, " ,.;");
    if (!token)
    {
        clearScreen ();
        fprintf (stderr, "Insira palavras validas!\n");
        getchar (); CLRBUF;
        return 0;
    }
    while (token)
    {
        int i=0;
        char del;
        strToLower (token);
        Tree* search = treeSearch (*root, token);
        while (!search)
        {
            treeKeyPush (root, token);
            search = treeSearch (*root, token);
        }
        k = strlen (eng);
        del = tokenPunct (token, punct, &i);
        if (k)
        {
            eng[k++] = del;
            eng[k++] = '\0';
        }
        strcat (eng, search->en);
        k+= strlen (search->en);
        if (del != ' ') eng[k++] = del;
        eng[k] = '\0';
        token = strtok (NULL, " ,.;");
    }
    k = strlen (eng);
    strcat (&eng[k], ".");
    strcpy (texto, eng);
    free (eng);
    free (punct);
    return 1;
}

void treeKeyPush (Tree** root, char pt[])
{
    char handle[50];
    printf ("Palavra:   %s   nao encontrada no dicionario, por favor, insira sua traducao: ", pt);
    CLRBUF;
    scanf ("%[^\n]", handle);
    clearScreen ();
    if (!(*root)) *root = treePush (*root, pt, handle);
    else treePush (*root, pt, handle);
}

void treeOnFile (Tree** root)
{
    if (freopen (ARQ_DICT, "wt", dict))
    {
        treeWrite (*root);
        *root = treeFree (*root);
    }
    else
    {
        fprintf (stderr, "Erro na reabertura do arquivo %s\n", ARQ_DICT);
        exit (1608);
    }
}

void treeWrite (Tree* root)
{
    if (!root) return;
    fprintf (dict, "%s=%s\n", root->pt, root->en);
    treeWrite (root->left);
    treeWrite (root->right);
}

Tree* treeFree (Tree* root)
{
    if (!root) return NULL;
    treeFree (root->left);
    treeFree (root->right);
    free (root);
}

int fileCheck (void)
{
    int i = 0;
    char ch = fgetc (dict);
    while (ch != EOF)
    {
        if (ch == '\n') i++;
        ch = fgetc (dict);
    }
    rewind (dict);
    return i;
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

void strToLower (char w[])
{
    int i;
    for (i=0; w[i]; i++)
        if (isalpha(w[i]))
            if (isupper (w[i]))
                w[i] = tolower (w[i]);
}

int strIsAlpha (char w[])
{
    int i;
    for (i=0; w[i]; i++)
        if (!isalpha(w[i])) return 0;
    return 1;
}

Del* strPunct (void)
{
    int i = strCountPunct (texto);
    if (i)
    {
        Del* pct = (Del*) MALLOC (sizeof(Del)*i);
        int j;
        i = j = 0;
        while (texto[i])
        {
            if (ispunct (texto[i]))
            {
                pct[j].info = texto[i];
                pct[j++].pos = i;
            }
            i++;
        }
        return pct;
    }
    return NULL;
}

int strCountPunct (char w[])
{
    int i, k = 0;
    for (i=0; w[i]; i++)
    {
        if (ispunct (w[i])) k++;
    }
    return k;
}

char tokenPunct (char* tok, Del* v, int* i)
{
    int len = strlen (tok);
    if (len == v[*i].pos)
    {
        return v[(*i)++].info;
    }
    return ' ';
}
