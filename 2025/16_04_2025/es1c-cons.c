/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che utilizzano un’area di memoria condivisa.
Si supponga che tale area di memoria sia costituita da uno struct contenente due variabili intere e una variabile
float. Si supponga che al processo produttore sia associato un utente che scrive su tastiera due valori interi e li
assegna ai due campi di tipo int della struct. Il processo consumatore legge i due campi int, calcola la media di
tali numeri, stampa a video tale valore e lo scrive nel campo float della struct. Si supponga che i due processi
terminino quando il processo produttore imposta a 0 i due campi interi; dunque, nel caso in cui l’utente collegato
al processo produttore non voglia più continuare, il programma del processo produttore inserisce il valore 0 nei
due campi interi. In questo caso, il processo consumatore non dovrà leggere i campi interi, né calcolare la media
ma semplicemente terminare.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_PRODUTTORE (key_t)1234
#define CHIAVE_CONSUMATORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012

typedef struct DatiCondivisiS
{
  int a, b;
  float media;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1;
  void* ShmP;
  DatiCondivisiT* P;
  
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
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(DatiCondivisiT), IPC_CREAT | 0666);
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
  P = (DatiCondivisiT*)ShmP;
  
  while(Running)
  {
    if(SEM_P(ReaderID) == -1)
    {
      fprintf(stderr, "P() fallita.\n");
      exit(EXIT_FAILURE);
    }
    if((P->a == 0) && (P->b == 0)) Running = 0;
    else
    {
      P->media = (((float)P->a + (float)P->b) / 2.0);
      printf("Valori inseriti:\n");
      printf("a = %d.\n", P->a);
      printf("b = %d.\n", P->b);
      printf("media = %.2f.\n", P->media);
    }
    if(SEM_V(WriterID) == -1)
    {
      fprintf(stderr, "V() fallita.\n");
      exit(EXIT_FAILURE);
    }
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
  if(shmctl(ShmID, IPC_RMID, NULL) == -1)
  {
    fprintf(stderr, "Impossibile eliminare la shared memory.\n");
    exit(EXIT_FAILURE);      
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}
