/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia una stringa di dimensione scelta a piacere dallo studente, il cui valore viene assegnato di volta in
volta mediante lettura da tastiera. Il server fornisce al client il numero di vocali contenuti nella stringa.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread in modalità detached nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define STR_LEN 256

typedef struct DatiCondivisiS
{
  char Stringa[STR_LEN];
  int NumVocali;
} DatiCondivisiT;


int main(void)
{
  int sockfd, server_len, val, i;
  struct sockaddr_in server_address;
  DatiCondivisiT D;
  
  printf("CLIENT - PID = %d.\n", getpid());
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1)
  {
    fprintf(stderr, "Impossibile creare la socket.\n");
    exit(EXIT_FAILURE);
  }
  
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9000);
  inet_aton("127.0.0.1", &server_address.sin_addr);
  
  server_len = sizeof(server_address);
  
  printf("Invio richiesta di connessione al SERVER.\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata...\n");

  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(D.Stringa, STR_LEN, stdin);
    for(i = 0; D.Stringa[i] != '\0'; i++) if(D.Stringa[i] == '\n') D.Stringa[i] = '\0';
    
    printf("Hai inserito -> %s \n", D.Stringa);
    
    printf("Invio dei dati...\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di vocali nella stringa e' %d \n", D.NumVocali);
    do
    {
      printf("Vuoi continuare (0: no, 1: si)? ");
      scanf("%d", &val);
      while(getchar() != '\n');
    } while((val != 0) && (val != 1));
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}
