#include <stdio.h>
#include <stdlib.h>
#include "banco.h"
#include <time.h>

void criaconta(){
    CONTA novaConta;
    printf("Digite seu nome completo: ");
    scanf(" %59[^\n]", novaConta.nome);
    novaConta.saldo = 0.0;
    
    int tentativas = 0;
    
    do{
    novaConta.numConta = rand() % 10000000 + 1000000; // Gera um número de conta aleatório de 7 dígitos
    tentativas++;

    if(tentativas > 10){
        printf("Não foi possível gerar um número de conta único. Tente novamente mais tarde.\n");
        return;
    }
    }while(existeConta(novaConta.numConta));

    FILE* f;
    f =fopen("banco.dat", "ab");
    if (f == NULL) {
        printf("Erro no banco de dados!\n");
        return;
    }
    
    fwrite(&novaConta, sizeof(CONTA), 1, f);
    printf("Conta criada com sucesso!\n");
    printf("O código da sua conta é: %d\n", novaConta.numConta);
    printf("Seu saldo inicial é: R$ %.2f\n", novaConta.saldo);
    fclose(f);
}

void listarContas(){
    FILE* f = fopen("banco.dat", "rb");
    if (f == NULL){
        printf("Erro no banco de dados!\n");
        exit(1);
    }
    
    CONTA contaLida;
    while(fread(&contaLida, sizeof(CONTA), 1, f)){
        printf("Nome: %s\n", contaLida.nome);
        printf("Numero da Conta: %d\n", contaLida.numConta);
        printf("Saldo: R$ %.2f\n", contaLida.saldo);
        printf("-------------------------\n");
    }
    fclose(f);
}

void acessaExibeConta(int codConta){
    FILE* f = fopen("banco.dat", "rb");
    if (f == NULL){
        printf("Erro no banco de dados!\n");
        exit(1);
    }

    CONTA contaLida;
    int achouConta = 0;
    while(fread(&contaLida, sizeof(CONTA), 1, f)){
        if(contaLida.numConta == codConta){
            printf("Bem-vindo, %s!\n", contaLida.nome);
            printf("Seu saldo atual é: R$ %.2f\n", contaLida.saldo);
            achouConta = 1;
            break;
        }
    }
    fclose(f);
    if(!achouConta){
        printf("Conta não encontrada!\n");
    }
}

void depositaValor(int codConta, double valor){
   FILE* f;
   f = fopen("banco.dat", "rb+");
   if (f == NULL){
         printf("Erro no banco de dados!\n");
         exit(1);
   }   
    CONTA contaLida;
    int achouConta = 0;

    while(fread(&contaLida, sizeof(CONTA), 1, f)){
        if(codConta == contaLida.numConta){
            contaLida.saldo = contaLida.saldo + valor;
            fseek(f, -sizeof(CONTA), SEEK_CUR);
            fwrite(&contaLida, sizeof(CONTA), 1, f);
            printf("Depósito de R$ %.2f realizado com sucesso!\n", valor);
            achouConta = 1;
            break;
        }
    }
    if(!achouConta){
        printf("Conta não encontrada!\n");
    }
    fclose(f);
}

int saque(double qtdSaque, int codConta){
    FILE* f;
    f = fopen("banco.dat", "rb+");
    if(f == NULL){
        printf("Erro no banco de dados!\n");
        exit(1);
    }

    CONTA contaLida;

    while(fread(&contaLida, sizeof(CONTA), 1, f)){
        if(codConta == contaLida.numConta){
            if(contaLida.saldo >= qtdSaque){
                contaLida.saldo = contaLida.saldo - qtdSaque;
                fseek(f, -sizeof(CONTA), SEEK_CUR);
                fwrite(&contaLida, sizeof(CONTA), 1, f);
                printf("Saque de R$ %.2f realizado com sucesso!\n", qtdSaque);
                printf("Seu saldo atual é: R$ %.2f\n", contaLida.saldo);
                fclose(f);
                return 1;
                // esqueci de validar o saque
            }else{
                printf("Saldo insuficiente para realizar o saque!\n");
                fclose(f);
                return 0;
            }
        }
        
    }
    fclose(f);
    printf("Conta não encontrada!\n");
    return 0; // Conta não encontrada
}

