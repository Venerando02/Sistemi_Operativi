/*
Scrivere un programma che crea un processo figlio. Processo padre e processo figlio condividono un file (il cui
nome è a scelta dello studente).
Si supponga che il processo figlio scriva nel file una frase scelta a piacere dallo studente di lunghezza non
inferiore a 20 caratteri. Appena il processo figlio finisce di scrivere tale frase, il processo padre deve copiare
gli ultimi 12 caratteri di questo file in un nuovo file (il cui nome è a scelta dello studente) e terminare.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NOME_FILE_INPUT "input.txt"
#define NOME_FILE_OUTPUT "output.txt" 
#define STR_LEN 128

int main(void)
{
  pid_t pid;
  int fd1, fd2;
  char message[STR_LEN] = "Ciao sono il processo figlio.";
  char ch;
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  fd1 = open(NOME_FILE_INPUT, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd1 == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  fd2 = open(NOME_FILE_OUTPUT, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo una frase sul file.\n");
    write(fd1, message, strlen(message));
    printf("FIGLIO: concludo.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa di sincronizzarmi con la terminazione di PID = %d.\n", pid);
  wait(NULL);
  lseek(fd1, -12, SEEK_END);
  while(read(fd1, &ch, sizeof(ch)) > 0)
  {
    write(fd2, &ch, sizeof(ch));
  }
  printf("PADRE: ho copiato gli ultimi 12 caratteri su %s.\n", NOME_FILE_OUTPUT);
  close(fd1);
  close(fd2);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}

