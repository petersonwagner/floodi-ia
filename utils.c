#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"


nodo* cria_nodo(){
  nodo* no = NULL;

  no = (nodo*)malloc(sizeof(nodo));
  no->distancia = no->percorrido = 0;

  no->vizinhos = (nodo**)malloc(4*sizeof(nodo*));
  memset( no->vizinhos, 0, sizeof( nodo* ) * 4 );
  no->n_vizinhos = no->alocado = 4;

  return( no );
}

void enfileira(fila * f, nodo *no){
  no_fila * n = (no_fila *) malloc (sizeof(no_fila));
  n->prox = NULL;
  n->atual = no;
  if (!f->primeiro){
    f->primeiro=f->ultimo = n;
    return;
  }
  f->ultimo->prox=n;
  f->ultimo=n;
}

nodo * desenfileira(fila *fila){
  if(!fila->primeiro)
    return NULL;
  no_fila * aux = fila->primeiro;
  nodo * no = aux->atual;
  fila->primeiro = fila->primeiro->prox;
  free(aux);
  return no;
}

int mod (int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}


int nao_eh_vizinho(nodo *a, nodo * b){
  for(int i = 0; i < a->n_vizinhos; i++){
    if(a->vizinhos[i] == b)
      return 0;
    }
  return 1;
}

void remove_nodo(nodo *n, nodo *remover){
  nodo * temp;
  for(int i = 0;i < remover->n_vizinhos; i++){
    temp = remover->vizinhos[i];
    if(temp){
      for(int j = 0;j < temp->n_vizinhos; j++){
        if(temp->vizinhos[j]){
          if(remover == temp->vizinhos[j]){
            temp->vizinhos[j] = nao_eh_vizinho(temp,n) ? n : NULL;
          }
        }
      }
    }
  }
  enfileira(&des, remover);
  remover->alocado = 0;
}


struct nodo ** insere_vizinho(struct nodo ** vizinhos, nodo * novo, int * n_vizinhos){
  for (int i = 0; i < *n_vizinhos; i++){
    if(novo == vizinhos[i]){
      return vizinhos;
    }
  }

  nodo ** temp = vizinhos;
  *n_vizinhos = *n_vizinhos + 1;
  vizinhos = (nodo**) malloc(*n_vizinhos*sizeof(nodo*));
  for(int i = 0; i < *n_vizinhos-1; i++)
    vizinhos[i] = temp[i];
  free(temp);
  vizinhos[*n_vizinhos-1] = novo;
  return vizinhos;
}


int calcula_profundidade(fila * f, int d){
  if (!f->primeiro)
    return d;

  nodo * n = desenfileira(f);
  if (!n->alocado || n->percorrido>contador)
    return d;

  nodo_mais_distante_aux = n;
  int i = -1, nVizinhos = n->n_vizinhos;
  n->percorrido = contador + 1;
  while(++i < nVizinhos){
    if(n->vizinhos[i] != NULL){
      if(n->vizinhos[i]->percorrido < contador){
        n->vizinhos[i]->distancia = n->distancia + 1;
        enfileira(f,n->vizinhos[i]);
      }
      n->vizinhos[i]->percorrido = contador;
    }
  }
  calcula_profundidade(f, n->distancia);
  return 0;
}


void acha_distancia(nodo * inicial){
  fila * f = (fila*) malloc(sizeof(fila));
  f->primeiro = f->ultimo = NULL;
  enfileira(f,inicial);
  calcula_profundidade(f, 0);
  free(f);
}

