/*
Scrivere un programma che inizialmente alloca un vettore dinamico di float (di lunghezza passata tramite i
parametri argc e argv del main). Il programma riempie il vettore in modo casuale. Il programma chiede
all’utente l’inserimento di un float. A questo punto il programma crea un thread, che inizia a cercare il valore
float inserito, nella seconda metà del vettore; il thread conta quante volte trova il valore nella seconda metà del
vettore. Il thread principale (il main) fa la stessa operazione ma si limita alla prima metà del vettore, ossi
ricerca quante volte l’elemento float inserito si trova nella prima metà del vettore. Quando i due threads
finiscono di calcolare i due contatori (numeri di ricorrenze nelle due metà del vettore), i valori di tali contatori
devono essere stampati a video.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define LIM 10

float * vettore;
int dim, occ_sec = 0;

void * ThreadFunction(void* arg);

int main(int argc, char* argv[])
{
  int i, occ_first = 0, res, meta;
  pthread_t tid;
  float valore;
  
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
  
  printf("-----------------\n");
  for(i = 0; i<dim; i++) printf("v[%d] = %.2f \n", i, vettore[i]);
  printf("-----------------\n");
  
  printf("MAIN: inserisci un valore: ");
  scanf("%f", &valore);
  while(getchar() != '\n');
    
  res = pthread_create(&tid, NULL, ThreadFunction, (void*)&valore);
  if(res != 0)
  {
    fprintf(stderr, "Impossibile creare il thread.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("MAIN: mi sincronizzo con la terminazione del thread.\n");
  pthread_join(tid, NULL);
  
  meta = dim/2;
  
  for(i = 0; i<meta; i++) if(vettore[i] == valore) occ_first++;
  
  printf("MAIN: il numero di occorrenze della prima meta' e' %d.\n", occ_first);
  printf("MAIN: il numero di occorrenze della seconda meta' e' %d.\n", occ_sec);
  
  printf("MAIN: posso concludere.\n");
  exit(EXIT_SUCCESS);
}


void * ThreadFunction(void* arg)
{
  float value = *(float*)arg;
  int meta = dim/2;
  int i;
  
  for(i = meta; i<dim; i++) if(vettore[i] == value) occ_sec++;
 
  printf("THREAD: posso dunque terminare.\n");
  pthread_exit(NULL);
}
