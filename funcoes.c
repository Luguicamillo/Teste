#include "funcoes.h"
#include <time.h>
#include <ctype.h>

char nome[50] = "Luis";
char cpf[15] = "12345678910";
int senha = 999999;
float saldo_real = 0;
float saldo_bitcoin = 0;
float saldo_ethe = 0;
float saldo_ripple = 0;
float cotacao_bit = 416328.72;
float cotacao_ethe = 15201.58;
float cotacao_ripple = 2.99;
Transacao extrato[MAX_EXTRATO];
int num_transacoes = 0;

void ler_arquivo() {
    FILE *arquivo = fopen("dados.txt", "r");
    if (arquivo == NULL) {
        gravar_dados();
        return;
    }

    char linha[MAX_LINHA];
    int linha_atual = 0;

    while (fgets(linha, MAX_LINHA, arquivo) != NULL && linha_atual < 7) {
        if (linha_atual == 0) sscanf(linha, "Saldo em Real: %f", &saldo_real);
        else if (linha_atual == 1) sscanf(linha, "Saldo em Bitcoin: %f", &saldo_bitcoin);
        else if (linha_atual == 2) sscanf(linha, "Saldo em Ethereum: %f", &saldo_ethe);
        else if (linha_atual == 3) sscanf(linha, "Saldo em Ripple: %f", &saldo_ripple);
        else if (linha_atual == 4) sscanf(linha, "Cotação Bitcoin: R$%f", &cotacao_bit);
        else if (linha_atual == 5) sscanf(linha, "Cotação Ethereum: R$%f", &cotacao_ethe);
        else if (linha_atual == 6) sscanf(linha, "Cotação Ripple: R$%f", &cotacao_ripple);
        linha_atual++;
    }

    // Pular linha do extrato
    fgets(linha, MAX_LINHA, arquivo);
    
    num_transacoes = 0;
    while (fgets(linha, MAX_LINHA, arquivo) != NULL && num_transacoes < MAX_EXTRATO) {
        Transacao t;
        if (sscanf(linha, "%19[0-9-:] %1[+-] %f %3s CT: %f TX: %f REAL: %f BTC: %f ETH: %f XRP: %f",
                  t.data_hora, t.sinal, &t.valor, t.moeda, &t.cotacao, &t.taxa,
                  &t.saldo_real, &t.saldo_bitcoin, &t.saldo_ethe, &t.saldo_ripple) >= 7) {
            extrato[num_transacoes++] = t;
        }
    }

    fclose(arquivo);
}

void gravar_dados() {
    FILE *arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "Saldo em Real: %.2f\n", saldo_real);
    fprintf(arquivo, "Saldo em Bitcoin: %.4f\n", saldo_bitcoin);
    fprintf(arquivo, "Saldo em Ethereum: %.2f\n", saldo_ethe);
    fprintf(arquivo, "Saldo em Ripple: %.2f\n", saldo_ripple);
    fprintf(arquivo, "Cotação Bitcoin: R$%.2f\n", cotacao_bit);
    fprintf(arquivo, "Cotação Ethereum: R$%.2f\n", cotacao_ethe);
    fprintf(arquivo, "Cotação Ripple: R$%.2f\n", cotacao_ripple);
    fprintf(arquivo, "\nExtrato de Transações:\n");
    
    for (int i = 0; i < num_transacoes; i++) {
        Transacao t = extrato[i];
        fprintf(arquivo, "%s %s %.2f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.4f ETH: %.2f XRP: %.2f\n",
                t.data_hora, t.sinal, t.valor, t.moeda, t.cotacao, t.taxa,
                t.saldo_real, t.saldo_bitcoin, t.saldo_ethe, t.saldo_ripple);
    }

    fclose(arquivo);
}

void consultar_saldo() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario == senha) {
        printf("\nNome: %s\n", nome);
        printf("CPF: 123.456.789-10\n\n");
        printf("Saldo em Real: %.2f\n", saldo_real);
        printf("Saldo em Bitcoin: %.4f\n", saldo_bitcoin);
        printf("Saldo em Ethereum: %.2f\n", saldo_ethe);
        printf("Saldo em Ripple: %.2f\n", saldo_ripple);
    } else {
        printf("Senha ou CPF invalidos\n");
    }
}

