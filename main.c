#include "funcoes.h"
#include <string.h>
#include <ctype.h>

int main() {
    srand(time(NULL));
    ler_arquivo();
    
    char cpf_usuario[15];
    int senha_usuario;
    
    printf("Por favor, informe o seu CPF: ");
    scanf("%s", cpf_usuario);
    printf("Por favor, informe a sua senha: ");
    scanf("%d", &senha_usuario);
    
    if (strcmp(cpf_usuario, cpf) == 0 && senha_usuario == senha) {
        int sair = 0;
        while (!sair) {
            menu();
            printf("Escolha uma das opções acima: ");
            
            char opcao[10];
            scanf("%s", opcao);
            
            if (strcmp(opcao, "1") == 0) {
                consultar_saldo();
                sair = voltar();
            } else if (strcmp(opcao, "2") == 0) {
                consultar_extrato();
                sair = voltar();
            } else if (strcmp(opcao, "3") == 0) {
                depositar();
                sair = voltar();
            } else if (strcmp(opcao, "4") == 0) {
                sacar();
                sair = voltar();
            } else if (strcmp(opcao, "5") == 0) {
                comprar_cripto();
                sair = voltar();
            } else if (strcmp(opcao, "6") == 0) {
                vender_cripto();
                sair = voltar();
            } else if (strcmp(opcao, "7") == 0) {
                atualizar_cot();
                sair = voltar();
            } else if (strcmp(opcao, "0") == 0) {
                printf("Saindo, obrigado!\n");
                sair = 1;
            } else {
                printf("Opção inválida\n");
            }
        }
    } else {
        printf("Senha ou CPF inválidos\n");
    }
    
    return 0;
}