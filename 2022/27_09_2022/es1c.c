/*
Scrivere un programma che crea un certo numero (n) di thread, passato come argomento del main().
Al thread i-esimo (con i che va da 0 a n-1) viene passato un intero pari ad ((i+1)*2).
Ogni thread va in sleep per un tempo pari al valore intero ricevuto e poi termina.
Risolvere tale esercizio tramite la tecnica di threading SINCRONO.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct ThreadS
{
  pthread_t tid;
  int parametro;
} ThreadT;

void* ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  ThreadT* v;
  int nThread, i, res;
    
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
  printf("MAIN: sono il processo con PID = %d.\n", getpid()); 
  
  for(i = 0; i<nThread; i++)
  {
    v[i].parametro = ((i+1)*2);
    res = pthread_create(&v[i].tid, NULL, ThreadFunction, (void*)&v[i].parametro);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    } 
  }
  
  printf("MAIN: mi sincronizzo con la terminazione dei thread.\n");
  
  for(i = 0; i<nThread; i++)
  {
    pthread_join(v[i].tid, NULL);
  }
  
  printf("MAIN: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}


void* ThreadFunction(void* arg)
{
  int num = *(int*)arg;
  printf("THREAD: vado in sleep per %d secondi.\n", num);
  sleep((unsigned int)num);
  pthread_exit(NULL);
}
