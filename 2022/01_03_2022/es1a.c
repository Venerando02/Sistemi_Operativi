/*
Scrivere un programma che crea due processi (uno padre e uno figlio) ed utilizzi i parametri del main() argc e
argv. Entrambi i processi scrivono su uno stesso file condiviso, il cui nome viene passato come primo parametro.
Il padre scrive “Come ”. Il processo figlio scrive “stai?”. Alla fine del programma il file di testo deve contenere
la frase “Come stai?”.
L’esercizio deve essere risolto senza l’uso dei semafori.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234

int main(int argc, char* argv[])
{
  int fd, semaforo;
  pid_t pid;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: NomeFile.\n");
    exit(EXIT_FAILURE);
  }
  
  fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
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
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_P(semaforo) == -1) exit(EXIT_FAILURE);
    printf("FIGLIO: scrivo una frase sul file.\n");
    write(fd, "stai?", 5);
    printf("FIGLIO: posso concludere.\n");
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, "Come ", 5);
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: mi sincronizzo con la terminazione del figlio.\n");
  wait(NULL);
  if(SEM_DEL(semaforo) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  close(fd);
  printf("PADRE: posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}
