/*
Sviluppare una applicazione produttore e una consumatore (due programmi diversi) che utilizzano una zona di
memoria condivisa. Si supponga che la memoria condivisa sia realizzata da un vettore di float. Il processo
produttore riempie il vettore con valori positivi casuali e il processo consumatore legge i valori e li stampa a
video. Supporre che il numero di cicli di produzione e di consumo sia 5. Si utilizzino dei semafori per la
gestione della regione critica
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
#define SIZE 10
#define LIM 100
#define N_CICLI 5

int main(void)
{
  int WriterID, ReaderID, ShmID, cont = 0, i;
  void * ShmP;
  float * P;
  
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
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(float)*SIZE, IPC_CREAT | 0666);
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
  P = (float*)ShmP;
  
  while(cont < N_CICLI)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    printf("Riempio il vettore con valori casuali. \n");
    printf("CICLO %d.\n", cont+1);
    for(i = 0; i<SIZE; i++) P[i] = rand() % LIM;
    cont++;
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
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
    fprintf(stderr, "Impossibile sganciare la shared memory.\n");
    exit(EXIT_FAILURE);  
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

