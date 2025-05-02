/*
Scrivere un programma che fa uso dei segnali per la gestione dei processi Zombie. Si supponga che il processo
padre crei un insieme di processi figli. La terminazione di tali processi deve essere gestita tramite un apposito
signal handler, come spiegato a lezione. Lo studente può liberamente scegliere il contenuto del programma
purché rispetti le indicazioni fornite. Se desidera, lo studente può replicare l’esempio fatto a lezione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define N 20E8
#define N_CHILD 5

void SignalHandlerPadre(int segnale)
{
  pid_t pid;
  int StatVal;
  while((pid = wait(&StatVal)) > 0)
  {
    if(WIFEXITED(StatVal))
    {
      printf("PADRE: il processo %d e' terminato in modo NORMALE.\n", pid);
      printf("PADRE: il valore ritornato e' %d.\n", WEXITSTATUS(StatVal));
      return;
    }
  }
}

int main(void)
{
  pid_t pid;
  int i;
  struct sigaction action;
  
  action.sa_handler = SignalHandlerPadre;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  
  sigaction(SIGCHLD, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo %d figli.\n", N_CHILD);
  
  for(i = 0; i<N_CHILD; i++)
  {
    pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0)
    {
      printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
      exit(i+1);
    }
  }
  
  printf("PADRE: eseguo una operazione di calcolo lungo.\n");
  
  for(i = 0; i<N; i++);
  
  exit(EXIT_SUCCESS);
}

