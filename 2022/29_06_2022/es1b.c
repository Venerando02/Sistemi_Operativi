/*
Supporre che vi sia un processo padre che crea un processo figlio. I due processi si scambiano diversi segnali. In
particolare, il processo padre invia il segnale SIGUSR1 al processo figlio, che reagisce inviando il segnale
SIGUSR2 al padre. Il processo padre, una volta ricevuto il segnale SIGUSR2, termina il processo figlio. Per ogni
segnale ricevuto dai processi padre e figlio, lo studente deve prevedere una funzione di signal handler che venga
eseguita all’arrivo di esso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGUSR2)
  {
    printf("PADRE: ho ricevuto correttamente SIGUSR2.\n");
    return;
  }
}

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: ho ricevuto correttamente SIGUSR1 dal processo PADRE PID = %d.\n", getppid());
    printf("FIGLIO: invio SIGUSR2 al padre.\n");
    kill(getppid(), SIGUSR2);
    return;
  }
  if(segnale == SIGTERM)
  {
    printf("FIGLIO: sono stato terminato da PPID = %d.\n", getppid());
    exit(EXIT_SUCCESS);
  }
}

int main(void)
{
  pid_t pid;
  struct sigaction action;
  
  action.sa_handler = SignalHandlerFiglio;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  action.sa_handler = SignalHandlerPadre;
  sigaction(SIGUSR2, &action, NULL);
  
  printf("PADRE: invio SIGUSR1.\n");
  kill(pid, SIGUSR1);
  pause();
  printf("PADRE: termino il processo.\n");
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}

