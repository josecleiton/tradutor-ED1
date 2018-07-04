#include "main.h"
#ifndef INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#endif


/*
PROBLEMA DE BUF APARENTEMENTE RESOLVIDO
*/

FILE* dict;

int main (void)
{
    Tree *root = NULL;
    char* texto = NULL;
    char handle[TAM] = {'\0'};
    char op;
    int count;
    OPENFILE (dict, ARQ_DICT, "rt");
    treeFPush (&root);
    //printf("NOS = %d\nFOLHAS = %d\n", countNode(root), countLeaf(root));
    puts ("\n\t\tTRADUTOR RUDIMENTAR DE PORTUGUES PARA INGLES\n");
    PAUSE;
    while (1)
    {
        clearScreen ();
        puts ("MENU:\n a= Traduzir texto do arquivo\n t= Traduzir frase do teclado\n c= Mudar a traducao de uma palavra\n m= Mostre palavras iniciada por determinada letra\n p= Print Tree\n h= Mostra arvore por altura\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen();
                fileTranslate(&texto);
                count = mainOutput(&root, &texto);
                PAUSE;
                break;
            case 't':
                clearScreen ();
                if (!texto) texto = (char*) MALLOC (2048);
                puts ("Digite uma frase: ");
                scanf ("%[^\n]%*c", texto);
                count = mainOutput(&root, &texto);
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
                clearScreen();
                puts("Arvore em memoria: ");
                treePrintO(root);
                PAUSE;
                break;
            case 'r':
                clearScreen();
                puts("Digite a palavra para retirar: ");
                scanf("%[^\n]%*c", handle);
                printf("Palavra: %s retirada!\n", handle);
                root=pop(root, handle);
                PAUSE;
                break;
            case 'h':
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
                free (texto);
                return 0;
            default: 
                puts ("Opcao invalida.\n");
                PAUSE;
        }
    }
    
    return 0;
}

int mainOutput (Tree** root, char** txt)
{
    int count;
    clearScreen();
    printf ("A ser traduzido:\n\n%s\n\n\n", *txt);
    count = aux_transl (root, txt);
    if (!count)
    {
        fprintf (stderr, "Insira um texto valido!\n");
        return 0;
    }
    printf ("A traducao é: \n\n%s\n\n\tNumero de palavras traduzidas: %d!\n\n", *txt, count);
    return count;
}

int treeFPush (Tree** root)
{
    int i = 0;
    char pt[TAM], en[TAM];
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

int aux_transl (Tree** root, char** txt)
{
    char* texto = *txt;
    if(!texto || *texto == '\0' || !strIsTxt(texto)) return 0; // senão houver texto retorne falso
    size_t len = strlen(texto);
    char* en = (char*) MALLOC (len*2+1);
    *en = '\0';
    char token[TAM];
    int i = 0; //cursor da string em pt
    int j; //cursor do token
    int k = 0; //conta as palavras traduzidas
    bool punct;
    while (texto[i])
    {
        j = 0;
        punct = false;
        while (texto[i] && isalpha(texto[i])) token[j++] = texto[i++];
        token[j] = '\0';
        len = strlen (token);
        if(!len)
        {
            token[j++] = texto[i++];
            token[j] = '\0';
            punct = true;
        }
        else
        {
            transl(root, token);
            strcat(en, token);
            k++;
        }
        if(punct) strcat(en, token);
        *token = '\0';
    }
    strcpy(texto, en);
    free(en);
    return k;
}

void transl (Tree** root, char tk[])
{
    size_t len = strlen (tk);
    if(!len) return;
    Tree* search = NULL;
    bool upper = false;
    if(isupper(*tk)) upper = true;
    strToLower(tk);
    search = treeSearch (*root, tk);
    if(!search) search = treeKeyPush (root, tk);
    strcpy(tk, search->en);
    if(upper) *tk = toupper(*tk);
}


Tree* treeKeyPush (Tree** root, char pt[])
{
    char handle[TAM];
    Tree* aux = NULL;
    bool flag = false;
    do{
        clearScreen();
        if(flag)
        {
            fprintf(stderr, "Insira uma palavra valida!\n");
            PAUSE;
            clearScreen();
        }
        printf ("Palavra:   %s   nao encontrada no dicionario, por favor, insira sua traducao: ", pt);
        scanf ("%[^\n]%*c", handle);
        flag = true;
    }while(!strIsAlpha(handle));
    clearScreen ();
    strToLower(handle);
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

void treePrintO (Tree* root)
{
    if(root)
    {
        treePrintO(root->left);
        printf("[%s=%s]\n", root->pt, root->en);
        treePrintO(root->right);
    }
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
    char w[TAM];
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
        char w2[TAM] = {'\0'};
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

void fileTranslate (char** texto)
{
    FILE* in = NULL;
    OPENFILE (in, INPUT, "r");
    fseek(in, 0, SEEK_END);
    long fSize = ftell (in);
    rewind(in);
    if(!(*texto)) *texto = MALLOC (fSize+1);
    fread (*texto, fSize, 1, in);
    (*texto)[fSize] = '\0';
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
    int t = 0;
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

bool strIsAlpha (char w[])
{
    int i;
    for (i=0; w[i]; i++)
        if (!isalpha(w[i])) return false;
    return true;
}

bool strIsTxt (char* txt){
    int i;
    for(i=0; txt[i]; i++)
        if(isdigit(txt[i])) return false;
    return true;
}

int countNode (Tree* root){
    if(!root) return 0;
    return countNode(root->left)+countNode(root->right)+1;
}

int countLeaf (Tree* root){
    if(!root) return 0;
    if(root->left == NULL && root->right == NULL) return 1;
    else
        return countLeaf(root->left)+countLeaf(root->right);

}

Tree* pop (Tree* root, char s[]){
    if(root){
        if(strcmp(s, root->pt)<0) root->left = pop(root->left, s);
        else if (strcmp(s, root->pt)>0) root->right = pop(root->right, s);
        else{
            if(!(root->left)){
                Tree* aux = root;
                root=root->right;
                free(aux);
            }
            else if(!(root->right)){
                Tree* aux = root;
                root=root->left;
                free(aux);
            }
            else root->right=aux_pop(root, root->right);
            return root;
        }
    }
}

Tree* aux_pop (Tree* root, Tree* next){
    if(next->left==NULL){
        strcpy(next->pt, root->pt);
        Tree* aux = next;
        next=next->right;
        free(aux);
    }
    else next->left=aux_pop(root, next->left);
    return next;
}