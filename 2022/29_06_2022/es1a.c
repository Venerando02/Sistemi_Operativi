/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Entrambi i processi accedono ad uno
stesso file condiviso. Si supponga che il processo figlio scriva nel file una frase scelta a piacere dallo studente.
Appena il processo figlio finisce di scrivere tale frase, il processo padre deve conteggiare il numero di caratteri
scritti nel file e visualizzare tale numero.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NOME_FILE "input.txt"
#define STR_LEN 128

int main(void)
{
  int fd, cont = 0;
  pid_t pid;
  char message[STR_LEN] = "Ciao sono il processo figlio.\n";
  char ch;
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    write(fd, message, strlen(message));
    printf("FIGLIO: ho scritto una frase sul file.\n");
    printf("FIGLIO: posso dunque concludere.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa di sincronizzarmi col processo con PID = %d.\n", getpid());
  wait(NULL);
  lseek(fd, 0, SEEK_SET);
  printf("PADRE: conto il numero di caratteri sul file %s.\n", NOME_FILE);
  while(read(fd, &ch, sizeof(ch)) > 0)
  {
    cont++;
  }
  printf("PADRE: nel file sono stati inseriti %d caratteri.\n", cont);
  close(fd);
  exit(EXIT_SUCCESS);
}

