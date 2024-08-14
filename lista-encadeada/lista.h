/*TRABALHO DE ESTRUTURAS DE DADOS
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include "utilitarios.h"

#ifndef LISTA_H
#define LISTA_H

typedef struct list
{
	cadastro *cadastro;
	struct list *prox;
} list;

list *criarNo(cadastro *novoCadastro);

/*busca e retorna uma pessoa da lista, se nao encontrar retorna NULL*/
list *buscarPessoa(list *cidadaos, char cpf[]);

/*recebe um endereço de ponteiro pra lista de cadastro e retorna a lista atualizada com a nova pessoa*/
list *inserirOrdenado(list **cidadaos, cadastro *novoCadastro);

/*recebe um endereço de ponteiro pra lista de cadastro e insere no inicio*/
list *inserirNoInicio(list **cidadaos, list *novaPessoa);

/*recebe um arquivo json e retorna uma lista/{struct cadastro}*/
int criarLista(list **cidadaos, char *nomeArquivo);

/*recebe um ponteiro pro inicio de uma lista e retorna outro ponteiro apontando pra uma copia dessa lista ordenada*/
list *copiarLista(list *cidadaos);

/*recebe uma lista e dois numeros inteiros que representa uma faixa etaria, e mostra na tela informacoes
  das pessoas que estao nessa faixa etaria*/
void listarPorFaixaEtaria(list *cidadaos, char *idadeMinima, char *idadeMaxima);

void destruirLista(list *lista);

void relatorio(list *lst, int idadeMinima, int idadeMaxima);

/*recebe um ponteiro do tipo Struct cadastro pra uma lista encadeada
  e salva as informações de todas as pessoas em um arquivo de nome "pessoas.txt"*/
void salvarPessoasTxt(list *listaPessoas);

/*recebe um ponteiro do tipo Struct cadastro pra uma lista encadeada
  e insere nela todas as pessoas que estao salvas no arquivo de nome "pessoas.txt"*/
int carregarDados(list **cidadaos);

int tamanhoLista(list *cidadaos);

#endif