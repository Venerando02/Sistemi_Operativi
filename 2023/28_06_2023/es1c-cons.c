/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da una struct contenente un campo intero
e un campo composto da un vettore di 10 elementi float. Normalmente, il processo produttore riempie
casualmente il vettore. Il processo consumatore calcola la media dei valori contenuti nel vettore e la stampa a
video. Si supponga che i due processi terminano quando il processo produttore inserisce il valore 0 nel campo
intero (che normalmente viene messo a 1 dal processo produttore). Si utilizzino i semafori per la gestione della
regione critica.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "semafori.h"

#define CHIAVE_PRODUTTORE (key_t)1234
#define CHIAVE_CONSUMATORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define SIZE 10

typedef struct DatiCondivisiS
{
  float v[SIZE];
  int valore;
  float media;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1, i;
  float somma;
  DatiCondivisiT* P;
  void* ShmP;
  
  printf("processo 'consumatore' con PID = %d.\n", getpid());
  
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
  if(SEM_SET(WriterID, 1) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(ReaderID, 0) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  printf("Semafori settati correttamente.\n");
  
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(DatiCondivisiT), IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile effettuare l'aggancio della memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  P = (DatiCondivisiT*)ShmP;
  
  printf("Memoria condivisa settata correttamente.\n");
  
  while(Running)
  {
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    if(P->valore == 0) Running = 0;
    else
    {
      somma = 0.0;
      printf("-------------\n");
      for(i = 0; i<SIZE; i++)
      {
        printf("v[%d] = %.2f \n", i, P->v[i]);
        somma += P->v[i];
      }
      printf("-------------\n");
      P->media = somma / (float)SIZE;
      printf("La media del vettore e' %.2f.\n", P->media);
    }
    if(SEM_V(WriterID) == -1) exit(EXIT_FAILURE);
  }
  
  if(SEM_DEL(ReaderID) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_DEL(WriterID) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile sganciare la shared memory.\n");
    exit(EXIT_FAILURE); 
  }
  if(shmctl(ShmID, IPC_RMID, 0) == -1)
  {
    fprintf(stderr, "Impossibile eliminare la shared memory.\n");
    exit(EXIT_FAILURE);   
  }
  
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

