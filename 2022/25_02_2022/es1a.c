/*
Scrivere un programma che crea due processi (uno padre e uno figlio) ed utilizzi i parametri del main() argc e
argv. Entrambi i processi accedono ad uno stesso file condiviso, il cui nome viene passato come primo
parametro. Si supponga che il processo figlio scriva nel file un determinato numero di caratteri passato come
secondo parametro. Appena il processo figlio conclude la propria esecuzione, il processo padre deve
visualizzare un numero di caratteri pari al valore passato come terzo parametro, a partire dall’inizio del file. Si
suppone che il numero passato come terzo parametro sia sempre inferire al numero passato come secondo
parametro.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define STR_LEN 64

int main(int argc, char * argv[])
{
  int fd, nWrite, nRead, cont = 0, i = 0;
  char ch;
  char stringa[STR_LEN] = "";
  pid_t pid;
  
  if(argc != 4)
  {
    fprintf(stderr, "Inserisci 3 parametri: NomeFile, nWrite, nRead.\n");
    exit(EXIT_FAILURE);
  }
  
  nWrite = atoi(argv[2]);
  nRead = atoi(argv[3]);
  
  if(nRead > nWrite)
  {
    fprintf(stderr, "Il parametro nRead deve essere minore a nWrite.\n");
    exit(EXIT_FAILURE);
  }
  
  fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");

  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    while(cont < nWrite)
    {
      printf("FIGLIO: inserisci un carattere: ");
      scanf("%c", &ch);
      while(getchar() != '\n');
      write(fd, &ch, sizeof(ch));
      cont++;
    }
    printf("FIGLIO: concludo la scrittura, dunque termino.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: mi sincronizzo con la terminazione del processo.\n");
  wait(NULL);
  lseek(fd, 0, SEEK_SET);
  while(cont < nRead)
  {
    read(fd, &ch, sizeof(char));
    cont++;
    stringa[i++] = ch;
  }
  stringa[i] = '\0';
  printf("PADRE: ho visualizzato i seguenti caratteri -> %s \n", stringa);
  close(fd);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}
