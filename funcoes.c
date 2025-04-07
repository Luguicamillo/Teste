#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

char nome[50] = "Luis, Lucas e Leo";
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
    bool lendo_extrato = false;

    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        if (!lendo_extrato) {
            if (linha_atual == 0) sscanf(linha, "Saldo em Real: %f", &saldo_real);
            else if (linha_atual == 1) sscanf(linha, "Saldo em Bitcoin: %f", &saldo_bitcoin);
            else if (linha_atual == 2) sscanf(linha, "Saldo em Ethereum: %f", &saldo_ethe);
            else if (linha_atual == 3) sscanf(linha, "Saldo em Ripple: %f", &saldo_ripple);
            else if (linha_atual == 4) sscanf(linha, "Cotação Bitcoin: R$%f", &cotacao_bit);
            else if (linha_atual == 5) sscanf(linha, "Cotação Ethereum: R$%f", &cotacao_ethe);
            else if (linha_atual == 6) sscanf(linha, "Cotação Ripple: R$%f", &cotacao_ripple);
            else if (strstr(linha, "Extrato de Transações:") != NULL) lendo_extrato = true;
            linha_atual++;
        } else {
            if (num_transacoes < MAX_EXTRATO) {
                Transacao t;
                if (sscanf(linha, "%19[0-9-:] %1[+-] %f %3s CT: %f TX: %f REAL: %f BTC: %f ETH: %f XRP: %f",
                          t.data_hora, t.sinal, &t.valor, t.moeda, &t.cotacao, &t.taxa,
                          &t.saldo_real, &t.saldo_bitcoin, &t.saldo_ethe, &t.saldo_ripple) >= 7) {
                    extrato[num_transacoes++] = t;
                }
            }
        }
    }
    fclose(arquivo);
}

void gravar_dados() {
    FILE *arquivo = fopen("dados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de dados!\n");
        return;
    }

    fprintf(arquivo, "Saldo em Real: %.2f\n", saldo_real);
    fprintf(arquivo, "Saldo em Bitcoin: %.6f\n", saldo_bitcoin);
    fprintf(arquivo, "Saldo em Ethereum: %.4f\n", saldo_ethe);
    fprintf(arquivo, "Saldo em Ripple: %.2f\n", saldo_ripple);
    fprintf(arquivo, "Cotação Bitcoin: R$%.2f\n", cotacao_bit);
    fprintf(arquivo, "Cotação Ethereum: R$%.2f\n", cotacao_ethe);
    fprintf(arquivo, "Cotação Ripple: R$%.2f\n", cotacao_ripple);
    fprintf(arquivo, "\nExtrato de Transações:\n");
    
    for (int i = 0; i < num_transacoes; i++) {
        Transacao t = extrato[i];
        fprintf(arquivo, "%s %s %.2f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.6f ETH: %.4f XRP: %.2f\n",
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
        printf("Saldo em Real: R$%.2f\n", saldo_real);
        printf("Saldo em Bitcoin: %.6f BTC (R$%.2f)\n", saldo_bitcoin, saldo_bitcoin * cotacao_bit);
        printf("Saldo em Ethereum: %.4f ETH (R$%.2f)\n", saldo_ethe, saldo_ethe * cotacao_ethe);
        printf("Saldo em Ripple: %.2f XRP (R$%.2f)\n", saldo_ripple, saldo_ripple * cotacao_ripple);
        printf("Saldo Total: R$%.2f\n", saldo_real + (saldo_bitcoin * cotacao_bit) + 
              (saldo_ethe * cotacao_ethe) + (saldo_ripple * cotacao_ripple));
    } else {
        printf("Senha inválida!\n");
    }
}

void consultar_extrato() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario != senha) {
        printf("Senha inválida!\n");
        return;
    }

    printf("\n=== Extrato de Transações ===\n");
    printf("Cliente: %s\n", nome);
    printf("CPF: 123.456.789-10\n\n");
    
    if (num_transacoes == 0) {
        printf("Nenhuma transação registrada.\n");
        return;
    }

    for (int i = 0; i < num_transacoes; i++) {
        Transacao t = extrato[i];
        printf("[%s] %s %s %.2f (Cotação: R$%.2f, Taxa: %.0f%%)\n", 
               t.data_hora, t.sinal, t.moeda, t.valor, t.cotacao, t.taxa * 100);
        printf("   Saldos: R$%.2f | BTC: %.6f | ETH: %.4f | XRP: %.2f\n\n",
               t.saldo_real, t.saldo_bitcoin, t.saldo_ethe, t.saldo_ripple);
    }
}

