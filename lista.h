#include "sim.h"
//lista duplamente encadeada com cabeca

typedef struct ListaHead ListaHead;
typedef struct ListaNo ListaNo;

struct ListaHead {
    ListaNo *primeiro;
    ListaNo *ultimo;
    int tamanho;
};

struct ListaNo{
    ListaNo *anterior;
    ListaNo *proximo;

    Pagina *pagina; //pagina na memoria
};

// CRIAR LISTA
// RECEBE PONTEIRO ONDE RETORNARA LISTA
// RETORNA ponteiro com lista

ListaHead *CriarLista ();

// INSERENOFIM
// CRIA UM NO E INSERE NO FIM DA LISTA
// RETORNA 1 EM CASO DE SUCESSO e 0 EM CASO DE FRACASSO

int InsereNoFim (ListaHead *head, Pagina *pagina);

// INSERENOINICIO
// CRIA UM NO E INSERE NO INICIO DA LISTA
// RETORNA 1 EM CASO DE SUCESSO e 0 EM CASO DE FRACASSO

int InsereNoInicio (ListaHead *head, Pagina *pagina);

// ENVIAPARAINICIO
// DESC: ACHA O NO COM O VALOR E O ENVIA PARA O INICIO DA LISTA

int EnviarParaInicio(ListaHead *head, int index);

// REMOVE NO
// DESC: REMOVE UM NO DA LISTA
// RETORNA 1 SE NO FOI REMOVIDO, 0 SE NAO FOI

int RemoveNo(ListaHead *head, ListaNo *NoVitima);

// REMOVE DO FIM
// DESC: REMOVE ULTIMO NO DA LISTA
// RETORNA 1 SE NO FOI REMOVIDO, 0 SE NAO FOI

int RemoveDoFim (ListaHead *head);

// OBTER PAGINA
// DESC: ACHA NO QUE CONTEM PAGINA COM INDEX E RETORNA ESSA PAGINA
// RECEBE LISTA HEAD E INDEX (ID DA PG)
// RETORNA A PAGINA ACHADA, OU
// RETORNA NULL SE NAO ACHAR

Pagina *obterPagina(ListaHead *head, int index);