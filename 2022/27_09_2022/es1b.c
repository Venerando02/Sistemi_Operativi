/*
Scrivere un programma che si comporti da processo padre e crei un solo processo figlio. Il processo padre dopo
aver creato il processo figlio, invia ad esso un segnale scelto dallo studente.
Il processo figlio, appena viene creato, aspetta l’arrivo del segnale (scelto dello studente) e poi termina
normalmente. All’arrivo del segnale (scelto dello studente) il processo figlio dovrà stampare a video la frase
“Segnale arrivato”.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: Segnale arrivato.\n");
    printf("FIGLIO: posso dunque concludere.\n");
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
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  printf("PADRE: invio SIGUSR1 al processo PID = %d.\n", pid);
  kill(pid, SIGUSR1);
  wait(NULL);
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}

