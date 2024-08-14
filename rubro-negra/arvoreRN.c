/*TRABALHO DE ESTRUTURAS DE DADOS*/
/*INTEGRANTES: Rafael P. Ebata - Luiz Henrique S.S - Gabriel Romero*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h" // Inclui a biblioteca cJSON
#include "arvoreRN.h"


nodeRN *criarNo(cadastro *novaPessoa, nodeRN *nullptr)
{
    nodeRN *novoNo;

    novoNo = (nodeRN *)malloc(sizeof(nodeRN));

    if (!novoNo)
        return NULL;

    novoNo->cadastro = novaPessoa;
    novoNo->esq = nullptr;
    novoNo->dir = nullptr;
    novoNo->vermelho = 1;

    return novoNo;
}

nodeRN *buscarPessoa(nodeRN *raiz, char *cpf, nodeRN *nullptr)
{
    if (raiz == nullptr)
    {
        return NULL; // Árvore vazia ou chave não encontrada
    }

    if (strcmp(cpf, raiz->cadastro->cpf) == 0)
    {
        return raiz; // Encontrou o nó com a chave desejada
    }
    else if (strcmp(cpf, raiz->cadastro->cpf) < 0)
    {
        return buscarPessoa(raiz->esq, cpf, nullptr); // Busca na subárvore esquerda
    }
    else if(strcmp(cpf, raiz->cadastro->cpf) > 0)
    {
        return buscarPessoa(raiz->dir, cpf, nullptr); // Busca na subárvore direita
    }
}

nodeRN *inserirNaArvoreRN(nodeRN **raiz, cadastro *pessoa, nodeRN *nullptr)
{
    if ((*raiz) == nullptr)
    {
        return criarNo(pessoa, nullptr);
    }

    if (strcmp(pessoa->cpf, (*raiz)->cadastro->cpf) < 0)
    {
        (*raiz)->esq = inserirNaArvoreRN(&(*raiz)->esq, pessoa, nullptr);

        int operacao = verificarPropriedadeRnEsquerda(*raiz);
        *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 0);
    }
    else if (strcmp(pessoa->cpf, (*raiz)->cadastro->cpf) > 0)
    {
        (*raiz)->dir = inserirNaArvoreRN(&(*raiz)->dir, pessoa, nullptr);
        
        int operacao = verificarPropriedadeRnDireita(*raiz);
        *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 1);
    }

    return *raiz;
}

int verificarPropriedadeRnDireita(nodeRN *avo){
    
    nodeRN *pai = avo->dir;
    nodeRN *tio = avo->esq;

    nodeRN *netoE = pai->esq;
    nodeRN *netoD = pai->dir;

    if (pai->vermelho == 0)
    {
        return 0;
    }
    
    if (netoE->vermelho == 0 && netoD->vermelho == 0)
    {
        return 0;
    }

    if (tio->vermelho == 1)
    {
        return 1; // significa mudança de cor
    }

    if (netoD->vermelho == 1)
    {
        return 2; //rotacao simples
    }

    if (netoE->vermelho == 1)
    {
        return 3; // rotacao dupla
    }
}

int verificarPropriedadeRnEsquerda(nodeRN *avo){
    
    nodeRN *pai = avo->esq;
    nodeRN *tio = avo->dir;

    nodeRN *netoE = pai->esq;
    nodeRN *netoD = pai->dir;

    if (pai->vermelho == 0)
    {
        return 0;
    }
    
    if (netoE->vermelho == 0 && netoD->vermelho == 0)
    {
        return 0;
    }

    if (tio->vermelho == 1)
    {
        return 1; // significa mudança de cor
    }

    if (netoE->vermelho == 1)
    {
        return 2; // rotacao simples
    }

    if (netoD->vermelho == 1)
    {
        return 3; //rotacao dupla
    }
}

nodeRN *chamarRotacaoOuMudarCor(nodeRN *raiz, int operacao, int lado){
    
    if (operacao == 1)
    {
        raiz->esq->vermelho = 0;
        raiz->dir->vermelho = 0;
        raiz->vermelho = 1;

        return raiz;
    }

    if (operacao == 2)
    {
        if (lado == 1)
        {
            raiz = rotacaoEsquerda(raiz);
            raiz->esq->vermelho = 1;
        }
        else
        {
            raiz = rotacaoDireita(raiz);
            raiz->dir->vermelho = 1;
        }

        raiz->vermelho = 0;
    }

    if (operacao == 3)
    {
        if (lado == 1)
        {
            raiz = rotacaoDuplaEsquerda(raiz);
            raiz->esq->vermelho = 1;
        }
        else
        {
            raiz = rotacaoDuplaDireita(raiz);
            raiz->dir->vermelho = 1;

        }

        raiz->vermelho = 0;
    }

    return raiz;
}

//Funcao para rotacionar para direita
struct nodeRN* rotacaoDireita(struct nodeRN* raiz) {
    
    struct nodeRN* filhoEsquerdo = raiz->esq;
    struct nodeRN* neto = filhoEsquerdo->dir;

    //Realiza a rotacao
    filhoEsquerdo->dir = raiz;
    raiz->esq = neto;

    //Retorna nova raiz
    return filhoEsquerdo;
}

//Funcao para rotacionar para esquerda
struct nodeRN* rotacaoEsquerda(struct nodeRN* raiz) {

    struct nodeRN* filhoDireito = raiz->dir;
    struct nodeRN* neto = filhoDireito->esq;

    //Realiza rotacao/
    filhoDireito->esq = raiz;
    raiz->dir = neto;

    //Retorna nova raiz/
    return filhoDireito;
}

//Funcao para rotacao dupla para esquerda/
struct nodeRN* rotacaoDuplaEsquerda(struct nodeRN* raiz) {
    
    raiz->dir = rotacaoDireita(raiz->dir);
    
    return rotacaoEsquerda(raiz);
}

//Funcao de rotacao dupla para direita/
struct nodeRN* rotacaoDuplaDireita(struct nodeRN* raiz) {

    raiz->esq = rotacaoEsquerda(raiz->esq);
    
    return rotacaoDireita(raiz);
}

nodeRN *preOrdem(nodeRN *raiz, nodeRN *nullptr)
{

    if (raiz != nullptr){
        mostrarInformacoesPessoa(raiz->cadastro);
    }
    else
        return NULL;

    preOrdem(raiz->esq, nullptr);
    preOrdem(raiz->dir, nullptr);

    return raiz;
}

int criarLista(nodeRN **cidadaos, char *nomeArquivo, nodeRN *nullptr)
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

        nodeRN *p = buscarPessoa(*cidadaos, novaPessoa->cpf, nullptr); // verifica se ja esta cadastrada
        
        registroGeral *novoRg = cadastrarRg(estadoRg, rg->valuestring);

        if (p == NULL)
        {
            inserirRgNalista(novaPessoa, novoRg);
            *cidadaos = inserirNaArvoreRN(cidadaos, novaPessoa, nullptr);
            (*cidadaos)->vermelho = 0;
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

void salvarPessoasTxt(nodeRN *listaPessoas, nodeRN *nullptr)
{
    FILE *arquivo;

    arquivo = fopen("pessoas.txt", "w");

    percorrerArvoreSalvarPessoas(listaPessoas, arquivo, nullptr);

    fclose(arquivo);
}

nodeRN *percorrerArvoreSalvarPessoas(nodeRN *raiz, FILE *arquivo, nodeRN *nullptr)
{

    if (raiz != nullptr)
        escreverNoArquivo(raiz->cadastro, arquivo);
    else
        return NULL;

    percorrerArvoreSalvarPessoas(raiz->esq, arquivo, nullptr);
    percorrerArvoreSalvarPessoas(raiz->dir, arquivo, nullptr);

    free(raiz);

    return NULL;
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

int carregarDados(nodeRN **cidadaos, nodeRN *nullptr)
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

            nodeRN *p = buscarPessoa(*cidadaos, novaPessoa->cpf, nullptr);

            if (!p){ // condição para continuar consumindo as linhas do arquivo sem inserir ninguem na lista
                *cidadaos = inserirNaArvoreRN(cidadaos, novaPessoa, nullptr);
                (*cidadaos)->vermelho = 0;
            }

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

void relatorio(nodeRN *cidadaos, int idadeMinima, int idadeMaxima, nodeRN **nullptr) {
    nodeRN **hash = criarEstruturaRelatorio(*nullptr);

    hash = copiarEstruturaPrincipal(cidadaos, hash, retornaAno(idadeMinima), retornaAno(idadeMaxima), nullptr);
    
    for(int i = 0; i < 27; i++){

        if(hash[i] != *nullptr){
            
            printf("\n%s\n", hash[i]->cadastro->estado);
            listarPorFaixaEtaria(hash[i], *nullptr);
        }
    }
}

nodeRN **criarEstruturaRelatorio(nodeRN *nullptr){
    nodeRN **hash;

    hash = (nodeRN **)malloc(27 * sizeof(nodeRN*));

    if (!hash)
    {
        printf("erro ao alocar memoria estrutura relatorio!\n");
    }

    inicializarEstrutura(hash, nullptr);

    return hash;
}

void inicializarEstrutura(nodeRN **hash, nodeRN *nullptr){
    
    int i;
    for(i = 0; i < 27; i++)
        hash[i] = nullptr;
}

nodeRN **copiarEstruturaPrincipal(nodeRN *raiz, nodeRN **hash, char *idadeMin, char *idadeMax, nodeRN **nullptr)
{

    if (raiz != *nullptr)
        copiarEstruturaPrincipal(raiz->esq, hash, idadeMin, idadeMax, nullptr);
    else
        return nullptr;

    if (strcmp(&raiz->cadastro->data[6], idadeMin) <= 0 && strcmp(&raiz->cadastro->data[6], idadeMax) >= 0)
    {
        int estado = funcaoHash(raiz->cadastro->estado);

        cadastro *novaPessoa = copiarPessoa(raiz->cadastro);

        hash[estado] = inserirRelatorio(&(hash[estado]) , novaPessoa, *nullptr);
    }
    
    copiarEstruturaPrincipal(raiz->dir, hash, idadeMin, idadeMax, nullptr);

    return hash;
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

nodeRN *listarPorFaixaEtaria(nodeRN *raiz, nodeRN *nullptr)
{
    if (raiz != nullptr)
    {
        listarPorFaixaEtaria(raiz->esq, nullptr);
    }
    else{
        return nullptr;
    }

    mostrarInformacoesRelatorio(raiz->cadastro);
    
    listarPorFaixaEtaria(raiz->dir, nullptr);
    
    return raiz;
}

nodeRN *inserirRelatorio(nodeRN **raiz, cadastro *pessoa, nodeRN *nullptr)
{

    if ((*raiz) == nullptr)
    {
        return criarNo(pessoa, nullptr);
    }

    if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) > 0)
    {
        (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa, nullptr);

        int operacao = verificarPropriedadeRnEsquerda(*raiz);
        *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 0);
        
    }
    else if (strcmp(&pessoa->data[6], &(*raiz)->cadastro->data[6]) < 0)
    {
        (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa, nullptr);

        int operacao = verificarPropriedadeRnDireita(*raiz);
        *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 1);

    }
    else // idades iguas
    {
        if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) < 0)
        {
            (*raiz)->esq = inserirRelatorio(&(*raiz)->esq, pessoa, nullptr);

            int operacao = verificarPropriedadeRnEsquerda(*raiz);
            *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 0);

        }
        else if (strcmp(pessoa->nome, (*raiz)->cadastro->nome) > 0)
        {
            (*raiz)->dir = inserirRelatorio(&(*raiz)->dir, pessoa, nullptr);

            int operacao = verificarPropriedadeRnDireita(*raiz);
            *raiz = chamarRotacaoOuMudarCor(*raiz, operacao, 1);
        }
    }

    
    
    return *raiz;
}
