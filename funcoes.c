#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

Produto estoque[MAX_PRODUTOS];
int totalProdutos = 0;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void carregarEstoque() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (arquivo == NULL) {
        printf("Arquivo de dados não encontrado. Criando novo estoque.\n");
        return;
    }
    
    totalProdutos = fread(estoque, sizeof(Produto), MAX_PRODUTOS, arquivo);
    fclose(arquivo);
    printf("Estoque carregado com sucesso. %d produtos registrados.\n", totalProdutos);
}

void salvarEstoque() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar dados.\n");
        return;
    }
    
    fwrite(estoque, sizeof(Produto), totalProdutos, arquivo);
    fclose(arquivo);
    printf("Estoque salvo com sucesso.\n");
}

void cadastrarProduto() {
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        return;
    }
    
    Produto novo;
    printf("\n=== CADASTRO DE NOVO PRODUTO ===\n");
    
    printf("Codigo: ");
    scanf("%d", &novo.codigo);
    limparBuffer();
    
    // Verificar se código já existe
    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == novo.codigo) {
            printf("Erro: Codigo ja existe no estoque!\n");
            return;
        }
    }
    
    printf("Nome: ");
    fgets(novo.nome, 50, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    
    printf("Descricao: ");
    fgets(novo.descricao, 100, stdin);
    novo.descricao[strcspn(novo.descricao, "\n")] = '\0';
    
    printf("Quantidade inicial: ");
    scanf("%d", &novo.quantidade);
    
    printf("Preco unitario: ");
    scanf("%f", &novo.preco_unitario);
    limparBuffer();
    
    estoque[totalProdutos] = novo;
    totalProdutos++;
    
    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {
    printf("\n=== LISTA DE PRODUTOS ===\n");
    printf("%-6s %-20s %-30s %-10s %-10s %-12s\n", 
           "Cod", "Nome", "Descricao", "Qtd", "Preco Uni", "Total");
    
    for (int i = 0; i < totalProdutos; i++) {
        printf("%-6d %-20s %-30s %-10d %-10.2f %-12.2f\n", 
               estoque[i].codigo, 
               estoque[i].nome, 
               estoque[i].descricao, 
               estoque[i].quantidade, 
               estoque[i].preco_unitario,
               estoque[i].quantidade * estoque[i].preco_unitario);
    }
}

void buscarProduto() {
    int opcao;
    printf("\n=== BUSCAR PRODUTO ===\n");
    printf("1. Por codigo\n");
    printf("2. Por nome\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    limparBuffer();
    
    if (opcao == 1) {
        int codigo;
        printf("Digite o codigo: ");
        scanf("%d", &codigo);
        limparBuffer();
        
        for (int i = 0; i < totalProdutos; i++) {
            if (estoque[i].codigo == codigo) {
                printf("\nProduto encontrado:\n");
                printf("Codigo: %d\n", estoque[i].codigo);
                printf("Nome: %s\n", estoque[i].nome);
                printf("Descricao: %s\n", estoque[i].descricao);
                printf("Quantidade: %d\n", estoque[i].quantidade);
                printf("Preco unitario: %.2f\n", estoque[i].preco_unitario);
                printf("Valor total: %.2f\n", estoque[i].quantidade * estoque[i].preco_unitario);
                return;
            }
        }
        printf("Produto nao encontrado!\n");
    } else if (opcao == 2) {
        char termo[50];
        printf("Digite o nome (ou parte dele): ");
        fgets(termo, 50, stdin);
        termo[strcspn(termo, "\n")] = '\0';
        
        printf("\nResultados da busca por '%s':\n", termo);
        int encontrados = 0;
        
        for (int i = 0; i < totalProdutos; i++) {
            if (strstr(estoque[i].nome, termo) != NULL) {
                printf("%d - %s (Qtd: %d, Preco: %.2f)\n", 
                       estoque[i].codigo, 
                       estoque[i].nome, 
                       estoque[i].quantidade, 
                       estoque[i].preco_unitario);
                encontrados++;
            }
        }
        
        if (encontrados == 0) {
            printf("Nenhum produto encontrado.\n");
        }
    } else {
        printf("Opcao invalida!\n");
    }
}

void registrarEntrada() {
    int codigo, quantidade;
    printf("\n=== REGISTRAR ENTRADA DE PRODUTO ===\n");
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    printf("Quantidade a adicionar: ");
    scanf("%d", &quantidade);
    limparBuffer();
    
    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }
    
    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            estoque[i].quantidade += quantidade;
            printf("Entrada registrada. Nova quantidade: %d\n", estoque[i].quantidade);
            return;
        }
    }
    
    printf("Produto nao encontrado!\n");
}

void registrarSaida() {
    int codigo, quantidade;
    printf("\n=== REGISTRAR SAIDA DE PRODUTO ===\n");
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    printf("Quantidade a remover: ");
    scanf("%d", &quantidade);
    limparBuffer();
    
    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }
    
    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            if (estoque[i].quantidade < quantidade) {
                printf("Erro: Quantidade em estoque insuficiente!\n");
                printf("Estoque atual: %d\n", estoque[i].quantidade);
                return;
            }
            
            estoque[i].quantidade -= quantidade;
            printf("Saida registrada. Nova quantidade: %d\n", estoque[i].quantidade);
            return;
        }
    }
    
    printf("Produto nao encontrado!\n");
}

void gerarRelatorio() {
    FILE *relatorio = fopen(ARQUIVO_RELATORIO, "w");
    if (relatorio == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }
    
    fprintf(relatorio, "=== RELATORIO DE ESTOQUE ===\n\n");
    fprintf(relatorio, "%-6s %-20s %-30s %-10s %-10s %-12s\n", 
            "Cod", "Nome", "Descricao", "Qtd", "Preco Uni", "Total");
    
    float valorTotalEstoque = 0;
    
    for (int i = 0; i < totalProdutos; i++) {
        float valorTotal = estoque[i].quantidade * estoque[i].preco_unitario;
        valorTotalEstoque += valorTotal;
        
        fprintf(relatorio, "%-6d %-20s %-30s %-10d %-10.2f %-12.2f\n", 
                estoque[i].codigo, 
                estoque[i].nome, 
                estoque[i].descricao, 
                estoque[i].quantidade, 
                estoque[i].preco_unitario,
                valorTotal);
    }
    
    fprintf(relatorio, "\nVALOR TOTAL DO ESTOQUE: %.2f\n", valorTotalEstoque);
    fclose(relatorio);
    
    printf("Relatorio gerado com sucesso no arquivo '%s'.\n", ARQUIVO_RELATORIO);
}

void menuPrincipal() {
    int opcao;
    do {
        printf("\n=== SISTEMA DE CONTROLE DE ESTOQUE ===\n");
        printf("1. Cadastrar novo produto\n");
        printf("2. Listar todos os produtos\n");
        printf("3. Buscar produto\n");
        printf("4. Registrar entrada de produtos\n");
        printf("5. Registrar saida de produtos\n");
        printf("6. Gerar relatorio de estoque\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: buscarProduto(); break;
            case 4: registrarEntrada(); break;
            case 5: registrarSaida(); break;
            case 6: gerarRelatorio(); break;
            case 0: 
                salvarEstoque();
                printf("Saindo do sistema...\n");
                break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 0);
}