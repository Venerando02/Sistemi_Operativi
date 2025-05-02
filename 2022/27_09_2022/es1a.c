/*
Scrivere un programma che crea due processi (uno padre e uno figlio) ed utilizza i parametri del main() argc e
argv. Entrambi i processi accedono ad uno stesso file condiviso, il cui nome viene passato come primo
parametro del main.
Si supponga che il processo figlio scriva nel file una stringa inserita da tastiera. Appena il processo figlio
conclude la propria esecuzione, il processo padre deve contare il numero di caratteri scritti dal processo figlio,
visualizzando tale numero sullo schermo, e successivamente deve aggiungere alla fine del file un determinato
numero di caratteri ‘-‘ (il numero di ‘-‘ da scrivere viene passato come secondo parametro del main).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define STR_LEN 128

int main(int argc, char * argv[])
{
  int fd, NumCaratteri, cont1 = 0, cont2 = 0;
  pid_t pid;
  char stringa[STR_LEN];
  char cinput;
  
  if(argc != 3)
  {
    fprintf(stderr, "Parametro mancante: NomeFile, NumCaratteri.\n");
    exit(EXIT_FAILURE);
  }
  NumCaratteri = atoi(argv[2]);
  if(NumCaratteri < 0)
  {
    fprintf(stderr, "Inserisci un valore positivo.\n");
    exit(EXIT_FAILURE);  
  }
  fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    printf("FIGLIO: inserisci una stringa: ");
    fgets(stringa, STR_LEN, stdin);
    stringa[strlen(stringa)-1] = '\0';
    write(fd, stringa, strlen(stringa));
    printf("FIGLIO: posso dunque concludere.\n");
    exit(EXIT_SUCCESS);
  }
  
  wait(NULL);
  printf("PADRE: conto quanti caratteri ha scritto il processo %d.\n", pid);
  lseek(fd, 0, SEEK_SET);
  while(read(fd, &cinput, sizeof(char)) > 0)
  {
    cont1++;
  }
  printf("PADRE: sono stati contati %d caratteri.\n", cont1);
  while(cont2 < NumCaratteri)
  {
    printf("PADRE: inserisci un carattere: ");
    scanf("%c", &cinput);
    while(getchar() != '\n');
    write(fd, &cinput, sizeof(char));
    cont2++;
  }
  printf("PADRE: posso dunque terminare.\n");
  close(fd);
  exit(EXIT_SUCCESS);
}

