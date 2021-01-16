// REALIZADO POR: Dário Félix - 2018275530 | TRABALHO FINAL - PPP 2019/2020
// FICHEIRO / PARTE: API - Estrutura de Dados - Listas Ligadas Simples Baseadas em Matriz (Ficheiro C)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * LISTAGEM DAS ESCOLHAS FEITAS - Comportamento do Programa nas Situações não Previstas no Enunciado:
 * -> Listas e atributos-de-string com tamanho estático (alterável pelas constantes);
 * -> É utilizado a constante VAZIO no membro '.seguinte' para saber se a célula da lista está ocupado.
 * -> NULO (no membro '.seguinte') significa que está a apontar para nenhures (fim da lista);
 * -> Para saber o orcamento mensal (soma de todos os orcamentos em cada tipo) para o calculo do desvio
 * global, no momento em que é colocado as entidades-orcamentos na lista é incrementado a variável
 * total_orcamento (incializado a zero, no inicio) - existindo uma função para retornar o seu valor;
 * -> Na colocação de orcamentos nas listas é verificado se há repetição de orcamentos para o mesmo tipo;
 * -> Não há verificações de repetições de despesas na lista-de-despesas no momento de colocação de
 * novas despesas na lsita;
 * -> Existem funcoes para verificar se as listas estão vazias (nulas);
 * -> É possivel através de 2 funções diferentes [getProximoElementoListaDespesasPorOrcamento() e
 * getProximoElementoListaDespesas()] retirar uma despesa conforme um tipo/orcamento ou retirar
 * simplesmente a próxima (perto da raiz). Aquanado lista nula/vazia é enviado 'ERRO'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



// BIBLIOTECA
#include "trabalho.h"



// VARS GLOBAIS STATIC
static ElementoListaDespesas lista_despesas[MAXLISTADESPESAS];
static ElementoListaOrcamentos lista_orcamentos[MAXLISTAORCAMENTOS];
static int entrada_lista_despesas;
static int entrada_lista_orcamentos;
static double total_orcamento;



// FUNCOES
void inicializarListas(void) {
    /* DESCRICAO: inicializar a variavel-total-orcamentado, as Variaveis-'Entrada' para as 2 listas e tambem os membros 'seguinte' das listas
     * INPUT: ---
     * OUTPUT: --- (variaveis globais) */

    int i;

    entrada_lista_despesas = NULO;
    entrada_lista_orcamentos = NULO;
    total_orcamento = (double) 0;

    for (i=0; i < MAXLISTADESPESAS; i++) {
        lista_despesas[i].seguinte = VAZIO;
    }

    for (i=0; i < MAXLISTAORCAMENTOS; i++) {
        lista_orcamentos[i].seguinte = VAZIO;
    }
}


int colocarElementoListaDespesas(EntidadeDespesa dados) {
    /* DESCRICAO: colocar dados da entidade-despesa na lista-despesas
     * INPUT: estrutura com os dados da despesa
     * OUTPUT: 0 ou 1 > sucesso ou erro (lista cheia) */

    int id, i;

    // Procurar posicao disponivel na lista
    for (id = 0; id < MAXLISTADESPESAS && lista_despesas[id].seguinte != VAZIO; id++);

    // (if) Lista Cheia
    if (id >= MAXLISTADESPESAS) {
        return ERRO;
    }

    // Inserir elemento na lista
    lista_despesas[id].dados = dados;
    lista_despesas[id].seguinte = NULO;

    // Definir membro seguinte do ultimo elemento colocado na fila anterior a este
    if (entrada_lista_despesas == NULO) {
        entrada_lista_despesas = id;
    }
    else {
        for (i = entrada_lista_despesas; lista_despesas[i].seguinte != NULO; i = lista_despesas[i].seguinte);
        lista_despesas[i].seguinte = id;
    }

    return SUCESSO;
}


