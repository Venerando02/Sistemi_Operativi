/*
Scrivere un programma che si comporti da processo padre e crei un certo numero di processi figli. Si utilizzino
i parametri del main (argc e argv) per passare da tastiera il numero di figli da creare. Supporre che i figli
stampino a video una frase del tipo “Sono il processo figlio di PID = (valore del pid)” e terminino prima che il
padre termina. Gestire la terminazione dei processi figli tramite segnali, definendo una opportuna funzione di
SignalHandler che eviti che i figli diventino zombie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define N 20E8

void SignalHandlerPadre(int segnale)
{  
  pid_t pid;
  int StatVal;
  
  while((pid = wait(&StatVal)) > 0)
  {
    if(WIFEXITED(StatVal))
    {
      printf("PADRE: processo PID = %d terminato.\n", pid);
      printf("PADRE: valore ritornato -> %d.\n", WEXITSTATUS(StatVal));
      return;
    }
  }
}

int main(int argc, char* argv[])
{
  int i, nChild;
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
    fprintf(stderr, "Inserisci un valore positivo.\n");
    exit(EXIT_FAILURE);  
  }
  
  action.sa_handler = SignalHandlerPadre;
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
      printf("Sono il processo con PID = %d.\n", getpid());
      exit(i+1);
    }
  }
  
  printf("PADRE: eseguo un calcolo lungo.\n");
  for(i = 0; i<N; i++);
  exit(EXIT_SUCCESS);
}

