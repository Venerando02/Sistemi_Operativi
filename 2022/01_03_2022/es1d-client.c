/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un array di 10 caratteri inseriti da tastiera. Il
server fornisce al client un valore intero corrispondente al numero di caratteri ‘A’ presenti nel vettore.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Verrà considerato
errore se il numero di scambi dati tra client e dal server è pari ad 1. Si supponga di utilizzare i thread nel Server
per la gestione di più client.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 10

typedef struct DatiCondivisiS
{
  char v[SIZE];
  int NumLetteraA;
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
  
  printf("Connessione accettata...\n");
  
  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(D.v, SIZE, stdin);
    D.v[strlen(D.v) - 1] = '\0';
    
    printf("Hai inserito la stringa -> %s.\n", D.v);
    
    printf("\nInvio i dati.\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di caratteri 'A' e' %d.\n", D.NumLetteraA);
    
    printf("Vuoi continuare (0: per uscire)? ");
    scanf("%d", &val);
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione avvenuta.\n");
  exit(EXIT_SUCCESS);
}

