/*
Si risolva il seguente problema di precedenza tramite l’uso di semafori. Si supponga che due processi
padre/figlio condividano un file di testo. È richiesto che il processo padre scriva nel file, per primo, una frase
a scelta dello studente. Appena il processo padre ha terminato la scrittura nel file, il processo figlio deve eseguire
la lettura del contenuto del file, calcolare il numero di vocali in esso presente, visualizzare tale numero sullo
schermo e poi terminare. Il processo padre dovrà attendere la terminazione del processo figlio e poi terminare.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "semafori.h"

#define CHIAVE_SEMAFORO (key_t)1234
#define STR_LEN 64
#define NOME_FILE "input.txt"

int main(void)
{
  int semaforo, fd, cont;
  pid_t pid;
  char message[STR_LEN] = "Ciao! Come va? Tutto bene?";
  char ch;
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  
  fd = open(NOME_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if(fd == -1)
  {
    fprintf(stderr, "Impossibile aprire il file.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("PADRE: file aperto correttamente.\n");
  
  semaforo = semget(CHIAVE_SEMAFORO, 1, IPC_CREAT | 0666);
  if(semaforo == -1)
  {
    fprintf(stderr, "Impossibile creare il semaforo.\n");
    exit(EXIT_FAILURE);
  }
  if(SEM_SET(semaforo, 0) == -1) exit(EXIT_FAILURE);
  
  printf("PADRE: semaforo settato correttamente.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    if(SEM_P(semaforo) == -1) exit(EXIT_FAILURE);
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    lseek(fd, 0, SEEK_SET);
    cont = 0;
    while(read(fd, &ch, sizeof(ch)) > 0)
    {
      if((ch == 'a') || (ch == 'e') || (ch == 'i') || (ch == 'o') || (ch == 'u'))
      {
        cont++;
      }
    } 
    printf("FIGLIO: ho visualizzato %d vocali.\n", cont);
    printf("FIGLIO: posso terminare.\n");
    exit(EXIT_SUCCESS);
  }
  
  printf("PADRE: scrivo una frase sul file.\n");
  write(fd, message, strlen(message));
  if(SEM_V(semaforo) == -1) exit(EXIT_FAILURE);
  printf("PADRE: in attesa che termini il processo figlio.\n");
  wait(NULL);
  if(SEM_DEL(semaforo) == -1) exit(EXIT_FAILURE);
  close(fd);
  printf("PADRE: semaforo eliminato e file chiuso.\n");
  printf("PADRE: posso concludere correttamente.\n");
  exit(EXIT_SUCCESS);
} 

