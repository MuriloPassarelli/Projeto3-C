CCP230/CC2632 T.010.610 / Projeto 1

João Guilherme Faber      RA: 24.124.060-7
Murilo Passarelli         RA: 24.124.041-5
Gabriel Furlani           RA: 24.124.062-1

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Objetivo do Projeto

O objetivo deste projeto é desenvolver uma Exchange de Criptomoedas em C, que permita aos usuários comprar, vender e gerenciar criptomoedas. A Exchange suportará três tipos de criptomoedas: Bitcoin, Ethereum e Ripple, cada uma com diferentes taxas para transações de compra e venda. O sistema permitirá realizar operações básicas como login, consulta de saldo, depósitos, saques, e também a compra e venda de criptomoedas, com funcionalidades adicionais para a atualização de cotações e armazenamento de dados.

Especificações

A Exchange suportará as seguintes criptomoedas e taxas:

Bitcoin (BTC)

Taxa de Compra: 2%

Taxa de Venda: 3%

Ethereum (ETH)

Taxa de Compra: 1%

Taxa de Venda: 2%

Ripple (XRP)

Taxa de Compra: 1%

Taxa de Venda: 1%

Funcionalidades

Login


Usuários devem autenticar-se com CPF e senha.

Consulta de Saldo


Exibir saldo de reais, bitcoin, ethereum e ripple da carteira de investimentos.

Consulta de Extrato


Exibir o extrato de operações, incluindo data, valores e taxas, salvo em arquivo texto.

Depósito


Permitir o depósito de reais na carteira de investimento.

Saque


Permitir o saque de reais, com validação de senha.

Compra de Criptomoedas


Permitir a compra de criptomoedas, exibindo informações da compra e taxa cobrada.

Venda de Criptomoedas


Permitir a venda de criptomoedas, exibindo informações da venda e taxa cobrada.

Atualização de Cotação


Atualizar a cotação das criptomoedas com variação aleatória de no máximo 5% e mínimo -5% do valor atual.

Armazenamento de Dados


Armazenar dados de até 10 usuários e o histórico das últimas 100 transações por usuário em arquivos binários.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Compilação


Utilize um compilador de C como gcc. Navegue até o diretório do projeto e compile o código com o comando:  gcc -o exchange exchange.c

Execução


Após a compilação, execute o programa gerado com o comando: ./exchange

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

1. Registro de Usuário

   
Para registrar um novo usuário:
Selecione a opção 1. Registrar no menu principal.
Informe o CPF (deve conter 11 dígitos) e a senha (6 dígitos).
O usuário será registrado com um saldo inicial de R$0,00 e sem criptomoedas.

3. Login
Para acessar a conta:
Selecione a opção 2. Login no menu principal.
Informe seu CPF e senha.
Se as credenciais estiverem corretas, você será autenticado e terá acesso ao Menu do Investidor.

3. Menu do Investidor
Depois de um login bem-sucedido, o Menu do Investidor oferece as seguintes opções:

1. Consultar saldo
Exibe o saldo atual em reais, Bitcoin, Ethereum e Ripple.

3. Depositar
Permite depositar um valor em reais na conta. Informe o valor a ser depositado (deve ser maior que zero).

3. Sacar
Permite sacar reais da conta. Informe o valor do saque e a senha para validação. O valor do saque deve ser menor ou igual ao saldo disponível.

5. Comprar criptomoedas
Permite comprar Bitcoin, Ethereum ou Ripple. Informe a criptomoeda desejada, a quantidade a ser comprada e a senha para validação. O valor da compra incluirá a taxa correspondente.

5. Vender criptomoedas
Permite vender Bitcoin, Ethereum ou Ripple. Informe a criptomoeda desejada, a quantidade a ser vendida e a senha para validação. O valor da venda será creditado na sua conta, descontando a taxa correspondente.

7. Atualizar cotação
Atualiza a cotação das criptomoedas (Bitcoin, Ethereum e Ripple) com uma variação aleatória entre -5% e +5%.

7. Sair
Sai do Menu do Investidor e retorna ao Menu Principal.

4. Menu Principal
   
1. Registrar
Para registrar um novo usuário (veja acima).

3. Login
Para fazer login na conta existente (veja acima).

3. Sair
Para sair do programa.

Observações
O programa utiliza arquivos de texto para armazenar informações sobre os usuários e suas transações. As atualizações nas informações dos usuários são salvas automaticamente após cada operação.
As cotações das criptomoedas são atualizadas com uma variação aleatória sempre que a opção "Atualizar cotação" é selecionada.




