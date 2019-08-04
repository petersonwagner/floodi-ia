#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"

int movimentos = 0;
int contador = 1;
fila des;
nodo *  nodo_mais_distante_aux;

//le a entrada e joga numa matriz para depois transformar num grafo
nodo *gera_nodos(tmapa *m, int i, int j){
  int l_cima, l_baixo, c_esq, c_dir;
  nodo *no = NULL;
  nodo* matriz_nodos[m->nlinhas][m->ncolunas]; //matriz auxiliar

  //inicializa uma matriz de nodos
  //só pra termos os endereços de todos os nodos
  for (int l = 0; l < m->nlinhas; ++l)
  {
    for (int c = 0; c < m->ncolunas; ++c)
    {
      no = cria_nodo();
      no->cor = m->mapa[l][c];

      matriz_nodos[l][c] = no;
    }
  }

  //atribui os 4 vizinhos para todos os nodos
  for (int l = 0; l < m->nlinhas; ++l)
  {
    for (int c = 0; c < m->ncolunas; ++c)
    {
      no = matriz_nodos[l][c];

      l_cima  = mod(l -1, m->nlinhas);
      l_baixo = mod(l + 1, m->nlinhas);
      c_dir   = mod(c + 1, m->ncolunas);
      c_esq   = mod(c - 1, m->ncolunas);

      no->vizinhos[0] = matriz_nodos[l_cima][c];
      no->vizinhos[1] = matriz_nodos[l_baixo][c];
      no->vizinhos[2] = matriz_nodos[l][c_esq];
      no->vizinhos[3] = matriz_nodos[l][c_dir];
    }
  }

  return matriz_nodos[i][j];
}

nodo * unifica_nodo(nodo * inicial, nodo * n){
  if (!n)
    return NULL;
  if (!n->alocado || n->percorrido>=contador)
    return NULL;

  n->percorrido = contador;
  int tam = n->n_vizinhos;

  for(int i=0 ; i < tam ; i++){
    if(n->vizinhos[i] != NULL){
      if(n->vizinhos[i]->cor == inicial->cor){  
        unifica_nodo(inicial,n->vizinhos[i]);
        n->vizinhos[i] = NULL;
      }
      //se tiver cor diferente
      else{
        if(inicial != n){
          //adiciona o nodo como vizinho
          inicial->vizinhos = insere_vizinho(inicial->vizinhos, n->vizinhos[i], &inicial->n_vizinhos);
        }
      }
    }
  }
  if((inicial->cor == n->cor) && (inicial != n)){
    remove_nodo (inicial, n);
    return NULL;
  }
  n->percorrido = contador + 1;
  return n;
}

//une todos os nós adjacentes com a mesma cor
void agrupa_nodos(nodo * no){
  if (!no)
    return;
  if((!no->alocado) || (no->percorrido == contador + 1))
    return;

  no = unifica_nodo(no, no);
  int i = -1;

  if(no != NULL){
    while (++i<no->n_vizinhos){
      if(no->vizinhos[i]){
        agrupa_nodos(no->vizinhos[i]);
      }
    }
  }
}

void acha_sequencia(nodo *n, int *sequencia_cores, int fator){
  int d = n->distancia;
  int menor = 10000000;
  int j=-1,k;

  if (fator < 0)
      k=0;
  else
    k = d/fator;

  while(d > k){
    for(int i = 0;i < n->n_vizinhos; i++){
      if(n->vizinhos[i]){
        if(n->vizinhos[i]->distancia < menor){
          j = i;
          menor = n->vizinhos[i]->distancia;
        }
      }
    }

    n->cor = n->vizinhos[j]->cor;
    sequencia_cores[++movimentos] = n->cor;
    contador = contador + 2;
    agrupa_nodos(n);
    n->distancia = menor;
    d = menor;
    menor = 10000000;
  }
}

int acha_tamanho_regiao(tmapa *m, int l, int c){
  int tam=1, cor=m->mapa[l][c];
  int rl, rc;
  m->mapa[l][c] = -1;

  rl = mod(l+1, m->nlinhas);
  rc = c;
  if(m->mapa[rl][rc] == cor)
    tam+=acha_tamanho_regiao(m, rl, rc);

  rl = mod(l -1, m->nlinhas);
  rc = c;
  if(m->mapa[rl][rc] == cor)
    tam+=acha_tamanho_regiao(m, rl, rc);

  rl = l;
  rc = mod(c + 1, m->ncolunas);
  if(m->mapa[rl][rc] == cor)
  tam+=acha_tamanho_regiao(m, rl, rc);

  rl = l;
  rc = mod(c - 1, m->ncolunas);
  if(m->mapa[rl][rc] == cor)
    tam+=acha_tamanho_regiao(m, rl, rc);

  return tam;
}

void acha_inicio(tmapa *m, int *coord_ini){
  int maior=0, tamanho_regiao;
  for(int l=0;l<m->nlinhas;l++)
    for(int c=0;c<m->ncolunas;c++)
      if( m->mapa[l][c] != -1 ){
        tamanho_regiao=acha_tamanho_regiao(m, l, c);

        if ( tamanho_regiao > maior ){
          coord_ini[0] = l;
          coord_ini[1] = c;
          maior = tamanho_regiao;
        }
      }
}

int main(int argc, char **argv) {
  tmapa m;
  nodo *nodo_mais_distante = NULL;
  grafo *G;
  int *sequencia_cores;
  int coord_ini[2];


  sequencia_cores = (int*) malloc(1000*sizeof(int));

  scanf("%d", &(m.nlinhas));
  scanf("%d", &(m.ncolunas));
  scanf("%d", &(m.ncores));

  m.mapa = (int**) malloc(m.nlinhas * sizeof(int*));
  for(int x = 0; x < m.nlinhas; x++){
    m.mapa[x] = (int*) malloc(m.ncolunas * sizeof(int));
    for(int y = 0; y < m.ncolunas; y++)
      scanf("%d", &(m.mapa[x][y]));
  }

  G = (grafo *)malloc(sizeof(grafo));
  G->topo = gera_nodos(&m,0,0);

  acha_inicio(&m, coord_ini);

  for(int i=0; i<coord_ini[0]; i++)
    G->topo = G->topo->vizinhos[1];
  for(int i=0; i<coord_ini[1]; i++)
    G->topo = G->topo->vizinhos[3];

  contador = 1;
  agrupa_nodos(G->topo);
  contador = contador + 2;

  while(nodo_mais_distante != G->topo){
    contador = contador + 2;
    acha_distancia(G->topo);
    contador = contador + 2;
    nodo_mais_distante_aux->distancia = 0;
    nodo_mais_distante = nodo_mais_distante_aux;
    acha_distancia(nodo_mais_distante_aux);
    contador = contador + 2;
    acha_sequencia(G->topo, sequencia_cores, 2);
  }

  sequencia_cores[0] = movimentos;
  movimentos = 0;

  nodo * no = desenfileira(&des);

  if(no){
    free(no->vizinhos);
    free(no);
  }

  free(G->topo->vizinhos);
  free(G->topo);
  free(G);
  contador = contador + 2;


  printf("%d %d %d\n", coord_ini[0], coord_ini[1], sequencia_cores[0]);
  for(int i = 1; i <= sequencia_cores[0]; i++)
    printf("%d ",sequencia_cores[i]);

  printf("\n");
  return 0;
}
