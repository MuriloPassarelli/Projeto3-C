#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USUARIOS 100
#define MAX_CRIPTOMOEDAS 50
#define MAX_NOME 100
#define MAX_CPF 12
#define MAX_SENHA 20
#define MAX_MOEDA_TAMANHO 100
#define ARQUIVO_COTACAO "cotacao.txt"
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_TRANSACOES "transacoes.txt"
#define ARQUIVO_ADM "adm_usuario.txt"


typedef struct {
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char senha[MAX_SENHA];
    float saldo;
} Usuario;

typedef struct {
    char nome[MAX_NOME];
    float cotacao;
    float taxa_compra;
    float taxa_venda;
} Criptomoeda;

Usuario usuarios[MAX_USUARIOS];
Criptomoeda criptomoedas[MAX_CRIPTOMOEDAS];
int num_usuarios = 0, num_criptomoedas = 0;

void carregar_usuarios() {
    FILE *file = fopen("usuarios.txt", "r");
    if (!file) return;
    while (fscanf(file, "%s %s %s %f\n", usuarios[num_usuarios].nome, usuarios[num_usuarios].cpf,
                  usuarios[num_usuarios].senha, &usuarios[num_usuarios].saldo) != EOF) {
        num_usuarios++;
    }
    fclose(file);
}

void carregar_criptomoedas() {
    FILE *file = fopen("criptomoedas.txt", "r");
    if (!file) return;
    while (fscanf(file, "%s %f %f %f\n", criptomoedas[num_criptomoedas].nome, &criptomoedas[num_criptomoedas].cotacao,
                  &criptomoedas[num_criptomoedas].taxa_compra, &criptomoedas[num_criptomoedas].taxa_venda) != EOF) {
        num_criptomoedas++;
    }
    fclose(file);
}

void salvar_usuarios() {
    FILE *file = fopen("usuarios.txt", "w");
    if (!file) return;
    for (int i = 0; i < num_usuarios; i++) {
        fprintf(file, "%s %s %s %.2f\n", usuarios[i].nome, usuarios[i].cpf, usuarios[i].senha, usuarios[i].saldo);
    }
    fclose(file);
}

void salvar_criptomoedas() {
    FILE *file = fopen("criptomoedas.txt", "w");
    if (!file) return;
    for (int i = 0; i < num_criptomoedas; i++) {
        fprintf(file, "%s %.2f %.2f %.2f\n", criptomoedas[i].nome, criptomoedas[i].cotacao, criptomoedas[i].taxa_compra,
                criptomoedas[i].taxa_venda);
    }
    fclose(file);
}

void cadastrar_usuario() {
    Usuario novo_usuario;
    printf("Digite o CPF: ");
    scanf("%s", novo_usuario.cpf);
    printf("Digite a senha: ");
    scanf("%s", novo_usuario.senha);
    novo_usuario.saldo = 0.0;
    usuarios[num_usuarios++] = novo_usuario;
    salvar_usuarios();
    printf("Usuário cadastrado com sucesso.\n");
}

int cadastrar_administrador() {
    FILE *arquivo;
    char cpf[MAX_CPF], senha[MAX_SENHA];

    arquivo = fopen("administrador.txt", "w");
    if (arquivo == NULL) {
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

int efetuar_login() {
    FILE *arquivo;
    char cpf[MAX_CPF], senha[MAX_SENHA];
    char cpf_arquivo[MAX_CPF], senha_arquivo[MAX_SENHA];


    arquivo = fopen("administrador.txt", "r");
    if (arquivo == NULL) {
        printf("Administrador não cadastrado.\n");
        return 0; 
    }

    printf("Digite o CPF do administrador: ");
    scanf("%s", cpf);
    printf("Digite a senha do administrador: ");
    scanf("%s", senha);

    fscanf(arquivo, "%s\n%s", cpf_arquivo, senha_arquivo);

    fclose(arquivo);

    if (strcmp(cpf, cpf_arquivo) == 0 && strcmp(senha, senha_arquivo) == 0) {
        printf("Login bem-sucedido!\n");
        return 1; 
    } else {
        printf("Login falhou! CPF ou senha incorretos.\n");
        return 0; 
    }
}
void excluir_usuario() {
    char cpf[MAX_CPF];
    printf("Digite o CPF do usuário a ser excluído: ");
    scanf("%s", cpf);

    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            printf("Usuário encontrado: %s, CPF: %s, Saldo: %.2f\n", usuarios[i].nome, usuarios[i].cpf, usuarios[i].saldo);
            printf("Confirma a exclusão? (s/n): ");
            char confirmacao;
            scanf(" %c", &confirmacao);
            if (confirmacao == 's' || confirmacao == 'S') {
                for (int j = i; j < num_usuarios - 1; j++) {
                    usuarios[j] = usuarios[j + 1];
                }
                num_usuarios--;
                salvar_usuarios();
                printf("Usuário excluído com sucesso.\n");
            }
            return;
        }
    }
    printf("Usuário não encontrado.\n");
}

