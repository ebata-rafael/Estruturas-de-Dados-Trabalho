#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" // Inclui a biblioteca cJSON
#include "digital.h"


node* criaNo(void) {
    node *novoNo = (node *)malloc(sizeof(node));
    if (novoNo != NULL) {
        novoNo->final_palavra = false;
        for (int i = 0; i < 10; i++) {
            novoNo->filho[i] = NULL;
        }
        novoNo->cadastro = NULL;
    }
    return novoNo;
}

node* buscarPessoa(node *raiz, char *cpf) {
    node *novoNo = raiz;
    int aux, i;

    for (i = 0; i < strlen(cpf); i++) {
        aux = cpf[i] - '0';
        if (!novoNo->filho[aux]) {
            return NULL;
        }
        novoNo = novoNo->filho[aux];
    }

    if (novoNo != NULL && novoNo->final_palavra) {
        return novoNo; // Retorna o nó encontrado
    } else {
        return NULL;
    }
}

void inserirNaArvore(node *raiz, cadastro *pessoa) {
    node *novoNo = raiz;
    int aux, i;

    // Garantir que o CPF tenha exatamente 11 dígitos
    if (strlen(pessoa->cpf) != 11) {
        printf("Erro: CPF inválido\n");
        return;
    }

    // Garantir que todos os caracteres do CPF são dígitos
    for (i = 0; i < 11; i++) {
        if (pessoa->cpf[i] < '0' || pessoa->cpf[i] > '9') {
            printf("Erro: CPF inválido\n");
            return;
        }
    }

    // Inserir o CPF na árvore digital
    for (i = 0; i < 11; i++) {
        aux = pessoa->cpf[i] - '0';
        if (!novoNo->filho[aux]) {
            novoNo->filho[aux] = criaNo();
        }
        novoNo = novoNo->filho[aux];
    }

    novoNo->cadastro = pessoa;
    novoNo->final_palavra = true;
}

void preOrdem(node *novoNo, char *buffer, int depth) {
    if (novoNo == NULL) {
        return;
    }

    // Processa o nó atual
    if (novoNo->final_palavra) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
    }

    // Percorre todos os filhos do nó
    for (int i = 0; i < 10; i++) {
        if (novoNo->filho[i] != NULL) {
            buffer[depth] = i + '0';
            preOrdem(novoNo->filho[i], buffer, depth + 1);
        }
    }
}

void simetrica(node *novoNo, char *buffer, int depth) {
    if (novoNo == NULL) {
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (novoNo->filho[i] != NULL) {
            buffer[depth] = i + '0';
            simetrica(novoNo->filho[i], buffer, depth + 1);
        }

        // Processa o nó atual após percorrer o primeiro filho
        if (i == 4 && novoNo->final_palavra) {
            buffer[depth] = '\0';
            printf("%s\n", buffer);
        }
    }
}

void posOrdem(node *novoNo, char *buffer, int depth) {
    if (novoNo == NULL) {
        return;
    }

    // Percorre todos os filhos do nó
    for (int i = 0; i < 10; i++) {
        if (novoNo->filho[i] != NULL) {
            buffer[depth] = i + '0';
            posOrdem(novoNo->filho[i], buffer, depth + 1);
        }
    }

    // Processa o nó atual
    if (novoNo->final_palavra) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
    }
}

