/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia una struct contenente un vettore di interi di dimensione scelta a piacere dallo studente riempito
casualmente (si supponga che i valori siano positivi e inferiori a 10). Il server fornisce al client la media di tali
valori.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define N 10
#define LIM 10

typedef struct DatiCondivisiS
{
  int v[N];
  float media;
} DatiCondivisiT;

int main(void)
{
  int sockfd, server_len, val, i;
  struct sockaddr_in server_address;
  DatiCondivisiT D;
  
  printf("CLIENT PID = %d.\n", getpid());
  
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
  
  printf("Invio richiesta di connessione al server..\n");
  
  if((connect(sockfd, (struct sockaddr*)&server_address, server_len)) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata..\n");
    exit(EXIT_FAILURE);
  }
  
  while(1)
  {
    printf("Riempio il vettore casualmente.\n");
    
    for(i = 0 ; i<N; i++) D.v[i] = rand() % LIM;
    
    printf("--------------\n");
    for(i = 0 ; i<N; i++) printf("v[%d] = %d \n", i, D.v[i]);
    printf("--------------\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("La media e' %.2f.\n", D.media);
    
    printf("Vuoi continuare? (0: per terminare) ");
    scanf("%d", &val);
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione riuscita.\n");
  exit(EXIT_SUCCESS);
}  
