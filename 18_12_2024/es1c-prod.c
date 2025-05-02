/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che utilizzano un’area di memoria condivisa.
Si supponga che tale area di memoria sia costituita da una stringa di dimensione scelta a piacere dallo
studente. Il processo produttore riempie da stringa mediante lettura da tastiera. Il processo consumatore legge la
stringa, ne calcola la lunghezza e stampa a video tale valore. Si supponga che i due processi terminino quando il
processo produttore assegna il valore ‘0’ al primo carattere della stringa. È richiesto che l’area di memoria
condivisa sia realizzata da un vettore di caratteri; implementazioni diverse da un semplice vettore (ad
esempio uno struct contenente un vettore) verranno considerate errate.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define STR_LEN 256

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1, i;
  void* ShmP;
  char* P;
  
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
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    printf("Inserisci una stringa (Inserisci 0 come primo carattere per terminare): ");
    fgets(P, STR_LEN, stdin);
    for(i = 0; P[i] != '\0'; i++) if(P[i] == '\n') P[i] = '\0';
    if(P[0] == '0') Running = 0;
    else
    {
      printf("Hai inserito la stringa -> %s \n", P);
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

