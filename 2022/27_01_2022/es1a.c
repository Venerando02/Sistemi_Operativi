/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Entrambi i processi scrivono su uno
stesso file condiviso. Il padre scrive “Hello “. Il processo figlio scrive “World !”. Alla fine del programma il
file di testo deve contenere la frase “Hello World !”
Il compito può essere risolto in due modi:
• Senza l’uso dei semafori (valore max 4 punti)
• Con l’uso dei semafori (valore max 8 punti)
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define NOME_FILE "input.txt"

int main(void)
{
  int semaforo, fd;
  pid_t pid;
  
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
    printf("FIGLIO: scrivo una frase sul file.\n");
    write(fd, "World !", 7);
    printf("FIGLIO: posso concludere.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, "Hello ", 6);
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: in attesa di sincronizzazione con il figlio.\n");
  wait(NULL);
  printf("PADRE: posso dunque terminare.\n");
  if(SEM_DEL(semaforo) == -1) exit(EXIT_FAILURE);
  close(fd);
  exit(EXIT_SUCCESS);
}

