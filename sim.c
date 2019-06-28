/* AUTOR: STEFANO VIVACQUA PEREIRA */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"

/******** DECLARACOES GLOBAIS ********/

int debug = FALSE;
    //debug pode ser 1 e isso ativara prints de algumas funcoes do codigo

int *PageTable;
    //pagetable guarda se a pagina de index i esta na memoria (se pagetable[i] = TRUE ou FALSE)

ListaHead *MemoriaFisica;
    //lista de paginas na memoria

char algoritmo[3];
    //onde sera guardado o argv[1]; o tipo de algoritimo

/******** FIM DECLARACOES GLOBAIS ********/

/***************** CODIGO FUNCOES ******************/

// AGUARDA ENTER
// DESC: AGUARDA UM ENTER PRESS PRA PROSSEGUIR A EXECUCAO
// NAO RECEBE NADA NEM RETORNA NADA

void AguardaEnter(){
    printf("\nDEBUG: pressione ENTER para continuar a execucao...");
    getchar();
    fflush(stdin);
}

// CALCULA S
// DESC: CALCULA O NUMERO DE BITS REFERENTES AO TAMANHO DA PAGINA, PARA FUTURA OBTENCAO DOS BITS DE INDEX
// RECEBE TAMANHO DA PAGINA
// RETORNA S

int calculaS(int tamanhoPagina){
    int s;

    s = tamanhoPagina/8 +12; //12 ja eh a potencia de 2 para 4kb

    return s;
}

// SWAP
// DESC: REALIZA A TROCA DE PAGINAS DA MEMORIA DE ACORDO COM O ALGORITMO INPUTADO
// RECEBE INDEX DA PAGINA A SER SWAPPED OUT
// NAO RETORNA NADA

void Swap(Pagina *pagina){

    if( (strcmp(algoritmo, "LRU") == 0) || (strcmp(algoritmo, "lru") == 0) ){
        //se algoritmo eh LRU

        if(debug == TRUE){
            printf("DEBUG: Fazendo SWAP (LRU)\n");
        }
        
        //remove presenca da page table
        PageTable[MemoriaFisica->ultimo->pagina->indice] = FALSE;

        //Remove a pagina 'mais velha' (esta no fim da lista)
        RemoveDoFim(MemoriaFisica);
        if(debug == TRUE){
            printf("Index da pagina removida no swap: %d", MemoriaFisica->primeiro->pagina->indice);
        }
        
        //Insere pagina nova (no inicio da lista)
        InsereNoInicio(MemoriaFisica, pagina);
    }
    else if( (strcmp(algoritmo, "NRU") == 0) || (strcmp(algoritmo, "nru") == 0) ){
        //se algoritmo eh NRU

        if(debug == TRUE){
            printf("DEBUG: Fazendo SWAP (NRU)\n");
        }

        //prepara NoCorrente no whiles
        ListaNo *NoCorrente = MemoriaFisica->ultimo;
        //caso r = false e w = false
        while(NoCorrente != NULL){

            if(NoCorrente->pagina->r == FALSE && NoCorrente->pagina->w == FALSE){
                //se r = false e w = false

                //remove do page table
                PageTable[NoCorrente->pagina->indice] = FALSE;

                //remove no da lista
                RemoveNo(MemoriaFisica, NoCorrente);
                if(debug == TRUE){
                    printf("Index da pagina removida no swap: %d\n", NoCorrente->pagina->indice);
                }

                //insere pagina nova na lista
                InsereNoInicio(MemoriaFisica, pagina);

                return;
            }

            NoCorrente = NoCorrente->anterior;
        }
        //prepara NoCorrente no whiles
        NoCorrente = MemoriaFisica->ultimo;
        //caso r = false e w = True
        while(NoCorrente != NULL){

            if(NoCorrente->pagina->r == FALSE && NoCorrente->pagina->w == TRUE){
                //se r = false e w = false

                //remove do page table
                PageTable[NoCorrente->pagina->indice] = FALSE;

                //remove no da lista
                RemoveNo(MemoriaFisica, NoCorrente);
                if(debug == TRUE){
                    printf("Index da pagina removida no swap: %d\n", NoCorrente->pagina->indice);
                }

                //insere pagina nova na lista
                InsereNoInicio(MemoriaFisica, pagina);

                return;
            }

            NoCorrente = NoCorrente->anterior;
        }
        //prepara NoCorrente no whiles
        NoCorrente = MemoriaFisica->ultimo;
        //caso r = True e w = false
        while(NoCorrente != NULL){

            if(NoCorrente->pagina->r == TRUE && NoCorrente->pagina->w == FALSE){
                //se r = false e w = false

                //remove do page table
                PageTable[NoCorrente->pagina->indice] = FALSE;

                //remove no da lista
                RemoveNo(MemoriaFisica, NoCorrente);
                if(debug == TRUE){
                    printf("Index da pagina removida no swap: %d\n", NoCorrente->pagina->indice);
                }

                //insere pagina nova na lista
                InsereNoInicio(MemoriaFisica, pagina);

                return;
            }

            NoCorrente = NoCorrente->anterior;
        }
        //prepara NoCorrente no whiles
        NoCorrente = MemoriaFisica->ultimo;
        //caso r = True e w = True
        while(NoCorrente != NULL){

            if(NoCorrente->pagina->r == TRUE && NoCorrente->pagina->w == TRUE){
                //se r = false e w = false

                //remove do page table
                PageTable[NoCorrente->pagina->indice] = FALSE;

                //remove no da lista
                RemoveNo(MemoriaFisica, NoCorrente);
                if(debug == TRUE){
                    printf("Index da pagina removida no swap: %d\n", NoCorrente->pagina->indice);
                }

                //insere pagina nova na lista
                InsereNoInicio(MemoriaFisica, pagina);

                return;
            }

            NoCorrente = NoCorrente->anterior;
        }        
    }

    return;
}

