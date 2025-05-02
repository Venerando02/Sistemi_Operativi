/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia una stringa inserita da tastiera. Il server fornisce
al client, il conteggio del numero di vocali presenti nella stringa.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define STR_LEN 128

typedef struct DatiCondivisiS
{
  char stringa[STR_LEN];
  int NumVocali;
} DatiCondivisiT;

int main(void)
{
  int sockfd, server_len, val;
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
  inet_aton("127.0.0.1", &server_address.sin_addr);
  server_address.sin_port = htons(9000);
  
  server_len = sizeof(server_address);
  
  printf("Invio richiesta di connessione...\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata dal server.\n");
  
  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(D.stringa, STR_LEN, stdin);
    D.stringa[strlen(D.stringa) - 1] = '\0';
    
    printf("Hai inserito la stringa: %s.\n", D.stringa);
    
    printf("\nInvio il dato.\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di vocali e' %d.\n", D.NumVocali);
    
    printf("\nVuoi continuare (0: per terminare)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione effettuata.\n");
  exit(EXIT_SUCCESS);
}

