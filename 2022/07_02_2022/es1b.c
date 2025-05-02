/*
Scrivere un programma che si comporti da processo padre e crei un certo numero di processi figli. Si utilizzino
i parametri del main (argc e argv) per passare da tastiera il numero di figli da creare. Ciascun processo figlio
stampa a video una frase del tipo “Sono il processo figlio con PID = (valore del pid)” e termina. È richiesta la
gestione della terminazione dei processi figli tramite l’uso dei segnali, definendo una opportuna funzione di
SignalHandler che eviti che i figli diventino zombie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define N 20E8

void SignalHandlerPadre(int segnale)
{
  pid_t pid;
  int StatVal;
  
  while((pid = wait(&StatVal)) > 0)
  {
    if(WIFEXITED(StatVal))
    {
      printf("PADRE: processo %d concluso correttamente.\n", pid);
      printf("PADRE: valore di ritorno -> %d.\n", WEXITSTATUS(StatVal));
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
    fprintf(stderr, "Parametro mancante: nChild.\n");
    exit(EXIT_FAILURE);
  }
  
  nChild = atoi(argv[1]);
  
  if(nChild < 0)
  {
    fprintf(stderr, "Inserisci un numero positivo.\n");
    exit(EXIT_FAILURE);  
  }
  
  action.sa_handler = SignalHandlerPadre;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGCHLD, &action, NULL);
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo %d figli.\n", nChild);
  
  for(i = 0; i<nChild; i++)
  {
    pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0)
    {
      printf("Sono il processo figlio con PID = %d.\n", getpid());
      exit(1+i);
    }
  }
  
  printf("PADRE: eseguo un calcolo lungo.\n");
  for(i = 0; i<N; i++);
  exit(EXIT_SUCCESS);
}


