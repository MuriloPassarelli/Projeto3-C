#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CPF_TAMANHO 12
#define MAX_SENHA_TAMANHO 7
#define ARQUIVO_USUARIOS "usuarios.txt"
#define MAX_USUARIOS 10

// Estrutura para armazenar dados do usuario
typedef struct {
    char cpf[MAX_CPF_TAMANHO];
    char senha[MAX_SENHA_TAMANHO];
    double saldo;
    double bitcoin;
    double ethereum;
    double ripple;
    int num_transacoes;
} Usuario;

Usuario usuarios[MAX_USUARIOS];
int num_usuarios = 0; // Contador de usuarios

// Funcao para salvar usuarios e saldos em um arquivo binario
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

// Funcao para ler usuarios e saldos do arquivo binario
void ler_usuarios() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }
    fread(&num_usuarios, sizeof(int), 1, file);
    fread(usuarios, sizeof(Usuario), num_usuarios, file);
    fclose(file);
}

// Funcao para registrar um novo usuario
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

// Funcao para verificar se o usuario existe e a senha esta correta
int verificar_usuario(const char *cpf, const char *senha) {
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(cpf, usuarios[i].cpf) == 0 && strcmp(senha, usuarios[i].senha) == 0) {
            return i;
        }
    }
    return -1;
}

// Funcao para visualizar saldo
void visualizar_saldo(int index) {
    printf("Seu saldo atual eh: %.2f\n", usuarios[index].saldo);
}

// Funcao para realizar um deposito
void realizar_deposito(int index) {
    double valor;
    printf("Informe o valor do deposito: ");
    scanf("%lf", &valor);
    if (valor <= 0) {
        printf("Valor invalido. O deposito deve ser positivo.\n");
        return;
    }
    usuarios[index].saldo += valor;
    salvar_usuarios();
    printf("Deposito realizado com sucesso! Novo saldo: %.2f\n", usuarios[index].saldo);
}

// Funcao para realizar um saque
void realizar_saque(int index) {
    double valor;
    printf("Informe o valor do saque: ");
    scanf("%lf", &valor);
    if (valor <= 0) {
        printf("Valor invalido. O saque deve ser positivo.\n");
        return;
    }
    if (valor > usuarios[index].saldo) {
        printf("Saldo insuficiente para realizar o saque.\n");
        return;
    }
    usuarios[index].saldo -= valor;
    salvar_usuarios();
    printf("Saque realizado com sucesso! Novo saldo: %.2f\n", usuarios[index].saldo);
}

// Funcao para exibir o menu principal
void menu_principal() {
    int opcao;
    do {
        printf("\n===================\n");
        printf("    Menu Principal   \n");
        printf("===================\n");
        printf("1. Registrar\n");
        printf("2. Login\n");
        printf("3. Sair\n");
        printf("===================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                registrar();
                break;
            case 2: {
                char cpf[MAX_CPF_TAMANHO];
                char senha[MAX_SENHA_TAMANHO];
                printf("Informe seu CPF: ");
                scanf("%s", cpf);
                printf("Informe sua senha: ");
                scanf("%s", senha);

                int index = verificar_usuario(cpf, senha);
                if (index != -1) {
                    printf("Login bem-sucedido! Bem-vindo, %s!\n", usuarios[index].cpf);
                    int opcao_usuario;
                    do {
                        printf("\n1. Visualizar Saldo\n");
                        printf("2. Realizar Deposito\n");
                        printf("3. Realizar Saque\n");
                        printf("4. Sair\n");
                        printf("Escolha uma opcao: ");
                        scanf("%d", &opcao_usuario);

                        switch (opcao_usuario) {
                            case 1:
                                visualizar_saldo(index);
                                break;
                            case 2:
                                realizar_deposito(index);
                                break;
                            case 3:
                                realizar_saque(index);
                                break;
                            case 4:
                                printf("Saindo do menu do usuario...\n");
                                break;
                            default:
                                printf("Opcao invalida.\n");
                        }
                    } while (opcao_usuario != 4);
                } else {
                    printf("CPF ou senha incorretos.\n");
                }
                break;
            }
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 3);
}

int main() {
    ler_usuarios(); // Carrega usuarios do arquivo
    menu_principal(); // Exibe menu principal
    return 0;
}