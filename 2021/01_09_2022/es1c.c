/*
Scrivere un programma che inizialmente alloca un vettore dinamico di float (di lunghezza passata tramite i
parametri argc e argv[] del main). A questo punto il programma crea un thread, che si occupa di riempire in
modo casuale il vettore dinamico, limitatamente alla seconda metà del vettore. Il thread principale (il main) fa
la stessa operazione ma si limita alla prima metà del vettore, ossi riempie in modo casuale gli elementi relativi
alla prima metà del vettore. Quando i due threads finiscono di riempire i due sottovettori, si proceda (nel main)
a stampare a video tutti gli elementi del vettore dinamico.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>

#define LIM 10

float * v = NULL;
int dim;

void* ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int i, meta, res;
  pthread_t tid;
  
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
  
  printf("MAIN: hai inserito la dimensione %d.\n", dim);
  printf("MAIN: alloco il vettore.\n");
  v = (float*)malloc(sizeof(float)*dim);
  
  printf("MAIN: creo il thread.\n");
  res = pthread_create(&tid, NULL, ThreadFunction, NULL);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("MAIN: mi sincronizzo con la terminazione del thread.\n");
  pthread_join(tid, NULL);
  
  meta = dim/2;
  printf("MAIN: riempio la prima meta.\n");
  for(i = 0; i<meta; i++) v[i] = rand() % LIM;
  
  printf("MAIN: visualizzo dunque l'intero array.\n");
  printf("--------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %.2f \n", i, v[i]);
  printf("--------------\n");
  
  printf("MAIN: posso terminare.\n");
  exit(EXIT_SUCCESS);
}


void* ThreadFunction(void* arg)
{
  int meta = dim/2;
  int i;
  
  for(i = meta; i<dim; i++) v[i] = rand() % LIM;
  
  printf("THREAD: posso concludere.\n");
  pthread_exit(NULL);
}
