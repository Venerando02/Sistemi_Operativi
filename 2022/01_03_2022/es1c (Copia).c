/*
Scrivere un programma che inizialmente alloca un vettore dinamico di interi (di lunghezza passata tramite i
parametri argc e argv[] del main). A questo punto il programma (main thread) si occupa di riempire in modo
casuale il vettore dinamico. Una volta riempito il vettore, il main lancia tre threads ciascuno dei quali eseguono
la somma dei valori contenuti nelle 3 porzioni di vettore. Si supponga di dividere idealmente il vettore in 3 parti
uguali; ciascun thread si occuperà di una di queste 3 porzioni. Immaginando una gestione sincrona, quando i 3
threads finiscono, il main thread calcola la somma dei 3 valori ottenuti ed effettua la stampa a video e termina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define NTHREADS 3
#define LIM 10

int * vettore = NULL;
int dim;
int min, passo, somma = 0;

typedef struct ThreadS
{
  pthread_t tid;
  int param;
} ThreadT;

void* ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int i, res, sommaTot = 0;
  ThreadT v[NTHREADS];
  
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
  vettore = (int*)malloc(sizeof(int)*dim);
  
  printf("MAIN: riempio il vettore.\n");
  
  for(i = 0; i<dim; i++) vettore[i] = rand() % LIM;
  
  printf("---------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %d \n", i, vettore[i]);
  printf("---------\n");
  
  passo = dim/NTHREADS;
  for(i = 0; i<NTHREADS; i++)
  {
    v[i].param = i;
    res = pthread_create(&v[i].tid, NULL, ThreadFunction, (void*)&v[i].param);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
  }
  
  printf("MAIN: mi sincronizzo con la terminazione dei 3 thread.\n");
  for(i = 0; i < NTHREADS; i++)
  {
    pthread_join(v[i].tid, NULL);
  }
  
  printf("MAIN: la somma totale del vettore e' %d.\n", somma);
  
  printf("MAIN: posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

void* ThreadFunction(void* arg)
{
  int parametro = *(int*)arg;
  min = passo * parametro;
  int max;
  if(parametro == (NTHREADS - 1)) max = dim;
  else max = min + passo;
  int i;
  
  for(i = min; i < max; i++)
  {
    somma += vettore[i];
  }
  
  printf("THREAD: operazione conclusa.\n");
  pthread_exit(NULL);
}