void depositar() {
    char data_hora[20];
    float valor;
    
    printf("Digite a data e hora (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Valor do depósito: R$");
    scanf("%f", &valor);
    
    if (valor <= 0) {
        printf("Valor inválido!\n");
        return;
    }

    saldo_real += valor;
    
    if (num_transacoes < MAX_EXTRATO) {
        Transacao t;
        strcpy(t.data_hora, data_hora);
        strcpy(t.sinal, "+");
        strcpy(t.moeda, "REAL");
        t.valor = valor;
        t.cotacao = 1.0;
        t.taxa = 0.0;
        t.saldo_real = saldo_real;
        t.saldo_bitcoin = saldo_bitcoin;
        t.saldo_ethe = saldo_ethe;
        t.saldo_ripple = saldo_ripple;
        
        extrato[num_transacoes++] = t;
    }
    
    printf("Depósito realizado! Novo saldo: R$%.2f\n", saldo_real);
    gravar_dados();
}

void sacar() {
    char data_hora[20];
    float valor;
    
    printf("Digite a data e hora (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Valor do saque: R$");
    scanf("%f", &valor);
    
    if (valor <= 0) {
        printf("Valor inválido!\n");
        return;
    }

    if (valor > saldo_real) {
        printf("Saldo insuficiente!\n");
        return;
    }

    saldo_real -= valor;
    
    if (num_transacoes < MAX_EXTRATO) {
        Transacao t;
        strcpy(t.data_hora, data_hora);
        strcpy(t.sinal, "-");
        strcpy(t.moeda, "REAL");
        t.valor = valor;
        t.cotacao = 1.0;
        t.taxa = 0.0;
        t.saldo_real = saldo_real;
        t.saldo_bitcoin = saldo_bitcoin;
        t.saldo_ethe = saldo_ethe;
        t.saldo_ripple = saldo_ripple;
        
        extrato[num_transacoes++] = t;
    }
    
    printf("Saque realizado! Novo saldo: R$%.2f\n", saldo_real);
    gravar_dados();
}

void comprar_cripto() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario != senha) {
        printf("Senha inválida!\n");
        return;
    }

    char data_hora[20], moeda[20];
    float valor_reais;
    
    printf("\nCotações Atuais:\n");
    printf("1. Bitcoin (BTC): R$%.2f\n", cotacao_bit);
    printf("2. Ethereum (ETH): R$%.2f\n", cotacao_ethe);
    printf("3. Ripple (XRP): R$%.2f\n", cotacao_ripple);
    
    printf("\nDigite a data e hora (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Escolha a criptomoeda (BTC/ETH/XRP): ");
    scanf("%s", moeda);
    printf("Valor em reais para compra: R$");
    scanf("%f", &valor_reais);
    
    if (valor_reais <= 0) {
        printf("Valor inválido!\n");
        return;
    }

    if (valor_reais > saldo_real) {
        printf("Saldo insuficiente em reais!\n");
        return;
    }

    for (int i = 0; moeda[i]; i++) {
        moeda[i] = toupper(moeda[i]);
    }

    if (strcmp(moeda, "BTC") == 0) {
        float taxa = 0.02; 
        float quantidade = (valor_reais / cotacao_bit) * (1 - taxa);
        
        saldo_real -= valor_reais;
        saldo_bitcoin += quantidade;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "+");
            strcpy(t.moeda, "BTC");
            t.valor = quantidade;
            t.cotacao = cotacao_bit;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Compra realizada! Você adquiriu %.6f BTC\n", quantidade);
    }
    else if (strcmp(moeda, "ETH") == 0) {
        float taxa = 0.01; 
        float quantidade = (valor_reais / cotacao_ethe) * (1 - taxa);
        
        saldo_real -= valor_reais;
        saldo_ethe += quantidade;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "+");
            strcpy(t.moeda, "ETH");
            t.valor = quantidade;
            t.cotacao = cotacao_ethe;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Compra realizada! Você adquiriu %.4f ETH\n", quantidade);
    }
    else if (strcmp(moeda, "XRP") == 0) {
        float taxa = 0.01; 
        float quantidade = (valor_reais / cotacao_ripple) * (1 - taxa);
        
        saldo_real -= valor_reais;
        saldo_ripple += quantidade;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "+");
            strcpy(t.moeda, "XRP");
            t.valor = quantidade;
            t.cotacao = cotacao_ripple;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Compra realizada! Você adquiriu %.2f XRP\n", quantidade);
    }
    else {
        printf("Criptomoeda inválida!\n");
        return;
    }
    
    gravar_dados();
}

