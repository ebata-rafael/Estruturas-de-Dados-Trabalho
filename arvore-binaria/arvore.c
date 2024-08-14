/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" // Inclui a biblioteca cJSON
#include "arvore.h"

node *criarNo(cadastro *novaPessoa)
{
    node *novoNo;

    novoNo = (node *)malloc(sizeof(node));

    if (!novoNo)
        return NULL;

    novoNo->cadastro = novaPessoa;
    novoNo->esq = NULL;
    novoNo->dir = NULL;

    return novoNo;
}

node **criarNoRelatorio(cadastro *novaPessoa)
{
    node *novoNo;

    novoNo = (node *)malloc(sizeof(node));

    if (!novoNo)
        return NULL;

    novoNo->cadastro = novaPessoa;
    novoNo->esq = NULL;
    novoNo->dir = NULL;

    node **endNovoNo;

    endNovoNo = (node**)malloc(sizeof(node*));
    if (!endNovoNo)
    {
        return NULL;
    }
    
    endNovoNo = &novoNo;
    
    return endNovoNo;
}

node *buscarPessoa(node *raiz, char *cpf)
{
    if (!raiz)
    {
        return NULL; // Árvore vazia ou chave não encontrada
    }

    if (strcmp(cpf, raiz->cadastro->cpf) == 0)
    {
        return raiz; // Encontrou o nó com a chave desejada
    }
    else if (strcmp(cpf, raiz->cadastro->cpf) < 0)
    {
        return buscarPessoa(raiz->esq, cpf); // Busca na subárvore esquerda
    }
    else if(strcmp(cpf, raiz->cadastro->cpf) > 0)
    {
        return buscarPessoa(raiz->dir, cpf); // Busca na subárvore direita
    }
}

node *inserirNaArvore(node **raiz, cadastro *pessoa)
{

    if ((*raiz) == NULL)
    {
        return criarNo(pessoa);
    }

    if (strcmp(pessoa->cpf, (*raiz)->cadastro->cpf) < 0)
    {
        // printf("esquerda: %s, %s\n", pessoa->cpf, (*raiz)->cadastro->cpf);
        (*raiz)->esq = inserirNaArvore(&(*raiz)->esq, pessoa);
    }
    else if (strcmp(pessoa->cpf, (*raiz)->cadastro->cpf) > 0)
    {
        // printf("direita: %s, %s\n", pessoa->cpf, (*raiz)->cadastro->cpf);
        (*raiz)->dir = inserirNaArvore(&(*raiz)->dir, pessoa);
    }
    else
    {
        // Chave já existe na árvore
        printf("Insercao invalida: chave ja existe. %s, %s\n", (*raiz)->cadastro->nome, (*raiz)->cadastro->cpf);
    }

    return *raiz;
}

node *preOrdem(node *raiz)
{

    if (raiz != NULL)
        mostrarInformacoesPessoa(raiz->cadastro);
    else
        return NULL;

    preOrdem(raiz->esq);
    preOrdem(raiz->dir);

    return raiz;
}

node *simetrica(node *raiz)
{

    if (raiz != NULL)
        simetrica(raiz->esq);
    else
        return NULL;

    mostrarInformacoesPessoa(raiz->cadastro);
    simetrica(raiz->dir);

    return raiz;
}

node *posOrdem(node *raiz)
{

    if (raiz != NULL)
        posOrdem(raiz->dir);
    else
        return NULL;

    mostrarInformacoesPessoa(raiz->cadastro);
    posOrdem(raiz->esq);

    return raiz;
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
            *cidadaos = inserirNaArvore(cidadaos, novaPessoa);
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
                *cidadaos = inserirNaArvore(cidadaos, novaPessoa);

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

node *percorrerArvoreSalvarPessoas(node *raiz, FILE *arquivo)
{

    if (raiz != NULL)
        escreverNoArquivo(raiz->cadastro, arquivo);
    else
        return NULL;

    percorrerArvoreSalvarPessoas(raiz->esq, arquivo);
    percorrerArvoreSalvarPessoas(raiz->dir, arquivo);

    free(raiz);

    return NULL;
}

void salvarPessoasTxt(node *listaPessoas)
{
    FILE *arquivo;

    arquivo = fopen("pessoas.txt", "w");

    percorrerArvoreSalvarPessoas(listaPessoas, arquivo);

    fclose(arquivo);
}

void inicializarEstrutura(node **hash){
    
    int i;
    for(i = 0; i < 27; i++)
        hash[i] = NULL;
}

node **criarEstruturaRelatorio(node *cidadaos){
    node **hash;

    hash = (node **)malloc(27 * sizeof(node*));

    if (!hash)
    {
        printf("erro ao alocar memoria estrutura relatorio!\n");
    }

    inicializarEstrutura(hash);

    return hash;
}

node *inserirRelatorio(node **raiz, cadastro *pessoa)
{

    if ((*raiz) == NULL)
    {
        return criarNo(pessoa);
    }

    if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) > 0)
    {

        (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa);
    }
    else if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) < 0)
    {

        (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa);
    }
    else // idades iguas
    {
        if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) < 0)
        {

            (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa);
        }
        else if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) > 0)
        {

            (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa);
        }
    }
    
    return *raiz;
}

