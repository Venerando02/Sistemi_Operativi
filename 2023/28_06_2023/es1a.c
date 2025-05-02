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

#define NOME_FILE "output.txt"
#define STR_LEN 64

int main(void)
{
  int fd;
  pid_t pid;
  char message1[STR_LEN] = "Salve, sono il processo padre\n";
  char message2[STR_LEN] = "Salve, sono il processo figlio\n";
  char message3[STR_LEN] = "Termino il programma";
  
  fd = open(NOME_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, message1, strlen(message1));
  printf("PADRE: creo un figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo un altra frase sul file.\n");
    write(fd, message2, strlen(message2));
    printf("FIGLIO: posso terminare.\n");
    exit(EXIT_SUCCESS);
  }  
  printf("PADRE: in attesa di sincronizzazione con il processo PID = %d.\n", pid);
  wait(NULL);
  write(fd, message3, strlen(message3));
  printf("PADRE: chiudo il file e concludo.\n");
  close(fd);
  exit(EXIT_SUCCESS);
}

