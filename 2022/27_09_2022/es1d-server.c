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
#include <pthread.h>

#define DIM 10

typedef struct DatiCondivisiS
{
  int v[DIM];
  int NumMaggiori;
} DatiCondivisiT;

void* ThreadFunction(void* clientfd);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  struct sockaddr_in server_address, client_address;
  int* sockval;
  char IndirizzoIP[20];
  pthread_t tid;
  
  printf("SERVER - PID = %d.\n", getpid());
  
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
  
  bind(sockfd, (struct sockaddr*)&server_address, server_len);
  
  listen(sockfd, 5);
  
  printf("In attesa di richieste di connessione...\n");
  
  while(1)
  {
    client_len = sizeof(client_address);
    if((clientfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len)) == -1)
    {
      fprintf(stderr, "OPPS: connessione rifiutata.\n");
      exit(EXIT_FAILURE);
    }
    
    inet_ntop(AF_INET, &client_address, IndirizzoIP, 20);
    printf("CONNESSIONE ACCETTATA.\n");
    printf("INDIRIZZO IP: %s.\n", IndirizzoIP);
    printf("NUMERO DI PORTA: %d.\n", ntohs(client_address.sin_port));
    
    sockval = (int*)malloc(sizeof(int));
    *sockval = clientfd;
    
    res = pthread_create(&tid, NULL, ThreadFunction, (void*)sockval);
    if(res != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
    pthread_detach(tid);
  }
}

void* ThreadFunction(void* clientfd)
{
  int descriptor = *(int*)clientfd;
  DatiCondivisiT D;
  int i;
  
  while(read(descriptor, &D, sizeof(D)) > 0)
  {
    D.NumMaggiori = 0;
    for(i = 0; i < DIM; i++)
    {
      if(D.v[i] > 25) D.NumMaggiori++;
    }
    write(descriptor, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso.\n");
  close(descriptor);
  free(clientfd);
  pthread_exit(NULL);
}
