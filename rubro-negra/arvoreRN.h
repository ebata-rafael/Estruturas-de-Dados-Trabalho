/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include "utilitarios.h"

#ifndef ARVORERN_H
#define ARVORERN_H

// Definição da estrutura de um nó da árvore
typedef struct nodeRN {
    cadastro *cadastro;
    struct nodeRN* esq;
    struct nodeRN* dir;
    int vermelho;
}nodeRN;

// Função para criar um novo nó
nodeRN *criarNo(cadastro *novaPessoa, nodeRN *nullptr);

// Função de busca em árvore binária de busca (versão recursiva)
nodeRN *buscarPessoa(nodeRN *raiz, char *cpf, nodeRN *nullptr);

// Função recursiva para inserir um nó na árvore
nodeRN *inserirNaArvoreRN(nodeRN **raiz, cadastro *pessoa, nodeRN *nullptr);

int verificarPropriedadeRnDireita(nodeRN *avo);

int verificarPropriedadeRnEsquerda(nodeRN *avo);

nodeRN *chamarRotacaoOuMudarCor(nodeRN *raiz, int operacao, int lado);

struct nodeRN* rotacaoDireita(struct nodeRN* raiz);

struct nodeRN* rotacaoEsquerda(struct nodeRN* raiz);

struct nodeRN* rotacaoDuplaEsquerda(struct nodeRN* raiz);

struct nodeRN* rotacaoDuplaDireita(struct nodeRN* raiz);

nodeRN *preOrdem(nodeRN *raiz, nodeRN *nullptr);

int criarLista(nodeRN **cidadaos, char *nomeArquivo, nodeRN *nullptr);

void salvarPessoasTxt(nodeRN *listaPessoas, nodeRN *nullptr);

nodeRN *percorrerArvoreSalvarPessoas(nodeRN *raiz, FILE *arquivo, nodeRN *nullptr);

void escreverNoArquivo(cadastro *pessoaAtual, FILE *arquivo);

int carregarDados(nodeRN **cidadaos, nodeRN *nullptr);

void relatorio(nodeRN *cidadaos, int idadeMinima, int idadeMaxima, nodeRN **nullptr);

nodeRN **criarEstruturaRelatorio(nodeRN *nullptr);

void inicializarEstrutura(nodeRN **hash, nodeRN *nullptr);

nodeRN **copiarEstruturaPrincipal(nodeRN *raiz, nodeRN **hash, char *idadeMin, char *idadeMax, nodeRN **nullptr);

int funcaoHash(char *chave);

cadastro *copiarPessoa(cadastro *pessoa);

nodeRN *listarPorFaixaEtaria(nodeRN *raiz, nodeRN *nullptr);

nodeRN *inserirRelatorio(nodeRN **raiz, cadastro *pessoa, nodeRN *nullptr);

#endif