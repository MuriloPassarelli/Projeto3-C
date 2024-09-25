// Função para comprar criptomoedas
void comprar_criptomoedas(int index) {
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;
    
    printf("Informe sua senha (6 dígitos): ");
    scanf("%s", senha);
    if (strcmp(senha, usuarios[index].senha) != 0) {
        printf("Senha incorreta.\n");
        return;
    }

    printf("\nOpções disponíveis para compra:\n");
    printf("1. Bitcoin (Preço Atual: R$%.2f)\n", obter_cotacao("Bitcoin"));
    printf("2. Ethereum (Preço Atual: R$%.2f)\n", obter_cotacao("Ethereum"));
    printf("3. Ripple (Preço Atual: R$%.2f)\n", obter_cotacao("Ripple"));
    printf("Informe o número correspondente à criptomoeda desejada: ");
    scanf("%d", &opcao);
    
    const char *moeda;
    double taxa;
    
    switch (opcao) {
        case 1:
            moeda = "Bitcoin";
            taxa = 0.03;
            break;
        case 2:
            moeda = "Ethereum";
            taxa = 0.02;
            break;
        case 3:
            moeda = "Ripple";
            taxa = 0.01;
            break;
        default:
            printf("Opção inválida.\n");
            return;
    }

    double cotacao = obter_cotacao(moeda);
    if (cotacao <= 0) {
        printf("Erro ao obter a cotação da criptomoeda.\n");
        return;
    }

    printf("Informe a quantidade a ser comprada (em unidades): ");
    scanf("%lf", &quantidade);
    if (quantidade <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }

    double valor_total = quantidade * cotacao;
    double taxa_total = valor_total * taxa;
    double valor_final = valor_total + taxa_total;

    if (usuarios[index].saldo >= valor_final) {
        printf("Confirma a compra de %.2lf unidades de %s por R$%.2f (taxa de %.2f%%: R$%.2f)? (s/n): ",
               quantidade, moeda, valor_total, taxa * 100, taxa_total);
        char confirmacao;
        scanf(" %c", &confirmacao);
        if (confirmacao == 's' || confirmacao == 'S') {
            usuarios[index].saldo -= valor_final;
            if (opcao == 1) usuarios[index].bitcoin += quantidade;
            else if (opcao == 2) usuarios[index].ethereum += quantidade;
            else if (opcao == 3) usuarios[index].ripple += quantidade;

            salvar_usuarios();
            registrar_transacao(index, valor_final, taxa, 0); // 0 para compra
            printf("Compra realizada com sucesso!\n");
            exibir_resumo(index);
        } else {
            printf("Compra cancelada.\n");
        }
    } else {
        printf("Saldo insuficiente.\n");
    }
}


// Função para vender criptomoedas
void vender_criptomoedas(int index) {
    char senha[MAX_SENHA_TAMANHO];
    int opcao;
    double quantidade;

    printf("Informe sua senha (6 dígitos): ");
    scanf("%s", senha);
    if (strcmp(senha, usuarios[index].senha) != 0) {
        printf("Senha incorreta.\n");
        return;
    }

    printf("\nOpções disponíveis para venda:\n");
    printf("1. Bitcoin (Quantidade: %.8f)\n", usuarios[index].bitcoin);
    printf("2. Ethereum (Quantidade: %.8f)\n", usuarios[index].ethereum);
    printf("3. Ripple (Quantidade: %.8f)\n", usuarios[index].ripple);
    printf("Informe o número correspondente à criptomoeda desejada: ");
    scanf("%d", &opcao);

    const char *moeda;
    double taxa;

    switch (opcao) {
        case 1:
            moeda = "Bitcoin";
            taxa = 0.03;
            if (usuarios[index].bitcoin <= 0) {
                printf("Você não possui Bitcoin.\n");
                return;
            }
            break;
        case 2:
            moeda = "Ethereum";
            taxa = 0.02;
            if (usuarios[index].ethereum <= 0) {
                printf("Você não possui Ethereum.\n");
                return;
            }
            break;
        case 3:
            moeda = "Ripple";
            taxa = 0.01;
            if (usuarios[index].ripple <= 0) {
                printf("Você não possui Ripple.\n");
                return;
            }
            break;
        default:
            printf("Opção inválida.\n");
            return;
    }

    double cotacao = obter_cotacao(moeda);
    if (cotacao <= 0) {
        printf("Erro ao obter a cotação da criptomoeda.\n");
        return;
    }

    printf("Informe a quantidade a ser vendida (em unidades): ");
    scanf("%lf", &quantidade);
    if (quantidade <= 0 || (opcao == 1 && quantidade > usuarios[index].bitcoin) ||
        (opcao == 2 && quantidade > usuarios[index].ethereum) ||
        (opcao == 3 && quantidade > usuarios[index].ripple)) {
        printf("Quantidade inválida.\n");
        return;
    }

    double valor_total = quantidade * cotacao;
    double taxa_total = valor_total * taxa;
    double valor_final = valor_total - taxa_total;

    printf("Confirma a venda de %.2lf unidades de %s por R$%.2f (taxa de %.2f%%: R$%.2f)? (s/n): ",
           quantidade, moeda, valor_total, taxa * 100, taxa_total);
    char confirmacao;
    scanf(" %c", &confirmacao);
    if (confirmacao == 's' || confirmacao == 'S') {
        if (opcao == 1) usuarios[index].bitcoin -= quantidade;
        else if (opcao == 2) usuarios[index].ethereum -= quantidade;
        else if (opcao == 3) usuarios[index].ripple -= quantidade;

        usuarios[index].saldo += valor_final;
        salvar_usuarios();
        registrar_transacao(index, valor_final, taxa, 1); // 1 para venda
        printf("Venda realizada com sucesso!\n");
        exibir_resumo(index);
    } else {
        printf("Venda cancelada.\n");
    }
}
