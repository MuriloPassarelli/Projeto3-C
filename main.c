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

// Funcao para obter as cotacoes das moedas
double obter_cotacao(const char *criptomoeda){
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    if(file == NULL){
        file = fopen(ARQUIVO_COTACAO, "w"); // Cria o arquivo de cotacoes se nao existir um
        if(file == NULL){
            printf("Erro ao criar o arquivo de cotacoes.\n");
            return -1;
        }

        fprintf(file, "Bitcoin 325751.00 0.02 0.03\n");
        fprintf(file, "Ethereum 15325.00 0.01 0.02\n");
        fprintf(file, "Ripple 1.90 0.01 0.01\n");
        fclose(file);

        file = fopen(ARQUIVO_COTACAO, "r"); // Volta pro modo leitura
        if(file == NULL){
            printf("Erro ao abrir o arquivo apos criacao.\n");
            return -1;
        }
    }

    char nome[MAX_MOEDA_TAMANHO];
    double cotacao;
    float taxa_c, taxa_v;
    while(fscanf(file, "%s %lf %f %f", nome, &cotacao, &taxa_c, &taxa_v) != EOF){
        if(strcmp(criptomoeda, nome) == 0){
            fclose(file);
            return cotacao;
        }
    }
    printf("Criptomoeda nao encontrada.\n");
    fclose(file);
    return -1;
}

float obter_taxa(const char *criptomoeda, int modo){ // modo: 0=compra, 1=venda
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    if(file == NULL){
        printf("Erro ao abrir o arquivo de cotacoes.\n");
        return -1;
    }

    char nome[MAX_MOEDA_TAMANHO];
    double cotacao;
    float taxa_c, taxa_v;
    while(fscanf(file, "%s %lf %f %f", nome, &cotacao, &taxa_c, &taxa_v) != EOF){
        if(strcmp(criptomoeda, nome) == 0){
            fclose(file);
            if(modo == 0){
                return taxa_c;
            }else if(modo == 1){
                return taxa_v;
            }
        }
    }
    printf("Criptomoeda nao encontrada.\n");
    fclose(file);
    return -1;
}

//Funcao para atualizar as cotacoes apos uma transacao
void atualizar_cotacao(){
    char nome[MAX_MOEDA_TAMANHO];
    double cotacao, variacao;
    float taxa_c, taxa_v;
    
    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    if(file == NULL){
        printf("Erro ao abrir o arquivo de cotações.\n");
        return;
    }

    Criptomoeda criptomoedas[MAX_USUARIOS];
    int contador = 0;
    while(fscanf(file, "%s %lf %f %f", nome, &cotacao, &taxa_c, &taxa_v) != EOF){
        variacao =(rand() /(double)RAND_MAX) * 0.1 - 0.05; // Variação entre -5% e +5%
        cotacao *=(1 + variacao);

        strncpy(criptomoedas[contador].nome, nome, MAX_MOEDA_TAMANHO);
        criptomoedas[contador].cotacao = cotacao;
        criptomoedas[contador].taxa_compra = taxa_c;
        criptomoedas[contador].taxa_venda = taxa_v;
        contador++;
    }
    fclose(file);

    file = fopen(ARQUIVO_COTACAO, "w");
    if(file == NULL){
        printf("Erro ao reabrir o arquivo de cotações para escrita.\n");
        return;
    }
    for(int i = 0; i < contador; i++){
        fprintf(file, "%s %lf %f %f\n", criptomoedas[i].nome, criptomoedas[i].cotacao, criptomoedas[i].taxa_compra, criptomoedas[i].taxa_venda);
    }
    fclose(file);
}

// Funcao para salvar usuarios e saldos em um arquivo binario
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

// Funcao para verificar se o usuario existe e a senha esta correta
int verificar_usuario(const char *cpf, const char *senha){
    for(int i = 0; i < num_usuarios; i++){
        if(strcmp(cpf, usuarios[i].cpf) == 0 && strcmp(senha, usuarios[i].senha) == 0){
            return i;
        }
    }
    return -1;
}

// Funcao para exibir o resumo das informacoes do usuario
void exibir_resumo(int index){
    printf("\nResumo da Conta:\n");
    printf("CPF: %s\n", usuarios[index].cpf);
    printf("Reais: R$%.2f\n", usuarios[index].saldo);

    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    int linhas = 0;
    char caractere;
    while((caractere = fgetc(file)) != EOF){
        if(caractere == '\n'){
            linhas++;
        }
    }
    fclose(file);

    for(int i = 0; i < linhas; i++){
        if(strlen(usuarios[index].cripto[i].nome) > 0){
            printf("%s: %.8f\n", usuarios[index].cripto[i].nome, usuarios[index].cripto[i].quantidade);
        }
    }
}

