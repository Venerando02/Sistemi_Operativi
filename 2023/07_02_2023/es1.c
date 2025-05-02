/*
Scrivere un programma che crea due processi figli (dunque, il main() che rappresenta il processo padre e due
processi figli creati dal processo padre). I due processi figli creano in modo del tutto indipendente due file, che
vengono riempiti con caratteri (lo studente scelga come vuole cosa e come inserire nei due file). Appena ciascun
processo figlio termina, il processo padre deve contare il numero di caratteri scritti in ciascun file e stampare
tale numero di caratteri, per ciascuno dei due file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FILE1 "file1.txt"
#define FILE2 "file2.txt"

int main(void)
{
  int fd1, fd2;
  pid_t pid1, pid2;
  int contatore1 = 0, contatore2 = 0;
  char cinput, cterm;
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: inserisci un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  printf("\n");
  
  pid1 = fork();
  if(pid1 == -1) exit(EXIT_FAILURE);
  if(pid1 == 0)
  {
    fd1 = open(FILE1, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd1 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    do
    {
      printf("FIGLIO %d: inserisci un carattere ('%c' per terminare): ", getpid(), cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO %d: termino.\n", getpid());
    close(fd1);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa della terminazione del figlio %d.\n", pid1);
  wait(NULL);
  
  pid2 = fork();
  if(pid2 == -1) exit(EXIT_FAILURE);
  if(pid2 == 0)
  {
    fd2 = open(FILE2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd2 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    do
    {
      printf("FIGLIO %d: inserisci un carattere ('%c' per terminare): ", getpid(), cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd2, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO %d: termino.\n", getpid());
    close(fd2);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa della terminazione del figlio %d.\n", pid2);
  wait(NULL);
  
  fd1 = open(FILE1, O_RDONLY);
  fd2 = open(FILE2, O_RDONLY);
  
  printf("PADRE: conto il numero di caratteri sui due file.\n");
  
  while(read(fd1, &cinput, sizeof(cinput)) > 0)
  {
    contatore1++;
  }
  while(read(fd2, &cinput, sizeof(cinput)) > 0)
  {
    contatore2++;
  }
  
  printf("PADRE: per il file %s sono stati contati %d caratteri.\n", FILE1, contatore1);
  printf("PADRE: per il file %s sono stati contati %d caratteri.\n", FILE2, contatore2);
  
  close(fd1);
  close(fd2);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}
