/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un vettore di 10 float, riempito casualmente.
Il server fornisce al client il valore medio del contenuto del vettore.
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

#define SIZE 10
#define LIM 100

typedef struct DatiCondivisiS
{
  float vet[SIZE];
  float media;
} DatiCondivisiT;


int main(void)
{
  int sockfd, server_len, val, i;
  DatiCondivisiT D;
  struct sockaddr_in server_address;
  
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
  
  printf("Invio richiesta di connessione....\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata...\n");
  
  while(1)
  {
    for(i = 0; i<SIZE; i++) D.vet[i] = rand() % LIM;
    
    printf("------------\n");
    for(i = 0; i<SIZE; i++) printf("v[%d] = %.2f \n", i, D.vet[i]);
    printf("------------\n");
    
    printf("Invio un vettore di 10 float.\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("La media del vettore e' %.2f.\n", D.media);
    printf("Vuoi continuare (0 per terminare)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}

