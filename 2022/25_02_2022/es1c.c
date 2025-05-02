/*
Scrivere un programma che crea un certo numero (n) di thread, passato come argomento del main().
Al thread i-esimo (con i che va da 0 a n-1) viene passato un intero <arg> pare ad i+2.
Ogni thread stampa la tabellina del numero passato come argomento. Dopo ogni stampa va in sleep per 1 secondo.
Una volta stampata la tabellina del numero passato come argomento, il thread termina.
Risolvere tale esercizio tramite la tecnica di threading SINCRONO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

typedef struct ThreadS
{
  pthread_t tid;
  int numero;
} ThreadT;

void* ThreadFunction(void* clientfd);

int main(int argc, char * argv[])
{
  ThreadT * v;
  int i, res, nThread;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: nChild.\n");
    exit(EXIT_FAILURE);
  }
  nThread = atoi(argv[1]);
  if(nThread < 0)
  {
    fprintf(stderr, "Inserisci un valore positivo.\n");
    exit(EXIT_FAILURE);  
  }
  v = (ThreadT*)malloc(sizeof(ThreadT)*nThread);
  
  printf("MAIN: sono il processo con PID = %d.\n", getpid());
  printf("MAIN: creo %d thread secondari.\n", nThread);
  
  for(i = 0; i<nThread; i++)
  {
    v[i].numero = i+2;
    res = pthread_create(&v[i].tid, NULL, ThreadFunction, (void*)&v[i].numero);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
  }
  
  printf("MAIN: mi sincronizzo con la terminazione di ciascun thread.\n");
  for(i = 0; i<nThread; i++)
  {
    pthread_join(v[i].tid, NULL);
  }
  printf("MAIN: dunque posso terminare.\n");
  exit(EXIT_SUCCESS);
}


void* ThreadFunction(void* clientfd)
{
  int num = *(int*)clientfd;
  int i;
  
  printf("THREAD: visualizzo la tabellina del %d.\n", num);
  for(i = 0; i<=10; i++)
  {
    printf("%d * %d = %d \n", i, num, i*num);
    sleep(1);
  }
  printf("----------------------\n");
  pthread_exit(NULL);
}
