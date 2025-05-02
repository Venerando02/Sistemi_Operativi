/*
Scrivere un programma che crei N processi figli. Si utilizzino i parametri del main (argc e argv) per passare il
numero (N) di figli da creare. Una volta creati i processi, il processo padre invia SIGUSR1 a tutti i processi figli.
Ciascun processo figlio, che è in costante attesa di un segnale, appena riceve SIGUSR1 stampa il proprio PID e
termina.
Il padre deve gestire la terminazione dei processi figli tramite l’uso dei segnali, definendo una opportuna funzione
di SignalHandler che eviti che i figli diventino zombie.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define N 20E8

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    printf("FIGLIO: sono il processo con PID = %d.\n", getpid());
    printf("FIGLIO: posso dunque terminare.\n");
    sleep(1);
    exit(EXIT_SUCCESS);
  }
}

void SignalHandlerPadre(int segnale)
{
  if(segnale == SIGCHLD)
  {
    pid_t pid;
    int StatVal;
    
    while((pid = wait(&StatVal)) > 0)
    {
      if(WIFEXITED(StatVal))
      {
        printf("PADRE: processo %d terminato correttamente. \n", pid);
        printf("PADRE: valore restituito %d. \n", WEXITSTATUS(StatVal));
        return;
      }
    }
  }
}

int main(int argc, char * argv[])
{
  int nChild, i;
  pid_t pid;
  struct sigaction actionFather, actionChild;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: Nchild.\n");
    exit(EXIT_FAILURE);
  }
  
  nChild = atoi(argv[1]);
  
  if(nChild < 0)
  {
    fprintf(stderr, "Inserisci un numero positivo.\n");
    exit(EXIT_FAILURE);
  }
  actionChild.sa_flags = 0;
  sigemptyset(&actionChild.sa_mask);
  actionChild.sa_handler = SignalHandlerFiglio;
  
  actionFather.sa_flags = 0;
  sigemptyset(&actionFather.sa_mask);
  actionFather.sa_handler = SignalHandlerPadre;
  
  sigaction(SIGUSR1, &actionChild, NULL);
  sigaction(SIGCHLD, &actionFather, NULL);
  
  printf("PADRE: creo %d figli.\n", nChild);
  
  for(i = 0; i<nChild; i++)
  {
    pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0)
    {
      for(;;) pause();
    }
    printf("PADRE: invio il segnale SIGUSR1 a %d.\n", pid);
    kill(pid, SIGUSR1); 
  }
  for(i = 0; i<N; i++);
  printf("PADRE: posso concludere.\n");
  exit(EXIT_SUCCESS);
}

