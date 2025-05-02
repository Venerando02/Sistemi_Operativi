/*
Scrivere un programma che crea un processo figlio. Processo padre e processo figlio condividono un file.
Il programma deve far sì che, alla sua conclusione, il contenuto del file condiviso sia il seguente:
“Salve, sono il processo padre” (questa frase deve essere stata scritta dal processo padre)
“Salve, sono il processo figlio” (questa frase deve essere stata scritta dal processo figlio)
“Termino il programma” (questa frase deve essere stata scritta dal processo padre)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILENAME "input.txt"
#define SSIZE 128

int main(void)
{
  int fd; 
  pid_t pid;
  char message1[SSIZE] = "Salve, sono il processo padre\n";
  char message2[SSIZE] = "Salve, sono il processo figlio\n";
  char message3[SSIZE] = "Termino il programma";
  
  fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: scrivo una frase sul file %s.\n", FILENAME);
  
  write(fd, message1, strlen(message1));
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0) 
  {
    printf("FIGLIO: scrivo una frase sul file %s.\n", FILENAME);
    write(fd, message2, strlen(message2));
    printf("FIGLIO: concludo.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con la terminazione del processo PID = %d.\n", pid);
  wait(NULL);
  printf("PADRE: scrivo una frase sul file %s.\n", FILENAME);
  write(fd, message3, strlen(message3));
  printf("PADRE: concludo.\n");
  close(fd);
  exit(EXIT_SUCCESS);
}

