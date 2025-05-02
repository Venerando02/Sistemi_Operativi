/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Entrambi i processi accedono ad uno
stesso file condiviso. Si supponga che il processo figlio scriva nel file una frase scelta a piacere dallo studente.
Appena il processo figlio finisce di scrivere tale frase, il processo padre deve conteggiare il numero di
caratteri scritti nel file e visualizzare tale numero. Si supponga di svolgere il seguente compito SENZA
utilizzare la funzione lseek().
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NOME_FILE "input.txt"
#define STR_LEN 128

int main(void)
{
  int fd, cont = 0;
  pid_t pid;
  char message[STR_LEN] = "Ciao! Sono il processo figlio.";
  char c;
  
  fd = open(NOME_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo una frase sul file.\n");
    write(fd, message, strlen(message));
    printf("FIGLIO: concludo.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa di sincronizzarmi con la terminazione del processo PID = %d.\n", pid);
  wait(NULL);
  printf("PADRE: chiudo e riapro il file.\n");
  close(fd);
  fd = open(NOME_FILE, O_RDONLY);
  while(read(fd, &c, sizeof(c)) > 0)
  {
    cont++;
  }
  printf("PADRE: sono stati visualizzati %d caratteri.\n", cont);
  close(fd);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}