void consultar_extrato() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario == senha) {
        printf("\nNome: %s\n", nome);
        printf("123.456.789-10\n\n");
        printf("Extrato de Transações:\n");
        for (int i = 0; i < num_transacoes; i++) {
            Transacao t = extrato[i];
            printf("%s %s %.2f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.4f ETH: %.2f XRP: %.2f\n",
                   t.data_hora, t.sinal, t.valor, t.moeda, t.cotacao, t.taxa,
                   t.saldo_real, t.saldo_bitcoin, t.saldo_ethe, t.saldo_ripple);
        }
    } else {
        printf("Senha ou CPF invalidos\n");
    }
}

void depositar() {
    char data_hora[50];
    float deposito;
    
    printf("Digite a data e hora da transação (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Qual o valor do deposito em real? R$");
    scanf("%f", &deposito);
    
    saldo_real += deposito;
    printf("Seu novo saldo é: %.2f \n", saldo_real);
    
    if (num_transacoes < MAX_EXTRATO) {
        Transacao t;
        strcpy(t.data_hora, data_hora);
        strcpy(t.sinal, "+");
        strcpy(t.moeda, "REAL");
        t.valor = deposito;
        t.cotacao = 0.0;
        t.taxa = 0.0;
        t.saldo_real = saldo_real;
        t.saldo_bitcoin = saldo_bitcoin;
        t.saldo_ethe = saldo_ethe;
        t.saldo_ripple = saldo_ripple;
        
        extrato[num_transacoes++] = t;
    }
    
    gravar_dados();
}

void sacar() {
    char data_hora[50];
    float saque;
    
    printf("Digite a data e hora da transação (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Qual o valor do saque em real? R$");
    scanf("%f", &saque);
    
    if (saldo_real - saque >= 0) {
        saldo_real -= saque;
        printf("Seu novo saldo é: R$%.2f \n", saldo_real);
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "-");
            strcpy(t.moeda, "REAL");
            t.valor = saque;
            t.cotacao = 0.0;
            t.taxa = 0.0;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        gravar_dados();
    } else {
        printf("\nOperação invalida, seu saldo não é suficiente\n");
        printf("Seu saldo continua sendo: R$%.2f \n", saldo_real);
    }
}

void comprar_cripto() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario == senha) {
        printf("Cotação Bitcoin: R$%.2f\n", cotacao_bit);
        printf("Cotação Ethereum: R$%.2f\n", cotacao_ethe);
        printf("Cotação Ripple: R$%.2f\n", cotacao_ripple);
        
        char data_hora[50], escolha[20];
        float compra;
        
        printf("Digite a data e hora da transação (dd-mm-aaaa hh:mm): ");
        scanf(" %[^\n]", data_hora);
        printf("Qual criptomoeda deseja comprar (Bitcoin/Ethereum/Ripple): ");
        scanf("%s", escolha);
        
        for (int i = 0; escolha[i]; i++) {
            escolha[i] = tolower(escolha[i]);
        }
        
        printf("Quanto deseja comprar? R$");
        scanf("%f", &compra);
        
        if (compra > saldo_real) {
            printf("Seu saldo é insuficiente\n");
            return;
        }
        
        if (strcmp(escolha, "bitcoin") == 0) {
            saldo_real -= compra;
            float compra_bit = compra / cotacao_bit * 0.98;
            saldo_bitcoin += compra_bit;
            printf("Seu novo saldo é: %.4f BTC\n", saldo_bitcoin);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "+");
                strcpy(t.moeda, "BTC");
                t.valor = compra_bit;
                t.cotacao = cotacao_bit;
                t.taxa = 0.02;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else if (strcmp(escolha, "ethereum") == 0) {
            saldo_real -= compra;
            float compra_ethe = compra / cotacao_ethe * 0.99;
            saldo_ethe += compra_ethe;
            printf("Seu novo saldo é: %.2f ETH\n", saldo_ethe);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "+");
                strcpy(t.moeda, "ETH");
                t.valor = compra_ethe;
                t.cotacao = cotacao_ethe;
                t.taxa = 0.01;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else if (strcmp(escolha, "ripple") == 0) {
            saldo_real -= compra;
            float compra_ripple = compra / cotacao_ripple * 0.99;
            saldo_ripple += compra_ripple;
            printf("Seu novo saldo é: %.2f XRP\n", saldo_ripple);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "+");
                strcpy(t.moeda, "XRP");
                t.valor = compra_ripple;
                t.cotacao = cotacao_ripple;
                t.taxa = 0.01;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else {
            printf("Opção inválida\n");
        }
        
        gravar_dados();
    } else {
        printf("Senha ou CPF inválidos\n");
    }
}

