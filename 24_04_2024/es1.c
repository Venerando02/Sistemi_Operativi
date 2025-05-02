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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NOME_FILE_INPUT "input.txt"
#define NOME_FILE_OUTPUT "output.txt" 
#define STR_LEN 36

int main(void)
{
  int fd1, fd2, cont = 0;
  pid_t pid;
  char message[STR_LEN] = "Ciao! come va? Io tutto bene.";
  char ch;   
  
  
  fd1 = open(NOME_FILE_INPUT, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd1 == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    write(fd1, message, strlen(message));
    printf("FIGLIO: posso concludere.\n");
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: mi sincronizzo con la terminazione del processo PID = %d.\n", pid);
  wait(NULL);
  lseek(fd1, 0, SEEK_SET);
  fd2 = open(NOME_FILE_OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  printf("PADRE: copio i primi 10 caratteri su fd2.\n");
  while(cont < 10)
  {
    read(fd1, &ch, sizeof(ch));
    write(fd2, &ch, sizeof(ch));
    cont++;
  }
  close(fd1);
  close(fd2);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}




