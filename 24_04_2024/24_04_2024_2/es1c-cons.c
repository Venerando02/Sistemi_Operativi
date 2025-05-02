/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da una stringa (in particolare, un array di
20 caratteri). Il processo produttore riempie la stringa mediante inserimento da tastiera. Il processo consumatore
calcola la lunghezza della stringa e la stampa a video. Si supponga che i due processi terminano quando il
processo produttore inserisce la stringa “End” oppure la stringa “Fine” nell’area di memoria condivisa. Si
utilizzino i semafori per la gestione della regione critica.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define SSIZE 20

typedef struct DatiCondivisiS
{
  char stringa[SSIZE];
  int lunghezza;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void* ShmP;
  DatiCondivisiT * P;
  
  printf("Sono il processo 'consumatore' con PID = %d.\n", getpid());
  
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
  if(SEM_SET(ReaderID, 0) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);  
  }
  if(SEM_SET(WriterID, 1) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
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
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    if((strncmp(P->stringa, "Fine", 4) == 0) || (strncmp(P->stringa, "End", 3) == 0)) Running = 0;
    else
    {
      P->lunghezza = strlen(P->stringa);
      printf("La lunghezza della stringa e' di %d.\n", P->lunghezza);
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
