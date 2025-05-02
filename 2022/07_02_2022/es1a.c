/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Entrambi i processi scrivono su uno
stesso file condiviso. Prima, il padre scrive una frase a scelta dello studente. Appena il padre conclude di
scrivere sul file, il processo figlio deve conteggiare il numero di caratteri scritti dal processo padre e
visualizzare tale numero.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define NOME_FILE "input.txt"
#define STR_LEN 128

int main(void)
{
  int fd, semaforo, cont = 0;
  pid_t pid;
  char message[STR_LEN] = "Ciao, sono il processo PADRE.\n";
  char ch;
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
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
    printf("FIGLIO: devo conteggiare il numero di caratteri.\n");
    lseek(fd, 0, SEEK_SET);
    while(read(fd, &ch, sizeof(ch)) > 0)
    {
      cont++;
    }
    printf("FIGLIO: nel file sono stati scritti %d caratteri dal padre.\n", cont);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, message, strlen(message));
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: in attesa che termini il processo figlio.\n");
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



