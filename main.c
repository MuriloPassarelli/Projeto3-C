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

// OBTER COTACAO NOVA SENDO FEITA!! NAO ESTA COMPLETA!!
char obter_cotacoes(){
    static double cotacoes[][];
    FILE fp* = fopen(ARQUIVO_COTACAO, "r+");
    char buffer[100];

    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        sscanf(buffer, "%d,%d,%d", &bitcoin, &ethereum, &ripple);
    }

    fclose(fp);
}
double obter_cotacao_especifica(const char *criptomoeda){
    obter_cotacoes();
}

//OBTER COTACAO ANTIGO!!!
double obter_cotacao(const char *criptomoeda) {
    static char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
    static double cotacoes[] = {325751, 15325, 2}; // Bitcoin, Ethereum, Ripple
   
    for (int i = 0; i < 3; i++) {
        if (strcmp(criptomoeda, nomes[i]) == 0) {
            double variacao = (rand() / (double)RAND_MAX) * 0.1 - 0.05; // Variação entre -5% e +5%
            double nova_cotacao = cotacoes[i] * (1 + variacao);
            cotacoes[i] = nova_cotacao;
            return nova_cotacao;
        }
    }
    printf("Criptomoeda nao encontrada.\n");
    return -1;
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

// Função para registrar um novo usuário
void registrar() {
    if (num_usuarios >= MAX_USUARIOS) {
        printf("Numero maximo de usuarios atingido.\n");
        return;
    }
    char cpf[MAX_CPF_TAMANHO];
    char senha[MAX_SENHA_TAMANHO];
    printf("Informe seu CPF (somente numeros): ");
    scanf("%s", cpf);
    if (strlen(cpf) != 11) {
        printf("CPF invalido. Deve conter 11 numeros.\n");
        return;
    }
    printf("Informe sua senha (6 digitos, somente numeros): ");
    scanf("%s", senha);
    if (strlen(senha) != 6) {
        printf("Senha invalida. Deve ter 6 digitos.\n");
        return;
    }
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(cpf, usuarios[i].cpf) == 0) {
            printf("CPF ja registrado.\n");
            return;
        }
    }
    Usuario novo_usuario;
    strcpy(novo_usuario.cpf, cpf);
    strcpy(novo_usuario.senha, senha);
    novo_usuario.saldo = 0.0;
    novo_usuario.bitcoin = 0.0;
    novo_usuario.ethereum = 0.0;
    novo_usuario.ripple = 0.0;
    novo_usuario.num_transacoes = 0;
    usuarios[num_usuarios++] = novo_usuario;
    salvar_usuarios();
    printf("Usuario registrado com sucesso!\n");
}

// Função para verificar se o usuário existe e a senha está correta
int verificar_usuario(const char *cpf, const char *senha) {
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(cpf, usuarios[i].cpf) == 0 && strcmp(senha, usuarios[i].senha) == 0) {
            return i;
        }
    }
    return -1;
}

// Função para exibir o resumo das informações do usuário
void exibir_resumo(int index) {
    printf("\nResumo da Conta:\n");
    printf("CPF: %s\n", usuarios[index].cpf);
    printf("Reais: R$%.2f\n", usuarios[index].saldo);
    printf("Bitcoin: %.8f\n", usuarios[index].bitcoin);
    printf("Ethereum: %.8f\n", usuarios[index].ethereum);
    printf("Ripple: %.8f\n", usuarios[index].ripple);
}

// Função para consultar saldo do investidor
void consultar_saldo(int index) {
    printf("Saldo do CPF %s:\n", usuarios[index].cpf);
    printf("Reais: R$%.2f\n", usuarios[index].saldo);
    printf("Bitcoin: %.8f\n", usuarios[index].bitcoin);
    printf("Ethereum: %.8f\n", usuarios[index].ethereum);
    printf("Ripple: %.8f\n", usuarios[index].ripple);
}

