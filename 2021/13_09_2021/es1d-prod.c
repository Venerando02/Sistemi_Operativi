/*
Sviluppare una applicazione produttore e una consumatore (due programmi diversi) che utilizzano una zona di
memoria condivisa (si supponga che tale zona di memoria sia costituita da uno struct contenente un int,
un float e un char). Si supponga che normalmente il processo produttore scrive un valore intero e un valore
reale inseriti da tastiera. Il processo consumatore legge questi numeri e li stampa a video. Quando il processo
produttore desidera terminare scrive il campo char inserendo il valore ‘f’. Se il processo consumatore legge il
valore ‘f’ nel campo char, allora esso termina. Si utilizzino dei semafori per la gestione della regione critica.
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
  float b;
  char c;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void* ShmP;
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
    printf("Inserisci un carattere ('f' per terminare): ");
    scanf("%c", &P->c);
    while(getchar() != '\n');
    if(P->c == 'f') Running = 0;
    else
    {
       printf("Inserisci un intero: ");
       scanf("%d", &P->a);
       while(getchar() != '\n');
     
       printf("Inserisci un float: ");
       scanf("%f", &P->b);
       while(getchar() != '\n');
    }
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE); 
  }
  
  if(shmdt(ShmP) == -1)
  {
    fprintf(stderr, "Impossibile sganciare la shared memory.\n");
    exit(EXIT_FAILURE);    
  }
  printf("Posso dunque conludere.\n");
  exit(EXIT_SUCCESS);
}

