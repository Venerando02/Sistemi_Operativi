/*
Scrivere un programma sull’utilizzo dei segnali. Il programma deve istanziare un processo padre e un processo
figlio che si scambiano i segnali SIGUSR1 e SIGUSR2. Il processo padre, appena avviato, deve stampare a
video delle informazioni a scelta del candidato e dopo aver effettuato questa operazione, deve passare il
controllo al processo figlio tramite l’invio di un segnale, che stamperà a sua volta altre informazioni. Terminata
questa operazione, il processo figlio passerà il controllo nuovamente al processo padre (sempre tramite l’invio
di un segnale), che termina il processo figlio e conclude.
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
    printf("FIGLIO: invio SIGUSR2 al processo padre.\n");
    kill(getppid(), SIGUSR2);
    printf("FIGLIO: termino.\n");
    exit(EXIT_SUCCESS);
  }
}

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGUSR2)
  {
    printf("PADRE: ho ricevuto il segnale SIGUSR2 dal figlio.\n");
    return;
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
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  action.sa_handler = SignalHandlerPadre;
  sigaction(SIGUSR2, &action, NULL);
  
  printf("PADRE: invio il segnale SIGUSR1 al processo PID = %d.\n", pid);
  kill(pid, SIGUSR1);
  pause();
  printf("PADRE: posso dunque terminare il processo figlio.\n");
  wait(NULL);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

