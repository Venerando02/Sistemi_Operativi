/*
Scrivere un programma che crea un processo figlio (dunque, il main() che rappresenta il processo padre e un
processo figlio creato dal processo padre). Il processo padre, dopo aver generato il processo figlio invia il segnale
SIGUSR1 al processo figlio e va in sleep per 1 secondo. Si supponga che il processo padre esegua queste
operazioni dentro un ciclo (si fissino a piacere il numero di iterazioni > 3). Appena concluso il ciclo, il processo
padre invia un segnale per terminare il processo figlio.
Il processo figlio attende un segnale dal padre. Se arriva il segnale SIGUSR1, stampa a video il proprio PID. Se
arriva il segnale di terminazione, stampa a video un messaggio a scelta dello studente e termina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 5

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
  pid_t pid;
  int i;
  
  struct sigaction action;
  
  action.sa_handler = SignalHandlerFiglio;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  printf("PADRE: invio il segnale SIGUSR1 al figlio per %d volte.\n", N);
  
  for(i = 0; i<N; i++)
  {
    kill(pid, SIGUSR1);
    printf("PADRE: segnale %d inviato.\n", SIGUSR1);
    sleep(1);
  }
  
  printf("PADRE: invio SIGTERM..\n");
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: posso terminare..\n");
  exit(EXIT_SUCCESS);
}