void vender_cripto() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario == senha) {
        printf("Cotação Bitcoin: R$%.2f\n", cotacao_bit);
        printf("Cotação Ethereum: R$%.2f\n", cotacao_ethe);
        printf("Cotação Ripple: R$%.2f\n", cotacao_ripple);
        
        char data_hora[50], escolha[20];
        float venda;
        
        printf("Digite a data e hora da transação (dd-mm-aaaa hh:mm): ");
        scanf(" %[^\n]", data_hora);
        printf("Qual criptomoeda deseja vender (Bitcoin/Ethereum/Ripple): ");
        scanf("%s", escolha);
        
        for (int i = 0; escolha[i]; i++) {
            escolha[i] = tolower(escolha[i]);
        }
        
        printf("Quanto deseja vender? ");
        scanf("%f", &venda);
        
        if (strcmp(escolha, "bitcoin") == 0) {
            if (venda > saldo_bitcoin) {
                printf("Seu saldo é insuficiente\n");
                return;
            }
            
            saldo_bitcoin -= venda;
            saldo_real += venda * cotacao_bit * 0.97;
            printf("Seu novo saldo é: %.4f BTC\n", saldo_bitcoin);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "-");
                strcpy(t.moeda, "BTC");
                t.valor = venda;
                t.cotacao = cotacao_bit;
                t.taxa = 0.03;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else if (strcmp(escolha, "ethereum") == 0) {
            if (venda > saldo_ethe) {
                printf("Seu saldo é insuficiente\n");
                return;
            }
            
            saldo_ethe -= venda;
            saldo_real += venda * cotacao_ethe * 0.98;
            printf("Seu novo saldo é: %.2f ETH\n", saldo_ethe);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "-");
                strcpy(t.moeda, "ETH");
                t.valor = venda;
                t.cotacao = cotacao_ethe;
                t.taxa = 0.02;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else if (strcmp(escolha, "ripple") == 0) {
            if (venda > saldo_ripple) {
                printf("Seu saldo é insuficiente\n");
                return;
            }
            
            saldo_ripple -= venda;
            saldo_real += venda * cotacao_ripple * 0.99;
            printf("Seu novo saldo é: %.2f XRP\n", saldo_ripple);
            
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                strcpy(t.data_hora, data_hora);
                strcpy(t.sinal, "-");
                strcpy(t.moeda, "XRP");
                t.valor = venda;
                t.cotacao = cotacao_ripple;
                t.taxa = 0.01;
                t.saldo_real = saldo_real;
                t.saldo_bitcoin = saldo_bitcoin;
                t.saldo_ethe = saldo_ethe;
                t.saldo_ripple = saldo_ripple;
                
                extrato[num_transacoes++] = t;
            }
        } else {
            printf("Opção inválida\n");
        }
        
        gravar_dados();
    } else {
        printf("Senha ou CPF inválidos\n");
    }
}

void atualizar_cot() {
    int vari_max = 5;
    
    int vari_bit = rand() % (2 * vari_max + 1) - vari_max;
    cotacao_bit = cotacao_bit * (1 + vari_bit / 100.0);
    
    int vari_ethe = rand() % (2 * vari_max + 1) - vari_max;
    cotacao_ethe = cotacao_ethe * (1 + vari_ethe / 100.0);
    
    int vari_ripple = rand() % (2 * vari_max + 1) - vari_max;
    cotacao_ripple = cotacao_ripple * (1 + vari_ripple / 100.0);
    
    printf("\nCotações atualizadas:\n");
    printf("Cotação Bitcoin: R$%.2f\n", cotacao_bit);
    printf("Cotação Ethereum: R$%.2f\n", cotacao_ethe);
    printf("Cotação Ripple: R$%.2f\n\n", cotacao_ripple);
    
    gravar_dados();
}

void menu() {
    printf("1. Consulta do Saldo\n");
    printf("2. Consulta de Extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar Criptomoedas\n");
    printf("6. Vender Criptomoedas\n");
    printf("7. Atualizar Cotação\n");
    printf("0. Sair\n");
}

bool voltar() {
    char opcao[10];
    printf("Deseja voltar para o menu (sim/não): ");
    scanf("%s", opcao);
    
    if (strcmp(opcao, "não") == 0 || strcmp(opcao, "nao") == 0) {
        printf("Saindo, obrigado!\n");
        return true;
    } else if (strcmp(opcao, "sim") != 0) {
        printf("Opção inválida\n");
    }
    return false;
}