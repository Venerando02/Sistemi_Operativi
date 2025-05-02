/*
Scrivere un programma che inizialmente alloca un vettore dinamico di float (di lunghezza passata dai parametri
argc e argv). Il programma riempie il vettore in modo casuale. A questo punto il programma crea un thread, che
inizia a fare la media degli elementi della seconda metà del vettore; il thread principale (il main) esegue nel
frattempo la media degli elementi della prima metà del vettore. Quando i due thread finiscono di calcolare le
due medie, esse devono essere stampate a video.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define LIM 10

float * vettore = NULL;
int dim;
float media_second;

void* ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int i, meta, res;
  pthread_t tid; 
  float media_first, somma = 0;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: dim.\n");
    exit(EXIT_FAILURE);
  }
  dim = atoi(argv[1]);
  if(dim < 0)
  {
    fprintf(stderr, "Inserisci un valore positivo di dim.\n");
    exit(EXIT_FAILURE);  
  }
  
  vettore = (float*)malloc(sizeof(float)*dim);
  for(i = 0; i<dim; i++) vettore[i] = rand() % LIM;
  
  printf("----------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %.2f \n", i, vettore[i]);
  printf("----------------\n");
  
  res = pthread_create(&tid, NULL, ThreadFunction, NULL);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(tid, NULL);
  
  meta = dim/2;
  printf("MAIN: calcolo la media per la prima meta'.\n");
  for(i = 0; i<meta; i++) somma += vettore[i];
  
  media_first = somma / (float)meta;
  
  printf("MAIN: la media per la prima meta' e' %.2f.\n", media_first);
  printf("MAIN: la media per la seconda meta' e' %.2f.\n", media_second);
  
  printf("MAIN: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

void* ThreadFunction(void* arg)
{
  int meta = dim/2;
  int i;
  float somma = 0;
  
  for(i = meta; i<dim; i++) somma += vettore[i];
  media_second = somma / (float)(dim-meta);
  
  printf("THREAD: concludo.\n");
  pthread_exit(NULL);
}
