/*
Scrivere un programma che attiva N processi figli e successivamente esegue un’operazione di calcolo molto
lunga (che richiede alcuni secondi e può essere realizzata, ad esempio, mediante un ciclo for o while
opportunamente dimensionato). Si supponga che ciascun processo figlio, visualizzi il proprio PID e termini. Si
supponga di gestire la terminazione dei processi figli tramite l’uso dei segnali. Si supponga infine che il numero
N di processi figli da attivare sia necessariamente > 2 e sia passato al main() tramite parametri (argc,argv).
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 20E8

void SignalHandlerFiglio(int segnale)
{
  pid_t pid;
  int StatVal;
  
  while((pid = wait(&StatVal)) > 0)
  {
    if(WIFEXITED(StatVal))
    {
      printf("PADRE: processo figlio %d terminato correttamente.\n", pid);
      printf("PADRE: valore ritornato %d.\n", WEXITSTATUS(StatVal));
      return;
    }
  }
}

int main(int argc, char* argv[])
{
  int nChild, i;
  pid_t pid;
  struct sigaction action;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro nChild mancante.\n");
    exit(EXIT_FAILURE);
  }
  
  nChild = atoi(argv[1]);
  if(nChild < 2)
  {
    fprintf(stderr, "Inserisci un numero maggiore di 2.\n");
    exit(EXIT_FAILURE);
  }
  
  action.sa_handler = SignalHandlerFiglio;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGCHLD, &action, NULL);
  
  printf("PADRE: creo %d figli.\n", nChild);
  
  for(i = 0; i<nChild; i++)
  {
    pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0)
    {
      printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
      exit(1+i);
    }
  }
  
  printf("PADRE: eseguo una operazione di calcolo.\n");
  
  for(i = 0; i<N; i++);
  
  exit(EXIT_SUCCESS);
}


