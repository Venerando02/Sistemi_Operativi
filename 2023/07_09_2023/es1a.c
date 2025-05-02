/*
Scrivere un programma che crea un processo figlio. Il processo figlio crea un file denominato “figlio.txt” e lo
riempie con caratteri a scelta dello studente. Appena il processo figlio termina di scrivere sul file, il processo
padre aprirà il file “figlio.txt” e creerà un altro file di nome “padre.txt”. In questo file dovrà scrivere “Il seguente
testo è stato scritto dal processo figlio: ” e, a seguire, dovrà copiare tutto il contenuto del file “figlio.txt”.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FILE_FIGLIO "figlio.txt"
#define FILE_PADRE "padre.txt"
#define STR_LEN 128

int main(void)
{
  int fd1, fd2;
  pid_t pid;
  char cinput, cterm;
  char message[STR_LEN] = "Il seguente testo è stato scritto dal processo figlio: ";
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  printf("PADRE: inserisci un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    fd1 = open(FILE_FIGLIO, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd1 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    printf("FIGLIO: scrivo sul file.\n");
    do
    {
      printf("FIGLIO: inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: posso dunque concludere.\n");
    close(fd1);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa di sincronizzazione con il processo figlio.\n");
  wait(NULL);
  
  fd1 = open(FILE_FIGLIO, O_RDONLY);
  fd2 = open(FILE_PADRE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  printf("PADRE: inizio la scrittura su %s.\n", FILE_PADRE);
  
  write(fd2, message, strlen(message));
  while(read(fd1, &cinput, sizeof(cinput)) > 0)
  {
    write(fd2, &cinput, sizeof(cinput));
  }
  
  close(fd1);
  close(fd2);
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}

