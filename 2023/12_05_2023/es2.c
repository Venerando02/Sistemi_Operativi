/*
Scrivere un programma che faccia uso del meccanismo dei processi padre e figlio, e che utilizzi i segnali. Si
supponga che il processo padre crei un processo figlio, che rimane sempre in attesa di ricevere segnali dal
processo padre. Il processo padre può inviargli: SIGUSR1, SIGUSR2 e SIGTERM. Si supponga che il processo
figlio abbia una variabile count di tipo int, inizializzata a zero. Ogni volta che riceve il segnale SIGUSR1 la
variabile count verrà incrementata, se riceve SIGUSR2 la variabile count sarà decrementata. Infine, se il processo
padre vuole terminare, il figlio invia il segnale SIGTERM. Si supponga di realizzare un menù nel processo padre
in modo che l’utente che esegue il programma possa scegliere il segnale da inviare al processo figlio.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int count = 0;

int menu()
{
  int scelta;
  printf("*******MENU'*******\n\n");
  printf("1) SIGUSR1.\n");
  printf("2) SIGUSR2.\n");
  printf("3) SIGTERM.\n\n");
  printf(">>> ");
  scanf("%d", &scelta);
  return scelta;
}

void SignalHandlerFiglio(int segnale)
{
  if(segnale == SIGUSR1)
  {
    count++;
    printf("FIGLIO: valore di count = %d.\n", count);
    return;
  }
  else if(segnale == SIGUSR2)
  {
    count--;
    printf("FIGLIO: valore di count = %d.\n", count);
    return;
  }
  else
  {
    printf("FIGLIO: sono stato terminato da PPID = %d.\n", getppid());
    exit(EXIT_SUCCESS);
  }
}

int main(void)
{
  pid_t pid;
  struct sigaction action;
  int scelta;
  
  action.sa_handler = SignalHandlerFiglio;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  
  sigaction(SIGUSR1, &action, NULL);
  sigaction(SIGUSR2, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
 
  printf("PADRE: sono il processo con PID = %d.\n", getpid());
  printf("PADRE: creo il processo figlio.\n");
  
  pid = fork();
  if(pid == -1) exit(EXIT_FAILURE);
  if(pid == 0)
  {
    for(;;) pause();
  }
  
  do
  {
    scelta = menu();
    switch(scelta)
    {
      case 1:
      {
        printf("PADRE: invio SIGUSR1.\n");
        kill(pid, SIGUSR1);
        break;
      }
      case 2:
      {
        printf("PADRE: invio SIGUSR2.\n");
        kill(pid, SIGUSR2);
        break;
      }
      case 3:
      {
        printf("PADRE: invio SIGTERM.\n");
        kill(pid, SIGTERM);
        wait(NULL);
        printf("PADRE: posso concludere.\n");
        exit(EXIT_SUCCESS);
      }
    }
  }  while(scelta != 0);
}

