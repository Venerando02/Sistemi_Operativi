/*
Si risolva il seguente problema di precedenza tramite l’uso di semafori. Si supponga che due processi
padre/figlio condividano un file di testo. È richiesto che il processo padre scriva nel file, per primo, una frase
a scelta dello studente. Appena il processo padre ha terminato la scrittura nel file, il processo figlio deve eseguire
la lettura del contenuto del file, calcolare il numero di vocali in esso presente, visualizzare tale numero sullo
schermo e poi terminare. Il processo padre dovrà attendere la terminazione del processo figlio e poi terminare.
*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define FILENAME "input.txt"
#define STR_LEN 64

int main(void)
{
  int fd, SemID, cont_vocali = 0;
  pid_t pid;
  char message[STR_LEN] = "frase a scelta dello studente.\n";
  char cinput;
  
  fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibil creare il file.\n");
    exit(EXIT_FAILURE);
  }
  SemID = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(SemID == -1)
  {
    fprintf(stderr, "Impossibil creare il semaforo.\n");
    exit(EXIT_FAILURE);    
  }
  if(SEM_SET(SemID, 0) == -1)
  {
    fprintf(stderr, "Impossibil settare il semaforo.\n");
    exit(EXIT_FAILURE);      
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_V(SemID) == -1) exit(EXIT_FAILURE);
    lseek(fd, 0, SEEK_SET);
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: eseguo la lettura sul file.\n");
    while(read(fd, &cinput, sizeof(cinput)) > 0)
    {
      if((cinput == 'a') || (cinput == 'e') || (cinput == 'i') || (cinput == 'o') || (cinput == 'u')) cont_vocali++;
    }
    printf("FIGLIO: ho conteggiato %d vocali.\n", cont_vocali);
    printf("FIGLIO: termino.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: eseguo la scrittura sul file.\n");
  write(fd, message, strlen(message));
  if(SEM_V(SemID) == -1) exit(EXIT_FAILURE);
  printf("PADRE: sincronizzazione con la terminazione di PID = %d.\n", pid);
  wait(NULL);
  printf("PADRE: elimino il semaforo e chiudo il file.\n");
  close(fd);
  if(SEM_DEL(SemID) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
  
}

