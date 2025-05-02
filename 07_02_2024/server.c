/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia una struct contenente due valori float. Il server
fornisce al client il prodotto di tali valori.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct DatiCondivisiS
{
  float a, b, prodotto;
} DatiCondivisiT;

void* ConnectionHandler(void* descriptor);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  struct sockaddr_in server_address, client_address;
  int* sockval;
  char IP[20];
  pthread_t tid;
  
  printf("SERVER : PID = %d.\n", getpid());
  
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
      fprintf(stderr, "OPPS: connessione rifiutata.\n");
      exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &client_address.sin_addr, IP, 20);
    
    printf("CONNESSIONE ACCETTATA..\n");
    printf("INDIRIZZO IP: %s.\n", IP);
    printf("NUMERO PORTA: %d.\n", ntohs(client_address.sin_port));
    
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

void* ConnectionHandler(void* descriptor)
{
  int clientfd = *(int*)descriptor;
  DatiCondivisiT D;
  
  while(read(clientfd, &D, sizeof(D)) > 0)
  {
    D.prodotto = D.a * D.b;
    write(clientfd, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso.\n");
  close(clientfd);
  free(descriptor);
  pthread_exit(NULL);
}

