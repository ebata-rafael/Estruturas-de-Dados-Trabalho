/*TRABALHO DE ESTRUTURAS DE DADOS
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#ifndef UTILITARIOS_H
#define UTILITARIOS_H

typedef struct registroGeral
{
	char estado[3];
	char numero[10];
	struct registroGeral *prox;
} registroGeral;

typedef struct cadastro
{
	char nome[70];
	char data[11];
	char cpf[12];
	int qtdRg;
	char cidade[30];
	char estado[3];
	registroGeral *rg; // (lista)
} cadastro;

/*---------------------------------------------------------------------------------------------------------*/
/*FUNÇÕES GERAIS*/

void menuPrincipal();

/*recebe um numero em segundos, transforma em horas minutos e segundos e mostra na tela*/
void converterSegundos(double segundos);

/*ex: recebe "Acre" e altera para sua sigla "AC"*/
void converterSiglaEstado(char uf[]);

/*recebe uma string data formato "dd/mm/aaaa" e retorna a idade atual
  usa a data do sistema operacional para o calculo*/
char* retornaAno(int idade);

/*recebe um ponteiro pro tipo struct cadastro e mostra na tela todas as informações desse cadastro*/
void mostrarInformacoesPessoa(cadastro *pessoa);

/*recebe um ponteiro pro tipo struct cadastro e mostra na tela todas as informações, exigidas no relatorio, desse cadastro*/
void mostrarInformacoesRelatorio(cadastro *pessoa);

/*captura o tempo de clock do sistema*/
clock_t tempoInicial ();

/*recebe um tempo de clock, captura um segundo tempo e mostra a diferença de tempo*/
void tempoFinal (clock_t tempoInicial);

/*retorna um ponteiro do tipo Struct cadastro com as informacoes preenchidas, em caso de erro retorna NULL*/
cadastro *cadastrarPessoa(char nome[], char cpf[], char data[], char cidade[], char estado[]);

/*FUNÇÕES GERAIS*/
/*---------------------------------------------------------------------------------------------------------*/
/*FUNÇÕES PARA CRIAR E ARMAZENAR RGS*/

/*recebe um ponteiro do tipo struct cadastro que aponta pra uma pessoa,
 e insere, sempre no inicio, um novo rg na lista de rgs*/
void inserirRgNalista(cadastro *cidadaos, registroGeral *novoRg);

/*recebe um ponteiro do tipo registroGeral e um numero de rg
  retorna 1 se encontrar na lista ou 0 se nao encontrar*/
int existeRgNaLista(registroGeral *listaRg, char numeroRg[]);

/*cadastra um novo rg para ser inserido na lista de uma pessoa, retorna um endereço de memoria*/
registroGeral *cadastrarRg(char estadoRg[], char numeroRg[]);

/*FUNÇÕES PARA CRIAR E ARMAZENAR RGS*/
/*---------------------------------------------------------------------------------------------------------*/

#endif