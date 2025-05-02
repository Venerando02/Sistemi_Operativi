/*
Scrivere un programma che crea due processi figli (dunque vi saranno in tutto 3 processi, il main(), che
rappresenta il processo padre, e due processi figli creati dal processo padre). Ogni processo figlio crea un file,
che viene riempito con caratteri a scelta dello studente; dunque, il numero di file creati in totale sarà 2, uno per
ciascun processo figlio. Ogni file avrà un nome differente, a scelta dello studente. Appena entrambi i processi
figli terminano di creare e riempire i file (devono finire entrambi), il processo padre aprirà i due file e ne
stamperà il contenuto a video; lo studente scelga a piacere l’ordine con cui il processo padre apre e visualizza i
due file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILE1 "file1.txt"
#define FILE2 "file2.txt"
#define STR_LEN 128

int main(void)
{
  pid_t pid1, pid2;
  int fd1, fd2;
  char cinput, cterm;
  char stringa1[STR_LEN], stringa2[STR_LEN];
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: scegli un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  
  printf("PADRE: creo il primo figlio.\n");
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
      printf("Inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: posso concludere.\n");
    close(fd1);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con il figlio PID = %d.\n", pid1);
  wait(NULL);
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
      printf("Inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(fd2, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: posso concludere.\n");
    close(fd2);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con il figlio PID = %d.\n", pid2);
  wait(NULL);
  
  printf("PADRE: apro entrambi i file.\n");
  fd1 = open(FILE1, O_RDONLY);
  fd2 = open(FILE2, O_RDONLY);
  
  read(fd1, stringa1, STR_LEN);
  read(fd2, stringa2, STR_LEN);
  
  printf("PADRE: il contenuto del file %s e' %s.\n", FILE1, stringa1);
  printf("PADRE: il contenuto del file %s e' %s.\n", FILE2, stringa2);
  
  close(fd1);
  close(fd2);
  
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

