/*
Scrivere un programma che attiva un processo figlio. Questo processo figlio rimane in attesa di ricevere un
segnale dal processo padre. Il processo padre invia un segnale (ad esempio SIGUSR1), alla ricezione del quale,
il figlio dovrà stampare a video il proprio PID e il PID del processo padre. Successivamente, il processo padre
invierà un segnale di SIGTERM, attende la terminazione del figlio e conclude la propria esecuzione.
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
    printf("FIGLIO: PID = %d.\n", getpid());
    printf("FIGLIO: PPID = %d.\n", getppid());
    return; 
  }
  else if(segnale == SIGTERM)
  {
    printf("FIGLIO: posso concludere correttamente.\n");
    exit(EXIT_SUCCESS);
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
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  printf("PADRE: invio il segnale SIGUSR1 al figlio.\n");
  kill(pid, SIGUSR1);
  sleep(2);
  printf("PADRE: invio il segnale SIGTERM al figlio.\n");
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}