// Função para cadastrar uma nova criptomoeda
void cadastrar_criptomoeda() {
    FILE *file = fopen("criptomoedas.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char nome[30];
    float cotacaoInicial, taxaCompra, taxaVenda;

    printf("Nome da Criptomoeda: ");
    scanf("%s", nome);
    printf("Cotação Inicial: ");
    scanf("%f", &cotacaoInicial);
    printf("Taxa de Compra: ");
    scanf("%f", &taxaCompra);
    printf("Taxa de Venda: ");
    scanf("%f", &taxaVenda);

    fprintf(file, "%s %.2f %.2f %.2f\n", nome, cotacaoInicial, taxaCompra, taxaVenda);
    fclose(file);
    printf("Criptomoeda cadastrada com sucesso!\n");
}

// Função para excluir uma criptomoeda pelo nome
void excluir_criptomoeda() {
    FILE *file = fopen("cotacao.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char nome[30], nomeParaExcluir[30];
    float cotacaoInicial, taxaCompra, taxaVenda;

    printf("Nome da Criptomoeda para excluir: ");
    scanf("%s", nomeParaExcluir);

    int found = 0;
    while (fscanf(file, "%s %f %f %f", nome, &cotacaoInicial, &taxaCompra, &taxaVenda) != EOF) {
        if (strcmp(nome, nomeParaExcluir) != 0) {
            fprintf(tempFile, "%s %.2f %.2f %.2f\n", nome, cotacaoInicial, taxaCompra, taxaVenda);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove o arquivo original e renomeia o temporário
    remove("criptomoedas.txt");
    rename("temp.txt", "criptomoedas.txt");

    if (found) {
        printf("Criptomoeda excluída com sucesso!\n");
    } else {
        printf("Criptomoeda não encontrada.\n");
    }
}

void consultar_saldo() {
    char cpf[MAX_CPF];
    printf("Digite o CPF do usuário: ");
    scanf("%s", cpf);

    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            printf("Saldo de %s (CPF: %s): %.2f\n", usuarios[i].nome, usuarios[i].cpf, usuarios[i].saldo);
            return;
        }
    }
    printf("Usuário não encontrado.\n");
    // consultar no txt
}

void consultar_extrato(int index) {
    if (index < 0 || index >= num_usuarios) {
        printf("Índice de usuário inválido.\n");
        return;
    }

    FILE *fp = fopen("extrato.txt", "r");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de extratos.\n");
        return;
    }

    char linha[255];
    printf("\nExtratos de Transacoes do CPF: %s\n", usuarios[index].cpf);
    printf("-----------------------------------------------\n");
    while (fgets(linha, sizeof(linha), fp) != NULL) {
        printf("%s", linha);
    }
    printf("-----------------------------------------------\n");
    fclose(fp);
}

void atualizar_cotacao(){
    char nome[MAX_MOEDA_TAMANHO];
    double cotacao;
    double variacao;

    FILE *file = fopen(ARQUIVO_COTACAO, "r");

    int linhas = 0;
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        linhas++; 
    }
    rewind(file);

    Criptomoeda arquivo_novo[linhas];
    int contador = 0;
    while(fscanf(file, "%s %lf", nome, &cotacao) != EOF){
        variacao = (rand() / (double)RAND_MAX) * 0.1 - 0.05; // Variacao entre -5% e +5%
        cotacao *= (1 + variacao);

        strncpy(arquivo_novo[contador].nome, nome, MAX_MOEDA_TAMANHO);
        arquivo_novo[contador].cotacao = cotacao;
        contador++;
    }

    file = fopen(ARQUIVO_COTACAO, "w");
    for(int i=0; i < contador; i++){
        fprintf(file, "%s %lf\n", arquivo_novo[i].nome, arquivo_novo[i].cotacao);
    }

    fclose(file);
}

int main() {
    char cpf[MAX_CPF], senha[MAX_SENHA];
    int opcao, usuario_index;
    int admin_logado = 0;

    // Menu inicial para registrar ou fazer login
    do {
        printf("Escolha uma opção:\n");
        printf("1. Cadastrar administrador\n");
        printf("2. Login como administrador\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (cadastrar_administrador()) {
                    admin_logado = 1;
                }
                break;
            case 2:
                if (efetuar_login()) {
                    admin_logado = 1;
                }
                break;
            case 0:
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opção inválida!\n");
                break;
        }

    } while (admin_logado == 0);

     printf("Acesso concedido ao painel de administração.\n");
// Menu de administração
do {
    printf("\nMenu Principal:\n");
    printf("1. Cadastro de Usuário\n");
    printf("2. Excluir Usuário\n");
    printf("3. Cadastro de Criptomoeda\n");
    printf("4. Excluir Criptomoeda\n");
    printf("5. Consultar Saldo de Usuário\n");
    printf("6. Consultar Extrato de Usuário\n");
    printf("7. Atualizar Cotação de Criptomoedas\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch(opcao) {
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
            printf("Digite o índice do usuário para consultar o extrato: ");
            scanf("%d", &usuario_index);
            consultar_extrato(usuario_index); 
            break;
        case 7: 
            atualizar_cotacao(0); 
            break;
        case 0: 
            printf("Saindo...\n");
            break;
        default: 
            printf("Opção inválida!\n"); 
            break;
    }
} while (opcao != 0);

return 0;
}