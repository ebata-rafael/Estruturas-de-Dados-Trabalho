#include "utilitarios.h"

#ifndef HASH_H
#define HASH_H

#define TAMANHO_DA_HASH 249989

typedef struct Node
{
	cadastro *cadastro;
	struct Node *prox;
}Node;


void inserirNaHash(Node **tabelaHash, Node *novaPessoa);
int calculaFormulaDispersao(char *cpf);
Node *inserirNoInicioLista(Node **cidadaos, Node *novaPessoa);
int criarTabelaHash(Node **cidadaos, char *nomeArquivo);
int carregarDados(Node **cidadaos);
void salvarPessoasListaTxt(Node *listaPessoas, FILE *arquivo);
void SalvarPessoasHashTxt(Node **tabelaHash);
Node *buscarPessoaHash(Node **tabelaHash, char *cpf);
void mostraTodoMundodaHash(Node **tabelaHash);
void inicializaHash(Node **tabelaHash);
Node *criaRelatorio(Node **tabelaHash);
Node *inserirOrdenado(Node **cidadaos, cadastro *novoCadastro);
void relatorioHash(Node **tabelaHash, int idadeMinima, int idadeMaxima);
void listarPorFaixaEtaria(Node *cidadaos, char *idadeMinima, char *idadeMaxima);
Node *criarNode(cadastro *novoCadastro);
#endif