// Funcao para consultar saldo do investidor
void consultar_saldo(int index){
    printf("Saldo do CPF %s:\n", usuarios[index].cpf);
    printf("Reais: R$%.2f\n", usuarios[index].saldo);

    FILE *file = fopen(ARQUIVO_COTACAO, "r");
    int linhas = 0;
    char caractere;
    while((caractere = fgetc(file)) != EOF){
        if(caractere == '\n'){
            linhas++;
        }
    }
    fclose(file);

    for(int i = 0; i < linhas; i++){
        if(strlen(usuarios[index].cripto[i].nome) > 0){
            printf("%s: %.8f\n", usuarios[index].cripto[i].nome, usuarios[index].cripto[i].quantidade);
        }
    }
}

// Funcao para registrar uma transacao
void registrar_transacao(int index, double valor, double taxa, int tipo){
    if(usuarios[index].num_transacoes >= MAX_TRANSACOES){
        printf("Numero maximo de transacoes atingido.\n");
        return;
    }
    double data =(double)time(NULL); // Data em timestamp
    usuarios[index].transacoes[usuarios[index].num_transacoes][0] = data;
    usuarios[index].transacoes[usuarios[index].num_transacoes][1] = valor;
    usuarios[index].transacoes[usuarios[index].num_transacoes][2] = taxa;
    usuarios[index].transacoes[usuarios[index].num_transacoes][3] = tipo; // 0 = Compra, 1 = Venda
    usuarios[index].num_transacoes++;
    salvar_usuarios();
    FILE *file = fopen(ARQUIVO_EXTRATO, "a");
    if(file == NULL){
        printf("Erro ao abrir o arquivo de extrato.\n");
        return;
    }
    fprintf(file, "CPF: %s | Data: %.0f | Valor: R$%.2f | Taxa: %.2f%% | Tipo: %s\n",
            usuarios[index].cpf, data, valor, taxa * 100, tipo == 0 ? "Compra" : "Venda");
    fclose(file);
}

// Funcao para depositar dinheiro na conta do investidor
void depositar(int index){
    double valor;
    printf("Informe o valor a ser depositado: R$");
    scanf("%lf", &valor);
    if(valor <= 0){
        printf("Valor invalido. Deve ser maior que zero.\n");
        return;
    }
    usuarios[index].saldo += valor;
    salvar_usuarios();
    registrar_transacao(index, valor, 0, -1); // -1 para deposito
    printf("Deposito de R$%.2f realizado com sucesso!\n", valor);
    exibir_resumo(index);
}

// Funcao para sacar dinheiro da conta do investidor
void sacar(int index){
    char senha[MAX_SENHA_TAMANHO];
    double valor;
    printf("Informe sua senha(6 digitos): ");
    scanf("%s", senha);
    if(strcmp(senha, usuarios[index].senha) != 0){
        printf("Senha incorreta.\n");
        return;
    }
    printf("Informe o valor a ser sacado: R$");
    scanf("%lf", &valor);
    if(valor <= 0){
        printf("Valor invalido. Deve ser maior que zero.\n");
        return;
    }
    if(usuarios[index].saldo >= valor){
        usuarios[index].saldo -= valor;
        salvar_usuarios();
        registrar_transacao(index, -valor, 0, -1); // -1 para saque
        printf("Saque de R$%.2f realizado com sucesso!\n", valor);
        exibir_resumo(index);
    }else{
        printf("Saldo insuficiente.\n");
    }
}

// Funcao para comprar criptomoedas
void comprar_criptomoedas(int index){
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;

    printf("Informe sua senha(6 digitos): ");
    scanf("%s", senha);
    if(strcmp(senha, usuarios[index].senha) != 0){
        printf("Senha incorreta.\n");
        return;
    }

    printf("\nOpcoes disponiveis para compra:\n");
    for(int i = 0; i < usuarios[index].num_criptomoedas; i++){
        printf("%d. %s(Preco Atual: R$%.2f)\n", i + 1, usuarios[index].cripto[i].nome, obter_cotacao(usuarios[index].cripto[i].nome));
    }

    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    if(opcao < 1 || opcao > usuarios[index].num_criptomoedas){
        printf("Opcao invalida.\n");
        return;
    }

    int cripto_index = opcao - 1;
    double preco = obter_cotacao(usuarios[index].cripto[cripto_index].nome);
    float taxa = obter_taxa(usuarios[index].cripto[cripto_index].nome, 0);

    printf("Informe a quantidade que deseja comprar: ");
    scanf("%lf", &quantidade);
    if(quantidade <= 0){
        printf("Quantidade invalida. Deve ser maior que zero.\n");
        return;
    }

    double valor_total = quantidade * preco;
    double valor_taxa = valor_total * taxa;
    double valor_final = valor_total + valor_taxa;

    if(usuarios[index].saldo >= valor_final){
        usuarios[index].saldo -= valor_final;
        usuarios[index].cripto[cripto_index].quantidade += quantidade;
        
        salvar_usuarios();
        registrar_transacao(index, valor_final, taxa, 0); // 0 para compra
        printf("Compra realizada com sucesso!\n");
        printf("Quantidade: %.8f | Valor Total: R$%.2f(Taxa: R$%.2f)\n", quantidade, valor_final, taxa);
    }else{
        printf("Saldo insuficiente para realizar a compra.\n");
    }
}

