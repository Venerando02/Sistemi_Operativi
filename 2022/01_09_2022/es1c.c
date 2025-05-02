/*
Scrivere un programma che alloca un vettore di interi di dimensione passata come primo parametro del main()
argc e argv. Dopo aver riempito il vettore di numeri casuali compresi tra 0 e un valore massimo, passato come
secondo parametro del main(), il processo crea un thread che calcola la media degli elementi del vettore compresi
tra il primo elemento e la metà del vettore. Nel frattempo il processo calcola la media della seconda metà del
vettore. Alla conclusione del thread, il processo stampa i due valori così ottenuti.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

int * vettore = NULL;
int dim;
float media_primo;

void * ThreadFunction(void * arg);

int main(int argc, char * argv[])
{
  int lim, i, res, meta, somma_sec = 0;
  pthread_t tid;
  float media_sec;
  
  if(argc != 3)
  {
    fprintf(stderr, "Parametri mancanti: dim, lim.\n");
    exit(EXIT_FAILURE);
  }
  
  dim = atoi(argv[1]);
  lim = atoi(argv[2]);
  
  if((dim <= 0) || (lim <= 0))
  {
    fprintf(stderr, "Inserisci valori di lim e dim positivi.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("MAIN: allochiamo un vettore di dim = %d.\n", dim);
  vettore = (int*)malloc(sizeof(int)*dim);
  
  for(i = 0; i<dim; i++) vettore[i] = rand() % lim;
  
  printf("--------------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %d.\n", i, vettore[i]);
  printf("--------------------\n");
  
  meta = dim/2;
  res = pthread_create(&tid, NULL, ThreadFunction, (void*)&meta);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  printf("MAIN: mi sincronizzo con la terminazione del thread.\n");
  pthread_join(tid, NULL);
  
  printf("MAIN: calcolo la media della seconda meta'.\n");
  for(i = meta; i < dim; i++)
  {
    somma_sec += vettore[i];
  }
  media_sec = (float)somma_sec / (float)(dim-meta);
  
  printf("MAIN: nella prima meta' la media e' %.2f.\n", media_primo); 
  printf("MAIN: nella seconda meta' la media e' %.2f.\n", media_sec);
  
  printf("MAIN: dunque concludo.\n");
  exit(EXIT_SUCCESS);
}

void * ThreadFunction(void * arg)
{
  int meta = *(int*)arg;
  int i;
  int somma = 0;
  
  for(i = 0; i < meta; i++)
  {
    somma += vettore[i];
  }
  media_primo = (float)somma / (float)meta;
  printf("THREAD: posso concludere.\n");
  pthread_exit(NULL);
}

