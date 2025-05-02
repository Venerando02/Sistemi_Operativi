/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da un array di 10 float ed una variabile
intera. Il processo produttore riempie un solo elemento dell’array (scelto a caso) e inserirà il valore dell’indice in
cui ha effettuato l’inserimento, nella variabile intera della memoria condivisa. Il processo consumatore legge il
valore dell’indice e stampa a video il valore dell’elemento del vettore di quell’indice. Si supponga che i due
processi terminano quando il processo produttore imposta a -1 la variabile intera nell’area di memoria condivisa.
Si utilizzino i semafori per la gestione della regione critica.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define SIZE 10
#define LIM_INDICE 11

typedef struct DatiCondivisiS
{
  float v[SIZE];
  int indice;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void* ShmP;
  DatiCondivisiT * P;
  
  printf("Processo 'produttore' con PID = %d.\n", getpid());
  
  WriterID = semget(CHIAVE_PRODUTTORE, 1, IPC_CREAT | 0666);
  if(WriterID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  ReaderID = semget(CHIAVE_CONSUMATORE, 1, IPC_CREAT | 0666);
  if(ReaderID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(DatiCondivisiT), IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la shared memory.\n");
    exit(EXIT_FAILURE);    
  }
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la shared memory.\n");
    exit(EXIT_FAILURE);      
  }
  P = (DatiCondivisiT*)ShmP;
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    P->indice = ((rand() % LIM_INDICE) - 1);
    if(P->indice == -1) Running = 0;
    else
    {
      printf("Inserisci un valore nell'indice %d: ", P->indice);
      scanf("%f", &(P->v[P->indice]));
      while(getchar() != '\n');
    }
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
  }
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile sganciare la memoria condivisa.\n");
    exit(EXIT_FAILURE);    
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

