/*
Sviluppare un’applicazione produttore e una consumatore (due programmi diversi) che utilizzano un’area di
memoria condivisa. Si supponga che tale area di memoria sia costituita da una struct contenente due campi float
(base e altezza) corrispondenti alla lunghezza di base e altezza di un triangolo. Il processo produttore riempie i
due campi float mediante inserimento da tastiera. Il processo consumatore calcola l’area del triangolo
(base*altezza/2) e la stampa a video. Supporre che il numero di cicli di produzione e di consumo sia uguale a 4.
Si utilizzino i semafori per la gestione della regione critica.
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
#define N_CICLI 4

typedef struct TriangoloS
{
  float base, altezza, area;
} TriangoloT;

int main(void)
{
  int WriterID, ReaderID, ShmID, cont = 0;
  void* ShmP;
  TriangoloT* P;
  
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
  if(SEM_SET(ReaderID, 0) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);  
  }
  if(SEM_SET(WriterID, 1) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);    
  }
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(TriangoloT), IPC_CREAT | 0666);
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
  P = (TriangoloT*)ShmP;
  while(cont < N_CICLI)
  {
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    printf("La base del triangolo e' %.2f.\n", P->base);
    printf("L'altezza del triangolo e' %.2f.\n", P->altezza);
    P->area = ((P->base * P->altezza) / 2.0);
    printf("L'area del triangolo e' %.2f.\n", P->area);
    cont++;
    if(SEM_V(WriterID) == -1) exit(EXIT_FAILURE);
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
