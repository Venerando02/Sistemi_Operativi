/*
Scrivere un programma che crea un processo figlio. Il processo padre, ciclicamente, per 3 iterazioni, invia il
segnale SIGUSR1 al processo figlio e va in sleep per 1 secondo. Dopo le 3 iterazioni invia un segnale per
terminare il processo figlio.
Il processo figlio, ciclicamente, attende un segnale dal padre. Se arriva il segnale SIGUSR1, stampa a video il
proprio PID. Se arriva il segnale di terminazione, stampa a video un messaggio a scelta dello studente e termina
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    return;
  }
 else if(segnale == SIGTERM)
 {
   printf("FIGLIO: sono stato terminato da PPID = %d.\n", getppid());
   exit(EXIT_SUCCESS);
 }
}

int main(void)
{
  struct sigaction action;
  pid_t pid;
  int i;
  
  action.sa_handler = SignalHandlerFiglio;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  printf("PADRE: invio SIGUSR1 per 3 volte.\n");
  for(i = 0; i<3; i++)
  {
    kill(pid, SIGUSR1);
    sleep(1);
  }
  
  printf("PADRE: invio SIGTERM al figlio.\n");
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: concludo.\n");
  exit(EXIT_SUCCESS);
}

