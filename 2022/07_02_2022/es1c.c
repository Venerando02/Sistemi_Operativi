/*
Scrivere un programma che inizialmente alloca un vettore dinamico di unsigned int (di lunghezza passata
tramite i parametri del main argc e argv) e lo riempie con numeri casuali compresi tra 0 e 100. A questo punto
il programma chiede all’utente l’inserimento di un numero intero compreso tra 0 e 100. Viene creato un thread
che conta il numero di occorrenze di tale numero nella prima metà del vettore; il thread principale (il main),
invece, conta il numero di occorrenze di tale numero nella seconda metà del vettore. Quando i due thread
concludono l’operazione di conteggio delle occorrenze, i due valori calcolati devono essere stampati a video.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define LIM 101

unsigned int * vettore = NULL;
int dim, num_occorrenze_first;

void * ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int i, num_occorrenze_sec = 0, res, meta;
  unsigned int number;
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
  
  vettore = (unsigned int*)malloc(sizeof(unsigned int)*dim);
  
  printf("MAIN: riempio il vettore.\n");
  
  for(i = 0; i<dim; i++) vettore[i] = rand() % LIM;
  
  printf("------------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %u.\n", i, vettore[i]);
  printf("------------------\n");
  
  printf("Inserisci un numero su cui contare le occorrenze: ");
  scanf("%u", &number);
  while(getchar() != '\n');
  
  res = pthread_create(&tid, NULL, ThreadFunction, (void*)&number);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(tid, NULL);
  
  meta = dim/2;
  printf("Calcolo le occorrenze della seconda meta'.\n");
  for(i = meta; i<dim; i++)
  {
    if(vettore[i] == number) num_occorrenze_sec++;
  }
  
  printf("Il numero di occorrenze della prima meta' e' %d.\n", num_occorrenze_first);
  printf("Il numero di occorrenze della seconda meta' e' %d.\n", num_occorrenze_sec);
  
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}


void * ThreadFunction(void* arg)
{
  unsigned int num = *(unsigned int*)arg;
  int meta = dim/2;
  int i;
  num_occorrenze_first = 0;
  
  printf("THREAD: calcolo le occorrenze della prima meta'.\n");
  
  for(i = 0; i<meta; i++)
  {
    if(vettore[i] == num) num_occorrenze_first++;
  }
  
  pthread_exit(NULL);
}
