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

typedef struct DatiCondivisiS
{
  int vettore[SIZE];
  int NumValoriDispari;
} DatiCondivisiT;

void* ConnectionHandler(void* clientfd);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  struct sockaddr_in server_address, client_address;
  pthread_t tid;
  int* sockval;
  char IndirizzoIP[20];
  
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
    inet_ntop(AF_INET, &client_address.sin_addr, IndirizzoIP, 20);
    printf("CONNESSIONE ACCETTATA.\n");
    printf("INDIRIZZO IP: %s \n", IndirizzoIP);
    printf("NUMERO DI PORTA: %d \n", ntohs(client_address.sin_port));
    
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
  int descrittore = *(int*)clientfd;
  int i;
  DatiCondivisiT D;
  
  while(read(descrittore, &D, sizeof(D)) > 0)
  {
    D.NumValoriDispari = 0;
    for(i = 0; i<SIZE; i++)
    {
      if(D.vettore[i] % 2 != 0) D.NumValoriDispari ++;
    }
    write(descrittore, &D, sizeof(D));
  }
  printf("Il client si e' disconnesso.\n");
  
  close(descrittore);
  free(clientfd);
  
  pthread_exit(NULL);
}

