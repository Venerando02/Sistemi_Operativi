/*
Scrivere un programma che crea un processo figlio. Il processo figlio crea due file denominati “figlio1.txt”
e “figlio2.txt”, che vengono riempiti di caratteri, a scelta dello studente. Appena il processo figlio termina
di scrivere sui due file, il processo padre aprirà i due file e creerà un altro file di nome “padre.txt”. In questo
file dovrà scrivere l’intero contenuto del primo file “figlio1.txt” e, di seguito, l’intero contenuto del secondo
file “figlio2.txt”.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILE1 "figlio1.txt"
#define FILE2 "figlio2.txt"
#define FILE3 "padre.txt"

int main(void)
{
  int fd1, fd2, fd3;
  pid_t pid;
  char cinput, cterm;
  
  printf("PADRE: inserisci un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  
  printf("PADRE: creo un processo figlio.\n");
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    
    fd1 = open(FILE1, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd1 == -1)
    {
      fprintf(stderr, "Impossibile creare il file.\n");
      exit(EXIT_FAILURE);
    }
    fd2 = open(FILE2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd2 == -1)
    {
      fprintf(stderr, "Impossibile creare il file.\n");
      exit(EXIT_FAILURE);
    }
    
    printf("FIGLIO: scrittura su %s.\n", FILE1);
    do
    {
      printf("FIGLIO: inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: scrittura su %s.\n", FILE2);
    do
    {
      printf("FIGLIO: inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd2, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    close(fd1);
    close(fd2);
    printf("FIGLIO: posso concludere.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con la terminazione del processo PID = %d.\n", pid);
  wait(NULL);
  
  fd3 = open(FILE3, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd3 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  fd1 = open(FILE1, O_RDONLY);
  fd2 = open(FILE2, O_RDONLY);
  
  printf("PADRE: copio %s su %s.\n", FILE1, FILE3);
  while(read(fd1, &cinput, sizeof(char)) > 0)
  {
    write(fd3, &cinput, sizeof(char));
  }
  printf("PADRE: copio %s su %s.\n", FILE2, FILE3);
  while(read(fd2, &cinput, sizeof(char)) > 0)
  {
    write(fd3, &cinput, sizeof(char));
  }
  close(fd1);
  close(fd2);
  close(fd3);
  printf("PADRE: posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

