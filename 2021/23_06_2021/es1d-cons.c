/*
Sviluppare una applicazione produttore e una consumatore (due programmi diversi) che utilizzando una zona di
memoria condivisa (si supponga una stringa). Si utilizzino dei semafori per la gestione della regione critica
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234 
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define STR_LEN 128

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void * ShmP;
  char* P;
  
  printf("Processo 'consumatore' con PID = %d.\n", getpid());
  
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
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(char)*STR_LEN, IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la shared memory.\n");
    exit(EXIT_FAILURE);  
  }
  ShmP = shmat(ShmID, NULL, 0);
  if(ShmP == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la shared memory.\n");
    exit(EXIT_FAILURE);    
  }
  P = (char*)ShmP;
  
  while(Running) 
  {
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    printf("Ho letto la stringa %s.\n", P);
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
  if(shmctl(ShmID, IPC_RMID, 0) == -1)
  {
    fprintf(stderr, "Impossibile eliminare la shared memory.\n");
    exit(EXIT_FAILURE);          
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_FAILURE);
}

