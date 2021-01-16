// REALIZADO POR: Dário Félix - 2018275530 | TRABALHO FINAL - PPP 2019/2020
// FICHEIRO / PARTE: trabalho.h (Header File)

#ifndef TRABALHO_FINAL_TRABALHO_H
#define TRABALHO_FINAL_TRABALHO_H
#endif


// BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



// CONSTANTES
#define MAXLISTADESPESAS 500            /* MUTAVEL - Numero Maximo de slots na lista despesas */
#define MAXLISTAORCAMENTOS 50           /* MUTAVEL - Numero Maximo de slots na lista orçamentos */
#define MAXSTRTIPO 100                  /* MUTAVEL - Capacidade (n. chars) da string 'Tipo' */
#define MAXSTRDESCRICAO 150             /* MUTAVEL - Capacidade (n. chars) da string 'Descrição' */
#define MAXSTRTEMP 200                  /* MUTAVEL - Capacidade (n. chars) da string-variável-temporária */
#define NARGC 3
#define MAXSTRFICH 500                  /* MUTAVEL - Capacidade (n. chars) da string 'Nome do Ficheiro' */
#define NULO -1                         /* Significa fim da fila, ou seja, slot aponta para nenhures */
#define VAZIO -2                        /* Significa slot vazio/desocupado */
#define SUCESSO 0
#define ERRO 1

/* NOTA: MAXSTRTEMP tem que ser superior a MAXSTRDESCRICAO e a MAXSTRTIPO (senão erro de run-time) */
#if (MAXSTRDESCRICAO > MAXSTRTEMP) || (MAXSTRTIPO > MAXSTRTEMP)
    #undef MAXSTRTIPO
    #undef MAXSTRDESCRICAO
    #undef MAXSTRTEMP
    #define MAXSTRTIPO 100
    #define MAXSTRDESCRICAO 150
    #define MAXSTRTEMP 200
#endif



// ESTRUTURAS e DEFINICOES
typedef struct {                        /* Estrutura para dados da despesa */
    char tipo[MAXSTRTIPO];
    double valor;
    char descricao[MAXSTRDESCRICAO];
} EntidadeDespesa;

typedef struct {                        /* Estrutura para slot da lista-despesas */
    EntidadeDespesa dados;
    int seguinte;
} ElementoListaDespesas;


typedef struct {                        /* Estrutura para dados do orcamento */
    char tipo[MAXSTRTIPO];
    double orcamento;
} EntidadeOrcamento;

typedef struct {                        /* Estrutura para slot da lista-orcamentos */
    EntidadeOrcamento dados;
    int seguinte;
} ElementoListaOrcamentos;



// PROTOTIPOS estruturaDeDados.c
void inicializarListas(void);
int colocarElementoListaDespesas(EntidadeDespesa dados);
int colocarElementoListaOrcamentos(EntidadeOrcamento dados);
int getProximoElementoListaDespesasPorOrcamento(EntidadeDespesa *dados, const char orcamento[]);
int getProximoElementoListaOrcamentos(EntidadeOrcamento *dados);
int getProximoElementoListaDespesas(EntidadeDespesa *dados);
int isEntradaListaDespesasNulo(void);
int isEntradaListaOrcamentosNulo(void);
double getTotalOrcamentoMensal(void);

// PROTOTIPOS main.c
void getStringStdin(char string[], int n);
void introducao(void);
int abrirFicheiros(int argc_copia, const char * argv_copia [], FILE ** fich_saida_despesas, FILE ** fich_saida_orcamento, FILE ** fich_entrada_orcamento, FILE ** fich_entrada_despesas);
int entradaDados(FILE ** fich_entrada_orcamento, FILE ** fich_entrada_despesas);
int saidaDados(FILE ** fich_saida_despesas, FILE ** fich_saida_orcamento);
