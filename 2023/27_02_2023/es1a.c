/*
Scrivere un programma che crea due processi figli (dunque, il main() che rappresenta il processo padre e due
processi figli creati dal processo padre). I due processi figli creano in modo del tutto indipendente due file, che
vengono riempiti con caratteri (lo studente scelga a piacere cosa e come inserire nei due file). Appena entrambi
i processi figli terminano (devono finire entrambi), il processo padre deve effettuare il merge dei due file, ossia
deve creare un terzo file composto dal contenuto del file creato da un figlio e, a seguire, dal contenuto del file
creato dall’altro figlio. L’ordine con cui vengono disposti i due contenuti non ha importanza, è richiesto soltanto
che il file risultante sia composto dai contenuti dei due file creati dai processi figli.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FILE1 "file1.txt"
#define FILE2 "file2.txt"
#define FILE_OUTPUT "output.txt"

int main(void)
{
  int fd1, fd2, fd3;
  pid_t pid1, pid2;
  char cinput, cterm;
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: inserisci un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  
  printf("\nPADRE: creo il primo processo figlio.\n");
  pid1 = fork();
  if(pid1 == -1) exit(EXIT_FAILURE);
  if(pid1 == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    
    fd1 = open(FILE1, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd1 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    
    do
    {
      printf("FIGLIO: inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: chiudo il file e termino.\n");
    close(fd1);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo col processo dal PID = %d.\n", pid1);
  wait(NULL);
  
  printf("\nPADRE: creo il secondo processo figlio.\n");
  pid2 = fork();
  if(pid2 == -1) exit(EXIT_FAILURE);
  if(pid2 == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    
    fd2 = open(FILE2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd2 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    
    do
    {
      printf("FIGLIO: inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd2, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: chiudo il file e termino.\n");
    close(fd2);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo col processo dal PID = %d.\n", pid1);
  wait(NULL);
  
  fd1 = open(FILE1, O_RDONLY);
  fd2 = open(FILE2, O_RDONLY);
  fd3 = open(FILE_OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd3 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE); 
  }
  
  printf("PADRE: scrivo il contenuto di %s su %s.\n", FILE1, FILE_OUTPUT);
  while(read(fd1, &cinput, sizeof(char)) > 0)
  {
    write(fd3, &cinput, sizeof(char));
  }
  
  printf("PADRE: scrivo il contenuto di %s su %s.\n", FILE2, FILE_OUTPUT);
  while(read(fd2, &cinput, sizeof(char)) > 0)
  {
    write(fd3, &cinput, sizeof(char));
  }
  
  printf("PADRE: chiudo i file e concludo.\n");
  
  close(fd1);
  close(fd2);
  close(fd3);
  
  exit(EXIT_SUCCESS);
}
