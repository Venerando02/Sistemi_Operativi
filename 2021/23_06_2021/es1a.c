/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Entrambi i processi scrivono su uno
stesso file condiviso. Il figlio scrive “Hello “. Il padre scrive “World !”. Alla fine del programma il file di
testo deve contenere la frase “Hello World !”
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NOME_FILE "input.txt"

int main(void)
{
  pid_t pid;
  int fd;
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il proceso con PID = %d.\n", getpid());
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo una frase.\n");
    write(fd, "Hello ", 6);
    printf("FIGLIO: concludo.\n");
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: mi sincronizzo con la terminazione del processo con PID = %d.\n", pid);
  wait(NULL);
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, "World !", 7);
  close(fd);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}


