#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura para armazenar dados de investidor
typedef struct {
    char nome[50];
    char cpf[12];
    char senha[20];
    double saldo;
} Investidor;

// Estrutura para armazenar dados de criptomoeda
typedef struct {
    char nome[20];
    double cotacao;
    double taxaCompra;
    double taxaVenda;
} Criptomoeda;

// Funções auxiliares
bool verificarLogin(char cpf[], char senha[]);
void cadastrarInvestidor();
void excluirInvestidor();
void cadastrarCriptomoeda();
void excluirCriptomoeda();
void consultarSaldoInvestidor();
void consultarExtratoInvestidor();
void atualizarCotacaoCriptomoedas();

// Função principal
int main() {
    char cpf[12];
    char senha[20];
    
    printf("== Sistema Administrador de Exchange ==\n");
    printf("Digite seu CPF: ");
    scanf("%s", cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    // Verificar login
    if (!verificarLogin(cpf, senha)) {
        printf("CPF ou senha incorretos.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n== Menu Principal ==\n");
        printf("1. Cadastrar novo investidor\n");
        printf("2. Excluir investidor\n");
        printf("3. Cadastrar criptomoeda\n");
        printf("4. Excluir criptomoeda\n");
        printf("5. Consultar saldo de investidor\n");
        printf("6. Consultar extrato de investidor\n");
        printf("7. Atualizar cotacao de criptomoedas\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarInvestidor();
                break;
            case 2:
                excluirInvestidor();
                break;
            case 3:
                cadastrarCriptomoeda();
                break;
            case 4:
                excluirCriptomoeda();
                break;
            case 5:
                consultarSaldoInvestidor();
                break;
            case 6:
                consultarExtratoInvestidor();
                break;
            case 7:
                atualizarCotacaoCriptomoedas();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