// Função para registrar uma transação
void registrar_transacao(int index, double valor, double taxa, int tipo) {
    if (usuarios[index].num_transacoes >= MAX_TRANSACOES) {
        printf("Número maximo de transacoes atingido.\n");
        return;
    }
    double data = (double)time(NULL); // Data em timestamp
    usuarios[index].transacoes[usuarios[index].num_transacoes][0] = data;
    usuarios[index].transacoes[usuarios[index].num_transacoes][1] = valor;
    usuarios[index].transacoes[usuarios[index].num_transacoes][2] = taxa;
    usuarios[index].transacoes[usuarios[index].num_transacoes][3] = tipo; // 0 = Compra, 1 = Venda
    usuarios[index].num_transacoes++;
    salvar_usuarios();
    FILE *file = fopen(ARQUIVO_EXTRATO, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de extrato.\n");
        return;
    }
    fprintf(file, "CPF: %s | Data: %.0f | Valor: R$%.2f | Taxa: %.2f%% | Tipo: %s\n",
            usuarios[index].cpf, data, valor, taxa * 100, tipo == 0 ? "Compra" : "Venda");
    fclose(file);
}

// Função para depositar dinheiro na conta do investidor
void depositar(int index) {
    double valor;
    printf("Informe o valor a ser depositado: R$");
    scanf("%lf", &valor);
    if (valor <= 0) {
        printf("Valor invalido. Deve ser maior que zero.\n");
        return;
    }
    usuarios[index].saldo += valor;
    salvar_usuarios();
    registrar_transacao(index, valor, 0, -1); // -1 para depósito
    printf("Deposito de R$%.2f realizado com sucesso!\n", valor);
    exibir_resumo(index);
}

// Função para sacar dinheiro da conta do investidor
void sacar(int index) {
    char senha[MAX_SENHA_TAMANHO];
    double valor;
    printf("Informe sua senha (6 digitos): ");
    scanf("%s", senha);
    if (strcmp(senha, usuarios[index].senha) != 0) {
        printf("Senha incorreta.\n");
        return;
    }
    printf("Informe o valor a ser sacado: R$");
    scanf("%lf", &valor);
    if (valor <= 0) {
        printf("Valor invalido. Deve ser maior que zero.\n");
        return;
    }
    if (usuarios[index].saldo >= valor) {
        usuarios[index].saldo -= valor;
        salvar_usuarios();
        registrar_transacao(index, -valor, 0, -1); // -1 para saque
        printf("Saque de R$%.2f realizado com sucesso!\n", valor);
        exibir_resumo(index);
    } else {
        printf("Saldo insuficiente.\n");
    }
}

// Função para comprar criptomoedas
void comprar_criptomoedas(int index) {
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;
    printf("Informe sua senha (6 digitos): ");
    scanf("%s", senha);
    if (strcmp(senha, usuarios[index].senha) != 0) {
        printf("Senha incorreta.\n");
        return;
    }
    printf("\nOpcoes disponiveis para compra:\n");
    printf("1. Bitcoin (Preço Atual: R$%.2f)\n", obter_cotacao("Bitcoin"));
    printf("2. Ethereum (Preço Atual: R$%.2f)\n", obter_cotacao("Ethereum"));
    printf("3. Ripple (Preço Atual: R$%.2f)\n", obter_cotacao("Ripple"));
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    double preco, taxa, valor_total;
    switch (opcao) {
        case 1:
            preco = obter_cotacao("Bitcoin");
            taxa = 0.02;
            break;
        case 2:
            preco = obter_cotacao("Ethereum");
            taxa = 0.01;
            break;
        case 3:
            preco = obter_cotacao("Ripple");
            taxa = 0.01;
            break;
        default:
            printf("Opção invalida.\n");
            return;
    }
    printf("Informe a quantidade que deseja comprar: ");
    scanf("%lf", &quantidade);
    if (quantidade <= 0) {
        printf("Quantidade invalida. Deve ser maior que zero.\n");
        return;
    }
    valor_total = quantidade * preco;
    double valor_taxa = valor_total * taxa;
    double valor_final = valor_total + valor_taxa;
    if (usuarios[index].saldo >= valor_final) {
        usuarios[index].saldo -= valor_final;
        switch (opcao) {
            case 1: usuarios[index].bitcoin += quantidade; break;
            case 2: usuarios[index].ethereum += quantidade; break;
            case 3: usuarios[index].ripple += quantidade; break;
        }
        salvar_usuarios();
        registrar_transacao(index, valor_final, taxa, 0); // 0 para compra
        printf("Compra realizada com sucesso!\n");
        printf("Quantidade: %.8f | Valor Total: R$%.2f (Taxa: R$%.2f)\n", quantidade, valor_final, valor_taxa);
    } else {
        printf("Saldo insuficiente para realizar a compra.\n");
    }
}

