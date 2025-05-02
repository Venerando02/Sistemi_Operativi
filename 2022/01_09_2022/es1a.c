/*
Scrivere un programma che crea due processi (uno padre e uno figlio) ed utilizzi i parametri del main() argc e
argv. Entrambi i processi accedono ad uno stesso file condiviso, il cui nome viene passato come primo
parametro del main. Si supponga che il processo figlio scriva nel file un determinato numero di caratteri (l
numero di quanti caratteri scrivere viene passato come secondo parametro del main). Appena il processo figlio
conclude la propria esecuzione, il processo padre deve copiare il contenuto del file appena creato, in un secondo
file, il cui nome viene passato come terzo parametro del main.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
  int fd1, fd2, NumCaratteriMax, cont = 0;
  char cinput;
  pid_t pid;
  
  if(argc != 4)
  {
    fprintf(stderr, "Mancano i seguenti parametri: NomeFile1, NumCaratteriMax, NomeFile2.\n");
    exit(EXIT_FAILURE);
  }
  fd1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd1 == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  NumCaratteriMax = atoi(argv[2]);
  if(NumCaratteriMax <= 0)
  {
    fprintf(stderr, "Inserisci un valore positivo.\n");
    exit(EXIT_FAILURE);  
  }
  
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: scrivo %d caratteri su %s.\n", NumCaratteriMax, argv[1]);
    
    while(cont < NumCaratteriMax)
    {
      printf("FIGLIO: inserisci un carattere: ");
      scanf("%c", &cinput);
      while(getchar() != '\n');
      write(fd1, &cinput, sizeof(cinput));
      cont++;
    }
    
    printf("FIGLIO: posso dunque concludere.\n");
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: mi sincronizzo con la terminazione del processo figlio.\n");
  wait(NULL);
  
  lseek(fd1, 0, SEEK_SET);
  printf("PADRE: apro il file %s.\n", argv[3]);
  
  fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: copio il contenuto di %s su %s.\n", argv[1], argv[3]);
  while(read(fd1, &cinput, sizeof(cinput)) > 0)
  {
    write(fd2, &cinput, sizeof(cinput));
  }
  close(fd1);
  close(fd2);
  printf("PADRE: posso dunque concludere.\n");
  exit(EXIT_SUCCESS);
}

