#include <stdio.h>
#include <stdlib.h>
#include "lista.h"



// CRIAR LISTA
// RECEBE PONTEIRO ONDE RETORNARA LISTA
// RETORNA 1 SE LISTA FOI CRIADA
// RETORNA ) EM CASO DE ERRO QUALQUER

ListaHead* CriarLista (){

    ListaHead *head = (ListaHead *) malloc(sizeof(ListaHead));

    if(head == NULL){
        printf("LISTA ERRO CRIACAO LISTA");
        exit(-1);
    }

    head->primeiro = NULL;
    head->ultimo = NULL;
    head->tamanho = 0;

    return head;
    
}

// INSERENOFIM
// CRIA UM NO E INSERE NO FIM DA LISTA
// RETORNA 1 EM CASO DE SUCESSO e 0 EM CASO DE FRACASSO

int InsereNoFim (ListaHead *head, Pagina *pagina) {

    if(head == NULL){
        return 0;
        printf("LISTA ERRO INSERE LISTA: head nao existe");
        exit(-1);
    }

    //cria no
    ListaNo *No = (ListaNo *) malloc(sizeof(ListaNo));
    No->anterior = NULL;
    No->proximo = NULL;
    No->pagina = pagina;
        

    if(head->primeiro == NULL){
        //Se lista esta vazia

        //atualiza head
        head->primeiro = No;
        head->ultimo = No;
    }
    else{
        //se lista tem pelo menos 1 elemento

        //No criado aponta pro anterior
        No->anterior = head->ultimo;

        //no anterior aponta pro criado (atualiza o no que ja existia)
        No->anterior->proximo = No;

        //atualiza head
        head->ultimo = No;
    }

    //incrementa tamanho
    head->tamanho ++;
    return 1;
}

// INSERENOINICIO
// CRIA UM NO E INSERE NO INICIO DA LISTA
// RETORNA 1 EM CASO DE SUCESSO e 0 EM CASO DE FRACASSO

int InsereNoInicio (ListaHead *head, Pagina *pagina) {

    if(head == NULL){
        return 0;
        printf("LISTA ERRO INSERE LISTA: head nao existe");
        exit(-1);
    }

    //cria no
    ListaNo *No = (ListaNo *) malloc(sizeof(ListaNo));
    No->anterior = NULL;
    No->proximo = NULL;
    No->pagina = pagina;
        

    if(head->primeiro == NULL){
        //Se lista esta vazia

        //atualiza head
        head->primeiro = No;
        head->ultimo = No;
    }
    else{
        //se lista tem pelo menos 1 elemento

        //seta no criado
        No->proximo = head->primeiro;

        //atualiza no que ficou em segundo
        head->primeiro->anterior = No;

        //atualiza head
        head->primeiro = No;
    }

    //incrementa tamanho
    head->tamanho ++;
    return 1;
}

// ENVIAPARAINICIO
// DESC: ACHA O NO COM O VALOR E O ENVIA PARA O INICIO DA LISTA

int EnviarParaInicio(ListaHead *head, int index){

    if(head->tamanho == 0 || head->tamanho == 1){
        printf("LISTA ERRO ENVIAR PARA O INICIO: Lista esta vazia ou so tem 1 elemento");
        return 0;
    }

    ListaNo *NoCorrente = head->primeiro;

    while(NoCorrente != NULL){
        if(NoCorrente->pagina->indice == index){

            InsereNoInicio(head, NoCorrente->pagina);
            RemoveNo(head, NoCorrente);

            return 1;
        }

        NoCorrente = NoCorrente->proximo;
    }

    printf("LISTA ERRO ENVIAR PARA INICIO: No com valor especificado nao foi encontrado na lista\n");
    return 0;
}

// REMOVE NO
// DESC: REMOVE UM NO DA LISTA
// RETORNA 1 SE NO FOI REMOVIDO, 0 SE NAO FOI

int RemoveNo(ListaHead *head, ListaNo *NoVitima){
    
    if(head->tamanho == 0){
        printf("LISTA ERRO REMOVENO: impossivel remover de lista vazia");
        return 0;
    }
    if(head->tamanho == 1){
        free(head->primeiro);
        head->primeiro = NULL;
        head->ultimo = NULL;

        head->tamanho--;
        return 1;    
    }
    
    if(NoVitima->anterior == NULL){
        // se vitima eh o primeiro

        head->primeiro = NoVitima->proximo;
        NoVitima->proximo->anterior = NULL;
        free(NoVitima);
        
        head->tamanho--;
        return 1;
    }
    else if(NoVitima->proximo == NULL){
        // se vitima eh o ultimo

        head->ultimo->anterior->proximo = NULL;
        head->ultimo = head->ultimo->anterior;

        free(NoVitima);

        head->tamanho--;
        return 1;
    }
    else{
        //se vitima estiver no meio da lista

        NoVitima->anterior->proximo = NoVitima->proximo;
        NoVitima->proximo->anterior = NoVitima->anterior;
        free(NoVitima);

        head->tamanho--;
        return 1;

    }
}

// REMOVE DO FIM
// DESC: REMOVE ULTIMO NO DA LISTA
// RETORNA 1 SE NO FOI REMOVIDO, 0 SE NAO FOI


// TEM ALGO ESTRANHO AQUI DENTRO, DAVA ERRO E AGORA NAO DA MAIS

int RemoveDoFim (ListaHead *head){

    ListaNo *NoAux;

    if(head->tamanho == 0){
        //se lista esta vazia
        printf("LISTA ERRO REMOVE NO FIM: Lista esta vazia\n");
        return 0;
    }

    if(head->tamanho == 1){
        //se lista so tem um elemento

        free(head->primeiro);
        head->primeiro = NULL;
        head->ultimo = NULL;

        head->tamanho--;
        return 1;    
    }

    NoAux = head->ultimo;

    //acerta head
    head->ultimo->anterior->proximo = NULL;
    head->ultimo = head->ultimo->anterior;

    //deleta no
    free(NoAux);

    head->tamanho--;
    return 1;

}

Pagina *obterPagina(ListaHead *head, int index){
    
    ListaNo *NoCorrente = head->primeiro;

    while(NoCorrente != NULL){

        if(NoCorrente->pagina->indice == index){
            return NoCorrente->pagina;
        }

        NoCorrente = NoCorrente->proximo;
    }

    printf("Pagina nao encontrada na lista\n");
    return NULL;
}