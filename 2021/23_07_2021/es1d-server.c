/*
Sviluppare una applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia due valori interi e il server fornisce al client
un valore intero dato dal prodotto dei due valori ricevuti. Si supponga di utilizzare i threads nel Server per la
gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct DatiCondivisiS
{
  int a, b, prodotto;
} DatiCondivisiT;

void* ThreadFunction(void* clientfd);

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
  server_address.sin_port = htons(9000);
  inet_aton("127.0.0.1", &server_address.sin_addr);
  
  server_len = sizeof(server_address);
  
  bind(sockfd, (struct sockaddr*)&server_address, server_len);
  
  listen(sockfd, 5);
  
  printf("In attesa di richieste di connessione del client...\n");
  
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
    printf("INDIRIZZO IP: %s.\n", IndirizzoIP);
    printf("NUMERO DI PORTA: %d.\n", ntohs(client_address.sin_port));
    
    sockval = (int*)malloc(sizeof(int));
    *sockval = clientfd;
    
    if((res = pthread_create(&tid, NULL, ThreadFunction, (void*)sockval)) != 0)
    {
      fprintf(stderr, "Impossibile creare il thread.\n");
      exit(EXIT_FAILURE);
    }
    pthread_detach(tid);
  }
}

void* ThreadFunction(void* clientfd)
{
  int descrittore = *(int*)clientfd;
  DatiCondivisiT D;
  
  while(read(descrittore, &D, sizeof(D)) > 0)
  {
    D.prodotto = D.a * D.b;
    write(descrittore, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso.\n");
  close(descrittore);
  free(clientfd);
  pthread_exit(NULL);
}

