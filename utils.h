#ifndef __UTILS__
#define __UTILS__


typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int **mapa;
} tmapa;

typedef struct nodo {
  struct nodo ** vizinhos;
  int cor;
  int percorrido;
  int n_vizinhos;
  int distancia;
  int alocado;
} nodo;

typedef struct no_fila {
  struct no_fila * prox;
  struct nodo * atual;
} no_fila;

typedef struct fila {
  struct no_fila * primeiro;
  struct no_fila * ultimo;
} fila;

typedef struct {
  struct nodo * topo;
} grafo;

extern int movimentos;
extern int contador;
extern fila des;
extern nodo *  nodo_mais_distante_aux;


nodo* cria_nodo();

void enfileira(fila * f, nodo *no);

nodo * desenfileira(fila *fila);

int mod (int a, int b);

int nao_eh_vizinho(nodo *a, nodo * b);         // Verifica se o nodo b é vizinho de a

void remove_nodo(nodo *n, nodo *remover);        // Remove o no, desaloca-o e some com os apontadores dos vizinhos para aquele no, apontando agora pro no master

int calcula_profundidade(fila * f, int d);

void acha_distancia(nodo * inicial);

struct nodo ** insere_vizinho(struct nodo ** vizinhos, nodo * novo, int * n_vizinhos);


#endif 