/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzando una zona di
memoria condivisa. Si supponga che tale memoria contenga uno struct con due campi: un vettore di interi e un
valore intero. Il produttore riempie il vettore con numeri casuali e il consumatore calcola la media e la
visualizza. Supporre che produttore e consumatore vengano eseguiti in un ciclo. Quando il processo produttore
intende uscire dal ciclo, scrive il valore -1 nel campo intero dello struct. Se il consumatore legge questo valore
(-1), allora termina anche lui. Si utilizzino i semafori per la gestione della regione critica
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
#define DIM 10
#define LIM 100

typedef struct DatiCondivisiS
{
  int v[DIM];
  int val;
  float media;
} DatiCondivisiT;

int main(void) 
{
  int WriterID, ReaderID, ShmID, Running = 1, i;
  void* ShmP;
  float somma;
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
  if((ShmP = shmat(ShmID, NULL, 0)) == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la shared memory.\n");
    exit(EXIT_FAILURE);      
  }
  P = (DatiCondivisiT*)ShmP;
  
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    printf("Inserisci un valore intero (-1 per terminare): ");
    scanf("%d", &P->val);
    while(getchar() != '\n');
    if(P->val == -1) Running = 0;
    else
    {
      for(i = 0; i<DIM; i++) P->v[i] = rand() % LIM;
      printf("-----------\n");
      for(i = 0; i<DIM; i++) printf("v[%d] = %d \n", i, P->v[i]);
      printf("-----------\n");
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

