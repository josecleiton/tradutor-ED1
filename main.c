#include "main.h"
#ifndef INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif

FILE* dict;
char* texto;
char file;
char tk;

enum tokens
{
    WORD=1, DEL
};

int main (void)
{
    Tree *root = NULL;
    texto = NULL;
    char op;
    file = 0;
    OPENFILE (dict, ARQ_DICT, "rt");
    treeFPush (&root);
    puts ("\n\t\tTRADUTOR RUDIMENTAR DE PORTUGUES PARA INGLES\n");
    getchar ();
    CLRBUF;
    while (1)
    {
        clearScreen ();
        puts ("MENU:\n a= Traduzir texto do arquivo\n t= Traduzir frase do teclado\n c= Mudar a traducao de uma palavra\n m= Mostre palavras iniciada por determinada letra\n e= Sair\n\nopcao = ");
        scanf ("%c", &op);
        CLRBUF;
        switch (op)
        {
            case 'a':
                clearScreen();
                fileTranslate();
                printf ("A ser traduzido:\n%s\n", texto);
                if (!transl (&root))
                {
                    puts ("Nao ha o que traduzir!");
                    break;
                }
                printf ("A traducao é: %s\n", texto);
                break;
            case 't':
                clearScreen ();
                if (!texto) texto = (char*) MALLOC (2048);
                puts ("Digite uma frase: ");
                scanf ("%[^\n]", texto);
                file = 0;
                clearScreen();
                printf ("A ser traduzido:\n%s\n", texto);
                //putchar ('\n');
                if (!transl (&root))
                {
                    puts ("Nao ha o que traduzir!");
                    break;
                }
                printf ("A traducao é: %s\n", texto);
                getchar ();
                CLRBUF;
                break;
            case 'm':
                clearScreen ();
                puts ("Qual é a letra?");
                scanf ("%c", &op);
                treePrintLetter (root, op);
                getchar ();
                CLRBUF;
                break;
            case 'c':
                clearScreen();
                puts ("Digite a palavra a ser mudada: ");
                treeChange(&root);
                getchar();
                CLRBUF;
                break;
            case 'e': 
                treeOnFile (&root);
                free (texto);
                return 0;
            default: 
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
    if (strcmp (pt, root->pt) < 0) root->left = treePush (root->left, pt, en);
    else if (strcmp (pt, root->pt) > 0) root->right = treePush (root->right, pt, en);
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
/*
int translate (Tree** root)
{
    char* eng = (char*) MALLOC (strlen(texto)+100);
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
*/
int transl (Tree** root)
{
    QPnt* pnt = NULL;
    tk=0;
    int txtLen = strlen(texto);
    char* eng = (char*) MALLOC (txtLen+100);
    *eng = '\0'; 
    int k=0, count=0;
    Tree* search = NULL;
    while (pega_token(&pnt))
    {
        count++;
        search = treeSearch(*root, texto);
        if(!search) search = treeKeyPush(root, texto);
        strcat(eng, search->en);
        char del[2];
        *del = catDel(&pnt); 
        strcat(eng, del);
        if (strchr(".,;?!:><", del)) strcat(eng, " ");
    }
    return count;
}

int pega_token (QPnt** queue)
{
    if(tk)
    {
        while(isalpha(*texto)) texto++;
        *texto = ' ';
    }
    tk=0;
    BOOL isw = 0;
    if(*texto == '\0') return tk;
    while(*texto == ' ') texto++;
    isw = (isalpha(*texto)) ? 1 : 0;
    register char c;
    register int k=0;
    while(isalpha(texto[k]) && isw)
    {
        k++;
        isw=1;
    }
    tk=WORD;
    if((ispunct(texto[k]) && texto[k]!='-') || (texto[k]=='-' && !isw))
    {
        QPnt* aux = QPntPush(*queue, texto[k]);
        if(*queue==NULL) *queue = aux;
        tk=DEL;
    }
    texto[k] = '\0';
    return tk;
}

QPnt* QPntPush (QPnt* queue, char info)
{
    if(!queue)
    {
        queue = (QPnt*) MALLOC(sizeof(QPnt));
        queue->info = info;
        queue->prox = NULL;
        return queue;
    }
    queue->prox = QPntPush(queue->prox, info);
}

char catDel (QPnt** q)
{
    QPnt* aux=*q;
    if(!aux) return ' ';
    char ch = aux->info;
    QPnt* rem = aux;
    aux=aux->prox;
    free(rem);
    *q=aux;
    return ch; 
}

Tree* treeKeyPush (Tree** root, char pt[])
{
    char handle[50];
    Tree* aux = NULL;
    printf ("Palavra:   %s   nao encontrada no dicionario, por favor, insira sua traducao: ", pt);
    //CLRBUF;
    scanf ("%[^\n]%*c", handle);
    clearScreen ();
    aux = treePush (*root, pt, handle);
    if (*root==NULL)
    {
        *root = aux;
        return *root;
    }
    Tree* s = treeSearch (*root, pt);
    return s;
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

void treePrintLetter (Tree* root, char del)
{
    if (!root) return;
    treePrintLetter (root->left, del);
    if (*(root->pt) == del) puts (root->pt);
    treePrintLetter (root->right, del);
}

void treeChange (Tree** root)
{
    char w[50];
    Tree* s = NULL;
    puts ("Mudar a traducao de qual palavra?");
    scanf ("%[^\n]%*c", w);
    s = treeSearch (*root, w);
    if (s)
    {
        printf ("Digite a nova traducao de %s: ", s->pt);
        scanf("%[^\n]%*c", s->en);
        puts ("Mudancas realizadas!");
    }
    else
    {
        char w2[50];
        printf ("Palavra nao encontrada!\nInsira a traducao de %s: ", w);
        scanf("%[^\n]%*c", w2);
        if (*root == NULL) *root = treePush (*root, w, w2);
        else treePush (*root, w, w2);
        puts ("Insercao realizada!");
    }
}

void fileTranslate (void)
{
    FILE* in = NULL;
    OPENFILE (in, INPUT, "r");
    fseek(in, 0, SEEK_END);
    long fSize = ftell (in);
    rewind(in);
    if(!texto) texto = MALLOC (fSize+1);
    *texto = '\0';
    fread (texto, fSize, 1, in);
    texto[fSize] = '\0';
    fclose(in);
    //CLRBUF;
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

static int treeHeight (Tree* root)
{
    if (!root) return -1;
    return 1 + max(treeHeight(root->left),treeHeight(root->right));
}

int max (int a, int b)
{
    return (a > b) ? a : b;
}

void treePrint (Tree* root)
{
    if (!root)
    {
        fprintf(stderr,"Nao ha arvore para mostrar!\n");
        return;
    }
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
        if (ispunct (w[i])) k++;
    return k;
}

char tokenPunct (char* tok, Del* v, int* i)
{
    int len = strlen (tok);
    if (len == v[*i].pos)
        return v[(*i)++].info;
    return ' ';
}
