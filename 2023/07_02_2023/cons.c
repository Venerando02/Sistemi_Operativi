/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa (si supponga che tale zona di memoria sia costituita da una stringa). Si supponga che
normalmente il processo produttore riempia la stringa. Il processo consumatore legge la stringa, calcola la
lunghezza e la stampa a video. Quando il processo produttore desidera terminare deve inserire il valore “Fine”
nella stringa. Se il processo consumatore riceve tale valore, allora esso termina. Si utilizzino dei semafori per la
gestione della regione critica.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "semafori.h"

#define STR_LEN 256
#define CHIAVE_PRODUTTORE (key_t)1234
#define CHIAVE_CONSUMATORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012

typedef struct DatiCondivisiS
{
  char stringa[STR_LEN];
  int lunghezza;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void* ShmP;
  DatiCondivisiT* P;
  
  printf("Processo 'consumatore' PID = %d.\n", getpid());
  
  WriterID = semget(CHIAVE_CONSUMATORE, 1, IPC_CREAT | 0666);
  if(WriterID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  ReaderID = semget(CHIAVE_PRODUTTORE, 1, IPC_CREAT | 0666);
  if(ReaderID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(ReaderID, 0) == -1) exit(EXIT_FAILURE);
  if(SEM_SET(WriterID, 1) == -1) exit(EXIT_FAILURE);
  
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(DatiCondivisiT), IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile eseguire l'aggancio alla memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  P = (DatiCondivisiT*)ShmP;
  
  while(Running)
  {
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    if(strncmp(P->stringa, "End", 3) == 0) Running = 0;
    else
    {
      P->lunghezza = strlen(P->stringa);
      printf("La lunghezza di %s e' %d.\n", P->stringa, P->lunghezza);
    }
    if(SEM_V(WriterID) == -1) exit(EXIT_FAILURE);
  }
  
  if(SEM_DEL(WriterID) == -1) 
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  
  if(SEM_DEL(ReaderID) == -1) 
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
 
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile sganciare la shared memory.\n");
    exit(EXIT_FAILURE); 
  }
  
  if(shmctl(ShmID, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Impossibile eliminare la shared memory.\n");
    exit(EXIT_FAILURE);   
  }
  
  printf("Posso concludere.\n");
  exit(EXIT_SUCCESS);
}

