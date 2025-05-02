/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un array di 10 interi riempito in modo casuale
con valori tra 0 e 50. Il server fornisce al client, il valore massimo tra quelli presenti nell’array.
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

#define DIM_ARRAY 10

typedef struct DatiCondivisiS
{
  int v[DIM_ARRAY];
  int massimo;
} DatiCondivisiT;

void * ConnectionHandler(void* clientfd);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  pthread_t tid;
  int * sockval;
  char IndirizzoIP[20];
  struct sockaddr_in server_address, client_address;
  
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
  
  printf("In attesa di richieste di connessione dal client...\n");
  
  while(1)
  {
    client_len = sizeof(client_address);
    if((clientfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len)) == -1)
    {
      fprintf(stderr, "OPPS: Connessione rifiutata..\n");
      exit(EXIT_FAILURE);
    }
    
    inet_ntop(AF_INET, &client_address.sin_addr, IndirizzoIP, 20);
    
    printf("CONNESSIONE ACCETTATA.\n");
    printf("INDIRIZZO IP: %s \n", IndirizzoIP);
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

void * ConnectionHandler(void* clientfd)
{
  int descriptor = *(int*)clientfd;
  DatiCondivisiT D;
  int i;
  
  while(read(descriptor, &D, sizeof(D)) > 0)
  {
    D.massimo = D.v[0];
    for(i = 0; i<DIM_ARRAY; i++) if(D.v[i] > D.massimo) D.massimo = D.v[i];
    write(descriptor, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso.\n");
  close(descriptor);
  free(clientfd);
  pthread_exit(NULL);
}

