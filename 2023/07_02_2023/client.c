/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un array di 10 numeri generati casualmente. Il
server fornisce al client la somma dei 10 numeri ricevuti.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga di
utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DIM 10
#define LIM 100

typedef struct DatiCondivisiS
{
  int v[DIM];
  int somma;
} DatiCondivisiT;

int main(void)
{
  int sockfd, server_len, i, val;
  struct sockaddr_in server_address;
  DatiCondivisiT D;
  
  printf("CLIENT - PID = %d.\n", getpid());
  
  sockfd = socket(AF_INET, SOCK_STREAM , 0);
  if(sockfd == -1)
  {
    fprintf(stderr, "Impossibile creare la socket.\n");
    exit(EXIT_FAILURE);
  }
  
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9000);
  inet_aton("127.0.0.1", &server_address.sin_addr);
  
  server_len = sizeof(server_address);
  
  printf("Invio richiesta di connessione al server...\n");
  
  if((connect(sockfd, (struct sockaddr*)&server_address, server_len)) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata..\n");
    exit(EXIT_FAILURE);
  }
  
  while(1)
  {
    printf("Riempio casualmente il vettore..\n");
    
    for(i = 0; i<DIM; i++) D.v[i] = rand() % LIM;
    
    printf("----------------\n");
    for(i = 0; i<DIM; i++) printf("v[%d] = %d \n", i, D.v[i]);
    printf("----------------\n");
    
    printf("Invio dei dati..\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("La somma totale degli elementi del vettore e' %d.\n", D.somma);
    
    printf("Vuoi continuare (0: termina)? ");
    scanf("%d", &val);
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita...\n");
  exit(EXIT_SUCCESS);
}

