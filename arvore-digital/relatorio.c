/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" // Inclui a biblioteca cJSON
#include "relatorio.h"


nodeRelatorio **criarEstruturaRelatorio(){
    nodeRelatorio **hash;

    hash = (nodeRelatorio **)malloc(27 * sizeof(nodeRelatorio*));

    if (!hash)
    {
        printf("erro ao alocar memoria estrutura relatorio!\n");
    }

    inicializarEstrutura(hash);

    return hash;
}

nodeRelatorio *listarPorFaixaEtaria(nodeRelatorio *raiz)
{
    if (raiz != NULL)
    {
        listarPorFaixaEtaria(raiz->esq);
    }
    else{
        return NULL;
    }

    mostrarInformacoesRelatorio(raiz->cadastro);
    
    listarPorFaixaEtaria(raiz->dir);
    
    return raiz;
}

void inicializarEstrutura(nodeRelatorio **hash){
    
    int i;
    for(i = 0; i < 27; i++)
        hash[i] = NULL;
}

int funcaoHash(char *chave)
{

    if (strcmp(chave, "AC") == 0)
    {
        return 0;
    }
    else if (strcmp(chave, "AL") == 0)
    {
        return 1;
    }
    else if (strcmp(chave, "AP") == 0)
    {
        return 2;
    }
    else if (strcmp(chave, "AM") == 0)
    {
        return 3;
    }
    else if (strcmp(chave, "BA") == 0)
    {
        return 4;
    }
    else if (strcmp(chave, "CE") == 0)
    {
        return 5;
    }
    else if (strcmp(chave, "DF") == 0)
    {
        return 6;
    }
    else if (strcmp(chave, "ES") == 0)
    {
        return 7;
    }
    else if (strcmp(chave, "GO") == 0)
    {
        return 8;
    }
    else if (strcmp(chave, "MA") == 0)
    {
        return 9;
    }
    else if (strcmp(chave, "MT") == 0)
    {
        return 10;
    }
    else if (strcmp(chave, "MS") == 0)
    {
        return 11;
    }
    else if (strcmp(chave, "MG") == 0)
    {
        return 12;
    }
    else if (strcmp(chave, "PA") == 0)
    {
        return 13;
    }
    else if (strcmp(chave, "PB") == 0)
    {
        return 14;
    }
    else if (strcmp(chave, "PR") == 0)
    {
        return 15;
    }
    else if (strcmp(chave, "PE") == 0)
    {
        return 16;
    }
    else if (strcmp(chave, "PI") == 0)
    {
        return 17;
    }
    else if (strcmp(chave, "RJ") == 0)
    {
        return 18;
    }
    else if (strcmp(chave, "RN") == 0)
    {
        return 19;
    }
    else if (strcmp(chave, "RS") == 0)
    {
        return 20;
    }
    else if (strcmp(chave, "RO") == 0)
    {
        return 21;
    }
    else if (strcmp(chave, "RR") == 0)
    {
        return 22;
    }
    else if (strcmp(chave, "SC") == 0)
    {
        return 23;
    }
    else if (strcmp(chave, "SP") == 0)
    {
        return 24;
    }
    else if (strcmp(chave, "SE") == 0)
    {
        return 25;
    }
    if (strcmp(chave, "TO") == 0)
    {
        return 26;
    }
}

nodeRelatorio *inserirRelatorio(nodeRelatorio **raiz, cadastro *pessoa, int *possoAtualizarFb)
{

    if ((*raiz) == NULL)
    {
        return criarNo(pessoa);
    }

    if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) > 0)
    {
        (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa, possoAtualizarFb);

        if (*possoAtualizarFb != 0)
        {
            (*raiz)->fb--;
            *possoAtualizarFb = (*raiz)->fb;
        }
    }
    else if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) < 0)
    {
        (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa, possoAtualizarFb);

        if (*possoAtualizarFb != 0)
        {
            (*raiz)->fb++;
            *possoAtualizarFb = (*raiz)->fb;
        }

    }
    else // idades iguas
    {
        if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) < 0)
        {
            (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa, possoAtualizarFb);

            if (*possoAtualizarFb != 0)
            {
                (*raiz)->fb--;
                *possoAtualizarFb = (*raiz)->fb;
            }

        }
        else if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) > 0)
        {
            (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa, possoAtualizarFb);

            if (*possoAtualizarFb != 0)
            {
                (*raiz)->fb++;
                *possoAtualizarFb = (*raiz)->fb;
            }
        }
    }
    
    return *raiz;
}

nodeRelatorio *criarNo(cadastro *novaPessoa)
{
    nodeRelatorio *novoNo;

    novoNo = (nodeRelatorio *)malloc(sizeof(nodeRelatorio));

    if (!novoNo)
        return NULL;

    novoNo->cadastro = novaPessoa;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->fb = 0;

    return novoNo;
}
