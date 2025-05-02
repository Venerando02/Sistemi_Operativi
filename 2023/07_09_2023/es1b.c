/*
Scrivere un programma che crei N processi figli. Si utilizzino i parametri del main (argc e argv) per passare il
numero (N) di figli da creare.
Una volta creati i processi figli, il processo padre entra in un ciclo for caratterizzato da un numero di iterazioni
molto alto (superiore al milione di iterazioni); per semplicità si supponga di inserire l’istruzione vuota (solamente
il ;) all’interno del ciclo. Una volta concluso il ciclo, il processo padre termina.
Ciascun processo figlio, genera un numero causale inferiore a 100, esegue anche lui un ciclo con un numero di
iterazioni pari al numero generato (anche in questo caso è possibile per semplicità inserire l’istruzione vuota,
ossia il solo ;). Una volta concluso il ciclo, il processo figlio termina.
Il processo padre deve gestire la terminazione dei processi figli UNICAMENTE tramite l’uso dei segnali,
definendo un’opportuna funzione di SignalHandler che eviti che i figli diventino zombie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define N 20E8
#define LIM 100

void SignalHandlerPadre(int segnale)
{
  pid_t pid;
  int StatVal;
  while((pid = wait(&StatVal)) > 0)
  {
    if(WIFEXITED(StatVal))
    {
      printf("PADRE: processo %d terminato in modo NORMALE.\n", pid);
      printf("PADRE: valore ritornato %d.\n", WEXITSTATUS(StatVal));
      return;
    }
  }
}

int main(int argc, char* argv[])
{
  pid_t pid;
  int nChild, i, j,  NumCasuale;
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
  
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: genero %d figli.\n", nChild);
  
  for(i = 0; i<nChild; i++)
  {
    pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0)
    {
      NumCasuale = rand() % LIM;
      for(j = 0; j < NumCasuale; j++);
      printf("FIGLIO: posso concludere.\n");
      exit(i+1);
    }
  } 
  
  printf("PADRE: eseguo un calcolo lungo.\n");
  for(j = 0; j<N; j++);
  
  exit(EXIT_SUCCESS);
}
