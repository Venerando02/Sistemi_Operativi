/*
Scrivere un programma che faccia uso del meccanismo dei processi padre e figlio, e che utilizzi i segnali. Si
supponga che il processo padre crei un processo figlio, che rimane in attesa di ricevere segnali dal processo padre.
Il processo padre invierà il segnale SIGUSR1 al processo figlio e ne attenderà la terminazione.
Il processo figlio, dopo aver ricevuto il segnale SIGUSR1, stamperà a video il proprio PID e una stringa a piacere
dello studente, risponderà al padre inviando il segnale SIGUSR2 e terminerà la propria esecuzione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void SignalHandlerFiglio(int segnale)
{
  char * Stringa = "Stringa a piacere dello studente.";
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: stampo -> %s \n", Stringa);
    kill(getppid(), SIGUSR2);
    exit(EXIT_SUCCESS);
  }
}

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGUSR2)
  {
    printf("PADRE: ho ricevuto correttamente SIGUSR2.\n");
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
  
  printf("PADRE: invio il segnale SIGUSR1 al figlio.\n");
  kill(pid, SIGUSR1);
  pause();
  printf("PADRE: mi sincronizzo con la terminazione del processo figlio.\n");
  wait(NULL);
  exit(EXIT_SUCCESS);
}

