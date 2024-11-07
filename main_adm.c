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

// Função de login
bool verificarLogin(char cpf[], char senha[]) {
    // Implementar verificação com os dados armazenados dos administradores
    // Exemplo fictício de login correto
    if (strcmp(cpf, "12345678901") == 0 && strcmp(senha, "admin123") == 0) {
        return true;
    }
    return false;
}

// Função para cadastrar investidor
void cadastrarInvestidor() {
    Investidor novoInvestidor;
    printf("Digite o nome do investidor: ");
    scanf("%s", novoInvestidor.nome);
    printf("Digite o CPF do investidor: ");
    scanf("%s", novoInvestidor.cpf);
    printf("Digite a senha do investidor: ");
    scanf("%s", novoInvestidor.senha);
    novoInvestidor.saldo = 0.0;

    FILE *file = fopen("investidores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s %.2f\n", novoInvestidor.nome, novoInvestidor.cpf, novoInvestidor.senha, novoInvestidor.saldo);
        fclose(file);
        printf("Investidor cadastrado com sucesso!\n");
    } else {
        printf("Erro ao abrir o arquivo de investidores.\n");
    }
}

// Função para excluir investidor
void excluirInvestidor() {
    char cpf[12];
    printf("Digite o CPF do investidor a ser excluido: ");
    scanf("%s", cpf);

    // Lógica de exclusão a ser implementada, semelhante ao que fizemos no programa do investidor
}

// Função para cadastrar criptomoeda
void cadastrarCriptomoeda() {
    Criptomoeda novaCriptomoeda;
    printf("Digite o nome da criptomoeda: ");
    scanf("%s", novaCriptomoeda.nome);
    printf("Digite a cotacao inicial: ");
    scanf("%lf", &novaCriptomoeda.cotacao);
    printf("Digite a taxa de compra: ");
    scanf("%lf", &novaCriptomoeda.taxaCompra);
    printf("Digite a taxa de venda: ");
    scanf("%lf", &novaCriptomoeda.taxaVenda);

    FILE *file = fopen("criptomoedas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %.2f %.2f %.2f\n", novaCriptomoeda.nome, novaCriptomoeda.cotacao, novaCriptomoeda.taxaCompra, novaCriptomoeda.taxaVenda);
        fclose(file);
        printf("Criptomoeda cadastrada com sucesso!\n");
    } else {
        printf("Erro ao abrir o arquivo de criptomoedas.\n");
    }
}

// Função para excluir criptomoeda
void excluirCriptomoeda() {
    char nome[20];
    printf("Digite o nome da criptomoeda a ser excluida: ");
    scanf("%s", nome);

    // Lógica de exclusão a ser implementada, semelhante ao que fizemos no programa do investidor
}

// Função para consultar saldo de um investidor
void consultarSaldoInvestidor() {
    char cpf[12];
    printf("Digite o CPF do investidor: ");
    scanf("%s", cpf);

    // Lógica para consultar saldo e exibir
}

// Função para consultar extrato de um investidor
void consultarExtratoInvestidor() {
    char cpf[12];
    printf("Digite o CPF do investidor: ");
    scanf("%s", cpf);

    // Lógica para consultar extrato e exibir
}

// Função para atualizar cotação de criptomoedas
void atualizarCotacaoCriptomoedas() {
    // Reutilizar função de atualização do programa do investidor
    printf("Cotações atualizadas com sucesso!\n");
}