int criarLista(node **cidadaos, char *nomeArquivo)
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

        cadastro *novaPessoa = cadastrarPessoa(nome->valuestring,
                                               cpf->valuestring,
                                               data->valuestring,
                                               cidade->valuestring,
                                               estado->valuestring);

        node *p = buscarPessoa(*cidadaos, novaPessoa->cpf); // verifica se ja esta cadastrada
        
        registroGeral *novoRg = cadastrarRg(estadoRg, rg->valuestring);

        if (p == NULL)
        {
            inserirRgNalista(novaPessoa, novoRg);
            inserirNaArvore(*cidadaos, novaPessoa);
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

int carregarDados(node **cidadaos)
{
    FILE *arquivo;
    cadastro *novaPessoa = NULL;
    registroGeral *novoRg = NULL;
    int controlaLaco;

    arquivo = fopen("pessoas.txt", "r");

    if (!arquivo)
        return 0;
    while (!feof(arquivo))
    {
        novaPessoa = (cadastro *)malloc(sizeof(cadastro));
        if (novaPessoa)
        {

            fscanf(arquivo, "%[^;];%[^;];%[^;];%[^;];%[^;];%d\n", novaPessoa->nome,
                   novaPessoa->data,
                   novaPessoa->cpf,
                   novaPessoa->cidade,
                   novaPessoa->estado,
                   &novaPessoa->qtdRg);

            novaPessoa->rg = NULL;

            node *p = buscarPessoa(*cidadaos, novaPessoa->cpf);

            if (!p) // condição para continuar consumindo as linhas do arquivo sem inserir ninguem na lista
                inserirNaArvore(*cidadaos, novaPessoa);

            controlaLaco = novaPessoa->qtdRg;
            for (int i = 0; i < controlaLaco; i++)
            {

                novoRg = (registroGeral *)malloc(sizeof(registroGeral));
                novoRg->prox = NULL;

                fscanf(arquivo, "%[^;];%[^\n]\n", novoRg->estado, novoRg->numero);

                if (!p) // condição para continuar consumindo as linhas do arquivo sem inserir ninguem na lista
                {
                    inserirRgNalista(novaPessoa, novoRg);
                    novaPessoa->qtdRg--;
                }
            }
        }
    }

    return 1;
}

void escreverNoArquivo(cadastro *pessoaAtual, FILE *arquivo)
{

    fprintf(arquivo, "%s;%s;%s;%s;%s;%d\n", pessoaAtual->nome,
            pessoaAtual->data,
            pessoaAtual->cpf,
            pessoaAtual->cidade,
            pessoaAtual->estado,
            pessoaAtual->qtdRg);

    registroGeral *rgAtual = pessoaAtual->rg; // ponteiro para percorrer a lista de rgs
    for (int i = 0; i < pessoaAtual->qtdRg; i++)
    {
        fprintf(arquivo, "%s;%s\n", rgAtual->estado, rgAtual->numero);
        rgAtual = rgAtual->prox;
    }
}

void percorrerArvoreSalvarPessoas(node *raiz, FILE *arquivo) {
    if (raiz == NULL) {
        return;
    }

    if (raiz->cadastro != NULL && raiz->final_palavra) {
        escreverNoArquivo(raiz->cadastro, arquivo);
    }

    for (int i = 0; i < 10; i++) {
        percorrerArvoreSalvarPessoas(raiz->filho[i], arquivo);
    }

    free(raiz);
}

void salvarPessoasTxt(node *listaPessoas)
{
    FILE *arquivo;

    arquivo = fopen("pessoas.txt", "w");

    percorrerArvoreSalvarPessoas(listaPessoas, arquivo);

    fclose(arquivo);
}

//funções pro relatorio

void relatorio(node *cidadaos, int idadeMinima, int idadeMaxima) {

    nodeRelatorio **hash = criarEstruturaRelatorio();

    hash = copiarEstruturaPrincipal(cidadaos, hash, retornaAno(idadeMinima), retornaAno(idadeMaxima));
    
    for(int i = 0; i < 27; i++){

        if(hash[i] != NULL){
            
            printf("\n%s\n", hash[i]->cadastro->estado);
            listarPorFaixaEtaria(hash[i]);
        }
    }
}

nodeRelatorio **copiarEstruturaPrincipal(node *raiz, nodeRelatorio **hash, char *idadeMin, char *idadeMax)
{

    if (raiz == NULL)
        return NULL;

    if (raiz->cadastro != NULL && raiz->final_palavra) {
        
        if (strcmp(&raiz->cadastro->data[6], idadeMin) <= 0 && strcmp(&raiz->cadastro->data[6], idadeMax) >= 0)
        {
            int estado = funcaoHash(raiz->cadastro->estado);

            cadastro *novaPessoa = copiarPessoa(raiz->cadastro);

            int possoAtualizarFb = 1;

            hash[estado] = inserirRelatorio(&(hash[estado]), novaPessoa, &possoAtualizarFb);
        }
    }
    
    for (int i = 0; i < 10; i++) {
        copiarEstruturaPrincipal(raiz->filho[i], hash, idadeMin, idadeMax);
    }

    return hash;
}



