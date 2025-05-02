/*
Creare un file di testo tramite un editor (ad esempio gedit); inserire a piacere delle righe contenete del testo
(almeno 5 righe diverse contenente parole con spazi tra loro). Scrivere un programma in C che copia il file
precedentemente creato in un altro file (di nome diverso). Si supponga che il main() riceva tra i parametri
attuali (tramite argv[] e argc) un numero intero. Il processo di copia deve iniziare a partire dal byte
identificato dal numero intero, fino alla fine del file. Ad esempio, se il main() riceve il numero 4, allora la
copia del file deve essere fatta a partire dal byte n.4 fino alla fine. Gli altri parametri del main (argc e argv)
sono rappresentati dal nome del file da copiare e il nome del file relativo alla copia.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  int fd1, fd2, NumIntero;
  char ch;
  
  if(argc != 4)
  {
    fprintf(stderr, "Parametri mancanti: NomeFileInput, NumeroByte, NomeFileOutput.\n");
    exit(EXIT_FAILURE);
  }
  NumIntero = atoi(argv[2]);
  if(NumIntero < 0)
  {
    fprintf(stderr, "Inserisci un numero positivo.\n");
    exit(EXIT_FAILURE);  
  }
  fd1 = open(argv[1], O_RDONLY);
  fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if(fd2 == -1)
  {
    fprintf(stderr, "Impossibile creare il file.\n");
    exit(EXIT_FAILURE);
  }
  
  lseek(fd1, NumIntero, SEEK_SET);
  
  printf("Copio %s su %s a partire dal byte %d.\n", argv[1], argv[3], NumIntero);
  
  while(read(fd1, &ch, sizeof(ch)) > 0)
  {
    write(fd2, &ch, sizeof(ch));
  }
  close(fd1);
  close(fd2);
  printf("Posso terminare.\n");
  exit(EXIT_SUCCESS);
}

