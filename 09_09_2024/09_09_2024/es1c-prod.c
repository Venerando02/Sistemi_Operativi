/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che utilizzano un’area di memoria condivisa.
Si supponga che tale area di memoria sia costituita da un vettore di caratteri, di dimensione scelta a piacere
dallo studente. Il processo produttore assegna dei valori casuali alle variabili char del vettore, generando numeri
casuali interi compresi tra 97 e 122 (corrispondono ai caratteri minuscoli dalla ‘a’ alla ‘z’). Il processo
consumatore legge i valori scritti dal produttore (codice ASCII di ogni carattere), calcola il numero di volte che
è presente il codice ASCII 100 (corrisponde al carattere ‘d’) e stampa a video il risultato ottenuto. Si supponga
che i due processi terminino quando il processo produttore assegna il carattere ‘*’ al primo elemento del vettore.
È richiesto che l’area di memoria condivisa sia realizzata da un vettore; implementazioni diverse da un
semplice vettore (ad esempio uno struct contenente un vett%ore) verranno considerate un errore e il voto
attribuito al quesito sarà 0, anche nel caso in cui risoluzione del quesito sia corretta.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define CHIAVE_MEMORIA_CONDIVISA (key_t)9012
#define DIM 20
#define LIM 26

typedef struct DatiCondivisiS
{
  char vettore[DIM];
  int NumOccorrenzaD;
} DatiCondivisiT;

int main(void)
{
  int WriterID, ReaderID, ShmID, Running = 1, i;
  void * ShmP;
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
    printf("Inserisci il valore iniziale ('*' se vuoi terminare): ");
    scanf("%c", &P->vettore[0]);
    while(getchar() != '\n');
    if(P->vettore[0] == '*') Running = 0;
    else
    {
      for(i = 1; i<DIM; i++) P->vettore[i] = ((rand() % LIM) + 97);
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