// Funcao para vender criptomoedas
void vender_criptomoedas(int index){
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;
    printf("Informe sua senha(6 digitos): ");
    scanf("%s", senha);
    if(strcmp(senha, usuarios[index].senha) != 0){
        printf("Senha incorreta.\n");
        return;
    }

    printf("\nOpcoes disponiveis para venda:\n");
    for(int i = 0; i < usuarios[index].num_criptomoedas; i++){
        printf("%d. %s(Preco Atual: R$%.2f)\n", i + 1, usuarios[index].cripto[i].nome, obter_cotacao(usuarios[index].cripto[i].nome));
    }

    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    if(opcao < 1 || opcao > usuarios[index].num_criptomoedas){
        printf("Opcao invalida.\n");
        return;
    }

    int cripto_index = opcao - 1;
    double preco = obter_cotacao(usuarios[index].cripto[cripto_index].nome);
    float taxa = obter_taxa(usuarios[index].cripto[cripto_index].nome, 0);
    
    printf("Informe a quantidade que deseja vender: ");
    scanf("%lf", &quantidade);
    if(quantidade <= 0){
        printf("Quantidade invalida. Deve ser maior que zero.\n");
        return;
    }

    double valor_total = quantidade * preco;
    double valor_taxa = valor_total * taxa;
    double valor_final = valor_total - valor_taxa;
    if(usuarios[index].cripto[cripto_index].quantidade < quantidade){
        printf("Quantidade insuficiente.\n");
    }else{
        usuarios[index].cripto[cripto_index].quantidade -= quantidade;
        usuarios[index].saldo += valor_final;
        salvar_usuarios();
        registrar_transacao(index, valor_final, taxa, 1); // 1 para venda
        printf("Venda realizada com sucesso!\n");
        printf("Quantidade: %.8f | Valor Total: R$%.2f(Taxa: R$%.2f)\n", quantidade, valor_final, taxa); 
    }
}

// Exibir extratos do usuario
void exibir_extrato(int index){
    FILE *fp = fopen(ARQUIVO_EXTRATO, "r");
    if(fp == NULL){
        printf("Erro ao abrir o arquivo de extratos.\n");
        return;
    }

    printf("\nExtratos de Transacoes do CPF: %s\n", usuarios[index].cpf);
    printf("-----------------------------------------------\n");

    char linha[255];
    while(fgets(linha, sizeof(linha), fp) != NULL){
        char cpf_check[MAX_CPF_TAMANHO];

        if(sscanf(linha, "CPF: %s |", cpf_check) == 1){
            if(strcmp(usuarios[index].cpf, cpf_check) == 0){
                printf("%s", linha);
            }
        }
    }

    printf("-----------------------------------------------\n");
    fclose(fp);
}

// Funcao principal
int main(){
    srand(time(NULL));
    ler_usuarios();
    int opcao;
    do{
        printf("\nMenu:\n");
        printf("1. Login\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:{
                char cpf[MAX_CPF_TAMANHO], senha[MAX_SENHA_TAMANHO];
                printf("Informe seu CPF: ");
                scanf("%s", cpf);
                printf("Informe sua senha: ");
                scanf("%s", senha);
                int index = verificar_usuario(cpf, senha);
                if(index >= 0){
                    int menu_opcao;
                    do{
                        printf("\nMenu Principal:\n");
                        printf("1. Consultar Saldo\n");
                        printf("2. Depositar\n");
                        printf("3. Sacar\n");
                        printf("4. Comprar Criptomoedas\n");
                        printf("5. Vender Criptomoedas\n");
                        printf("6. Exibir Extratos\n");
                        printf("7. Sair\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &menu_opcao);
                        switch(menu_opcao){
                            case 1:
                                consultar_saldo(index);
                                break;
                            case 2:
                                depositar(index);
                                break;
                            case 3:
                                sacar(index);
                                break;
                            case 4:
                                comprar_criptomoedas(index);
                                atualizar_cotacao();
                                break;
                            case 5:
                                vender_criptomoedas(index);
                                atualizar_cotacao();
                                break;
                            case 6:
                                exibir_extrato(index);
                                break;
                            case 7:
                                printf("Saindo...\n");
                                break;
                            default:
                                printf("Opcao invalida. Tente novamente.\n");
                        }
                    }while(menu_opcao != 7);
                }else{
                    printf("CPF ou senha incorretos.\n");
                }
                break;
            }
            case 2:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }while(opcao != 3);
    return 0;
}