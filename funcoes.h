#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_PRODUTOS 100
#define ARQUIVO_DADOS "estoque.dat"
#define ARQUIVO_RELATORIO "relatorio_estoque.txt"

typedef struct {
    int codigo;
    char nome[50];
    char descricao[100];
    int quantidade;
    float preco_unitario;
} Produto;

extern Produto estoque[MAX_PRODUTOS];
extern int totalProdutos;

// Protótipos das funções
void carregarEstoque();
void salvarEstoque();
void menuPrincipal();
void cadastrarProduto();
void listarProdutos();
void buscarProduto();
void registrarEntrada();
void registrarSaida();
void gerarRelatorio();
void limparBuffer();

#endif