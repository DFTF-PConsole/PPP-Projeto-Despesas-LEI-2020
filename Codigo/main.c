// REALIZADO POR: Dário Félix - 2018275530 | TRABALHO FINAL - PPP 2019/2020
// FICHEIRO / PARTE: Main (Ficheiro C)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * LISTAGEM DAS ESCOLHAS FEITAS - Comportamento do Programa nas Situações não Previstas no Enunciado:
 * -> O programa é case-sensitive e suporta UTF-8 (leitura, tratamento e escrita);
 * -> 3 Argumentos na Linha de Comandos: <programa.out> <fich_saida_1> <fich_saida_2>;
 * -> Na abertura dos ficheiros para leitura através do stdin é dado várias oportunidades ao utilizador
 * na introdução do nome/caminho caso erre (ou haja erro) na abertura desses ficheiros.
 * -> Na leitura de ficheiros, é lido linha a linha, se nessa linha (entidade) não constar o esquema
 * pedido (numero de atributos) ou se o valor é negativo, essa linha/entidade é ignorada;
 * -> Na leitura de ficheiros, atributos como tipo e descricao demasiadamente grandes serão considerados,
 * mas 'cortados' no momento em que ultrapassa a capacidade da variavel;
 * -> Se o ficheiro das despesas estiver vazio, o programa continua e assume que não há despesas. No
 * caso de acontecer no ficheiro com os orcamentos, o programa é abortado;
 * -> Se as listas ficarem cheias (e ainda houver dados nos ficheiros) o programa é terminado;
 * -> Na escrita dos ficheiros, é retirado um tipo/orcamento da lista em cada iteracao, e nessa iteracao
 * é percorrida a lista das despesas (num outro ciclo), onde é feita a acumulacao da despesa nesse tipo,
 * e escreve no ficheiro 1. Depois é feito o teste do desvio, se confirmar é calculado o desvio global
 * e guardado no ficheiro 2;
 * -> Em cada momento em que é retirado uma entidade das listas, 'retirado' tem um significado literal.
 * -> No final, é averiguado se sobrou despesas na lista (teoricamente correspondem a despesas que não
 * foram procuradas, pois o nome do tipo de orcamento não foi referido nos ficheiros para leitura). Nestes
 * casos é imprimida (stdout) uma listagem com estas despesas - e o programa continua com as restantes.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



// BIBLIOTECA
#include "trabalho.h"



// VARS GLOBAIS
/*  *vazio*  */



// FUNCOES
int main(int argc, char * argv []) {
    /* DESCRICAO: Funcao Principal. Apenas chama outras funcoes.
     * INPUT: 2 nomes de 2 ficheiros de saida na linha de comandos
     * OUTPUT: --- (objetivo do programa) */

    FILE * fich_saida_despesas = NULL;
    FILE * fich_saida_orcamento = NULL;
    FILE * fich_entrada_orcamento = NULL;
    FILE * fich_entrada_despesas = NULL;

    // Executa Introducao()
    introducao();

    // Executa AbrirFicheiros()   (Erro [grave] -> termina o programa)
    if (abrirFicheiros(argc, (const char **) argv, &fich_saida_despesas, &fich_saida_orcamento, &fich_entrada_orcamento, &fich_entrada_despesas) == ERRO) {
        exit(1);
    }

    // Executa EntradaDados()   (Erro [grave] -> termina o programa)
    if (entradaDados(&fich_entrada_orcamento, &fich_entrada_despesas) == ERRO) {
        exit(1);
    }

    // Fecha os ficheiros abertos para leitura
    fclose(fich_entrada_orcamento);
    fclose(fich_entrada_despesas);

    // Executa SaidaDados()
    saidaDados(&fich_saida_despesas, &fich_saida_orcamento);

    // Fecha os ficheiros abertos para escrita
    fclose(fich_saida_despesas);
    fclose(fich_saida_orcamento);

    printf("\n ***** PROGRAMA TERMINADO COM SUCESSO. *****\n");
    return 0;
}


void getStringStdin(char string[], int n) {
    /* DESCRICAO: Funcao personalizada para ler, guardar e tratar dados no STDIN, limpando o restante.
     * INPUT: string: local onde é guardada os chars recolhidos | n: tamanho máximo suportado por string
     * OUTPUT: --- */

    int i = 0, c;

    // ler stdin ate atingir o final do stdin ou da memoria disponivel
    while(i<(n-1) && ((string[i] = (char) getchar()) != '\n' && string[i] != EOF))
        i++;

    // (If) Sobrou dados no stdin > Limpar stdin
    if (string[i] != '\n' && string[i] != EOF)
        while((c = getchar()) != '\n' && c != EOF);

    // Colocar terminador '\0' no fim
    string[i] = '\0';
}


