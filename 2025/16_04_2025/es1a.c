/*
Si supponga che due processi padre e figlio condividano un file di testo. È richiesto che il processo padre
scriva nel file, per primo, una frase a scelta dello studente. Appena il processo padre ha terminato la scrittura
nel file, il processo figlio deve eseguire la lettura del contenuto del file, deve visualizzare la frase letta sullo
schermo e poi terminare. Si realizzi il programma utilizzando i semafori per la sincronizzazione dei due
processi padre e figlio. Si ricordi che il problema proposto è un problema di precedenza e non necessita
della gestione della regione critica. Soluzioni differenti dai semafori e che risolvano problemi differenti
da quelli della precedenza (ad esempio produttore/consumatore), non saranno prese in considerazione.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define FILENAME "input.txt"
#define STR_LEN 256

int main(void)
{
  int fd, semID;
  pid_t pid;
  char message[STR_LEN] = "Ciao! Come stai?";
  char * StringaLetta = NULL;
  
  fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  semID = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(semID == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(semID, 0) == -1)
  {
    fprintf(stderr, "Impossibile settare il semaforo.\n");
    exit(EXIT_FAILURE);    
  }
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo un figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_P(semID) == -1)
    {
      fprintf(stderr, "P() fallita.\n");
      exit(EXIT_FAILURE);
    }
    lseek(fd, 0, SEEK_SET);
    StringaLetta = (char*)malloc(sizeof(char)*STR_LEN);
    read(fd, StringaLetta, STR_LEN);
    printf("FIGLIO: ho letto la stringa -> %s \n", StringaLetta);
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, message, strlen(message));
  if(SEM_V(semID) == -1)
  {
    fprintf(stderr, "V() fallita.\n");
    exit(EXIT_FAILURE);
  }
  printf("PADRE: in attesa di sincronizzarmi col processo figlio.\n");
  wait(NULL);
  if(SEM_DEL(semID) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);    
  }
  close(fd);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}


