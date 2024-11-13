
CCP230/CC2632 T.010.610 / Projeto 3
## Participantes do Grupo

- Nome: João Guilherme Faber
  - RA: 24.124.060-7

- Nome: Murilo Passarelli 
  - RA: 24.124.041-5

- Nome: Gabriel Furlani 
  - RA: 24.124.062-1


## Objetivo do Projeto

O código implementa uma função de administrador para o antigo código de compra e vendas de criptomoedas, com as novas opções de cadastrar um novo investidor, excluir investidor, cadastrar criptomoeda, excluir criptomoeda, consultar saldo de um investidor, consultar extrado de um investidor, atualizar cotação de criptomoedas.
O Projeto tem as seguintes regras:
- seu projeto deve armazenar dados em arquivos binários e texto simples (não precisam armazenar a mesma informação e cada tipo de arquivo pode ser usado para coisas diferentes)
- seu projeto deve usar arrays, structs e ponteiros para armazenar e gerenciar informações do seu programa
- so do GitHub Workflow e branches para cada funcionalidade do projeto
- README.md contendo descrição do projeto, como compilar e executar
## Compilação e Execução

Para compilar e executar o projeto, siga os seguintes passos:

1. Certifique-se de que você tem um compilador C instalado.
2. Clone o repositório para o seu computador:

    ```sh
    git clone <URL_DO_REPOSITÓRIO>
    ```
   
3. Navegue até o diretório do projeto:

    ```sh
    cd <NOME_DO_DIRETÓRIO>
    ```

4. Compile o código:

    ```sh
    gcc -o Projeto3-C
    ```
5. De o Comit no código:

   ```sh
   git commit -m "Primeiro commit na branch master"
   ```
7. De o Push no código:

    ```sh
    git push -u origin master
    ```
## Especificações
O programa do administrador deve ser desenvolvido em C e conter as opções:

Efetuar login​ usando CPF e senha: o menu principal deve aparecer apenas se estas informações estiverem corretas;
Cadastro de novo investidor: o usuário deve informar nome, CPF e senha do novo investidor;
Excluir investidor: o usuário deve informar o CPF do investidor. O programa deve exibir os dados encontrados para o CPF e pedir a confirmação para realizar a exclusão;
Cadastro de criptomoeda: devem ser informados nome, cotação inicial, taxa de compra e taxa de venda;
Excluir criptomoeda: usuário deve informar o nome da criptomoeda. O programa deve exibir os dados da moeda e pedir a confirmação da exclusão;
Consultar saldo de um investidor: deve ser informado o CPF do investidor e o saldo deve ser exibido na tela
Consultar extrado de um investidor: usuário informa o CPF do investidor e o extrado é exibido na tela
Atualizar cotação de criptomoedas: a função de atualização deve usar a mesma função do programa do investidor.
O programa do investidor deve ser modificado para poder fazer a compra de qualquer criptomoeda que foi cadastrado pelo administrador, mantendo as outras funcionalidades como na versão entregue no projeto 1.

## Forma de Uso

Após compilar e executar o programa, siga as instruções exibidas na tela para conseguir ter as novas funções de administrador do seu banco de criptomoedas, para todas as opções que for escolhida deve ter cofirmação antes.
