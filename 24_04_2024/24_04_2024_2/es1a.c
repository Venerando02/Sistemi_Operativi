/*
Scrivere un programma che crea un processo figlio. Processo padre e processo figlio condividono un file (il cui
nome è a scelta dello studente).
Si supponga che il processo figlio scriva nel file una frase scelta a piacere dallo studente di lunghezza non
inferiore a 20 caratteri. Appena il processo figlio finisce di scrivere tale frase, il processo padre deve copiare i
primi 10 caratteri di questo file in un nuovo file (il cui nome è a scelta dello studente) e terminare.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define FILENAME_INPUT "input.txt"
#define FILENAME_OUTPUT "output.txt"
#define STR_LEN 128

int main(void)
{
  int fd1, fd2, cont = 0;
  char message[STR_LEN] = "Ciao sono il processo figlio, scrivo una frase.";
  char cinput;
  pid_t pid;
  
  fd1 = open(FILENAME_INPUT, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd1 == -1)
  {
    fprintf(stderr, "Impossibile creare o aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo una frase sul file.\n");
    write(fd1, message, strlen(message));
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con la terminazione di PID = %d.\n", pid);
  wait(NULL);
  lseek(fd1, 0, SEEK_SET);
 
  fd2 = open(FILENAME_OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile creare o aprire il file.\n");
    exit(EXIT_FAILURE);  
  }
  
  printf("PADRE: copio i primi 10 caratteri di %s su %s.\n", FILENAME_INPUT, FILENAME_OUTPUT);
  while(cont < 10)
  {
    read(fd1, &cinput, sizeof(cinput));
    write(fd2, &cinput, sizeof(cinput));
    cont++;
  }
  close(fd1);
  close(fd2);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