// ESCREVE NA MEMORIA
// DESC: ADICIONA UMA PAGINA A LISTA DE PAGINAS NA MEMORIA
// RECEBE INDICE (ENDERECO) E RW
// RETORNA NADA

void EscreveNaMemoria(int indice, char rw, int numMaxPags){

    //CRIA PAGINA
    Pagina *pagina = (Pagina *) malloc(sizeof(Pagina));

    pagina->indice = indice;
    if(rw == 'R'){
        pagina->r = TRUE;
        pagina->w = FALSE;
    }
    else{
        pagina->r = FALSE;
        pagina->w = TRUE;
    }

    if(MemoriaFisica->tamanho <  numMaxPags){
        //se memoria fisica nao esta cheia
        if(debug == TRUE){
            printf("DEBUG: memoria fisica NAO esta cheia; escrevendo na memoria\n");
        }

        //insere pagina na lista de paginas na memoria
        InsereNoInicio(MemoriaFisica, pagina);
    }
    else{
        //se a memoria fisica esta cheia
        if(debug == TRUE){
            printf("DEBUG: Memoria fisica cheia; fazendo swap\n");
        }

        Swap(pagina);
    }
    

    return;
}

// RESETA REFERENCIAS
// DESC: RETA O INT R PARA 0 DE TODAS AS PAGINAS PRESENTES NA MEMORIA
// RECEBE LISTA DE PAGINAS NA MEMORIA
// RETORNA NADA

void ResetaReferencias(ListaHead *MemoriaFisica){

    ListaNo *NoCorrente = MemoriaFisica->primeiro;

    //percorre a lista resentado
    while(NoCorrente != NULL){
        
        //reseta r
        NoCorrente->pagina->r = FALSE;
        
        //avanca na lista
        NoCorrente = NoCorrente->proximo;
    }

    return;
}

/************ FIM CODIGO FUNCOES **************/

/***************** CODIGO MAIN ******************/

