#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_CPF_TAMANHO 12
#define MAX_SENHA_TAMANHO 7
#define MAX_MOEDA_TAMANHO 20
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_EXTRATO "extrato.txt"
#define ARQUIVO_COTACAO "cotacao.txt"
#define ARQUIVO_ADM "adm_usuarios.txt"
#define MAX_CRIPTOMOEDAS 50
#define MAX_USUARIOS 10
#define MAX_TRANSACOES 100

// Estrutura com nome e valor das criptomoedas de um usuario
typedef struct{
    char nome[MAX_MOEDA_TAMANHO];
    double quantidade;
}Cripto_Usuario;

// Estrutura para armazenar dados do usuario
typedef struct{
    char cpf[MAX_CPF_TAMANHO];
    char senha[MAX_SENHA_TAMANHO];
    double saldo;
    Cripto_Usuario cripto[MAX_CRIPTOMOEDAS];
    int num_criptomoedas;
    int num_transacoes;
    double transacoes[MAX_TRANSACOES][4]; // [data, valor, taxa, tipo]
}Usuario;
Usuario usuarios[MAX_USUARIOS];
int num_usuarios = 0; // Contador de usuarios

// Estrutura das criptomoedas
typedef struct{
    char nome[MAX_MOEDA_TAMANHO];
    double cotacao;
    float taxa_compra;
    float taxa_venda;
}Criptomoeda;
Criptomoeda criptomoedas[MAX_CRIPTOMOEDAS];
int num_criptomoedas = 0;

void carregar_criptomoedas(){
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    if(!file) return;
    while(fscanf(file, "%s %lf %f %f\n", criptomoedas[num_criptomoedas].nome, &criptomoedas[num_criptomoedas].cotacao,
                  &criptomoedas[num_criptomoedas].taxa_compra, &criptomoedas[num_criptomoedas].taxa_venda) != EOF){
        num_criptomoedas++;
    }
    fclose(file);
}

void salvar_usuarios(){
    FILE *file = fopen(ARQUIVO_USUARIOS, "wb");
    if(file == NULL){
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }
    fwrite(&num_usuarios, sizeof(int), 1, file);
    fwrite(usuarios, sizeof(Usuario), num_usuarios, file);
    fclose(file);
}

// Funcao para ler usuarios e saldos do arquivo binario
void ler_usuarios(){
    FILE *file = fopen(ARQUIVO_USUARIOS, "rb");
    if(file == NULL){
        printf("Erro ao abrir o arquivo de usuarios. Criando novo arquivo.\n");
        return;
    }
    fread(&num_usuarios, sizeof(int), 1, file);
    fread(usuarios, sizeof(Usuario), num_usuarios, file);
    fclose(file);
}

// Funcao para registrar um novo usuario
void cadastrar_usuario(){
    if(num_usuarios >= MAX_USUARIOS){
        printf("Numero maximo de usuarios atingido.\n");
        return;
    }

    char cpf[MAX_CPF_TAMANHO];
    char senha[MAX_SENHA_TAMANHO];
    printf("Informe seu CPF(somente numeros): ");
    scanf("%s", cpf);
    if(strlen(cpf) != 11){
        printf("CPF invalido. Deve conter 11 numeros.\n");
        return;
    }

    printf("Informe sua senha(6 digitos, somente numeros): ");
    scanf("%s", senha);
    if(strlen(senha) != 6){
        printf("Senha invalida. Deve ter 6 digitos.\n");
        return;
    }
    
    for(int i = 0; i < num_usuarios; i++){
        if(strcmp(cpf, usuarios[i].cpf) == 0){
            printf("CPF ja registrado.\n");
            return;
        }
    }

    Usuario novo_usuario;
    strcpy(novo_usuario.cpf, cpf);
    strcpy(novo_usuario.senha, senha);
    novo_usuario.saldo = 0.0;
    novo_usuario.num_transacoes = 0;
    novo_usuario.num_criptomoedas = 0;

    char nome[MAX_MOEDA_TAMANHO];
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    if(file != NULL){
        while(fscanf(file, "%s %*lf %*f %*f\n", nome) != EOF){
            strcpy(novo_usuario.cripto[novo_usuario.num_criptomoedas].nome, nome);
            novo_usuario.cripto[novo_usuario.num_criptomoedas].quantidade = 0.0;
            novo_usuario.num_criptomoedas++;
        }
    }
    fclose(file);

    usuarios[num_usuarios++] = novo_usuario;
    salvar_usuarios();
    printf("Usuario registrado com sucesso!\n");
}

