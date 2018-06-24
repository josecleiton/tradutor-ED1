#include "main.h"
#ifndef INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif

/*
PROBLEMA DE BUF APARENTEMENTE RESOLVIDO
*/

FILE* dict;
char* texto;
char tk;
char token[50];
char op;

enum tokens
{
    WORD=1, DEL
};

int main (void)
{
    Tree *root = NULL;
    char* ftxt;
    ftxt = texto = NULL;
    OPENFILE (dict, ARQ_DICT, "rt");
    treeFPush (&root);
    puts ("\n\t\tTRADUTOR RUDIMENTAR DE PORTUGUES PARA INGLES\n");
    PAUSE;
    while (1)
    {
        clearScreen ();
        puts ("MENU:\n a= Traduzir texto do arquivo\n t= Traduzir frase do teclado\n c= Mudar a traducao de uma palavra\n m= Mostre palavras iniciada por determinada letra\n p= Print Tree\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen();
                fileTranslate();
                printf ("A ser traduzido:\n%s\n", texto);
                if (!translate (&root, &ftxt))
                {
                    puts ("Nao ha o que traduzir!");
                    break;
                }
                printf ("A traducao é: \n%s\n", texto);
                PAUSE;
                break;
            case 't':
                clearScreen ();
                if (!texto) texto = (char*) MALLOC (2048);
                puts ("Digite uma frase: ");
                scanf ("%[^\n]", texto);
                clearScreen();
                printf ("A ser traduzido:\n%s\n", texto);
                if (!translate (&root, &ftxt))
                {
                    puts ("Insira um texto valido!");
                    break;
                }
                printf ("A traducao é: %s\n", texto);
                PAUSE;
                break;
            case 'm':
                clearScreen ();
                puts ("Qual é a letra?");
                scanf ("%c%*c", &op);
                treePrintLetter (root, op);
                PAUSE;
                break;
            case 'p':
                clearScreen ();
                printf ("Arvore de altura %d em memoria:\n<pt> | <en>\n\n", treeHeight(root));
                treePrint(root);
                PAUSE;
                break;
            case 'c':
                clearScreen();
                puts ("Digite a palavra a ser mudada: ");
                treeChange(&root);
                PAUSE;
                break;
            case 'e': 
                treeOnFile (&root);
                free (ftxt);
                return 0;
            default: 
                puts ("Opcao invalida.\n");
                PAUSE;
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

int translate (Tree** root, char** ftxt)
{
    int k;
    for(k=0; texto[k]; k++)
    {
        if(isdigit(texto[k]))
            return 0;
    }
    k=0;
    QPnt* pnt = NULL;
    tk=0;
    size_t txtLen = strlen(texto);
    char* eng = (char*) MALLOC (txtLen+100);
    *eng = '\0'; 
    int count=0;
    Tree* search = NULL;
    *ftxt = texto;
    while (pega_token(&pnt))
    {
        count++;
        search = treeSearch(*root, token);
        if(!search) search = treeKeyPush(root, token);
        strcat(eng, search->en);
        char del[2];
        BOOL fl=0;
        for(*del = catDel(&pnt); *del != ' '; *del=catDel(&pnt), fl=1){
            strcat(eng, del);
            if((*del!='\n' && *del!='\r') || (*del=='-' && tk==DEL)) strcat(eng, " ");
        }
        if(!fl) strcat(eng, " ");
        

    }
    texto = *ftxt;
    if(count) strcpy (texto, eng);
    free(eng);
    return count;
}

int pega_token (QPnt** queue)
{
    tk=0;
    BOOL isw = 0;
    if(*texto == '\0') return tk;
    while(*texto == ' ') texto++;
    if(*texto == '\0') return tk;
    isw = (isalpha(*texto)) ? 1 : 0;
    register char c;
    register int k=0, i=0;
    while(isalpha(texto[k]) && isw)
    {
        k++;
        isw=1;
    }
    tk=WORD;
    i=k;
    if(((ispunct(texto[k]) || texto[k]=='\n' || texto[k] =='\r' )&& texto[k]!='-') || (texto[k]=='-' && !isw))
    {
        tk=DEL;
        while(ispunct(texto[k]) || texto[k]=='\n' || texto[k] =='\r')
        {
            QPnt* aux = QPntPush(*queue, texto[k++]);
            if(*queue==NULL) *queue = aux;
        }
    }
    texto[i] = '\0';
    int flag = (i==k) ? 1:0;
    strcpy(token, texto);
    while(k){
        texto++;
        k--;
    }
    if (flag) *texto = ' ';
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
    if(op=='t') PAUSE;
    op='b';
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
        char w2[50] = {'\0'};
        printf ("Palavra nao encontrada!\nInsira a traducao de %s: ", w);
        while(*w2=='\0')
        {
            scanf("%[^\n]%*c", w2);
            if(!strIsAlpha(w2))
            {
                puts("Palavra invalida!");
                *w2='\0';
            }
        }
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
    fread (texto, fSize, 1, in);
    texto[fSize] = '\0';
    fclose(in);
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

int treeHeight (Tree* root)
{
    if (!root) return -1;
    return 1 + max(treeHeight(root->left),treeHeight(root->right));
}

int max (int a, int b)
{
    return (a > b) ? a : b;
}

int treeDistance (Tree* root, char key[])
{
    if (!root) return -1;
    int distance = -1;
    if (!strcmp(root->pt, key) || (distance = treeDistance(root->left, key)) >=0 || (distance = treeDistance(root->right, key)) >= 0)
        return distance + 1;
    return distance;
}

void treePrint (Tree* root)
{
    if (!root)
    {
        fprintf(stderr,"Nao ha arvore em memoria!\n");
        return;
    }
    TreePrint* q = NULL;
    int t = 0, rd = 0;
    for(q = printPush(q, root); q; q = printPop(q))
    {
        if(q->info->left) printPush (q, q->info->left);
        if(q->info->right) printPush (q, q->info->right);
        t=treeDistance (root, q->info->pt);
        /*int i;
        //for (t=treeDistance (root, q->info->pt), i=t; i>0; i--); //putchar ('\t');
        */
        if (t)
        {
            int tmp;
            for(tmp=t*4; tmp>=0; tmp--) putchar('_');
            //putchar(' ');
        }
        printf("[%s=%s]\n", q->info->pt, q->info->en);
    }

}

TreePrint* printPush (TreePrint* q, Tree* info)
{
    if (!q)
    {
        q = (TreePrint*) MALLOC (sizeof(TreePrint));
        q->info = info;
        q->prox = NULL;
        return q;
    }
    q->prox = printPush(q->prox, info);
}

TreePrint* printPop (TreePrint* q)
{
    if(!q) return NULL;
    TreePrint* aux = q->prox;
    free(q);
    return aux;
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