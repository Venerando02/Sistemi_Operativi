/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un vettore di 20 interi, riempito casualmente.
Il server fornisce al client il numero di valori dispari presenti nel vettore.
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
#include <pthread.h>

#define SIZE 20

typedef struct DatiCondivisiS
{
  int vettore[SIZE];
  int NumValoriDispari;
} DatiCondivisiT;

void* ConnectionHandler(void* clientfd);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  pthread_t tid;
  struct sockaddr_in server_address, client_address;
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
  server_address.sin_port = htons(9000);
  inet_aton("127.0.0.1", &server_address.sin_addr);
  
  server_len = sizeof(server_address);
  
  bind(sockfd, (struct sockaddr*)&server_address, server_len);
  
  listen(sockfd, 5);
  
  printf("In attesa di richieste di connessione..\n");
  
  while(1)
  {
    client_len = sizeof(client_address);
    if((clientfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len)) == -1)
    {
      fprintf(stderr, "OPPS: connessione rifiutata.\n");
      exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &client_address.sin_addr, IndirizzoIP, 20);
    
    printf("CONNESSIONE ACCETTATA. \n");
    printf("NUMERO DI PORTA: %d. \n", ntohs(client_address.sin_port));
    printf("INDIRIZZO IP: %s. \n", IndirizzoIP);
    
    sockval = (int*)malloc(sizeof(int));
    *sockval = clientfd;
    
    if((res = pthread_create(&tid, NULL, ConnectionHandler, (void*)sockval)) != 0)
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
  int i;
  
  while(read(descriptor, &D, sizeof(D)) > 0)
  {
     D.NumValoriDispari = 0;
     for(i = 0; i<SIZE; i++) if(D.vettore[i] % 2 != 0) D.NumValoriDispari++;
     write(descriptor, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso.\n");
  close(descriptor);
  free(clientfd);
  pthread_exit(NULL);
}
