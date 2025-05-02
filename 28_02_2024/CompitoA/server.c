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
#include <pthread.h>
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

void* ConnectionHandler(void* clientfd);

int main(void)
{
  int sockfd, server_len, client_len, clientfd, res;
  pthread_t tid;
  struct sockaddr_in server_address, client_address;
  int * sockval;
  char IP[20];
  
  
  printf("SERVER PID = %d.\n", getpid());
  
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
  
  bind(sockfd, (struct sockaddr*)&server_address, server_len);
  
  listen(sockfd, 5);
  
  printf("In attesa di richieste di connessione...\n");
  
  while(1)
  {
    client_len = sizeof(client_address);
    if((clientfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len)) == -1)
    {
      fprintf(stderr, "OPPS: connessione non riuscita.\n");
      exit(EXIT_FAILURE);
    }
    
    inet_ntop(AF_INET, &client_address.sin_addr, IP, 20);
    printf("CONNESSIONE RIUSCITA.\n");
    printf("INDIRIZZO IP: %s \n", IP);
    printf("NUMERO PORTA: %d \n", ntohs(client_address.sin_port));
    
    sockval = (int*)malloc(sizeof(int));
    *sockval = clientfd;
    
    res = pthread_create(&tid, NULL, ConnectionHandler, (void*)sockval);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
    pthread_detach(tid);
  }
}

void* ConnectionHandler(void* clientfd)
{
  int descriptor = *(int*)clientfd;
  DatiCondivisiT D;
  int somma, i;
  
  while(read(descriptor, &D, sizeof(D)) > 0)
  {
    somma = 0;
    D.media = 0;
    for(i = 0; i<N; i++) somma += D.v[i];
    D.media = (float)somma / N;
    write(descriptor, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso...\n");
  close(descriptor);
  free(clientfd);
  pthread_exit(NULL);
}

