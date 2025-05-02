/*
Sviluppare una applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa (si supponga che tale zona di memoria sia costituita da uno struct contenente una stringa di
20 caratteri e un int). Si supponga che normalmente il processo produttore scriva una stringa inserita da tastiera;
il valore scritto nel campo int è normalmente 0. Il processo consumatore legge la stringa e la stampa a video.
Quando il processo produttore desidera terminare deve scrivere il valore 1 nel campo di tipo int; in tal caso non
è necessario scrivere anche il campo stringa. Se il processo consumatore legge il valore 1 nel campo int, allora
esso termina. Si utilizzino dei semafori per la gestione della regione critica.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define SIZE 20

typedef struct DatiCondivisiS
{
  char stringa[SIZE];
  int val;
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
  if((ShmP = shmat(ShmID, NULL, 0))  == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la shared memory.\n");
    exit(EXIT_FAILURE);      
  }
  P = (DatiCondivisiT*)ShmP;
  
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    do
    {
      printf("Inserisci un valore (1: per terminare, 0: continua): ");
      scanf("%d", &P->val);
      while(getchar() != '\n');
    } while((P->val != 0) && (P->val != 1));
    if(P->val == 1) Running = 0;
    else
    {
      printf("Inserisci una stringa: ");
      fgets(P->stringa, SIZE, stdin);
      P->stringa[strlen(P->stringa)-1] = '\0';
    }
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
  }
  if(shmdt(ShmP) == -1) 
  {
    fprintf(stderr, "Impossibile sganciare la shared memory.\n");
    exit(EXIT_FAILURE);    
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}
