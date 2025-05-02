/*
Scrivere un programma costituito da un ciclo infinito. Si supponga che internamente al ciclo venga eseguita una
stampa su schermo di una frase scelta a piacere dallo studente. Si supponga di impedire che l’utente termini il
processo tramite l’attivazione dei tasti CTRL+C, ovvero si voglia mascherare il segnale SIGINT prodotto
dall’attivazione dei tasti CTRL+C. È richiesto che tale segnale possa essere ignorato solo una volta, per cui la
ricezione del secondo segnale produce la terminazione del processo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void SignalHandlerPadre(int signo)
{
  printf("Imposto il comportamento di default.\n");
  struct sigaction action;
  
  action.sa_handler = SIG_DFL;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGINT, &action, NULL);
}

int main(void)
{
  struct sigaction action;
  
  action.sa_handler = SignalHandlerPadre;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  
  sigaction(SIGINT, &action, NULL);
  
  while(1)
  {
    printf("Sono il processo con PID = %d.\n", getpid());
    sleep(1);
  }
  
  printf("Termino il processo.\n");
  exit(EXIT_SUCCESS);
}