int funcaoHash(char *chave)
{

    if (strcmp(chave, "AC") == 0)
    {
        return 0;
    }
    else if (strcmp(chave, "AL") == 0)
    {
        return 1;
    }
    else if (strcmp(chave, "AP") == 0)
    {
        return 2;
    }
    else if (strcmp(chave, "AM") == 0)
    {
        return 3;
    }
    else if (strcmp(chave, "BA") == 0)
    {
        return 4;
    }
    else if (strcmp(chave, "CE") == 0)
    {
        return 5;
    }
    else if (strcmp(chave, "DF") == 0)
    {
        return 6;
    }
    else if (strcmp(chave, "ES") == 0)
    {
        return 7;
    }
    else if (strcmp(chave, "GO") == 0)
    {
        return 8;
    }
    else if (strcmp(chave, "MA") == 0)
    {
        return 9;
    }
    else if (strcmp(chave, "MT") == 0)
    {
        return 10;
    }
    else if (strcmp(chave, "MS") == 0)
    {
        return 11;
    }
    else if (strcmp(chave, "MG") == 0)
    {
        return 12;
    }
    else if (strcmp(chave, "PA") == 0)
    {
        return 13;
    }
    else if (strcmp(chave, "PB") == 0)
    {
        return 14;
    }
    else if (strcmp(chave, "PR") == 0)
    {
        return 15;
    }
    else if (strcmp(chave, "PE") == 0)
    {
        return 16;
    }
    else if (strcmp(chave, "PI") == 0)
    {
        return 17;
    }
    else if (strcmp(chave, "RJ") == 0)
    {
        return 18;
    }
    else if (strcmp(chave, "RN") == 0)
    {
        return 19;
    }
    else if (strcmp(chave, "RS") == 0)
    {
        return 20;
    }
    else if (strcmp(chave, "RO") == 0)
    {
        return 21;
    }
    else if (strcmp(chave, "RR") == 0)
    {
        return 22;
    }
    else if (strcmp(chave, "SC") == 0)
    {
        return 23;
    }
    else if (strcmp(chave, "SP") == 0)
    {
        return 24;
    }
    else if (strcmp(chave, "SE") == 0)
    {
        return 25;
    }
    if (strcmp(chave, "TO") == 0)
    {
        return 26;
    }
}

cadastro *copiarPessoa(cadastro *pessoa){
    
    cadastro *copiaCadastro = cadastrarPessoa(pessoa->nome,
                                        pessoa->cpf,
                                        pessoa->data,
                                        pessoa->cidade,
                                        pessoa->estado);

    registroGeral *atual = pessoa->rg;
    while (atual)
    {

        registroGeral *copiaRg = cadastrarRg(atual->estado, atual->numero);
        inserirRgNalista(copiaCadastro, copiaRg);
        atual = atual->prox;
    }

    return copiaCadastro;
}

node **copiarEstruturaPrincipal(node *raiz, node **hash, char *idadeMin, char *idadeMax)
{

    if (raiz != NULL)
        copiarEstruturaPrincipal(raiz->esq, hash, idadeMin, idadeMax);
    else
        return NULL;

    if (strcmp(&raiz->cadastro->data[6], idadeMin) <= 0 && strcmp(&raiz->cadastro->data[6], idadeMax) >= 0)
    {
        int estado = funcaoHash(raiz->cadastro->estado);

        cadastro *novaPessoa = copiarPessoa(raiz->cadastro);

        hash[estado] = inserirRelatorio(&(hash[estado]) , novaPessoa);
    }
    
    copiarEstruturaPrincipal(raiz->dir, hash, idadeMin, idadeMax);

    return hash;
}

node *listarPorFaixaEtaria(node *raiz)
{
    if (raiz != NULL)
    {
        listarPorFaixaEtaria(raiz->esq);
    }
    else{
        return NULL;
    }

    mostrarInformacoesRelatorio(raiz->cadastro);
    
    listarPorFaixaEtaria(raiz->dir);
    
    return raiz;
}

void relatorio(node *cidadaos, int idadeMinima, int idadeMaxima) {

    node **hash = criarEstruturaRelatorio(cidadaos);

    hash = copiarEstruturaPrincipal(cidadaos, hash, retornaAno(idadeMinima), retornaAno(idadeMaxima));
    
    for(int i = 0; i < 27; i++){

        if(hash[i] != NULL){
            
            printf("\n%s\n", hash[i]->cadastro->estado);
            listarPorFaixaEtaria(hash[i]);
        }
    }
}





