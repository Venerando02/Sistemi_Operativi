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
  printf("Semafori acquisiti correttamente.\n");
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
    printf("Inserisci un valore intero: ");
    scanf("%d", &P->a);
    while(getchar() != '\n');
    printf("Inserisci un valore float: ");
    scanf("%f", &P->b);
    while(getchar() != '\n');
    if((P->a == 0) && (P->b == 0)) Running = 0;
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

