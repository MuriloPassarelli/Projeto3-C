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

int efetuar_login(const char *cpf, const char *senha) {
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void cadastrar_usuario() {
    Usuario novo_usuario;
    printf("Digite o nome: ");
    scanf("%s", novo_usuario.nome);
    printf("Digite o CPF: ");
    scanf("%s", novo_usuario.cpf);
    printf("Digite a senha: ");
    scanf("%s", novo_usuario.senha);
    novo_usuario.saldo = 0.0;
    usuarios[num_usuarios++] = novo_usuario;
    salvar_usuarios();
    printf("Usuário cadastrado com sucesso.\n");
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

void cadastrar_criptomoeda() {
    Criptomoeda nova_criptomoeda;
    printf("Digite o nome da criptomoeda: ");
    scanf("%s", nova_criptomoeda.nome);
    printf("Digite a cotação inicial: ");
    scanf("%f", &nova_criptomoeda.cotacao);
    printf("Digite a taxa de compra: ");
    scanf("%f", &nova_criptomoeda.taxa_compra);
    printf("Digite a taxa de venda: ");
    scanf("%f", &nova_criptomoeda.taxa_venda);
    criptomoedas[num_criptomoedas++] = nova_criptomoeda;
    salvar_criptomoedas();
    printf("Criptomoeda cadastrada com sucesso.\n");

    // modificar no txt
}

void excluir_criptomoeda() {
    char nome[MAX_NOME];
    printf("Digite o nome da criptomoeda a ser excluída: ");
    scanf("%s", nome);

    for (int i = 0; i < num_criptomoedas; i++) {
        if (strcmp(criptomoedas[i].nome, nome) == 0) {
            printf("Criptomoeda encontrada: %s, Cotação: %.2f, Taxa de Compra: %.2f, Taxa de Venda: %.2f\n", criptomoedas[i].nome,
                   criptomoedas[i].cotacao, criptomoedas[i].taxa_compra, criptomoedas[i].taxa_venda);
            printf("Confirma a exclusão? (s/n): ");
            char confirmacao;
            scanf(" %c", &confirmacao);
            if (confirmacao == 's' || confirmacao == 'S') {
                for (int j = i; j < num_criptomoedas - 1; j++) {
                    criptomoedas[j] = criptomoedas[j + 1];
                }
                num_criptomoedas--;
                salvar_criptomoedas();
                printf("Criptomoeda excluída com sucesso.\n");
            }
            return;
        }
    }
    printf("Criptomoeda não encontrada.\n");
    // modificar no txt
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
    int index = 0;

    carregar_usuarios();
    carregar_criptomoedas();

    printf("Digite o CPF do administrador: ");
    scanf("%s", cpf);
    printf("Digite a senha do administrador: ");
    scanf("%s", senha);

    if (!efetuar_login(cpf, senha)) {
        printf("Login falhou! Tente novamente.\n");
        return 1;
    }

 int opcao, usuario_index;
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
            case 1: cadastrar_usuario(); break;
            case 2: excluir_usuario(); break;
            case 3: cadastrar_criptomoeda(); break;
            case 4: excluir_criptomoeda(); break;
            case 5: consultar_saldo(); break;
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