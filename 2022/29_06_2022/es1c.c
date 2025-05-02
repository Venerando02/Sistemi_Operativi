/*
Scrivere un programma che crea un certo numero di thread. Il numero di thread da creare viene passato come
argomento del main(). Si supponga di passare un numero casuale a ciascun thread. Ciascun thread deve
visualizzare tale numero e terminare. Risolvere tale esercizio tramite la tecnica di threading ASINCRONO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define LIM 10

typedef struct ThreadS
{
  pthread_t tid;
  int NumCasuale;
} ThreadT;

void * ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int nThread, i, res;
  ThreadT * v = NULL;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: nThread.\n");
    exit(EXIT_FAILURE);
  }
  
  nThread = atoi(argv[1]);
  
  if(nThread < 0)
  {
    fprintf(stderr, "Inserisci un valore positivo.\n");
    exit(EXIT_FAILURE);  
  }
  
  v = (ThreadT*)malloc(sizeof(ThreadT)*nThread);
  
  printf("MAIN: vado a creare %d thread.\n", nThread);
  
  for(i = 0; i<nThread; i++)
  {
    v[i].NumCasuale = rand() % LIM;
    res = pthread_create(&v[i].tid, NULL, ThreadFunction, (void*)&v[i].NumCasuale);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
    pthread_detach(v[i].tid);
  }
  sleep(2);
  printf("MAIN: posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

void * ThreadFunction(void* arg)
{
  int numero = *(int*)arg;
  printf("THREAD: ho visualizzato il numero %d.\n", numero);
  pthread_exit(NULL);
}