void cabecalho(){
    printf("---------------------------------\n");    
    printf("|            KvSinvesT          |\n");
    printf("---------------------------------\n");
}

void determinaacao(int* temconta, int* codConta){
    printf("Você já possui uma conta? (1-sim / 2-não / 3- Sou colaborador): ");
    scanf("%d", temconta);
    if(*temconta == 1){
        printf("Digite o código da sua conta: ");
        scanf(" %d", codConta);
        acessaExibeConta(*codConta);
    }else if(*temconta == 2){
        printf("Iniciando processo de criação de conta...\n");
        criaconta();
    }else if(*temconta == 3){
        printf("Digite seu acesso para colaboradores: \n");
        scanf(" %d", codConta);
        if(*codConta == 1011956 || *codConta == 1012018){
            printf("Acesso concedido.\n");
        }else{
            printf("Acesso negado. Código inválido.\n");
            exit(1);
        }
    }else{
        printf("Opção inválida. Por favor, tente novamente.\n");
    }
    
}

void escolheoperacao(int codConta){
    char acao;
    printf("O que deseja fazer? D-depositar / S-sacar / T- transferir: \n");
    scanf(" %c", &acao);
    
    switch(acao){
        case 'D':
        case 'd':
        // anotar esse escopo do switch 
        if(codConta == 1011956 || codConta == 1012018){
        printf("Ação inválida para este usuário.\n");
        break;
        }
        {
        double valorDeposito = 0.0;
        printf("Digite o valor a ser depositado: R$ ");
        scanf(" %lf", &valorDeposito);
        if(valorDeposito <= 0){
            printf("Valor de depósito inválido.\n");
            break;
        }
        depositaValor(codConta, valorDeposito);
        break;
    }
    
        case 's':
        case 'S':
            if(codConta == 1011956 || codConta == 1012018){
            printf("Ação inválida para este usuário.\n");
            break;
        }
        {
            double valorSaque = 0.0;
            printf("Digite o valor a ser sacado: R$ ");
            scanf(" %lf", &valorSaque);
            if(valorSaque <= 0){
                printf("Valor de saque inválido.\n");
                break;
            }
            saque(valorSaque, codConta);
            break;
        }
    
        case 't':
        case 'T':
        if(codConta == 1011956 || codConta == 1012018){
            printf("Ação inválida para este usuário.\n");
            break;
        }
        {
            int contaDestino;
            double valorTransf = 0.0;
            printf("Digite o número da conta destino: ");
            scanf(" %d", &contaDestino);
            if(contaDestino == codConta){
                printf("Não é possível transferir para a mesma conta.\n");
                break;
            }
            if (contaDestino == 1011956 || contaDestino == 1012018) {
            printf("Conta destino proibida para transferências.\n");
            break;
}
            if(!existeConta(contaDestino)){
                printf("Conta destino não existe.\n");
                break;
            }
            printf("Digite o valor a ser transferido: R$ ");
            scanf(" %lf", &valorTransf);
            if(valorTransf <= 0){
                printf("Valor de transferência inválido.\n");
                break;
            }
            int resultadoSaque = saque(valorTransf, codConta);
            if(resultadoSaque == 1){
            depositaValor(contaDestino, valorTransf);
            }
            break;
        }
    
        case 'L':
        case 'l':
        if(codConta == 1011956 || codConta == 1012018){
        listarContas();
    }else{
        printf("Ação inválida para este usuário.\n");  
    }
        break;
    
        default:
        printf("Ação inválida. Por favor, tente novamente.\n");
        break;
    }
}

int existeConta(int codConta){
 FILE* f;
 f = fopen("banco.dat", "rb");
if (f == NULL){
    printf("Erro no banco de dados!\n");
    exit(1);
}

CONTA contaLida;
while(fread(&contaLida, sizeof(CONTA), 1, f)){
    if(codConta == contaLida.numConta){
        fclose(f);
        return 1; // Conta existe
    }
}
fclose(f);
return 0; // Conta não existe
}

int main() {
    int temconta;
    int codConta;
    

    srand(time(0));
    cabecalho();
    INICIO:
    determinaacao(&temconta, &codConta);
    if(temconta == 1 || temconta == 3){
    escolheoperacao(codConta);
    }else{
    printf("Retornando ao menu inicial...\n");
    goto INICIO;
    }
    

 return 0;
}