void introducao(void) {
    /* DESCRICAO: Introducao imprimida no inicio do programa: explicacao breve sobre o programa (Inputs e Outputs)
     * INPUT: ---
     * OUTPUT: --- */

    puts("********************************************************************************\n");
    puts("Trabalho Final de PPP | Dário Félix - Nº 2018275530 | LEI/FCTUC 2019/2020\n");
    puts("> FUNÇÃO: Análise das despesas mensais de uma pessoa.");
    puts("> ENTRADA: 2 ficheiros txt (leitura). #1 ficheiro com uma lista dos orçamentos por tipo de despesa. 2# ficheiro com uma lista de despesas.");
    puts("> SAIDA: 2 ficheiros txt (escrita). #1 ficheiro com os totais das despesas em cada tipo. 2# ficheiro com orçamentos com o desvio da despesa supeiror a 10%%, incluindo o seu desvio global.");
    puts("> MANEJO: Introdução pela linha de comandos dos nomes/caminhos dos 2 ficheiros para escrita e introdução dos nomes/caminhos dos 2 ficheiros para leitura aquando pedido ao utilizador (stdin).\n");
    puts("> ESQUEMA DOS FICHEIROS:");
    puts("   > Ficheiro-Entrada #1: <Tipo> # <Valor>");
    puts("   > Ficheiro-Entrada #2: <Descrição> # <Valor> # <Tipo>");
    puts("   > Ficheiro-Saida #1: <Tipo> \\t <Despesa>");
    puts("   > Ficheiro-Saida #2: <Tipo> \\t <Desvio Global>\n");
    puts("********************************************************************************\n");
}


int abrirFicheiros(int argc_copia, const char * argv_copia [], FILE ** fich_saida_despesas, FILE ** fich_saida_orcamento, FILE ** fich_entrada_orcamento, FILE ** fich_entrada_despesas) {
    /* DESCRICAO: abre os ficheiros para leitura e escrita (4 ficheiros). Apresenta correcao de erros.
     * INPUT: argc do main | argv do main | ponteiros de ponteiros de ficheiros para 4 ficheiros
     * OUTPUT: 1 ou 0 > erro ou sucesso */

    char nome_ficheiro[MAXSTRFICH];
    int count_erros;

    // Verifica o numero de argumentos na linha de comandos
    if (argc_copia != NARGC) {
        printf("\n*ERRO* : Sao necessarios %d parametros na linha de comandos, mas foram introduzidos %d parametros! PROGRAMA TERMINADO.\n\n", NARGC, argc_copia);
        return ERRO;
    }

    // Abre os ficheiros para escrita atraves dos argumentos pela linha de comandos
    *fich_saida_despesas = fopen(argv_copia[1], "w");
    *fich_saida_orcamento = fopen(argv_copia[2], "w");

    // Verifica os ficheiros
    if (*fich_saida_despesas == NULL || *fich_saida_orcamento == NULL) {
        printf("\n*ERRO* : fopen() - Na abertura dos ficheiros de saída! PROGRAMA TERMINADO.\n\n");
        return ERRO;
    }

    // Abre os ficheiros para leitura atraves do input pelo stdin. Possibilita correcao de erros.
    count_erros = 0;
    do {
        // Ficheiro 1 ja aberto?
        if (*fich_entrada_orcamento == NULL) {
            // Pergunta o nome/caminho e abre o ficheiro 1 correspondente
            printf(">>> Introduza o nome/caminho para o ficheiro de leitura com os dados dos orçamentos: \n>");
            getStringStdin(nome_ficheiro, MAXSTRFICH);
            putchar('\n');
            *fich_entrada_orcamento = fopen(nome_ficheiro, "r");

            // Verifica o ficheiro 1
            if (*fich_entrada_orcamento == NULL) {
                count_erros++;
                printf("*ERRO* : fopen() - Na abertura do ficheiro para leitura! TENTE NAVAMENTE!\n");
                continue;
            } else {
                count_erros = 0;
            }
        }
        // Ficheiro 2 ja aberto?
        if (*fich_entrada_despesas == NULL) {
            // Pergunta o nome/caminho e abre o ficheiro 2 correspondente
            printf(">>> Introduza o nome/caminho para o ficheiro de leitura com os dados das despesas: \n>");
            getStringStdin(nome_ficheiro, MAXSTRFICH);
            putchar('\n');
            *fich_entrada_despesas = fopen(nome_ficheiro, "r");

            // Verifica o ficheiro 2
            if (*fich_entrada_despesas == NULL) {
                count_erros++;
                printf("*ERRO* : fopen() - Na abertura do ficheiro para leitura! TENTE NAVAMENTE!\n");
            } else {
                count_erros = -1;
            }
        }
    } while(count_erros < 3 && count_erros > 0);

    // Verifica se o ciclo-do-while terminou devido a erros
    if (count_erros >= 3) {
        printf("\n*ERRO* : fopen() - Demasiados erros na abertura dos ficheiros para leitura! PROGRAMA TERMINADO.\n\n");
        return ERRO;
    }

    return SUCESSO;
}


