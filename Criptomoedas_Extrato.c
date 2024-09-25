// Função para obter a cotação de uma criptomoeda
double obter_cotacao(const char *criptomoeda) {
    static double cotacoes[] = {325751, 15325, 2}; // Bitcoin, Ethereum, Ripple
    static char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};

    for (int i = 0; i < 3; i++) {
        if (strcmp(criptomoeda, nomes[i]) == 0) {
            double variacao = (rand() / (double)RAND_MAX) * 0.1 - 0.05; // Variação entre -5% e +5%
            double nova_cotacao = cotacoes[i] * (1 + variacao);
            cotacoes[i] = nova_cotacao;
            return nova_cotacao;
        }
    }
    printf("Criptomoeda não encontrada.\n");
    return -1;
}


// Função para registrar uma transação
void registrar_transacao(int index, double valor, double taxa, int tipo) {
    if (usuarios[index].num_transacoes >= MAX_TRANSACOES) {
        printf("Número máximo de transações atingido.\n");
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