int cadastrar_administrador(){
    FILE *arquivo;
    char cpf[MAX_CPF_TAMANHO], senha[MAX_SENHA_TAMANHO];

    arquivo = fopen(ARQUIVO_ADM, "w");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return 0; 
    }

    printf("Digite o CPF do administrador: ");
    scanf("%s", cpf);
    printf("Digite a senha do administrador: ");
    scanf("%s", senha);


    fprintf(arquivo, "%s\n%s\n", cpf, senha);

    fclose(arquivo);

    printf("Administrador registrado com sucesso!\n");
    return 1; 
}

int efetuar_login(){
    FILE *arquivo;
    char cpf[MAX_CPF_TAMANHO], senha[MAX_SENHA_TAMANHO];
    char cpf_arquivo[MAX_CPF_TAMANHO], senha_arquivo[MAX_SENHA_TAMANHO];


    arquivo = fopen(ARQUIVO_ADM, "r");
    if(arquivo == NULL){
        printf("Administrador nao cadastrado.\n");
        return 0; 
    }

    printf("Digite o CPF do administrador: ");
    scanf("%s", cpf);
    printf("Digite a senha do administrador: ");
    scanf("%s", senha);

    fscanf(arquivo, "%s\n%s", cpf_arquivo, senha_arquivo);

    fclose(arquivo);

    if(strcmp(cpf, cpf_arquivo) == 0 && strcmp(senha, senha_arquivo) == 0){
        printf("Login bem-sucedido!\n");
        return 1; 
    }else{
        printf("Login falhou! CPF ou senha incorretos.\n");
        return 0; 
    }
}

void excluir_usuario(){
    char cpf[MAX_CPF_TAMANHO];
    printf("Digite o CPF do usuario a ser excluido: ");
    scanf("%s", cpf);

    for(int i = 0; i < num_usuarios; i++){
        if(strcmp(usuarios[i].cpf, cpf) == 0){
            printf("Usuario encontrado: \nCPF: %s, Saldo em Reais: %.2f\n", usuarios[i].cpf, usuarios[i].saldo);
            printf("Confirma a exclusao?(s/n): ");
            char confirmacao;
            scanf(" %c", &confirmacao);
            if(confirmacao == 's' || confirmacao == 'S'){
                for(int j = i; j < num_usuarios - 1; j++){
                    usuarios[j] = usuarios[j + 1];
                }
                num_usuarios--;
                salvar_usuarios();
                printf("Usuario excluido com sucesso.\n");
            }
            return;
        }
    }
    printf("Usuario nao encontrado.\n");
}

