/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che utilizzano un’area di memoria condivisa.
Si supponga che tale area di memoria sia costituita da una struct contenente due variabili intere maggiori o uguali
a zero. Il processo produttore assegna alle due variabili intere dei valori compresi nell’intervallo [1, 5]. Il processo
consumatore legge i valori e stampa a video il loro prodotto. Si supponga che i due processi terminino quando il
processo produttore imposta a -1 entrambe le variabili intere.
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
  int a, b, prodotto;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void * ShmP;
  DatiCondivisiT* P;
  
  printf("Processo 'produttore' PID = %d.\n", getpid());
  
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
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);  
  }
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la memoria condivisa.\n");
    exit(EXIT_FAILURE);  
  }
  P = (DatiCondivisiT*)ShmP;
  
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    do
    {
      printf("Inserisci un valore intero di a (-1 per terminare): ");
      scanf("%d", &P->a);
    } while(((P->a < 1) || (P->a > 5)) && (P->a != -1));
    do
    {
      printf("Inserisci un valore intero di b (-1 per terminare): ");
      scanf("%d", &P->b);
    } while(((P->b < 1) || (P->b > 5)) && (P->b != -1));
    if((P->a == -1) && (P->b == -1)) Running = 0;
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
  }
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile effettuare lo sgancio dalla shared memory.\n");
    exit(EXIT_FAILURE);  
  }
  printf("Sgancio dalla memoria effettuato.\n");
  printf("Posso concludere.\n");
  exit(EXIT_SUCCESS);
}

