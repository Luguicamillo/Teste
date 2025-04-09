#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_EXTRATO 100
#define MAX_LINHA 256

typedef struct {
    char data_hora[50];
    char sinal[2];
    char moeda[10];
    float valor;
    float cotacao;
    float taxa;
    float saldo_real;
    float saldo_bitcoin;
    float saldo_ethe;
    float saldo_ripple;
} Transacao;

extern char nome[50];
extern char cpf[15];
extern int senha;
extern float saldo_real;
extern float saldo_bitcoin;
extern float saldo_ethe;
extern float saldo_ripple;
extern float cotacao_bit;
extern float cotacao_ethe;
extern float cotacao_ripple;
extern Transacao extrato[MAX_EXTRATO];
extern int num_transacoes;

void obter_data_hora_atual(char *data_hora);
void ler_arquivo();
void gravar_dados();
void consultar_saldo();
void consultar_extrato();
void depositar();
void sacar();
void comprar_cripto();
void vender_cripto();
void atualizar_cot();
void menu();
bool voltar();

#endif