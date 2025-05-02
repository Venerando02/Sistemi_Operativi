/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che condividono un file. Il processo produttore
scrive una stringa, che viene letta dal processo consumatore e poi stampata a video. Si suppone che il file venga
riscritto ad ogni produzione, dunque il contenuto della precedente produzione viene perso ad ogni produzione.
Si supponga che se il processo produttore vuole terminare, scriva END nel file. In tal modo entrambi i processi
terminano.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "semafori.h"

#define CHIAVE_PRODUTTORE (key_t)1234
#define CHIAVE_CONSUMATORE (key_t)5678
#define FILENAME "input.txt"
#define STR_LEN 256

int main(void)
{
  int WriterID, ReaderID, fd, Running = 1;
  char stringa[STR_LEN];
  
  printf("Sono il processo 'consumatore' con PID = %d.\n", getpid());
  
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
  
  while(Running)
  {
    if(SEM_P(ReaderID) == -1) exit(EXIT_FAILURE);
    fd = open(FILENAME, O_RDONLY);
    read(fd, stringa, STR_LEN);
    if(strncmp(stringa, "END", 3) == 0) Running = 0;
    else
    {
      printf("Ho letto la stringa -> %s \n", stringa);
      close(fd);
    }
    if(SEM_V(WriterID) == -1) exit(EXIT_FAILURE);
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
  close(fd);
  printf("Posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}



