#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" 
#include "hash.h"


// Função de hash que insere uma nova pessoa na tabela, seja num posição inicial ou em cadeia de sequencia
void inserirNaHash(Node **tabelaHash, Node *novaPessoa)
{
    int valorHash;

    valorHash = calculaFormulaDispersao(novaPessoa->cadastro->cpf);

    novaPessoa->prox = NULL;
    if(tabelaHash[valorHash] == NULL)
    {
        tabelaHash[valorHash] = novaPessoa;
    }
    else
    {
        inserirNoInicioLista(tabelaHash,novaPessoa);
    }
}

// Função de hash que calcula um valor de hash para uma string CPF
int calculaFormulaDispersao(char *cpf)
{
    int valorHash, i;  // Inicializa o valor de hash
    valorHash = 0;
    for(i = 0; cpf[i] != '\0'; i++)
    {
        valorHash = valorHash * 31 + cpf[i];
    }
    return abs(valorHash % TAMANHO_DA_HASH);
}

Node *inserirNoInicioLista(Node **listaDaHash, Node *novaPessoa)
{
    novaPessoa->prox = *listaDaHash;
    *listaDaHash = novaPessoa;
}

int criarTabelaHash(Node **cidadaos, char *nomeArquivo)
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

        Node *p = buscarPessoaHash(cidadaos, novaPessoa->cpf); // verifica se ja esta cadastrada

        registroGeral *novoRg = cadastrarRg(estadoRg, rg->valuestring);

        if (p == NULL)
        {
            inserirRgNalista(novaPessoa, novoRg);
            inserirNaHash(cidadaos, criarNode(novaPessoa));
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

Node *buscarPessoaLista(Node *cidadaos, char cpf[])
{
    Node *pessoa;
    pessoa = cidadaos;

    
    while (pessoa != NULL && strcmp(pessoa->cadastro->cpf, cpf) != 0)
    {
        pessoa = pessoa->prox;
    }

    return pessoa;
}


Node *buscarPessoaHash(Node **tabelaHash, char *cpf)
{
    Node *pessoa;
    int valorHash;

    valorHash = calculaFormulaDispersao(cpf);
    pessoa = buscarPessoaLista(tabelaHash[valorHash], cpf);
    return pessoa;
}

void salvarPessoasListaTxt(Node *listaPessoas, FILE *arquivo)
{
    Node *pessoaAtual, *lixo;

    pessoaAtual = listaPessoas;

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
}

void SalvarPessoasHashTxt(Node **tabelaHash)
{
    FILE *arquivo;
    int i;

    arquivo = fopen("pessoas.txt", "w");
    for(i = 0; i < TAMANHO_DA_HASH; i++)
    {
        if(tabelaHash[i] != NULL)
            salvarPessoasListaTxt(tabelaHash[i], arquivo);
    }
    fclose(arquivo);
}


int carregarDados(Node **cidadaos)
{
    FILE *arquivo;
    Node *novaPessoa = NULL;
    registroGeral *novoRg = NULL;
    int controlaLaco;

    arquivo = fopen("pessoas.txt", "r");

    if (!arquivo)
        return 0;

    while (1)
    {
        novaPessoa = (Node *)malloc(sizeof(Node));
        if (!novaPessoa)
        {
            fclose(arquivo);
            return 0; // Falha na alocação de memória
        }

        novaPessoa->cadastro = (cadastro *)malloc(sizeof(cadastro));
        if (!novaPessoa->cadastro)
        {
            free(novaPessoa);
            fclose(arquivo);
            return 0; // Falha na alocação de memória
        }

        if (fscanf(arquivo, "%[^;];%[^;];%[^;];%[^;];%[^;];%d\n", novaPessoa->cadastro->nome,
                   novaPessoa->cadastro->data,
                   novaPessoa->cadastro->cpf,
                   novaPessoa->cadastro->cidade,
                   novaPessoa->cadastro->estado,
                   &novaPessoa->cadastro->qtdRg) != 6)
        {
            free(novaPessoa->cadastro);
            free(novaPessoa);
            if (feof(arquivo))
                break; // Fim do arquivo
            else
            {
                fclose(arquivo);
                return 0; // Erro na leitura do arquivo
            }
        }

        novaPessoa->cadastro->rg = NULL;
        Node *p = buscarPessoaHash(cidadaos, novaPessoa->cadastro->cpf);
        if (!p) // Condição para continuar consumindo as linhas do arquivo sem inserir ninguém na lista
            inserirNaHash(cidadaos, novaPessoa);

        controlaLaco = novaPessoa->cadastro->qtdRg;
        for (int i = 0; i < controlaLaco; i++)
        {
            novoRg = (registroGeral *)malloc(sizeof(registroGeral));
            if (!novoRg)
            {
                // Falha na alocação de memória
                // Libere as memórias já alocadas antes de retornar
                free(novaPessoa->cadastro);
                free(novaPessoa);
                fclose(arquivo);
                return 0;
            }

            novoRg->prox = NULL;

            if (fscanf(arquivo, "%[^;];%[^\n]\n", novoRg->estado, novoRg->numero) != 2)
            {
                free(novoRg);
                if (feof(arquivo))
                    break; // Fim do arquivo
                else
                {
                    fclose(arquivo);
                    return 0; // Erro na leitura do arquivo
                }
            }

            if (!p) // Condição para continuar consumindo as linhas do arquivo sem inserir ninguém na lista
            {
                inserirRgNalista(novaPessoa->cadastro, novoRg);
                novaPessoa->cadastro->qtdRg--;
            }
        }
    }

    fclose(arquivo);
    return 1;
}


//VERIFICAR ESSA FUNÇÃO
void mostraTodoMundodaHash(Node **tabelaHash)
{
    Node *p;
    int i;

    // Verificar se a tabelaHash não é NULL
    if (!tabelaHash)
    {
        printf("A tabela hash está vazia ou não foi inicializada.\n");
        return;
    }

    for(i = 0; i < TAMANHO_DA_HASH; i++)
    {
        if(tabelaHash[i] != NULL)
        {
            p = tabelaHash[i];
            while(p != NULL)
            {
                printf("\nentrou em mostrar dados na posicao %d\n",i);   
                mostrarInformacoesPessoa(p->cadastro);
                printf("saiu de mostrar pessoa\n");
                p = p->prox;
            }
        }
    }
}

void inicializaHash(Node **tabelaHash)
{
    int i;
    for(i = 0;i < TAMANHO_DA_HASH; i++)
        tabelaHash[i] = NULL;
}

Node *inserirOrdenado(Node **cidadaos, cadastro *novoCadastro)
{
    Node *anterior, *atual, *novaPessoa = criarNode(novoCadastro);
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

void listarPorFaixaEtaria(Node *cidadaos, char *idadeMinima, char *idadeMaxima)
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


Node *criaRelatorio(Node **tabelaHash)
{
    Node *listaRelatorio = NULL, *p;
    cadastro *copiaCadastro;
    registroGeral *copiaRg = NULL, *atual;
    int i;

    for(i = 0;i < TAMANHO_DA_HASH; i++)
    {
        if(tabelaHash[i] != NULL)
        {
            p = tabelaHash[i];
            while (p != NULL)
            {
                copiaCadastro = cadastrarPessoa(p->cadastro->nome,
                                            p->cadastro->cpf,
                                            p->cadastro->data,
                                            p->cadastro->cidade,
                                            p->cadastro->estado);

                atual = p->cadastro->rg;
                while (atual)
                {
                    copiaRg = cadastrarRg(atual->estado, atual->numero);
                    inserirRgNalista(copiaCadastro, copiaRg);
                    atual = atual->prox;
                }  

                inserirOrdenado(&listaRelatorio, copiaCadastro);
                p = p->prox;
            }
        }
    }
    return listaRelatorio;
}

void relatorioHash(Node **tabelaHash, int idadeMinima, int idadeMaxima)
{
    Node *listaRelatorio = NULL;
    int i;

    printf("Criando relatorio...\n");
    listaRelatorio = criaRelatorio(tabelaHash);

    listarPorFaixaEtaria(listaRelatorio, retornaAno(idadeMinima), retornaAno(idadeMaxima));
}

Node *criarNode(cadastro *novoCadastro){

    Node *novaPessoa = (Node *)malloc(sizeof(Node));

    if (!novaPessoa){
        return NULL;
    }

    novaPessoa->cadastro = novoCadastro;
    novaPessoa->prox = NULL;
    
    return novaPessoa;
}