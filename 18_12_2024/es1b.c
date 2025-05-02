/*
Scrivere un programma che attiva un processo figlio. Questo processo figlio rimane in attesa di ricevere il segnale
SIGUSR1 dal processo padre. Non appena il processo padre invia tale segnale (SIGUSR1), il processo figlio
dovrà stampare a video il proprio PID e il PID del processo padre. Una volta fatto ciò, il processo figlio invierà
il segnale SIGUSR2 al processo padre, in risposta al quale il processo padre dovrà stampare una frase a scelta
dello studente, inviare il segnale SIGTERM, attendere la terminazione del figlio e concludere la propria
esecuzione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: mio padre e' %d.\n", getppid());
    kill(getppid(), SIGUSR2);
    return;
  }
  else if(segnale == SIGTERM)
  {
    printf("FIGLIO: sono stato terminato dal padre.\n");
    exit(EXIT_SUCCESS);
  }
}

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGUSR2)
  {
    printf("PADRE: ho ricevuto correttamente %d.\n", SIGUSR2);
    return;
  }
}

int main(void)
{
  pid_t pid;
  struct sigaction action;
  
  action.sa_handler = SignalHandlerFiglio;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  printf("PADRE: creo un processo figlio.\n");
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  action.sa_handler = SignalHandlerPadre;
  sigaction(SIGUSR2, &action, NULL);
  
  printf("PADRE: invio il segnale SIGUSR1 (%d).\n", SIGUSR1);
  kill(pid, SIGUSR1);
  pause();
  printf("PADRE: invio il segnale SIGTERM (%d).\n", SIGTERM);
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}
