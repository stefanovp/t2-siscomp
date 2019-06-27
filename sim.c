#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"

#define TRUE 1
#define FALSE 0

/******** DECLARACOES GLOBAIS ********/

//STRUCT PAGINA
struct pagina {
    int indice;
    int r;
    int w;

}; typedef struct pagina Pagina;

//STRUCT PageTableEntry
struct PageTableEntry {
    Pagina *pagina;
    int presenca;
}; typedef struct PageTableEntry PageTableEntry;

//VARIAVEIS GLOBAIS
int debug = FALSE;

PageTableEntry **PageTable;

ListaHead *MemoriaFisica;

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

// BUSCA PAGE TABLE
// DESC: PROCURA POR PAGINA DENTRO DA ESTRUTURA PAGE TABLE
// RECEBE INDICE DA PAGINA
// RETORNA 1 SE PAGINA EXISTE NO PAGE TABLE
// RETORNA 0 SE PAGINA NAO EXISTE NO PAGE TABLE

int buscaPageTable(int index){

    if(PageTable[index] == NULL){
        //pagina com tal index ainda nao existe na PT
        return 0;
    }
    else{
        //pagina com tal index existe na PT
        return 1;
    }
}

// ESCREVE NA MEMORIA
// DESC: ADICIONA UMA PAGINA A LISTA DE PAGINAS NA MEMORIA
// RECEBE INDICE (ENDERECO) E RW
// RETORNA NADA

void EscreveNaMemoria(int indice, char rw){

    Pagina *pagina = (Pagina *) malloc(sizeof(Pagina));

    return;
}

/************ FIM CODIGO FUNCOES **************/

/***************** CODIGO MAIN ******************/

int main (int argc,char* argv[]){
    
    /******** DECLARACOES **********/
    
    //misc
    int i;

    //argvs
    char algoritmo[3];
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
    int pageHits;
    int pageFaults;

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
        printf("Rodando em modo de debug\n");
    }
    else{
        printf("Rodando em modo de producao\n");
    }

    //calcula numero de paginas na memoria e na pagetable
    maxPagsMemoriaFisica = memoriaTotal/tamanhoPagina;
    qtdPagsPageTable = pow(2, 32 - calculaS(tamanhoPagina)); //32 eh o endereco inteiro; o indice eh 32 - os bits de tamanho da pagina (s)

    if(debug == TRUE){
        printf("DEBUG: tamanho da pagina: %d\nDEBUG: s calculado: %d\n", tamanhoPagina, calculaS(tamanhoPagina));
    }

    //aloca espaco da pagetable
    PageTable = (PageTableEntry **) malloc(qtdPagsPageTable*sizeof(PageTableEntry *));

    //inicializa estruturas com nil
    MemoriaFisica = CriarLista();

    for(i=0; i>qtdPagsPageTable; i++){
        PageTable[i] = NULL;
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
            
            //calcula index das pagnas
            indexPagina = enderecoLogico >> calculaS(tamanhoPagina);
  

            // TRATA PAGE TABLE
            if(debug == TRUE){
                printf("DEBUG: Verificando existencia de pagina no page table...\n");
            }

            if(buscaPageTable(indexPagina) != 1){
                //se pagna nao existe no page table:

                if(debug == TRUE){
                    printf("DEBUG: Pagina com index %d NAO existe na page table\n", indexPagina);
                }

                //cria pagina
                Pagina *p = (Pagina *) malloc(sizeof(Pagina));
                p->indice = indexPagina;
                if(rw = 'R'){
                    //se pagina for referenciada, r = true e w = false
                    p->r = TRUE;
                    p->w = FALSE;
                }
                else{
                    //se pagina for lida, r = false e w = true
                    p->r = FALSE;
                    p->w = TRUE;
                }

                //cria entrada do page table
                PageTableEntry *PTentry = malloc(sizeof(PageTableEntry));
                PTentry->pagina = p;
                PTentry->presenca = 0;

                //insere no page table
                PageTable[indexPagina] = PTentry;
            }
            else{
                //se pagina ja existe no page table:

                if(debug == TRUE){
                    printf("DEBUG: Pagina com index %d JA existe na page table\n", indexPagina);
                }
                
                //atualiza R ou W
                if(rw = 'R'){
                    //se pagina for referenciada atualiza r
                    PageTable[indexPagina]->pagina->r = TRUE;
                }
                else{
                    //se pagina for lida atualiza w
                    PageTable[indexPagina]->pagina->w = TRUE;
                }
            }

            // TRATA MEMORIA FISICA
            if(debug == TRUE){
                printf("DEBUG: Verificando existencia de pagina na memoria...\n");
            }

            if(PageTable[indexPagina]->presenca == TRUE){
                //se pagina esta na memoria
                if(debug == TRUE){
                    printf("DEBUG: Pagina ja se encontra na memoria\n");
                }

                pageHits++;
            }
            else{
                //se pagina NAO esta na memoria
                if(debug == TRUE){
                    printf("DEBUG: Pagina ja se encontra na memoria\n");
                }

                EscreveNaMemoria(indexPagina, rw);

                pageFaults++;
            }

        if(debug == TRUE){
            AguardaEnter();
        }

    }



    
    return 0;

}

    // ListaNo *NoCorrente = kbc->primeiro;

    // while(NoCorrente != NULL){
    //     printf("no corrente: %d\n", NoCorrente->valor);
    //     NoCorrente = NoCorrente->proximo;
    // }