// Função para vender criptomoedas
void vender_criptomoedas(int index) {
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;
    printf("Informe sua senha (6 digitos): ");
    scanf("%s", senha);
    if (strcmp(senha, usuarios[index].senha) != 0) {
        printf("Senha incorreta.\n");
        return;
    }
    printf("\nOpcoes disponiveis para venda:\n");
    printf("1. Bitcoin (Quantidade: %.8f)\n", usuarios[index].bitcoin);
    printf("2. Ethereum (Quantidade: %.8f)\n", usuarios[index].ethereum);
    printf("3. Ripple (Quantidade: %.8f)\n", usuarios[index].ripple);
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    double preco, taxa;
    switch (opcao) {
        case 1:
            if (usuarios[index].bitcoin <= 0) {
                printf("Voce nao possui Bitcoin.\n");
                return;
            }
            preco = obter_cotacao("Bitcoin");
            taxa = 0.03;
            break;
        case 2:
            if (usuarios[index].ethereum <= 0) {
                printf("Voce nao possui Ethereum.\n");
                return;
            }
            preco = obter_cotacao("Ethereum");
            taxa = 0.02;
            break;
        case 3:
            if (usuarios[index].ripple <= 0) {
                printf("Voce nao possui Ripple.\n");
                return;
            }
            preco = obter_cotacao("Ripple");
            taxa = 0.01;
            break;
        default:
            printf("Opcao invalida.\n");
            return;
    }
    printf("Informe a quantidade que deseja vender: ");
    scanf("%lf", &quantidade);
    if (quantidade <= 0) {
        printf("Quantidade invalida. Deve ser maior que zero.\n");
        return;
    }
    double valor_total = quantidade * preco;
    double valor_taxa = valor_total * taxa;
    double valor_final = valor_total - valor_taxa;
    switch (opcao) {
        case 1: 
            if (usuarios[index].bitcoin < quantidade) {
                printf("Quantidade de Bitcoin insuficiente.\n");
                return;
            }
            usuarios[index].bitcoin -= quantidade;
            break;
        case 2: 
            if (usuarios[index].ethereum < quantidade) {
                printf("Quantidade de Ethereum insuficiente.\n");
                return;
            }
            usuarios[index].ethereum -= quantidade;
            break;
        case 3: 
            if (usuarios[index].ripple < quantidade) {
                printf("Quantidade de Ripple insuficiente.\n");
                return;
            }
            usuarios[index].ripple -= quantidade;
            break;
    }
    usuarios[index].saldo += valor_final;
    salvar_usuarios();
    registrar_transacao(index, valor_final, taxa, 1); // 1 para venda
    printf("Venda realizada com sucesso!\n");
    printf("Quantidade: %.8f | Valor Total: R$%.2f (Taxa: R$%.2f)\n", quantidade, valor_final, valor_taxa);
}

// Exibir extratos do usuario
void exibir_extrato(const char *cpf){
    char extrato;
    FILE *fp = fopen(ARQUIVO_EXTRATO, "r");
        if(fp == NULL){
            printf("Erro ao abrir o arquivo de extratos.\n");
            return;
        }
        char linha[255];
        printf("\nExtratos de Transacoes do CPF: %S\n", cpf);
        printf("-----------------------------------------------\n");
        while(fgets(linha, sizeof(linha), fp) != NULL){
            printf("%s", linha);
        }
        printf("-----------------------------------------------\n");
    fclose(fp);
    return;
}

// Função principal
int main() {
    srand(time(NULL));
    ler_usuarios();
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