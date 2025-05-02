/*
Sviluppare un’applicazione client/server, con implementazione multi-thread nel server. Si supponga che il
processo client, una volta connesso, invii al server un array di 10 interi generati casualmente (valori compresi tra
0 e 50). Il server fornisce al client un valore intero corrispondente al conteggio dei numeri maggiori o uguali a
25 presenti nell’array. Si assuma di non supporre limiti al numero di scambi tra client e server (lo scambio dati
tra ogni client e il server continua fino a quando il client non si disconnette). Non appena il client si disconnette,
il server deve accorgersene automaticamente e chiudere la connessione con il client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DIM 10
#define LIM 51

typedef struct DatiCondivisiS
{
  int v[DIM];
  int NumMaggiori;
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
  inet_aton("127.0.0.1", &server_address.sin_addr);
  server_address.sin_port = htons(9000);
  
  server_len = sizeof(server_address);
  
  printf("Invio richiesta di connessione...\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  printf("Connessione accettata.\n");
  
  while(1)
  {
    printf("Riempio il vettore.\n");
    for(i = 0; i<DIM; i++) D.v[i] = rand() % LIM;
    printf("Visualizzo il vettore.\n");
    printf("-------------\n");
    for(i = 0; i<DIM; i++) printf("v[%d] = %d.\n", i, D.v[i]);
    printf("-------------\n");
    
    printf("Invio dati.\n");
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di elementi maggiore di 25 e' %d.\n", D.NumMaggiori);
    printf("Vuoi continuare (0 per terminare)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}

