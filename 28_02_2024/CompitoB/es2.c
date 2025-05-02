/*
Si risolva il seguente problema di precedenza. Si supponga che due processi padre/figlio condividano un file di
testo. È richiesto che il processo padre scriva, per primo, la frase “CompitoB”, e che il processo figlio conti il
numero di caratteri contenuto nel file, appena il processo padre ha terminato la scrittura nel file. Non appena il
processo figlio ha completato il conteggio, deve visualizzare il valore numerico ottenuto sullo schermo e poi
terminare. Appena il figlio termina, anche il processo padre può terminare.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "semafori.h"

#define NOME_FILE "input.txt"
#define CHIAVE_SEMAFORO (key_t)1234

int main(void)
{
  int fd, cont, semaforo;
  pid_t pid;
  char c;
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: file creato correttamente.\n");
  
  semaforo = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(semaforo == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(semaforo, 0) == -1) exit(EXIT_FAILURE);
  
  printf("PADRE: Semaforo settato correttamente.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_P(semaforo) == -1) exit(EXIT_FAILURE);
    cont = 0;
    lseek(fd, 0, SEEK_SET);
    printf("FIGLIO: effettuo il conteggio.\n");
    while(read(fd, &c, sizeof(c)) > 0)
    {
      cont++;
    }
    printf("FIGLIO: il numero di caratteri contenuto in %s e' %d.\n", NOME_FILE, cont);
    exit(EXIT_SUCCESS);
  }
  printf("PADRE: scrivo una frase su %s.\n", NOME_FILE);
  write(fd, "CompitoB", 8);
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: in attesa di sincronizzarmi col processo figlio.\n");
  wait(NULL);
  close(fd);
  printf("PADRE: posso terminare.\n");
  if(SEM_DEL(semaforo) == -1) exit(EXIT_FAILURE);
  exit(EXIT_SUCCESS);
}
