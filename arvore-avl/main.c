/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvoreavl.h"

int main(int argc, char *argv[])
{
    node *listaDeCadastros = NULL, *existePessoa;
    cadastro *novaPessoa;
    clock_t start;
    char op[3], cpf[12];
    int idadeMinima, idadeMaxima;

    printf("Inicializando sistema...\n");

    carregarDados(&listaDeCadastros);

    start = tempoInicial();
    for (int i = 1; i < argc; i++)
    {
        criarLista(&listaDeCadastros, argv[i]);
    }

    tempoFinal(start);
    
    do
    {
        existePessoa = NULL;
        novaPessoa = NULL; // reseta o valor da variavel
        printf("==========================================================\n");
        menuPrincipal();
        printf("-----------------------------\n");
        printf("Numero da operacao: ");
        scanf("%s", op);

        if (strcmp(op, "1") == 0)
        {
            printf("\n*cpf sem pontos e traco*\n");
            printf("Buscar por CPF: ");
            scanf(" %[^\n]", cpf);

            start = tempoInicial();
            
            existePessoa = buscarPessoa(listaDeCadastros, cpf);


            if (existePessoa)
            {
                novaPessoa = existePessoa->cadastro;
                mostrarInformacoesPessoa(novaPessoa);
                printf("\n");

                tempoFinal(start);
            }
            else
            {
                printf("NAO EXISTE UMA PESSOA COM ESTE CPF\n");
            }
            printf("-----------------------------\n");
        }
        else if (strcmp(op, "2") == 0)
        {
            if (listaDeCadastros)
            {
                printf("\nEste relatorio mostra pessoas que estao entre uma determinada faixa-etaria\n");
                printf("Insira uma idade minima e maxima para a busca\n");
                printf("\nIdade Minima: ");
                scanf("%d", &idadeMinima);
                printf("Idade Maxima: ");
                scanf("%d", &idadeMaxima);

                start = tempoInicial();

                relatorio(listaDeCadastros, idadeMinima, idadeMaxima);

                tempoFinal(start);
            }
            else
                printf("\nNAO EXISTEM PESSOAS CADASTRADAS NO SISTEMA\n");

            printf("-----------------------------\n");
        }
        else if (strcmp(op, "3") == 0)
        {
            printf("\nEncerrando programa...\n");

            start = tempoInicial();

            salvarPessoasTxt(listaDeCadastros);

            tempoFinal(start);

            printf("-----------------------------\n");
        }
        
    } while (strcmp(op, "3") != 0);

    return 0;
}
