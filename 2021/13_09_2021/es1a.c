/*
Creare un processo padre e un processo figlio che condividono un file di testo. La condivisione consiste nel
fare in modo che il processo figlio, appena viene creato, scriva sul file una frase (scelta a piacere dal
candidato) e quando conclude tale scrittura, il processo padre la legga e la visualizzi. Si supponga di passare il
nome del file da utilizzare per questo esercizio, tramite i parametri del main (argc e argv).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define STR_LEN 64

int main(int argc, char* argv[])
{
  char message[STR_LEN] = "Ciao sono il processo figlio. ";
  char stringa_letta[STR_LEN];
  pid_t pid;
  int fd;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: NomeFile.\n");
    exit(EXIT_FAILURE);
  }
  fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
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
    printf("FIGLIO: scrivo una frase.\n");
    write(fd, message, strlen(message));
    printf("FIGLIO: posso concludere.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con la terminazione di PID = %d.\n", pid);
  wait(NULL);
  
  lseek(fd, 0, SEEK_SET);
  read(fd, stringa_letta, STR_LEN);
  printf("PADRE: ho letto la stringa -> %s \n", stringa_letta);
  close(fd);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}


