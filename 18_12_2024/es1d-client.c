/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia un array di interi di dimensione scelta a piacere dallo studente, riempito mediante letture da tastiera.
Il server fornisce al client il numero di valori dispari presenti nell’array.
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
#include <pthread.h>

#define SIZE 10
#define LIM 100

typedef struct DatiCondivisiS
{
  int vettore[SIZE];
  int NumValoriDispari;
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
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata.\n");
  
  while(1)
  {
    printf("Riempio il vettore in maniera casuale.\n");
    
    for(i = 0; i<SIZE; i++) D.vettore[i] = rand() % LIM;
    
    printf("---------------------\n");
    for(i = 0; i<SIZE; i++) printf("v[%d] = %d \n", i, D.vettore[i]);
    printf("---------------------\n");
    
    printf("Invio i dati.\n");
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di elementi dispari e' %d.\n", D.NumValoriDispari);
    
    printf("\nVuoi continuare (0 per terminare)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione effettuata.\n");
  exit(EXIT_SUCCESS);
}