// Funcao para cadastrar uma nova criptomoeda
void cadastrar_criptomoeda(){
    FILE *file = fopen(ARQUIVO_COTACAO, "a");
    if(file == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char nome[30];
    float taxaCompra, taxaVenda;
    double cotacaoInicial;

    printf("Nome da Criptomoeda: ");
    scanf("%s", nome);
    printf("Cotacao Inicial: ");
    scanf("%lf", &cotacaoInicial);
    printf("Taxa de Compra: ");
    scanf("%f", &taxaCompra);
    printf("Taxa de Venda: ");
    scanf("%f", &taxaVenda);

    fprintf(file, "%s %.2lf %.2f %.2f\n", nome, cotacaoInicial, taxaCompra, taxaVenda);
    fclose(file);
    printf("Criptomoeda cadastrada com sucesso!\n");
}

// Funcao para excluir uma criptomoeda pelo nome
void excluir_criptomoeda(){
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if(file == NULL || tempFile == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char nome[MAX_MOEDA_TAMANHO], nomeParaExcluir[MAX_MOEDA_TAMANHO];
    float taxaCompra, taxaVenda;
    double cotacaoInicial;

    printf("Nome da Criptomoeda para excluir: ");
    scanf("%s", nomeParaExcluir);

    int found = 0;
    while(fscanf(file, "%s %lf %f %f", nome, &cotacaoInicial, &taxaCompra, &taxaVenda) != EOF){
        if(strcmp(nome, nomeParaExcluir) != 0){
            fprintf(tempFile, "%s %.2lf %.2f %.2f\n", nome, cotacaoInicial, taxaCompra, taxaVenda);
        }else{
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove o arquivo original e renomeia o temporario
    remove(ARQUIVO_COTACAO);
    rename("temp.txt", ARQUIVO_COTACAO);

    if(found){
        printf("Criptomoeda excluida com sucesso!\n");
    }else{
        printf("Criptomoeda nao encontrada.\n");
    }
}

void consultar_saldo(){
    char cpf[MAX_CPF_TAMANHO];
    printf("Digite o CPF do usuario: ");
    scanf("%s", cpf);

    for(int i = 0; i < num_usuarios; i++){
        if(strcmp(usuarios[i].cpf, cpf) == 0){
            printf("Saldo do CPF %s:\n", usuarios[i].cpf);
            printf("Reais: R$%.2f\n", usuarios[i].saldo);

            FILE *file = fopen(ARQUIVO_COTACAO, "r");
            int linhas = 0;
            char caractere;
            while((caractere = fgetc(file)) != EOF){
                if(caractere == '\n'){
                    linhas++;
                }
            }
            fclose(file);

            for(int j = 0; j < linhas; j++){
                if(strlen(usuarios[i].cripto[j].nome) > 0){
                    printf("%s: %.8f\n", usuarios[i].cripto[j].nome, usuarios[i].cripto[j].quantidade);
                }
            }
        }
    }
}

void consultar_extrato(){
    char cpf[MAX_CPF_TAMANHO];
    int usuario_encontrado = 0;

    printf("Digite o CPF do usuario: ");
    scanf("%s", cpf);

    for(int i = 0; i < num_usuarios; i++){
        if(strcmp(usuarios[i].cpf, cpf) == 0){
            usuario_encontrado = 1;
            break;
        }
    }
    if(!usuario_encontrado){
        printf("Usuario nao encontrado.\n");
        return;
    }

    FILE *fp = fopen(ARQUIVO_EXTRATO, "r");
    if(fp == NULL){
        printf("Erro ao abrir o arquivo de extratos.\n");
        return;
    }

    printf("\nExtratos de Transacoes do CPF: %s\n", cpf);
    printf("-----------------------------------------------\n");

    char linha[255];
    while(fgets(linha, sizeof(linha), fp) != NULL){
        char cpf_check[MAX_CPF_TAMANHO];

        if(sscanf(linha, "CPF: %s |", cpf_check) == 1){
            if(strcmp(cpf, cpf_check) == 0){
                printf("%s", linha);
            }
        }
    }

    printf("-----------------------------------------------\n");
    fclose(fp);
}

void atualizar_cotacao(){
    char nome[MAX_MOEDA_TAMANHO];
    double cotacao;
    float taxaCompra, taxaVenda;
    double variacao;

    FILE *file = fopen(ARQUIVO_COTACAO, "r");

    int linhas = 0;
    char buffer[MAX_CRIPTOMOEDAS];
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        linhas++; 
    }
    rewind(file);

    Criptomoeda arquivo_novo[linhas];
    int contador = 0;
    while(fscanf(file, "%s %lf %f %f", nome, &cotacao, &taxaCompra, &taxaVenda) != EOF){
        variacao =(rand() /(double)RAND_MAX) * 0.1 - 0.05; // Variacao entre -5% e +5%
        cotacao *=(1 + variacao);

        strncpy(arquivo_novo[contador].nome, nome, MAX_MOEDA_TAMANHO);
        arquivo_novo[contador].cotacao = cotacao;
        arquivo_novo[contador].taxa_compra = taxaCompra;
        arquivo_novo[contador].taxa_venda = taxaVenda;
        contador++;

        printf("%s: %lf\n", nome, cotacao);
    }

    file = fopen(ARQUIVO_COTACAO, "w");
    for(int i=0; i < contador; i++){
        fprintf(file, "%s %lf %f %f\n", arquivo_novo[i].nome, arquivo_novo[i].cotacao, arquivo_novo[i].taxa_compra, arquivo_novo[i].taxa_venda);
    }
    fclose(file);
}

int main(){
    srand(time(NULL));
    ler_usuarios();
    int opcao;
    int admin_logado = 0;

    // Menu inicial para registrar ou fazer login
    do{
        printf("Escolha uma opcao:\n");
        printf("1. Cadastrar administrador\n");
        printf("2. Login como administrador\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                if(cadastrar_administrador()){
                    admin_logado = 1;
                }
                break;
            case 2:
                if(efetuar_login()){
                    admin_logado = 1;
                }else{
                    printf("\nTente novamente. ");
                }
                break;
            case 0:
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opcao invalida!\n");
                break;
        }

    }while(admin_logado == 0);

    printf("Acesso concedido ao painel de administracao.\n");
    // Menu de administracao
    do{
        printf("\nMenu Principal:\n");
        printf("1. Cadastro de Usuario\n");
        printf("2. Excluir Usuario\n");
        printf("3. Cadastro de Criptomoeda\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Consultar Saldo de Usuario\n");
        printf("6. Consultar Extrato de Usuario\n");
        printf("7. Atualizar Cotacao de Criptomoedas\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        switch(opcao){
            case 1: 
                cadastrar_usuario(); 
                break;
            case 2: 
                excluir_usuario(); 
                break;
            case 3: 
                cadastrar_criptomoeda(); 
                break;
            case 4: 
                excluir_criptomoeda(); 
                break;
            case 5: 
                consultar_saldo(); 
                break;
            case 6:
                consultar_extrato(); 
                break;
            case 7: 
                atualizar_cotacao(0); 
                break;
            case 0: 
                printf("Saindo...\n");
                break;
            default: 
                printf("Opcao invalida!\n"); 
                break;
        }
    }while(opcao != 0);

    return 0;
}
