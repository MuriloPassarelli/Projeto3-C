// Função para exibir o menu principal
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
        printf("Escolha uma opção: ");
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
                    int opcao_investidor;
                    do {
                        printf("\n===================\n");
                        printf("  Menu do Investidor \n");
                        printf("===================\n");
                        printf("1. Consultar saldo\n");
                        printf("2. Depositar\n");
                        printf("3. Sacar\n");
                        printf("4. Comprar criptomoedas\n");
                        printf("5. Vender criptomoedas\n");
                        printf("6. Atualizar cotações\n");
                        printf("7. Sair\n");
                        printf("===================\n");
                        printf("Escolha uma opção: ");
                        scanf("%d", &opcao_investidor);
                        
                        switch (opcao_investidor) {
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
                            case 6: {
                                printf("Atualizando cotações...\n");
                                obter_cotacao("Bitcoin");
                                obter_cotacao("Ethereum");
                                obter_cotacao("Ripple");
                                printf("Cotações atualizadas com sucesso!\n");
                                break;
                            }
                            case 7:
                                printf("Saindo...\n");
                                break;
                            default:
                                printf("Opção inválida.\n");
                        }
                    } while (opcao_investidor != 7);
                } else {
                    printf("CPF ou senha incorretos.\n");
                }
                break;
            }
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 3);
}

int main() {
    srand((unsigned int)time(NULL)); // Inicializa gerador de números aleatórios
    ler_usuarios(); // Carrega usuários do arquivo
    menu_principal(); // Exibe menu principal
    return 0;
}
