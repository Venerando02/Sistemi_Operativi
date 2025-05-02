/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da una stringa (in particolare, un array di
20 caratteri). Il processo produttore riempie la stringa mediante inserimento da tastiera. Il processo consumatore
calcola la lunghezza della stringa e la stampa a video. Si supponga che i due processi terminano quando il
processo produttore inserisce la stringa “End” oppure la stringa “Fine” nell’area di memoria condivisa. Si
utilizzino i semafori per la gestione della regione critica.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "semafori.h"
#include <sys/shm.h>

#define STR_LEN 20
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
  int WriterID, ReaderID, ShmID, Running = 1, i;
  void* ShmP;
  DatiCondivisiT* P;
  
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
  
  printf("Semafori acquisiti correttamente.\n");
  
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(DatiCondivisiT), IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  P = (DatiCondivisiT*)ShmP;
  
  printf("Memoria condivisa settata correttamente.\n");
  
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    printf("Inserisci una stringa: ");
    fgets(P->stringa, STR_LEN, stdin);
    for(i = 0; P->stringa[i] != '\0'; i++) if(P->stringa[i] == '\n') P->stringa[i] = '\0';
    if((strncmp(P->stringa, "End", 3) == 0) || (strncmp(P->stringa, "Fine", 4) == 0)) Running = 0;
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
  }
  
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile effettuare lo sgancio dalla shared memory.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Posso concludere.\n");
  exit(EXIT_SUCCESS);
}
