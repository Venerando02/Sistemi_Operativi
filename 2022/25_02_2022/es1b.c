/*
Scrivere un programma che crea quattro processi (uno padre e tre figli). Il processo padre, dopo aver generato i
processi figli, ciclicamente, per 6 iterazioni invia il segnale SIGUSR1 ad uno dei processi figli scelto casualmente
e va in sleep per 1 secondo. Dopo le 6 iterazioni invia un segnale SIGUSR2 a tutti i processi figli, attende la loro
terminazione e termina esso stesso.
Ogni processo figlio, ciclicamente, attende un segnale dal padre. Se arriva il segnale SIGUSR1, stampa a video
il proprio PID. Se arriva il segnale SIGUSR2, stampa a video un messaggio a scelta dello studente e termina.
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
  else if(segnale == SIGUSR2) 
  {
    printf("FIGLIO: sono stato terminato da PPID = %d.\n", getppid());
    exit(EXIT_SUCCESS);
  }
}

int main(void)
{
  pid_t* p = NULL;
  int figlio, i, nChild = 3;
  struct sigaction action;
  
  action.sa_handler = SignalHandlerFiglio;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGUSR2, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo %d figli.\n", nChild);
  
  p = (pid_t*)malloc(sizeof(pid_t)*nChild);
  for(i = 0; i<nChild; i++)
  {
    p[i] = fork();
    if(p[i] == -1) exit(EXIT_FAILURE);
    if(p[i] == 0)
    {
      for(;;) pause();
    }
  }
  
  for(i = 0; i < 6; i++)
  {
    figlio = rand() % nChild;
    printf("PADRE: invio SIGUSR1 al processo figlio PID = %d.\n", p[figlio]);
    kill(p[figlio], SIGUSR1);
    sleep(1);
  }
  
  printf("PADRE: termino tutti i processi figli.\n");
  for(i = 0; i<nChild; i++)
  {
    kill(p[i], SIGUSR2);
    wait(NULL);
  }
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

