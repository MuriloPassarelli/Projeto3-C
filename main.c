#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CPF_TAMANHO 12
#define MAX_SENHA_TAMANHO 7
#define MAX_MOEDA_TAMANHO 20
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_EXTRATO "extrato.txt"
#define ARQUIVO_COTACOES "criptomoedas.txt" // Arquivo para cotações de criptomoedas
#define MAX_USUARIOS 10
#define MAX_TRANSACOES 100

// Estrutura para armazenar dados do usuário
typedef struct {
    char cpf[MAX_CPF_TAMANHO];
    char senha[MAX_SENHA_TAMANHO];
    double saldo;
    double bitcoin;
    double ethereum;
    double ripple;
    int num_transacoes;
    double transacoes[MAX_TRANSACOES][4]; // [data, valor, taxa, tipo]
} Usuario;

Usuario usuarios[MAX_USUARIOS];
int num_usuarios = 0; // Contador de usuários

// Variáveis globais para as cotações de criptomoedas
double cotacao_bitcoin = 325751;
double cotacao_ethereum = 15325;
double cotacao_ripple = 2;

// Função para ler as cotações de criptomoedas do arquivo
void ler_cotacoes() {
    FILE *file = fopen(ARQUIVO_COTACOES, "r");
    if (file == NULL) {
        printf("Arquivo de cotações não encontrado. Usando valores padrão.\n");
        return;
    }
    fscanf(file, "Bitcoin: %lf\nEthereum: %lf\nRipple: %lf\n", &cotacao_bitcoin, &cotacao_ethereum, &cotacao_ripple);
    fclose(file);
}

// Função para salvar as cotações de criptomoedas no arquivo
void salvar_cotacoes() {
    FILE *file = fopen(ARQUIVO_COTACOES, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de cotações.\n");
        return;
    }
    fprintf(file, "Bitcoin: %.2f\nEthereum: %.2f\nRipple: %.2f\n", cotacao_bitcoin, cotacao_ethereum, cotacao_ripple);
    fclose(file);
}

// Função para obter a cotação de uma criptomoeda e atualizá-la
double obter_cotacao(const char *criptomoeda) {
    double *cotacao;
    if (strcmp(criptomoeda, "Bitcoin") == 0) {
        cotacao = &cotacao_bitcoin;
    } else if (strcmp(criptomoeda, "Ethereum") == 0) {
        cotacao = &cotacao_ethereum;
    } else if (strcmp(criptomoeda, "Ripple") == 0) {
        cotacao = &cotacao_ripple;
    } else {
        printf("Criptomoeda nao encontrada.\n");
        return -1;
    }

    double variacao = (rand() / (double)RAND_MAX) * 0.1 - 0.05; // Variação entre -5% e +5%
    *cotacao = *cotacao * (1 + variacao);
    salvar_cotacoes(); // Atualiza o arquivo com a nova cotação

    return *cotacao;
}

// Função para salvar usuários e saldos em um arquivo binário
void salvar_usuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }
    fwrite(&num_usuarios, sizeof(int), 1, file);
    fwrite(usuarios, sizeof(Usuario), num_usuarios, file);
    fclose(file);
}

// Função para ler usuários e saldos do arquivo binário
void ler_usuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios. Criando novo arquivo.\n");
        return;
    }
    fread(&num_usuarios, sizeof(int), 1, file);
    fread(usuarios, sizeof(Usuario), num_usuarios, file);
    fclose(file);
}

// Outras funções (registrar, verificar_usuario, exibir_resumo, etc.) permanecem iguais

// Função principal
int main() {
    srand(time(NULL));
    ler_usuarios();
    ler_cotacoes(); // Ler as cotações do arquivo na inicialização

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Registrar\n");
        printf("2. Login\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrar();
                break;
            case 2: {
                char cpf[MAX_CPF_TAMANHO], senha[MAX_SENHA_TAMANHO];
                printf("Informe seu CPF: ");
                scanf("%s", cpf);
                printf("Informe sua senha: ");
                scanf("%s", senha);

                int index = verificar_usuario(cpf, senha);
                if (index >= 0) {
                    int menu_opcao;
                    do {
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

                        switch (menu_opcao) {
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
                                break;
                            case 5:
                                vender_criptomoedas(index);
                                break;
                            case 6:
                                exibir_extrato(index);
                                break;
                            case 7:
                                printf("Saindo...\n");
                                break;
                            default:
                                printf("Opção invalida. Tente novamente.\n");
                        }
                    } while (menu_opcao != 7);
                } else {
                    printf("CPF ou senha incorretos.\n");
                }
                break;
            }
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção invalida. Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}
