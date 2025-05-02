/*
Scrivere un programma che inizialmente alloca un vettore dinamico di float (di lunghezza passata tramite i
parametri argc e argv[] del main) e lo riempie in modo casuale. A questo punto il programma crea un thread,
che si occupa di calcolare il massimo degli elementi del vettore posti nella seconda metà del vettore. Il thread
principale (il main) fa la stessa operazione ma si limita alla prima metà del vettore, ossia calcola il massimo tra
i valori relativi alla prima metà del vettore. Quando i due threads finiscono di effettuare questi calcoli, si
proceda (nel main) a stampare a video i due valori massimi così ottenuti.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define LIM 10

float* vettore = NULL;
int dim;
float max_sec;

void* ThreadFunction(void * arg);

int main(int argc, char* argv[])
{
  int res, i, meta;
  pthread_t tid;
  float max_first;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: dim.\n");
    exit(EXIT_FAILURE);
  }
  dim = atoi(argv[1]);
  if(dim < 0)
  {
    fprintf(stderr, "Inserisci una dimensione positiva.\n");
    exit(EXIT_FAILURE);  
  }
  
  vettore = (float*)malloc(sizeof(float)*dim);
  for(i = 0; i<dim; i++) vettore[i] = rand() % LIM;
  
  printf("------------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %.2f \n", i, vettore[i]);
  printf("------------------\n");
  
  res = pthread_create(&tid, NULL, ThreadFunction, NULL);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("MAIN: mi sincronizzo con la terminazione del thread.\n");
  pthread_join(tid, NULL);
  
  meta = dim/2;
  max_first = vettore[0];
  
  for(i = 0; i<meta; i++) if(vettore[i] > max_first) max_first = vettore[i];
  
  printf("MAIN: il massimo della prima meta' e' %.2f.\n", max_first);
  printf("MAIN: il massimo della seconda meta' e' %.2f.\n", max_sec);
  
  printf("MAIN: concludo.\n");
  exit(EXIT_SUCCESS);
}


void* ThreadFunction(void * arg)
{
  int m = dim/2;
  max_sec = vettore[m];
  int i;
  
  for(i = m; i<dim; i++) if(vettore[i] > max_sec) max_sec = vettore[i];
  
  printf("THREAD: operazione conclusa.\n");
  pthread_exit(NULL);
}

