/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include "utilitarios.h"

#ifndef RELATORIO_H
#define RELATORIO_H

typedef struct nodeRelatorio {
    cadastro *cadastro;
    struct nodeRelatorio* esq;
    struct nodeRelatorio* dir;
    int fb;
}nodeRelatorio;

nodeRelatorio **criarEstruturaRelatorio();

nodeRelatorio *listarPorFaixaEtaria(nodeRelatorio *raiz);

void inicializarEstrutura(nodeRelatorio **hash);

int funcaoHash(char *chave);

nodeRelatorio *inserirRelatorio(nodeRelatorio **raiz, cadastro *pessoa, int *possoAtualizarFb);

nodeRelatorio *criarNo(cadastro *novaPessoa);


#endif