int colocarElementoListaOrcamentos(EntidadeOrcamento dados) {
    /* DESCRICAO: colocar dados da entidade-orcamento na lista-orcamento
     * INPUT: estrutura com os dados do orcamento
     * OUTPUT: 0 ou 1 > sucesso ou erro (lista cheia) */

    int id, i, iAnterior;

    // Procurar posicao disponivel na lista
    for (id = 0; id < MAXLISTAORCAMENTOS && lista_orcamentos[id].seguinte != VAZIO; id++);

    // (if) Lista Cheia
    if (id >= MAXLISTAORCAMENTOS) {
        return ERRO;
    }

    // Inserir elemento na lista
    lista_orcamentos[id].dados = dados;
    lista_orcamentos[id].seguinte = NULO;

    // Definir membro seguinte do ultimo elemento colocado na fila anterior a este
    if (entrada_lista_orcamentos == NULO) {
        entrada_lista_orcamentos = id;
    }
    else {
        for (iAnterior = i = entrada_lista_orcamentos; i != NULO; iAnterior = i, i = lista_orcamentos[i].seguinte){
            if (strcmp(lista_orcamentos[i].dados.tipo, lista_orcamentos[id].dados.tipo) == 0) {
                lista_orcamentos[id].seguinte = VAZIO;
                return ERRO;
            }
        }
        lista_orcamentos[iAnterior].seguinte = id;

        // incrementar no total do orcamento mensal
        total_orcamento = total_orcamento + dados.orcamento;
    }

    return SUCESSO;
}


int getProximoElementoListaDespesasPorOrcamento(EntidadeDespesa *dados, const char orcamento[]) {
    /* DESCRICAO: retirar proxima despesa da lista com concordancia com o tipo de orcamento pedido
     * INPUT: ponteiro para guardar a estrutura com os dados da despesa | tipo de orcamento a procurar
     * OUTPUT: 0 ou 1 > sucesso ou erro (ou seja, nao tem mais elementos) */

    int id_atual, id_anterior;

    // (if) Lista Vazia
    if (entrada_lista_despesas == NULO) {
        return ERRO;
    } else {
        // Procura elemento
        for (id_atual = entrada_lista_despesas, id_anterior = NULO; id_atual != NULO; id_anterior = id_atual, id_atual = lista_despesas[id_atual].seguinte) {
            if (strcmp(lista_despesas[id_atual].dados.tipo, orcamento) == 0) {
                *dados = lista_despesas[id_atual].dados;
                if (id_anterior == NULO) {
                    entrada_lista_despesas = lista_despesas[id_atual].seguinte;
                } else {
                    lista_despesas[id_anterior].seguinte = lista_despesas[id_atual].seguinte;
                }
                lista_despesas[id_atual].seguinte = VAZIO;
                return SUCESSO;
            }
        }
    }

    return ERRO;
}


int getProximoElementoListaOrcamentos(EntidadeOrcamento *dados) {
    /* DESCRICAO: retirar proximo orcamento da lista
     * INPUT: ponteiro para guardar a estrutura com os dados do orcamento
     * OUTPUT: 0 ou 1 > sucesso ou erro (ou seja, nao tem mais elementos) */

    int id_atual;

    // (if) Lista Vazia
    if (entrada_lista_orcamentos == NULO) {
        return ERRO;
    } else {
        // Proximo elemento
        id_atual = entrada_lista_orcamentos;
        entrada_lista_orcamentos = lista_orcamentos[id_atual].seguinte;
        *dados = lista_orcamentos[id_atual].dados;
        lista_orcamentos[id_atual].seguinte = VAZIO;
    }

    return SUCESSO;
}


int getProximoElementoListaDespesas(EntidadeDespesa *dados) {
    /* DESCRICAO: retirar proxima despesa da lista
     * INPUT: ponteiro para guardar a estrutura com os dados da despesa
     * OUTPUT: 0 ou 1 > sucesso ou erro (ou seja, nao tem mais elementos) */

    int id_atual;

    // (if) Lista Vazia
    if (entrada_lista_despesas == NULO) {
        return ERRO;
    } else {
        // Proximo elemento
        id_atual = entrada_lista_despesas;
        entrada_lista_despesas = lista_despesas[id_atual].seguinte;
        *dados = lista_despesas[id_atual].dados;
        lista_despesas[id_atual].seguinte = VAZIO;
    }

    return SUCESSO;
}


int isEntradaListaDespesasNulo(void) {
    /* DESCRICAO: para perceber se a lista-despesas esta vazia
     * INPUT: ---
     * OUTPUT: 1 ou 0 > verdade ou falso */

    return entrada_lista_despesas == NULO ? 1 : 0;
}


int isEntradaListaOrcamentosNulo(void) {
    /* DESCRICAO: para perceber se a lista-orcamentos esta vazia
     * INPUT: ---
     * OUTPUT: 1 ou 0 > verdade ou falso */

    return entrada_lista_orcamentos == NULO ? 1 : 0;
}


double getTotalOrcamentoMensal(void) {
    /* DESCRICAO: obter o valor do total do orcamento mensal
     * INPUT: ---
     * OUTPUT: valor variavel: total_orcamento */

    return total_orcamento;
}
