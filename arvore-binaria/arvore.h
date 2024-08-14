/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include "utilitarios.h"

#ifndef ARVORE_H
#define ARVORE_H

// Definição da estrutura de um nó da árvore
typedef struct node {
    cadastro *cadastro;
    struct node* esq;
    struct node* dir;
}node;

// Função para criar um novo nó
node *criarNo(cadastro *novaPessoa);

// Função de busca em árvore binária de busca (versão recursiva)
node *buscarPessoa(node *raiz, char *cpf);

// Função recursiva para inserir um nó na árvore
node *inserirNaArvore(node **raiz, cadastro *pessoa);

node *preOrdem(node *raiz);

node *simetrica(node *raiz);

node *posOrdem(node *raiz);

/*recebe um arquivo json e retorna uma arvore binaria que armazena struct cadastro*/
int criarLista(node **cidadaos, char *nomeArquivo);

/*recebe um ponteiro do tipo Struct cadastro pra uma lista encadeada
  e insere nela todas as pessoas que estao salvas no arquivo de nome "pessoas.txt"*/
int carregarDados(node **cidadaos);

void escreverNoArquivo(cadastro *pessoaAtual, FILE *arquivo);

node* percorrerArvoreSalvarPessoas(node* raiz, FILE *arquivo);

void salvarPessoasTxt(node *listaPessoas);

void inicializarEstrutura(node **hash);

node **criarEstruturaRelatorio(node *cidadaos);

node *inserirRelatorio(node **raiz, cadastro *pessoa);

int funcaoHash(char *chave);

cadastro *copiarPessoa(cadastro *pessoa);

node **copiarEstruturaPrincipal(node *raiz, node **hash, char *idadeMin, char *idadeMax);

node *listarPorFaixaEtaria(node *raiz);

void relatorio(node *cidadaos, int idadeMinima, int idadeMaxima);


#endif





