/*
Creare un file di testo tramite un editor; inserire a piacere delle righe contenete del testo (almeno 5 righe
diverse contenente parole con spazi tra loro). Scrivere un programma che conta il numero di caratteri
complessivo contenuti nel file e stampa a video tale numero. Si utilizzino i parametri del main (argc e argv)
per passare da tastiera il nome del file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  int fd, cont = 0;
  char cinput;
  
  if(argc != 2)
  {
    fprintf(stderr, "Parametro mancante: NomeFile.\n");
    exit(EXIT_FAILURE);
  }
  fd = open(argv[1], O_RDONLY);
  if(fd == -1)
  {
    fprintf(stderr, "File non aperto.\n");
    exit(EXIT_FAILURE);
  }
  
  while(read(fd, &cinput, sizeof(cinput)) > 0)
  {
    cont++;
  }
  
  printf("Ho contato nel file -> %d caratteri.\n", cont);
  
  printf("Posso dunque concludere.\n");
  
  close(fd);
  exit(EXIT_SUCCESS);
}
