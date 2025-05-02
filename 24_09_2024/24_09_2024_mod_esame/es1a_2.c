/*
Si supponga che due processi indipendenti condividano un’area di memoria costituita da una variabile intera.
I due processi eseguono le seguenti attività.
Processo UNO: legge da tastiera un numero intero e lo scrive nella memoria condivisa. Dopo aver eseguito la
scrittura in memoria condivisa, il processo UNO termina.
Processo DUE: aspetta che il processo UNO abbia scritto in memoria condivisa, legge il valore scritto e lo
stampa a video. Dopo anche lui termina.
Risolvere il problema utilizzando il meccanismo della memoria condivisa e utilizzando la sincronizzazione tra
i due processi tramite semaforo. Si evidenzia che il problema di sincronizzazione NON può essere risolto come
problema produttore consumatore, ma come problema di precedenza tramite l’uso di UN SOLO semaforo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define CHIAVE_MEMORIA_CONDIVISA (key_t)5678

int main(void)
{
  int SemID, ShmID;
  void* ShmP;
  int * P;
  
  printf("Sono il processo con PID = %d.\n", getpid());
  
  SemID = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(SemID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  ShmID = shmget(CHIAVE_MEMORIA_CONDIVISA, sizeof(int), IPC_CREAT | 0666);
  if(ShmID == -1)
  {
    fprintf(stderr, "Impossibile creare la memoria condivisa.\n");
    exit(EXIT_FAILURE);    
  }
  ShmP = shmat(ShmID, NULL, 0);
  if(ShmP == (void*)-1)
  {
    fprintf(stderr, "Impossibile agganciare la memoria condivisa.\n");
    exit(EXIT_FAILURE);      
  }
  P = (int*)ShmP;
  
  if(SEM_P(SemID) == -1) exit(EXIT_FAILURE);
  printf("Ho letto il valore-> %d dalla memoria condivisa.\n", (*P));
  
  printf("\nEseguo lo sgancio e l'eliminazione dalla shared memory.\n");
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
  printf("Elimino il semaforo.\n");
  if(SEM_DEL(SemID) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);              
  }
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

