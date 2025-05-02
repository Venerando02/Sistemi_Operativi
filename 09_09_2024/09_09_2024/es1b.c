/*
Scrivere un programma che attiva un processo figlio. Questo processo figlio rimane in attesa di ricevere un
segnale dal processo padre. Il processo padre invia il segnale SIGUSR2, alla ricezione del quale, il figlio dovrà
stampare a video il proprio PID e andare in sleep per un numero di secondi pari a un numero casuale compreso
tra 2 e 6. Una volta fatto ciò, il processo figlio invierà il segnale SIGUSR2 al processo padre, in risposta al quale
il processo padre dovrà inviare il segnale di SIGTERM, attendere la terminazione del figlio e concludere la
propria esecuzione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define LIM 5

void SignalHandlerFiglio(int segnale)
{
  int secondi;
  if(segnale == SIGUSR2)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    secondi = ((rand() % LIM) + 2);
    printf("FIGLIO: vado in sleep per %d secondi.\n", secondi);
    sleep((unsigned int)secondi);
    kill(getppid(), SIGUSR2);
    return;
  }
  else if(segnale == SIGTERM)
  {
    printf("FIGLIO: sono stato concluso da PPID = %d.\n", getppid());
    exit(EXIT_SUCCESS);
  }
}

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGUSR2)
  {
    printf("PADRE: ho correttamente ricevuto SIGUSR2.\n");
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
  
  action.sa_handler = SignalHandlerPadre;
  sigaction(SIGUSR2, &action, NULL);
  
  printf("PADRE: invio SIGUSR2 al processo PID = %d.\n", pid);
  kill(pid, SIGUSR2);
  pause();
  printf("PADRE: invio SIGTERM al processo PID = %d.\n", pid);
  kill(pid, SIGTERM);
  wait(NULL);
  printf("PADRE: termino.\n");
  exit(EXIT_SUCCESS);
}

