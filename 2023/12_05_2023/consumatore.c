/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da una struct contenente un intero e un
float. Il processo produttore inserisce i valori nei due campi, dunque un intero e un float. Il processo consumatore
calcola il prodotto dei due valori e lo stampa a video. Si supponga che i due processi terminano quando il processo
produttore inserisce il valore 0 in entrambi i campi. Si utilizzino dei semafori per la gestione della regione critica.
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

typedef struct DatiCondivisiS
{
  int a;
  float b, prodotto;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1; 
  void * ShmP;
  DatiCondivisiT* P;
  
  printf("Processo 'consumatore' PID = %d.\n", getpid());
  
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
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(ReaderID, 0) == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  printf("Semafori settati correttamente.\n");
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
    if((P->a == 0) && (P->b == 0)) Running = 0;
    else
    {
      P->prodotto = (float)P->a * P->b;
      printf("Il prodotto tra %d e %.2f e' %.2f.\n", P->a, P->b, P->prodotto);
    }
    if(SEM_V(WriterID) == -1) exit(EXIT_FAILURE);
  }
  
  if(SEM_DEL(ReaderID) == -1) exit(EXIT_FAILURE);
  if(SEM_DEL(WriterID) == -1) exit(EXIT_FAILURE);
  printf("Semafori eliminati correttamente.\n");
  
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile sganciare la memoria condivisa.\n");
    exit(EXIT_FAILURE);
  }
  
  if(shmctl(ShmID, IPC_RMID, 0) == -1)
  {
    fprintf(stderr, "Impossibile eliminare la memoria condivisa.\n");
    exit(EXIT_FAILURE);  
  }
  
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

