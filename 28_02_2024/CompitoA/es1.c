/*
Si risolva il seguente problema di precedenza. Si supponga che due processi padre/figlio condividano un file di
testo. È richiesto che il processo padre scriva, per primo, la frase “CompitoA”, e che il processo figlio la legga,
appena il processo padre ha terminato la scrittura nel file. Non appena il processo figlio ha completato la lettura,
deve visualizzare la frase sullo schermo e poi terminare. Appena il figlio termina, anche il processo padre può
terminare.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define NOME_FILE "input.txt"
#define CHIAVE_SEMAFORO (key_t)1234
#define STR_LEN 64

int main(void)
{
  int semaforo, fd;
  pid_t pid;
  char stringa[STR_LEN];
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  semaforo = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(semaforo == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  
  if(SEM_SET(semaforo, 0) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_P(semaforo) == -1) exit(EXIT_FAILURE);
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    lseek(fd, 0, SEEK_SET);
    read(fd, stringa, STR_LEN);
    printf("FIGLIO: ho letto la stringa %s.\n", stringa);
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: scrivo una frase su %s.\n", NOME_FILE);
  write(fd, "CompitoA", 8);
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: mi sincronizzo con la terminazione del processo figlio..\n");
  wait(NULL);
  printf("PADRE: posso terminare..\n");
  close(fd);
  if(SEM_DEL(semaforo) == -1) exit(EXIT_FAILURE);
  exit(EXIT_SUCCESS);
}

