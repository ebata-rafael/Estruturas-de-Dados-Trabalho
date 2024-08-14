/*TRABALHO DE ESTRUTURAS DE DADOS
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" // Inclui a biblioteca cJSON
#include "lista.h"

list *criarNo(cadastro *novoCadastro){

    list *novaPessoa = (list *)malloc(sizeof(list));

    if (!novaPessoa){
        return NULL;
    }

    novaPessoa->cadastro = novoCadastro;
    novaPessoa->prox = NULL;
    
    return novaPessoa;
}

list *buscarPessoa(list *cidadaos, char cpf[])
{

    list *pessoa;
    pessoa = cidadaos;

    while (pessoa && strcmp(pessoa->cadastro->cpf, cpf) != 0)
    {
        pessoa = pessoa->prox;
    }

    return pessoa;
}

list *inserirOrdenado(list **cidadaos, cadastro *novoCadastro)
{
    list *anterior, *atual, *novaPessoa = criarNo(novoCadastro);
    atual = *cidadaos;
    anterior = NULL;

    while (atual != NULL && strcmp(atual->cadastro->estado, novaPessoa->cadastro->estado) < 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    while (atual != NULL && strcmp(&atual->cadastro->data[6], &novaPessoa->cadastro->data[6]) > 0 && strcmp(atual->cadastro->estado, novaPessoa->cadastro->estado) == 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    while (atual != NULL && strcmp(atual->cadastro->nome, novaPessoa->cadastro->nome) < 0 && strcmp(&atual->cadastro->data[6], &novaPessoa->cadastro->data[6]) == 0 && strcmp(atual->cadastro->estado, novaPessoa->cadastro->estado) == 0)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (anterior)
    {
        novaPessoa->prox = atual;
        anterior->prox = novaPessoa;
    }
    else
    {
        novaPessoa->prox = *cidadaos;
        *cidadaos = novaPessoa;
    }

    return *cidadaos;
}

list *inserirNoInicio(list **cidadaos, list *novaPessoa)
{
    novaPessoa->prox = *cidadaos;
    *cidadaos = novaPessoa;
}

int criarLista(list **cidadaos, char *nomeArquivo)
{

    FILE *file = fopen(nomeArquivo, "r"); // Abre o arquivo JSON no modo de leitura
    if (!file)
    {
        printf("Erro ao abrir o arquivo JSON.\n"); // Se houver erro ao abrir o arquivo, exibe uma mensagem de erro
        return 1;                                  // Retorna 1 para indicar falha
    }

    fseek(file, 0, SEEK_END);    // Move o indicador de posição para o final do arquivo
    long fileSize = ftell(file); // Obtém o tamanho do arquivo
    fseek(file, 0, SEEK_SET);    // Move o indicador de posição de volta para o início do arquivo

    char *jsonData = (char *)malloc(fileSize + 1); // Aloca memória para armazenar os dados do arquivo JSON
    fread(jsonData, 1, fileSize, file);            // Lê os dados do arquivo e os armazena no buffer jsonData
    fclose(file);                                  // Fecha o arquivo após a leitura

    jsonData[fileSize] = '\0'; // Adiciona um caractere nulo ao final dos dados para garantir que seja uma string válida

    cJSON *root = cJSON_Parse(jsonData); // Faz o parse dos dados JSON e os armazena na estrutura cJSON
    if (!root)
    {
        printf("Erro ao fazer o parse do JSON.\n"); // Se houver erro ao fazer o parse do JSON, exibe uma mensagem de erro
        free(jsonData);                             // Libera a memória alocada para jsonData
        return 1;                                   // Retorna 1 para indicar falha
    }

    cJSON *uf = cJSON_GetObjectItem(root, "uf"); // Obtém o objeto "uf" do JSON
    if (!uf)
    {
        printf("Não foi possível encontrar o estado.\n"); // Se não for possível encontrar o estado, exibe uma mensagem de erro
        cJSON_Delete(root);                               // Libera a memória alocada para root
        free(jsonData);                                   // Libera a memória alocada para jsonData
        return 1;                                         // Retorna 1 para indicar falha
    }

    char estadoRg[30]; // usado para inserir um novo rg, contem a sigla do estado
    strcpy(estadoRg, uf->valuestring);

    converterSiglaEstado(estadoRg);

    cJSON *citizens = cJSON_GetObjectItem(root, "cidadãos"); // Obtém o objeto "cidadãos" do JSON
    if (!citizens)
    {
        printf("Não foi possível encontrar a lista de cidadãos.\n"); // Se não for possível encontrar a lista de cidadãos, exibe uma mensagem de erro
        cJSON_Delete(root);                                          // Libera a memória alocada para root
        free(jsonData);                                              // Libera a memória alocada para jsonData
        return 1;                                                    // Retorna 1 para indicar falha
    }

    int numCitizens = cJSON_GetArraySize(citizens); // Obtém o número de elementos no array "cidadãos"

    for (int i = 0; i < numCitizens; i++)
    {                                                     // Itera sobre cada cidadão no array
        cJSON *citizen = cJSON_GetArrayItem(citizens, i); // Obtém o objeto cidadão atual

        // Obtém os dados do cidadão atual
        cJSON *nome = cJSON_GetObjectItem(citizen, "nome");
        cJSON *cpf = cJSON_GetObjectItem(citizen, "cpf");
        cJSON *data = cJSON_GetObjectItem(citizen, "data_nasc");
        cJSON *rg = cJSON_GetObjectItem(citizen, "rg");
        cJSON *cidade = cJSON_GetObjectItem(citizen, "naturalidade")->child;
        cJSON *estado = cJSON_GetObjectItem(citizen, "naturalidade")->child->next;

        cadastro *novoCadastro = cadastrarPessoa(nome->valuestring,
                                           cpf->valuestring,
                                           data->valuestring,
                                           cidade->valuestring,
                                           estado->valuestring);

        list *p = buscarPessoa(*cidadaos, novoCadastro->cpf); // verifica se ja esta cadastrada

        registroGeral *novoRg = cadastrarRg(estadoRg, rg->valuestring);

        if (p == NULL)
        {
            inserirRgNalista(novoCadastro, novoRg);
            list *novaPessoa = criarNo(novoCadastro);
            inserirNoInicio(cidadaos, novaPessoa);
        }
        else
        {
            if (!existeRgNaLista(p->cadastro->rg, novoRg->numero))
                inserirRgNalista(p->cadastro, novoRg);
        }
    }

    cJSON_Delete(root); // Libera a memória alocada para root
    free(jsonData);     // Libera a memória alocada para jsonData

    return 0;
}

list *copiarLista(list *cidadaos)
{
    list *copiaLista = NULL;
    cadastro *copiaCadastro;
    registroGeral *copiaRg = NULL, *atual;

    while (cidadaos)
    {

        copiaCadastro = cadastrarPessoa(cidadaos->cadastro->nome,
                                        cidadaos->cadastro->cpf,
                                        cidadaos->cadastro->data,
                                        cidadaos->cadastro->cidade,
                                        cidadaos->cadastro->estado);

        atual = cidadaos->cadastro->rg;
        while (atual)
        {

            copiaRg = cadastrarRg(atual->estado, atual->numero);
            inserirRgNalista(copiaCadastro, copiaRg);
            atual = atual->prox;
        }

        inserirOrdenado(&copiaLista, copiaCadastro);

        cidadaos = cidadaos->prox;
    }

    return copiaLista;
}

void listarPorFaixaEtaria(list *cidadaos, char *idadeMinima, char *idadeMaxima)
{
    char estadoAtual[3];

    while (cidadaos != NULL)
    {

        if (strcmp(&cidadaos->cadastro->data[6], idadeMinima) <= 0 && strcmp(&cidadaos->cadastro->data[6], idadeMaxima) >= 0) // verifica se a idade esta dentro do valor minimo e maximo
        {
            if (strcmp(cidadaos->cadastro->estado, estadoAtual) != 0) // mostra a sigla do estado de cada agrupamento de pessoas do estado atual
            {
                strcpy(estadoAtual, cidadaos->cadastro->estado);
                printf("\n%s\n", estadoAtual);
            }

            mostrarInformacoesRelatorio(cidadaos->cadastro);
        }
        cidadaos = cidadaos->prox;
    }
}

void destruirLista(list *lista)
{
    list *lixo;

    while(lista){
        
        lixo = lista;
        lista = lista->prox;
        free(lixo);
    }

}

void relatorio(list *cidadaos, int idadeMinima, int idadeMaxima)
{
    list *copiaLista;
    char *dataIdadeMinima, *dataIdadeMaxima;

    printf("Criando relatorio...\n");
    copiaLista = copiarLista(cidadaos);
    
    listarPorFaixaEtaria(copiaLista, retornaAno(idadeMinima), retornaAno(idadeMaxima));

    destruirLista(copiaLista);
}

void salvarPessoasTxt(list *listaPessoas)
{
    FILE *arquivo;
    list *pessoaAtual, *lixo;

    pessoaAtual = listaPessoas;
    arquivo = fopen("pessoas.txt", "w");

    while (pessoaAtual != NULL)
    {
        fprintf(arquivo, "%s;%s;%s;%s;%s;%d\n", pessoaAtual->cadastro->nome,
                pessoaAtual->cadastro->data,
                pessoaAtual->cadastro->cpf,
                pessoaAtual->cadastro->cidade,
                pessoaAtual->cadastro->estado,
                pessoaAtual->cadastro->qtdRg);

        registroGeral *rgAtual = pessoaAtual->cadastro->rg; // ponteiro para percorrer a lista de rgs
        for (int i = 0; i < pessoaAtual->cadastro->qtdRg; i++)
        {
            fprintf(arquivo, "%s;%s\n", rgAtual->estado, rgAtual->numero);
            rgAtual = rgAtual->prox;
        }
        lixo = pessoaAtual;

        pessoaAtual = pessoaAtual->prox;

        free(lixo);
    }
    fclose(arquivo);
}

int carregarDados(list **cidadaos)
{

    FILE *arquivo;
    cadastro *novoCadastro = NULL;
    registroGeral *novoRg = NULL;
    int controlaLaco;

    arquivo = fopen("pessoas.txt", "r");

    if (!arquivo)
        return 0;

    while (!feof(arquivo))
    {
        novoCadastro = (cadastro *)malloc(sizeof(cadastro));
        if (novoCadastro)
        {

            fscanf(arquivo, "%[^;];%[^;];%[^;];%[^;];%[^;];%d\n", novoCadastro->nome,
                   novoCadastro->data,
                   novoCadastro->cpf,
                   novoCadastro->cidade,
                   novoCadastro->estado,
                   &novoCadastro->qtdRg);

            novoCadastro->rg = NULL;

            list *p = buscarPessoa(*cidadaos, novoCadastro->cpf);
            if (!p){ // condição para continuar consumindo as linhas do arquivo sem inserir ninguem na lista
                list *novaPessoa = criarNo(novoCadastro);
                inserirNoInicio(cidadaos, novaPessoa);
            }

            controlaLaco = novoCadastro->qtdRg;
            for (int i = 0; i < controlaLaco; i++)
            {

                novoRg = (registroGeral *)malloc(sizeof(registroGeral));
                novoRg->prox = NULL;

                fscanf(arquivo, "%[^;];%[^\n]\n", novoRg->estado, novoRg->numero);

                if (!p) // condição para continuar consumindo as linhas do arquivo sem inserir ninguem na lista
                {
                    inserirRgNalista(novoCadastro, novoRg);
                    novoCadastro->qtdRg--;
                }
            }
        }
    }

    return 1;
}

int tamanhoLista(list *cidadaos)
{

    int size = 0;
    list *p = cidadaos;

    while (p)
    {
        size++;
        p = p->prox;
    }

    return size;
}