int entradaDados(FILE ** fich_entrada_orcamento, FILE ** fich_entrada_despesas) {
    /* DESCRICAO: Le, trata e copia os dados dos ficheiros para as estruturas de dados do ficheiro autonomo. Apresenta correcao de erros.
     * INPUT: 2 ponteiros para 2 ponteiros de ficheiros para leitura (guardados no main)
     * OUTPUT: 1 ou 0 > erro ou sucesso */

    int i;
    EntidadeOrcamento temp_orcamento;
    EntidadeDespesa temp_despesa;
    char temp_string[MAXSTRTEMP];
    char temp_char;
    char * temp_ptr_char;

    // Inicializar listas e variaveis globais static do ficheiro autonomo
    inicializarListas();


    // ----- Ficheiro Entrada Orcamentos / Estrutura com Orcamentos | Ciclo ate atingir o final do ficheiro -----
    for (i=0; ((temp_string[i] = (char) getc(*fich_entrada_orcamento)) != EOF); i=0) {

        // --- 1# Atributo Entidade-Orcamento: Tipo ---
        for (i++; ((temp_string[i] = (char) getc(*fich_entrada_orcamento)) != EOF) && temp_string[i] != '\n' && temp_string[i] != '#' && i < MAXSTRTIPO-1; i++);

        temp_char = temp_string[i];
        temp_string[i] = '\0';

        // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
        if (temp_char == '\n' || temp_char == EOF) {
            printf("*AVISO* : entradaDados() - Entidade incompleta ou vazia foi ignorada. | Dado: %s\n", temp_string);
            continue;
        } else {
            if (temp_string[0] == '\0' && temp_char == '#') {
                while (((temp_char = (char) getc(*fich_entrada_orcamento)) != EOF) && temp_char != '\n');
                printf("*AVISO* : entradaDados() - Entidade vazia foi ignorada.\n");
                continue;
            } else {
                if (i >= MAXSTRTIPO - 1 && temp_char != '#') {
                    printf("*AVISO* : entradaDados() - Nome do tipo demasiadamente grande, mas considerado na mesma. | Dado: %s\n",
                           temp_string);
                    while (((temp_char = (char) getc(*fich_entrada_orcamento)) != EOF) && temp_char != '\n' &&
                           temp_char != '#');
                    if (temp_char != '#') {
                        printf("*AVISO* : entradaDados() - Entidade incompleta foi ignorada.\n");
                        continue;
                    }
                }

                // Estando sem erros, copia para a estrutura temporaria para ser colocado na lista
                strcpy(temp_orcamento.tipo, temp_string);
            }
        }

        // --- 2# Atributo Entidade-Orcamento: Valor ---
        for (i=0; ((temp_string[i] = (char) getc(*fich_entrada_orcamento)) != EOF) && temp_string[i] != '\n' && i < MAXSTRTEMP-1; i++);

        temp_char = temp_string[i];
        temp_string[i] = '\0';

        // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
        if ((temp_char == EOF || temp_char == '\n') && temp_string[0] == '\0') {
            printf("*AVISO* : entradaDados() - Entidade incompleta ou vazia foi ignorada.\n");
            continue;
        } else {

            // Estando sem erros, copia para a estrutura temporaria para ser colocado na lista
            temp_orcamento.orcamento = -1;
            temp_orcamento.orcamento = strtod(temp_string, &temp_ptr_char);

            // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
            if (temp_orcamento.orcamento < (double) 0 && (errno != 0 || temp_string == temp_ptr_char)) {
                printf("*AVISO* : entradaDados() - Entidade incompleta foi ignorada. (Erro no valor orcamento)| Dado: %s\n", temp_string);
                continue;
            }
        }

        // Inserir Entidade-Orcamento na lista
        if (colocarElementoListaOrcamentos(temp_orcamento) == ERRO) {
            printf("*ERRO* : colocarElementoListaOrcamentos() - Lista cheia ou orçamento duplicado. Possibilidade do esquema dos ficheiros esteja incorreto. PROGRAMA TERMINADO.\n");
            return ERRO;
        }
    }

    // Verificar se a lista com os orcamentos (ou o ficheiro) esta vazia
    if (isEntradaListaOrcamentosNulo()) {
        printf("*ERRO* : entradaDados() - Lista-orcamentos ou ficheiro estão vazios. PROGRAMA TERMINADO.\n");
        return ERRO;
    }


    // ----- Ficheiro Entrada Despesas / Estrutura com Despesas | Ciclo ate atingir o final do ficheiro -----
    for (i=0; ((temp_string[i] = (char) getc(*fich_entrada_despesas)) != EOF); i=0) {

        // --- 1# Atributo Entidade-Despesa: Descricao ---
        for (i++; ((temp_string[i] = (char) getc(*fich_entrada_despesas)) != EOF) && temp_string[i] != '\n' && temp_string[i] != '#' && i < MAXSTRDESCRICAO-1; i++);

        temp_char = temp_string[i];
        temp_string[i] = '\0';

        // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
        if (temp_char == '\n' || temp_char == EOF) {
            printf("*AVISO* : entradaDados() - Entidade incompleta ou vazia foi ignorada. | Dado: %s\n", temp_string);
            continue;
        } else {
            if (temp_string[0] == '\0' && temp_char == '#') {
                while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n');
                printf("*AVISO* : entradaDados() - Entidade vazia foi ignorada.\n");
                continue;
            } else {
                if (i >= MAXSTRDESCRICAO - 1 && temp_char != '#') {
                    printf("*AVISO* : entradaDados() - Descricao demasiadamente grande, mas considerado na mesma. | Dado: %s\n",temp_string);
                    while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n' && temp_char != '#');
                    if (temp_char != '#') {
                        printf("*AVISO* : entradaDados() - Entidade incompleta foi ignorada.\n");
                        continue;
                    }
                }

                // Estando sem erros, copia para a estrutura temporaria para ser colocado na lista
                strcpy(temp_despesa.descricao, temp_string);
            }
        }

        // --- 2# Atributo Entidade-Despesa: Valor ---
        for (i=0; ((temp_string[i] = (char) getc(*fich_entrada_despesas)) != EOF) && temp_string[i] != '\n' && temp_string[i] != '#' && i < MAXSTRTEMP-1; i++);

        temp_char = temp_string[i];
        temp_string[i] = '\0';

        // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
        if (temp_char == '\n' || temp_char == EOF) {
            printf("*AVISO* : entradaDados() - Entidade incompleta ou vazia foi ignorada. | Dado: %s\n", temp_string);
            continue;
        } else {
            if (temp_string[0] == '\0' && temp_char == '#') {
                while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n');
                printf("*AVISO* : entradaDados() - Entidade vazia foi ignorada.\n");
                continue;
            } else {
                if (i >= MAXSTRTEMP - 1 && temp_char != '#') {
                    printf("*AVISO* : entradaDados() - Valor da despesa demasiadamente grande. Entidade ignorada. | Dado: %s\n",temp_string);
                    while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n');
                    continue;
                }

                // Estando sem erros, copia para a estrutura temporaria para ser colocado na lista
                temp_despesa.valor = -1;
                temp_despesa.valor = strtod(temp_string, &temp_ptr_char);

                // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
                if (temp_despesa.valor < (double) 0 && (errno != 0 || temp_string == temp_ptr_char)) {
                    printf("*AVISO* : entradaDados() - Entidade incompleta foi ignorada. (Erro no valor da despesa) | Dado: %s\n", temp_string);
                    while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n');
                    continue;
                }
            }
        }

        // --- 3# Atributo Entidade-Despesa: Tipo ---
        for (i=0; ((temp_string[i] = (char) getc(*fich_entrada_despesas)) != EOF) && temp_string[i] != '\n' && i < MAXSTRTIPO-1; i++);

        temp_char = temp_string[i];
        temp_string[i] = '\0';

        // Despistagem de possiveis erros (continue -> aborta a entidade em questão, passando para a seguinte)
        if ((temp_char == EOF || temp_char == '\n') && temp_string[0] == '\0') {
            printf("*AVISO* : entradaDados() - Entidade incompleta ou vazia foi ignorada.\n");
            continue;
        } else {
            if (i >= MAXSTRTIPO - 1 && temp_char != '\n') {
                printf("*AVISO* : entradaDados() - Nome do tipo demasiadamente grande, mas considerado na mesma. | Dado: %s\n",temp_string);
                while (((temp_char = (char) getc(*fich_entrada_despesas)) != EOF) && temp_char != '\n');
            }

            // Resolver problema windows de fim da linha "\r\n" (mais tarde, nas comparacoes)
            if (i>0 && temp_string[i-1] == '\r') {
                temp_string[i-1] = '\0';
            }

            // Estando sem erros, copia para a estrutura temporaria para ser colocado na lista
            strcpy(temp_despesa.tipo, temp_string);
        }

        // Inserir Entidade-Despesas na lista
        if (colocarElementoListaDespesas(temp_despesa) == ERRO) {
            printf("*ERRO* : colocarElementoListaDespesas() - Lista cheia. PROGRAMA TERMINADO.\n");
            return ERRO;
        }
    }

    // Verificar se a lista com as despesas esta vazia
    if (isEntradaListaDespesasNulo()) {
        printf("*AVISO* : entradaDados() - Lista-despesas ou ficheiro estão vazios. IGNORADO: assumir que não existem despesas.\n");
    }

    return SUCESSO;
}


