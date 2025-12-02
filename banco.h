struct Conta {
    char nome[60];
    int numConta;
    double saldo;
};
typedef struct Conta CONTA;

void criaConta();
void acessaExibeConta(int codConta);
void listarContas();
void depositaValor(int codConta, double valor);
int saque(double qtdSaque, int codConta);
void escolheoperacao(int codConta);
void determinaacao(int* temconta, int* codConta);
void cabecalho();
int existeConta(int codConta);
void menuColaborador();