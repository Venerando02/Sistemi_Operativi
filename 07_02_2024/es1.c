/*
Scrivere un programma che crea un processo figlio. Il processo figlio crea due file denominati “figlio1.txt”
e “figlio2.txt”, che vengono riempiti di caratteri, a scelta dello studente. Appena il processo figlio termina
di scrivere sui due file, il processo padre aprirà i due file e creerà un altro file di nome “padre.txt”. In questo
file dovrà scrivere l’intero contenuto del primo file “figlio1.txt” e, di seguito, l’intero contenuto del secondo
file “figlio2.txt”.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STR_LEN 64

int main(void)
{
  char cinput, cterm;
  int f1, f2, f3;
  pid_t pid;
  char stringa[STR_LEN];
  
  printf("PADRE: sono il processo PID = %d.\n", getpid());
  printf("PADRE: scegli un carattere di terminazione: ");
  scanf("%c", &cterm);
  while(getchar() != '\n');
  
  f3 = open("padre.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(f3 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    f1 = open("figlio1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(f1 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    f2 = open("figlio2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(f2 == -1)
    {
      fprintf(stderr, "Impossibile aprire il file.\n");
      exit(EXIT_FAILURE);
    }
    
    printf("FIGLIO: scrittura su figlio1.txt.\n");
    do
    {
      printf("Inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(f1, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: scrittura su figlio2.txt.\n");
    do
    {
      printf("Inserisci un carattere ('%c' per terminare): ", cterm);
      scanf("%c", &cinput);
      while(getchar() != '\n');
      if(cinput != cterm) write(f2, &cinput, sizeof(cinput));
    } while(cinput != cterm);
    
    printf("FIGLIO: chiudo i file.\n");
    close(f1);
    close(f2);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: in attesa di sincronizzazione col figlio.\n");
  wait(NULL);
  
  f1 = open("figlio1.txt", O_RDONLY);
  f2 = open("figlio2.txt", O_RDONLY);
  
  printf("PADRE: copio figlio1.txt su padre.txt\n");
  while(read(f1, stringa, STR_LEN) > 0)
  {
    write(f3, stringa, strlen(stringa));
  } 
  printf("PADRE: copio figlio2.txt su padre.txt\n");
  while(read(f2, stringa, STR_LEN) > 0)
  {
    write(f3, stringa, strlen(stringa));
  }
  close(f1);
  close(f2);
  close(f3);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}


