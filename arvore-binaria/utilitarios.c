/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilitarios.h"

void menuPrincipal()
{

    printf("1 - BUSCA POR REGISTRO GERAL\n");
    printf("2 - RELATORIO\n");
    printf("3 - SAIR\n");
}

void converterSegundos(double segundos)
{
    int h, m, s;
    double resto;

    h = (int)(segundos / 3600); // Calcula as horas
    resto = segundos - h * 3600;
    m = (int)(resto / 60);     // Calcula os minutos
    s = (int)(resto - m * 60); // Calcula os segundos

    printf("TEMPO// %d:%d:%d\n", h, m, s);
}

void converterSiglaEstado(char uf[])
{

    if (strcmp(uf, "Acre") == 0)
    {
        strcpy(uf, "AC");
    }
    else if (strcmp(uf, "Alagoas") == 0)
    {
        strcpy(uf, "AL");
    }
    else if (strcmp(uf, "Amapá") == 0)
    {
        strcpy(uf, "AP");
    }
    else if (strcmp(uf, "Amazonas") == 0)
    {
        strcpy(uf, "AM");
    }
    else if (strcmp(uf, "Bahia") == 0)
    {
        strcpy(uf, "BA");
    }
    else if (strcmp(uf, "Ceará") == 0)
    {
        strcpy(uf, "CE");
    }
    else if (strcmp(uf, "Distrito Federal") == 0)
    {
        strcpy(uf, "DF");
    }
    else if (strcmp(uf, "Espírito Santo") == 0)
    {
        strcpy(uf, "ES");
    }
    else if (strcmp(uf, "Goiás") == 0)
    {
        strcpy(uf, "GO");
    }
    else if (strcmp(uf, "Maranhão") == 0)
    {
        strcpy(uf, "MA");
    }
    else if (strcmp(uf, "Mato Grosso") == 0)
    {
        strcpy(uf, "MT");
    }
    else if (strcmp(uf, "Mato Grosso do Sul") == 0)
    {
        strcpy(uf, "MS");
    }
    else if (strcmp(uf, "Minas Gerais") == 0)
    {
        strcpy(uf, "MG");
    }
    else if (strcmp(uf, "Pará") == 0)
    {
        strcpy(uf, "PA");
    }
    else if (strcmp(uf, "Paraíba") == 0)
    {
        strcpy(uf, "PB");
    }
    else if (strcmp(uf, "Paraná") == 0)
    {
        strcpy(uf, "PR");
    }
    else if (strcmp(uf, "Pernambuco") == 0)
    {
        strcpy(uf, "PE");
    }
    else if (strcmp(uf, "Piauí") == 0)
    {
        strcpy(uf, "PI");
    }
    else if (strcmp(uf, "Rio de Janeiro") == 0)
    {
        strcpy(uf, "RJ");
    }
    else if (strcmp(uf, "Rio Grande do Norte") == 0)
    {
        strcpy(uf, "RN");
    }
    else if (strcmp(uf, "Rio Grande do Sul") == 0)
    {
        strcpy(uf, "RS");
    }
    else if (strcmp(uf, "Rondônia") == 0)
    {
        strcpy(uf, "RO");
    }
    else if (strcmp(uf, "Roraima") == 0)
    {
        strcpy(uf, "RR");
    }
    else if (strcmp(uf, "Santa Catarina") == 0)
    {
        strcpy(uf, "SC");
    }
    else if (strcmp(uf, "São Paulo") == 0)
    {
        strcpy(uf, "SP");
    }
    else if (strcmp(uf, "Sergipe") == 0)
    {
        strcpy(uf, "SE");
    }
    if (strcmp(uf, "Tocantins") == 0)
    {
        strcpy(uf, "TO");
    }
}

char* retornaAno(int idade)
{
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    int ano = (tm.tm_year + 1900) - idade;

    char *buffer = (char *)malloc(5 * sizeof(char));
    sprintf(buffer, "%d", ano);
    return buffer;
}

void mostrarInformacoesPessoa(cadastro *pessoa)
{
    registroGeral *p; // percorrer a lista de RGs

    printf("\nNome: %s\n", pessoa->nome);
    printf("CPF: %s\n", pessoa->cpf);
    printf("Data de Nascimento: %s\n", pessoa->data);
    printf("Naturalidade: %s-%s\n", pessoa->cidade, pessoa->estado);

    if (pessoa->qtdRg)
    {
        printf("LISTA DE RGs: \n");
        p = pessoa->rg;
        for (int i = 0; i < pessoa->qtdRg; i++)
        {
            printf("%s - %s\n", p->numero, p->estado);
            p = p->prox;
        }
    }
    else
        printf("NENHUM RG CADASTRADO\n");
}

void mostrarInformacoesRelatorio(cadastro *pessoa)
{
    printf("Registro Geral: %s\n", pessoa->cpf);
    printf("Nome: %s\n", pessoa->nome);
    printf("Naturalidade: %s-%s\n\n", pessoa->cidade, pessoa->estado);
}

void inserirRgNalista(cadastro *cidadaos, registroGeral *novoRg)
{
    novoRg->prox = cidadaos->rg;
    cidadaos->rg = novoRg;
    cidadaos->qtdRg++;
}

int existeRgNaLista(registroGeral *listaRg, char numeroRg[])
{

    registroGeral *atual = listaRg;

    while (atual)
    {
        if (strcmp(atual->numero, numeroRg) == 0)
            return 1;
        atual = atual->prox;
    }

    return 0;
}

registroGeral *cadastrarRg(char estadoRg[], char numeroRg[])
{
    registroGeral *novoRg;

    novoRg = (registroGeral *)malloc(sizeof(registroGeral));

    if(!novoRg)
        printf("Erro ao alocar memoria\n");

    strcpy(novoRg->estado, estadoRg);
    strcpy(novoRg->numero, numeroRg);
    novoRg->prox = NULL;

    return novoRg;
}

clock_t tempoInicial (){

    return clock(); // tempo de inicio
}

void tempoFinal (clock_t tempoInicial){
    
    clock_t tempoFinal;
    double time_taken = 0;

    tempoFinal = clock() - tempoInicial; // tempo do final
    time_taken = ((double)tempoFinal) / CLOCKS_PER_SEC;
    printf("\nO programa levou %.8lf segundos para ser executado\n", time_taken);
    converterSegundos(time_taken);
}

cadastro *cadastrarPessoa(char nome[], char cpf[], char data[], char cidade[], char estado[])
{
    cadastro *novaPessoa;

    novaPessoa = (cadastro *)malloc(sizeof(cadastro));
    if (!novaPessoa)
        return NULL;

    strcpy(novaPessoa->nome, nome);
    strcpy(novaPessoa->data, data);
    strcpy(novaPessoa->cpf, cpf);
    strcpy(novaPessoa->cidade, cidade);
    strcpy(novaPessoa->estado, estado);

    novaPessoa->qtdRg = 0;

    novaPessoa->rg = NULL;

    return novaPessoa;
}
