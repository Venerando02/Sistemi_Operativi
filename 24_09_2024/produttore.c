/*
Sviluppare due programmi, uno produttore e l’altro consumatore, che condividono un file. Il processo produttore
scrive una stringa, che viene letta dal processo consumatore e poi stampata a video. Si suppone che il file venga
riscritto ad ogni produzione, dunque il contenuto della precedente produzione viene perso ad ogni produzione.
Si supponga che se il processo produttore vuole terminare, scriva END nel file. In tal modo entrambi i processi
terminano.
*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_CONSUMATORE (key_t)1234
#define CHIAVE_PRODUTTORE (key_t)5678
#define NOME_FILE "daticondivisi.txt"
#define SIZE 128

int main(void)
{
  int fd, WriterID, ReaderID, Running = 1, i;
  char * Stringa = NULL;
  
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
  while(Running)
  {
    if(SEM_P(WriterID) == -1) exit(EXIT_FAILURE);
    fd = open(NOME_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    Stringa = (char*)malloc(sizeof(char)*SIZE);
    printf("Inserisci una stringa: ");
    fgets(Stringa, SIZE, stdin);
    for(i = 0; Stringa[i] != '\0'; i++) if(Stringa[i] == '\n') Stringa[i] = '\0';
    write(fd, Stringa, strlen(Stringa));
    if(strncmp(Stringa, "END", 3) == 0)
    {
      Running = 0;
      close(fd);
    }
    if(SEM_V(ReaderID) == -1) exit(EXIT_FAILURE);
  }
  
  printf("Posso concludere correttamente.\n");
  exit(EXIT_SUCCESS);
}