int main (int argc,char* argv[]){
    
    /******** DECLARACOES **********/
    
    //misc
    int i;
    int iteracao = 0;

    //argvs ( lembando que algoritmo foi declarado globalmente)
    char arquivoNome[20]; //atencao aqui nesse tamanho
    int tamanhoPagina;
    int memoriaTotal;

    //'tamanho dos indices' das estruturas pagetable e memoriafisica
    int maxPagsMemoriaFisica;
    int qtdPagsPageTable;

    //arquivo
    FILE *arquivo;
    unsigned int enderecoLogico;
    char rw;

    //enderecos
    int indexPagina;

    //contagens
    int pageHits = 0;
    int pageFaults = 0;

    /******** FIM DECLARACOES **********/

    /*********** INICIALIZACAO DO PROGRAMA (CHECAGEM E LOADING DE VARIAVEIS) *********/

    if(argc != 6){
        printf("Uso do programa: ./sim <ALG> <ARQUIVO> <TAMANHO DA PAGINA> <MEMORIA TOTAL> <DEBUG>\n");
        printf("OBSERVACOES:\n<DEBUG> varia de 0 ou 1;\ntamanho da pagina deve ser em KB;\ntamanho da memoria total em MB\n");
        exit(-1);
    }

    strcpy(algoritmo, argv[1]);
    strcpy(arquivoNome, argv[2]);
    tamanhoPagina = atoi(argv[3]); //passado em kb
    memoriaTotal = atoi(argv[4]) * 1024; //eh passado em mb, transformo em kb
    debug = atoi(argv[5]);

    if(debug == TRUE){
        printf("\n!!!!! Rodando em modo de debug !!!!!\n\n");
    }
    else{
        printf("\n!!!!! Rodando em modo de producao !!!!!\n\n");
    }

    //calcula numero de paginas na memoria e na pagetable
    maxPagsMemoriaFisica = memoriaTotal/tamanhoPagina;
    qtdPagsPageTable = pow(2, 32 - calculaS(tamanhoPagina)); //32 eh o endereco inteiro; o indice eh 32 - os bits de tamanho da pagina (s)

    if(debug == TRUE){
        printf("DEBUG: tamanho da pagina: %d\nDEBUG: s calculado: %d\n", tamanhoPagina, calculaS(tamanhoPagina));
    }

    //aloca espaco da pagetable
    PageTable = (int*) malloc(qtdPagsPageTable*sizeof(int));

    //inicializa estruturas com nil
    MemoriaFisica = CriarLista();

    for(i=0; i<qtdPagsPageTable; i++){
        PageTable[i] = FALSE; //no inicio nenhuma pagina esta na memoria
    }

    //inicializa arquivo
    arquivo = fopen(arquivoNome, "r");
    if(arquivo == NULL){
        printf("ERRO: problema na leitura do arquivo especificado\n");
        exit(-1);
    }

    /********* FIM INICIALIZACAO *********/

    /********* LEITURA DO ARQUIVO **********/

    while(fscanf(arquivo,"%x %c\n", &enderecoLogico, &rw) == 2){

            if(debug == TRUE){
                printf("DEBUG: Iteracao: %d\n", iteracao);
                printf("DEBUG: Edereco lido do arquivo: %08X\n", enderecoLogico);
                printf("DEBUG: RW lido do arquivo: %c\n", rw);
            }
            //calcula index das paginas
            indexPagina = enderecoLogico >> calculaS(tamanhoPagina);

            if(debug == TRUE){
                printf("DEBUG: INDEX calculado a partir do endereco: %d\n", indexPagina);
            }

            if(debug == TRUE){
                printf("DEBUG: Verificando existencia de pagina na memoria...\n");
            }

            // TRATA MEMORIA FISICA

            if(PageTable[indexPagina] == TRUE){
                //se pagina esta na memoria
                if(debug == TRUE){
                    printf("DEBUG: PAGE HIT! Pagina ja se encontra na memoria\n");
                }

                //obtem pagina para editar rw
                Pagina *paginaHitada;
                paginaHitada = obterPagina(MemoriaFisica, indexPagina);

                if(paginaHitada == NULL){
                    printf("ERRO OBTENCAO PAGINA HITADA\n");
                    printf("Iteracao do erro: %d\n", iteracao);
                    printf("Index da iteracao do erro: %d\n", indexPagina);
                    exit(-1);
                }
                
                //atualiza rw da pagina
                if(rw == 'R'){
                    paginaHitada->r = 1;
                }
                else{
                    paginaHitada->w = 1;
                }

                //Se o algoritmo for LRU, envia pagina para inicio da fila
                if( (strcmp(algoritmo, "LRU") == 0) || (strcmp(algoritmo, "lru") == 0) ){
                    if(debug == TRUE){
                        printf("DEBUG: LRU: Enviando pagina para inicio da lista\n");
                    }

                    EnviarParaInicio(MemoriaFisica,indexPagina);
                }

                pageHits++;
            }
            else{
                //se pagina NAO esta na memoria
                if(debug == TRUE){
                    printf("DEBUG: PAGE FAULT! Pagina NAO se encontra na memoria\n");
                }

                //marca no page table
                PageTable[indexPagina] = TRUE;

                EscreveNaMemoria(indexPagina, rw, maxPagsMemoriaFisica);

                //atualiza rw da pagina

                pageFaults++;
            }

        iteracao++;

        //Se o algoritmo for NRU, reseta R de todas as paginas a cada 100000 (cem mil) iteracoes
        if( (strcmp(algoritmo, "NRU") == 0) || (strcmp(algoritmo, "nru") == 0) ){
            if(iteracao % 100000 == 0){
                if(debug == TRUE){
                    printf("Resetando bits R\n");
                }

                ResetaReferencias(MemoriaFisica);
            }
        }

        if(debug == TRUE){
            AguardaEnter();
        }

    }

    printf("Fim da simulacao\n");
    printf("Total de page faults: %d\n", pageFaults);
    printf("Tptal de page hits: %d\n", pageHits);
    printf("Total de iteracoes: %d\n", iteracao);

    return 0;

}

    // ListaNo *NoCorrente = kbc->primeiro;

    // while(NoCorrente != NULL){
    //     printf("no corrente: %d\n", NoCorrente->valor);
    //     NoCorrente = NoCorrente->proximo;
    // }
