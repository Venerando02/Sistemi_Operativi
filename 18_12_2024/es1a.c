/*
Si risolva il seguente problema di precedenza. Si supponga che due processi padre/figlio condividano un file
di testo. È richiesto che il processo padre scriva, per primo, la frase “Esame di Sistemi Operativi”. Quando il
processo padre ultima la scrittura nel file, il processo figlio deve leggere gli ultimi 18 caratteri. Quando il
processo figlio completa la lettura, deve visualizzare sullo schermo quanto letto dal file e poi terminare. Quando
il figlio termina, anche il processo padre può terminare.
Risolvere il problema utilizzando il meccanismo di sincronizzazione (scegliendo tra semafori e segnali) che lo
studente ritenga più adatto.
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
#define STR_LEN 128

int main(void)
{
  int semaforo, fd;
  pid_t pid;
  char message[STR_LEN] = "Esame di Sistemi Operativi";
  char lettura[STR_LEN];
  
  fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  semaforo = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(semaforo == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(semaforo, 0) == -1)
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
    if(SEM_P(semaforo) == -1) exit(EXIT_FAILURE);
    lseek(fd, -18, SEEK_END);
    printf("FIGLIO: leggo gli ultimi 18 caratteri.\n");
    read(fd, lettura, STR_LEN);
    printf("FIGLIO: ho letto -> %s \n", lettura);
    printf("FIGLIO: termino.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, message, strlen(message));
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: mi sincronizzo con la terminazione del processo.\n");
  wait(NULL);
  if(SEM_DEL(semaforo) == -1)
  {
    fprintf(stderr, "Impossibile eliminare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  close(fd);
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}


