/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include "utilitarios.h"
#include "stdbool.h"
#include "relatorio.h"

#ifndef DIGITAL_H
#define DIGITAL_H

typedef struct node{
    struct node *filho[10];
    cadastro *cadastro; 
    bool final_palavra;
}node;

// Função para criar um novo nó
node *criaNo();

// Função de busca em árvore binária de busca (versão recursiva)
node *buscarPessoa(node *raiz, char *cpf);

// Função recursiva para inserir um nó na árvore
void inserirNaArvore(node *raiz, cadastro *pessoa);

void preOrdem(node *raiz, char *buffer, int depth);

void simetrica(node *raiz, char *buffer, int depth);

void posOrdem(node *raiz, char *buffer, int depth);

/*recebe um arquivo json e retorna uma arvore binaria que armazena struct cadastro*/
int criarLista(node **cidadaos, char *nomeArquivo);

/*recebe um ponteiro do tipo Struct cadastro pra uma lista encadeada
  e insere nela todas as pessoas que estao salvas no arquivo de nome "pessoas.txt"*/
int carregarDados(node **cidadaos);

void escreverNoArquivo(cadastro *pessoaAtual, FILE *arquivo);

void percorrerArvoreSalvarPessoas(node* raiz, FILE *arquivo);

void salvarPessoasTxt(node *listaPessoas);

//funcoes pro relatorio

void relatorio(node *cidadaos, int idadeMinima, int idadeMaxima);

nodeRelatorio **copiarEstruturaPrincipal(node *raiz, nodeRelatorio **hash, char *idadeMin, char *idadeMax);

#endif
