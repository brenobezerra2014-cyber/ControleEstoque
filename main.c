```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 10

typedef struct {
    int id;
    char descricao[50];
    float preco;
    int estoque;
} Produto;

typedef struct {
    int idVenda;
    int qtdProdutos;
    int idsProdutos[MAX_PRODUTOS];
} Venda;

/* ================= PRODUTOS ================= */

void cadastrarProduto() {
    FILE *arq = fopen("produtos.dat", "ab");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    Produto p;

    printf("ID: ");
    scanf("%d", &p.id);

    printf("Descricao: ");
    scanf(" %[^\n]", p.descricao);

    printf("Preco: ");
    scanf("%f", &p.preco);

    printf("Estoque: ");
    scanf("%d", &p.estoque);

    fwrite(&p, sizeof(Produto), 1, arq);

    fclose(arq);

    printf("Produto cadastrado!\n");
}

void listarProdutos() {
    FILE *arq = fopen("produtos.dat", "rb");

    if (arq == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto p;

    while (fread(&p, sizeof(Produto), 1, arq)) {
        printf("\nID: %d", p.id);
        printf("\nDescricao: %s", p.descricao);
        printf("\nPreco: %.2f", p.preco);
        printf("\nEstoque: %d\n", p.estoque);
    }

    fclose(arq);
}

void buscarProduto() {
    FILE *arq = fopen("produtos.dat", "rb");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    int achou = 0;
    Produto p;

    printf("ID do produto: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Produto), 1, arq)) {
        if (p.id == id) {
            printf("\nDescricao: %s", p.descricao);
            printf("\nPreco: %.2f", p.preco);
            printf("\nEstoque: %d\n", p.estoque);
            achou = 1;
            break;
        }
    }

    if (!achou)
        printf("Produto nao encontrado.\n");

    fclose(arq);
}

void atualizarProduto() {
    FILE *arq = fopen("produtos.dat", "rb+");

    if (arq == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    int achou = 0;
    Produto p;

    printf("ID do produto: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Produto), 1, arq)) {
        if (p.id == id) {

            printf("Nova descricao: ");
            scanf(" %[^\n]", p.descricao);

            printf("Novo preco: ");
            scanf("%f", &p.preco);

            printf("Novo estoque: ");
            scanf("%d", &p.estoque);

            fseek(arq, -sizeof(Produto), SEEK_CUR);
            fwrite(&p, sizeof(Produto), 1, arq);

            achou = 1;
            break;
        }
    }

    if (achou)
        printf("Produto atualizado.\n");
    else
        printf("Produto nao encontrado.\n");

    fclose(arq);
}

void excluirProduto() {
    FILE *orig = fopen("produtos.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (orig == NULL) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int id;
    Produto p;

    printf("ID do produto a excluir: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(Produto), 1, orig)) {
        if (p.id != id)
            fwrite(&p, sizeof(Produto), 1, temp);
    }

    fclose(orig);
    fclose(temp);

    remove("produtos.dat");
    rename("temp.dat", "produtos.dat");

    printf("Produto removido.\n");
}

/* ================= VENDAS ================= */

void cadastrarVenda() {
    FILE *arq = fopen("vendas.dat", "ab");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    Venda v;

    printf("ID da venda: ");
    scanf("%d", &v.idVenda);

    printf("Quantidade de produtos: ");
    scanf("%d", &v.qtdProdutos);

    if (v.qtdProdutos > MAX_PRODUTOS)
        v.qtdProdutos = MAX_PRODUTOS;

    for (int i = 0; i < v.qtdProdutos; i++) {
        printf("ID do produto %d: ", i + 1);
        scanf("%d", &v.idsProdutos[i]);
    }

    fwrite(&v, sizeof(Venda), 1, arq);

    fclose(arq);

    printf("Venda cadastrada.\n");
}

void listarVendas() {
    FILE *arq = fopen("vendas.dat", "rb");

    if (arq == NULL) {
        printf("Nenhuma venda cadastrada.\n");
        return;
    }

    Venda v;

    while (fread(&v, sizeof(Venda), 1, arq)) {

        printf("\nVenda %d\n", v.idVenda);

        for (int i = 0; i < v.qtdProdutos; i++)
            printf("Produto ID: %d\n", v.idsProdutos[i]);
    }

    fclose(arq);
}

void buscarVenda() {
    FILE *arq = fopen("vendas.dat", "rb");

    if (arq == NULL)
        return;

    int id;
    int achou = 0;
    Venda v;

    printf("ID da venda: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Venda), 1, arq)) {

        if (v.idVenda == id) {

            printf("\nVenda %d\n", v.idVenda);

            for (int i = 0; i < v.qtdProdutos; i++)
                printf("Produto ID: %d\n", v.idsProdutos[i]);

            achou = 1;
            break;
        }
    }

    if (!achou)
        printf("Venda nao encontrada.\n");

    fclose(arq);
}

void atualizarVenda() {
    FILE *arq = fopen("vendas.dat", "rb+");

    if (arq == NULL)
        return;

    int id;
    int achou = 0;
    Venda v;

    printf("ID da venda: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Venda), 1, arq)) {

        if (v.idVenda == id) {

            printf("Nova quantidade de produtos: ");
            scanf("%d", &v.qtdProdutos);

            if (v.qtdProdutos > MAX_PRODUTOS)
                v.qtdProdutos = MAX_PRODUTOS;

            for (int i = 0; i < v.qtdProdutos; i++) {
                printf("ID do produto %d: ", i + 1);
                scanf("%d", &v.idsProdutos[i]);
            }

            fseek(arq, -sizeof(Venda), SEEK_CUR);
            fwrite(&v, sizeof(Venda), 1, arq);

            achou = 1;
            break;
        }
    }

    if (achou)
        printf("Venda atualizada.\n");
    else
        printf("Venda nao encontrada.\n");

    fclose(arq);
}

void excluirVenda() {
    FILE *orig = fopen("vendas.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (orig == NULL)
        return;

    int id;
    Venda v;

    printf("ID da venda a excluir: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(Venda), 1, orig)) {

        if (v.idVenda != id)
            fwrite(&v, sizeof(Venda), 1, temp);
    }

    fclose(orig);
    fclose(temp);

    remove("vendas.dat");
    rename("temp.dat", "vendas.dat");

    printf("Venda removida.\n");
}

/* ================= RELATORIO ================= */

void relatorioCompleto() {

    FILE *vendas = fopen("vendas.dat", "rb");

    if (vendas == NULL) {
        printf("Nenhuma venda cadastrada.\n");
        return;
    }

    Venda v;
    Produto p;

    printf("\n===== RELATORIO =====\n");

    while (fread(&v, sizeof(Venda), 1, vendas)) {

        printf("\nVenda %d\n", v.idVenda);

        for (int i = 0; i < v.qtdProdutos; i++) {

            FILE *produtos = fopen("produtos.dat", "rb");

            while (fread(&p, sizeof(Produto), 1, produtos)) {

                if (p.id == v.idsProdutos[i]) {
                    printf("- %s\n", p.descricao);
                    break;
                }
            }

            fclose(produtos);
        }
    }

    fclose(vendas);
}

/* ================= MAIN ================= */

int main() {

    int op;

    do {

        printf("\n===== MENU =====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Buscar Produto\n");
        printf("4 - Atualizar Produto\n");
        printf("5 - Excluir Produto\n");

        printf("6 - Cadastrar Venda\n");
        printf("7 - Listar Vendas\n");
        printf("8 - Buscar Venda\n");
        printf("9 - Atualizar Venda\n");
        printf("10 - Excluir Venda\n");

        printf("11 - Relatorio Completo\n");
        printf("0 - Sair\n");

        printf("Opcao: ");
        scanf("%d", &op);

        switch (op) {

            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: buscarProduto(); break;
            case 4: atualizarProduto(); break;
            case 5: excluirProduto(); break;

            case 6: cadastrarVenda(); break;
            case 7: listarVendas(); break;
            case 8: buscarVenda(); break;
            case 9: atualizarVenda(); break;
            case 10: excluirVenda(); break;

            case 11: relatorioCompleto(); break;
        }

    } while (op != 0);

    return 0;
}
```