void vender_cripto() {
    int senha_usuario;
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (senha_usuario != senha) {
        printf("Senha inválida!\n");
        return;
    }

    char data_hora[20], moeda[20];
    float quantidade;
    
    printf("\nCotações Atuais:\n");
    printf("1. Bitcoin (BTC): R$%.2f\n", cotacao_bit);
    printf("2. Ethereum (ETH): R$%.2f\n", cotacao_ethe);
    printf("3. Ripple (XRP): R$%.2f\n", cotacao_ripple);
    
    printf("\nDigite a data e hora (dd-mm-aaaa hh:mm): ");
    scanf(" %[^\n]", data_hora);
    printf("Escolha a criptomoeda (BTC/ETH/XRP): ");
    scanf("%s", moeda);
    printf("Quantidade para vender: ");
    scanf("%f", &quantidade);
    
    if (quantidade <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    for (int i = 0; moeda[i]; i++) {
        moeda[i] = toupper(moeda[i]);
    }

    if (strcmp(moeda, "BTC") == 0) {
        if (quantidade > saldo_bitcoin) {
            printf("Saldo insuficiente de Bitcoin!\n");
            return;
        }
        
        float taxa = 0.03;
        float valor_recebido = quantidade * cotacao_bit * (1 - taxa);
        
        saldo_bitcoin -= quantidade;
        saldo_real += valor_recebido;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "-");
            strcpy(t.moeda, "BTC");
            t.valor = quantidade;
            t.cotacao = cotacao_bit;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Venda realizada! Você recebeu R$%.2f\n", valor_recebido);
    }
    else if (strcmp(moeda, "ETH") == 0) {
        if (quantidade > saldo_ethe) {
            printf("Saldo insuficiente de Ethereum!\n");
            return;
        }
        
        float taxa = 0.02;
        float valor_recebido = quantidade * cotacao_ethe * (1 - taxa);
        
        saldo_ethe -= quantidade;
        saldo_real += valor_recebido;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "-");
            strcpy(t.moeda, "ETH");
            t.valor = quantidade;
            t.cotacao = cotacao_ethe;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Venda realizada! Você recebeu R$%.2f\n", valor_recebido);
    }
    else if (strcmp(moeda, "XRP") == 0) {
        if (quantidade > saldo_ripple) {
            printf("Saldo insuficiente de Ripple!\n");
            return;
        }
        
        float taxa = 0.01;
        float valor_recebido = quantidade * cotacao_ripple * (1 - taxa);
        
        saldo_ripple -= quantidade;
        saldo_real += valor_recebido;
        
        if (num_transacoes < MAX_EXTRATO) {
            Transacao t;
            strcpy(t.data_hora, data_hora);
            strcpy(t.sinal, "-");
            strcpy(t.moeda, "XRP");
            t.valor = quantidade;
            t.cotacao = cotacao_ripple;
            t.taxa = taxa;
            t.saldo_real = saldo_real;
            t.saldo_bitcoin = saldo_bitcoin;
            t.saldo_ethe = saldo_ethe;
            t.saldo_ripple = saldo_ripple;
            
            extrato[num_transacoes++] = t;
        }
        
        printf("Venda realizada! Você recebeu R$%.2f\n", valor_recebido);
    }
    else {
        printf("Criptomoeda inválida!\n");
        return;
    }
    
    gravar_dados();
}

void atualizar_cot() {
    int variacao_max = 5;
    
    int variacao = rand() % (2 * variacao_max + 1) - variacao_max;
    cotacao_bit *= 1 + variacao / 100.0;
    
    variacao = rand() % (2 * variacao_max + 1) - variacao_max;
    cotacao_ethe *= 1 + variacao / 100.0;
    
    variacao = rand() % (2 * variacao_max + 1) - variacao_max;
    cotacao_ripple *= 1 + variacao / 100.0;
    
    printf("\nCotações atualizadas com sucesso!\n");
    printf("Novos valores:\n");
    printf("Bitcoin: R$%.2f\n", cotacao_bit);
    printf("Ethereum: R$%.2f\n", cotacao_ethe);
    printf("Ripple: R$%.2f\n\n", cotacao_ripple);
    
    gravar_dados();
}

void menu() {
    printf("\n=== Menu Principal ===\n");
    printf("1. Consultar Saldos\n");
    printf("2. Consultar Extrato\n");
    printf("3. Realizar Depósito\n");
    printf("4. Realizar Saque\n");
    printf("5. Comprar Criptomoedas\n");
    printf("6. Vender Criptomoedas\n");
    printf("7. Atualizar Cotações\n");
    printf("0. Sair\n");
    printf("======================\n");
}

bool voltar() {
    char opcao[5];
    printf("\nDeseja voltar ao menu principal? (s/n): ");
    scanf("%s", opcao);
    
    if (tolower(opcao[0]) == 'n') {
        printf("Encerrando sessão...\n");
        return true;
    }
    return false;
}