int saidaDados(FILE ** fich_saida_despesas, FILE ** fich_saida_orcamento) {
    /* DESCRICAO: Le e trata os dados das estrutura de dados (listas) e guarda os resultados nos ficheiros de saida. Apresenta correcao de erros.
     * INPUT: 2 ponteiros para 2 ponteiros de ficheiros para escrita (guardados no main)
     * OUTPUT: 1 ou 0 > erro ou sucesso */

    EntidadeOrcamento dados_orcamento;
    EntidadeDespesa dados_despesa;
    double total_despesa_orcamento;
    double orcamento_desvio;
    double total_orcamento_mensal;
    double percentagem_desvio_global;

    // Obter o orcamento mensal (somatorio dos orcamentos de cada tipo)
    total_orcamento_mensal = getTotalOrcamentoMensal();

    // Ler/retirar a cada orcamento retirado da lista, todos as despesas da lista a ele relacionado
    while (getProximoElementoListaOrcamentos(&dados_orcamento) != ERRO) {
        // Calcular o orcamento limite, ou seja, orcamento com o desvio 10%
        orcamento_desvio = dados_orcamento.orcamento + (dados_orcamento.orcamento * 0.10);

        // Calcular despesa total num determinado tipo/orcamento
        total_despesa_orcamento = (double) 0;
        while (getProximoElementoListaDespesasPorOrcamento(&dados_despesa, dados_orcamento.tipo) != ERRO) {
            total_despesa_orcamento = total_despesa_orcamento + dados_despesa.valor;
        }

        // Escrever no ficheiro 1 <tipo orcamento> <despesa total neste tipo>
        fprintf(*fich_saida_despesas, "%s\t%.2lf\n", dados_orcamento.tipo, total_despesa_orcamento);

        // Verificar se a despesa ultrapassa o desvio >10% do orcamentado
        if (total_despesa_orcamento >= orcamento_desvio && total_despesa_orcamento > 0) {
            // Prevenir erro divisao por zero
            if (total_orcamento_mensal == (double) 0) {
                printf("*AVISO* : saidaDados() - Orçamento mensal (somatório de todos os tipos) é igual a zero. Impossivel calcular desvio global. (IGNORADO)\n");
                continue;
            }

            // Calcular desvio global entre o que foi gasto a mais da categoria e o orcamento mensal
            percentagem_desvio_global = ((total_despesa_orcamento - dados_orcamento.orcamento) / total_orcamento_mensal ) * 100;

            // Escrever no ficheiro 2 <tipo orcamento> <% desvio global>
            fprintf(*fich_saida_orcamento, "%s\t%.1lf%%\n", dados_orcamento.tipo, percentagem_desvio_global);
        }
    }

    printf("*AVISO* : saidaDados() - FORAM ESCRITOS 2 FICHEIROS COM SUCESSO.\n");

    // Verificar se sobrou entidades-despesa nas listas (sem correspondencia com um orcamento que seja conhecido)
    if (getProximoElementoListaDespesas(&dados_despesa) != ERRO) {
        printf("\n*AVISO* : saidaDados() - EXISTEM (ENTIDADES) DESPESAS SEM O ORÇAMENTO INTRODUZIDO/CONHECIDO. Foram ignoradas as seguintes entidades:\n");
        printf("> Descrição: %s | Valor: %.2lf | Tipo: %s\n",dados_despesa.descricao ,dados_despesa.valor, dados_despesa.tipo);
        while (getProximoElementoListaDespesas(&dados_despesa) != ERRO) {
            printf("> Descrição: %s | Valor: %.2lf | Tipo: %s\n",dados_despesa.descricao ,dados_despesa.valor, dados_despesa.tipo);
        }
    }

    return SUCESSO;
}
