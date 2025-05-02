/*
Scrivere un programma che crea due processi (uno padre e uno figlio). Il processo padre invia al processo figlio
3 segnali: SIGUSR1, SIGUSR2 e SIGTERM. I segnali vengono inviati dal processo padre in sequenza,
intervallati da almeno 2 secondi di pausa. Per ogni segnale ricevuto dal processo figlio, lo studente deve
prevedere una funzione di signal handler che venga eseguita all’arrivo di esso.
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
    printf("FIGLIO: ho ricevuto il segnale SIGUSR1 da PPID = %d.\n", getppid());
    return;
  }
  else if(segnale == SIGUSR2)
  {
    printf("FIGLIO: ho ricevuto il segnale SIGUSR2 da PPID = %d.\n", getppid());
    return;  
  }
  else
  {
    printf("FIGLIO: ho ricevuto il segnale SIGTERM da PPID = %d.\n", getppid());
    printf("FIGLIO: concludo.\n");
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
  sigaction(SIGUSR2, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo un processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause(); 
  }
  
  printf("PADRE: invio il segnale SIGUSR1.\n");
  kill(pid, SIGUSR1);
  sleep(2);
  
  printf("PADRE: invio il segnale SIGUSR2.\n");
  kill(pid, SIGUSR2);
  sleep(2);
  
  printf("PADRE: invio il segnale SIGTERM.\n");
  kill(pid, SIGTERM);
  wait(NULL);
  
  printf("PADRE: posso dunque terminare.\n");
  exit(EXIT_SUCCESS);
}

