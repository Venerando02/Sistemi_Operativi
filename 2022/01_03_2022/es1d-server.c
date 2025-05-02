/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un array di 10 caratteri inseriti da tastiera. Il
server fornisce al client un valore intero corrispondente al numero di caratteri ‘A’ presenti nel vettore.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Verrà considerato
errore se il numero di scambi dati tra client e dal server è pari ad 1. Si supponga di utilizzare i thread nel Server
per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 10

typedef struct DatiCondivisiS
{
  char v[SIZE];
  int NumLetteraA;
} DatiCondivisiT;

void* ThreadFunction(void* arg);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  struct sockaddr_in server_address, client_address;
  char IndirizzoIP[20];
  int* sockval;
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
    
    inet_ntop(AF_INET, &client_address.sin_addr, IndirizzoIP, 20);
    
    printf("CONNESSIONE ACCETTATA.\n");
    printf("INDIRIZZO IP: %s.\n", IndirizzoIP);
    printf("NUMERO PORTA: %d.\n", ntohs(client_address.sin_port));
    
    sockval = (int*)malloc(sizeof(int));
    *sockval = clientfd;
    
    res = pthread_create(&tid, NULL, ThreadFunction, (void*)sockval);
    if(res != 0)
    {
      fprintf(stderr, "Thread non creato.\n");
      exit(EXIT_FAILURE);    
    }
    pthread_detach(tid);
  }
}

void* ThreadFunction(void* arg)
{
  int descriptor = *(int*)arg;
  int i;
  DatiCondivisiT D;
  
  while(read(descriptor, &D, sizeof(D)) > 0)
  {
    D.NumLetteraA = 0;
    for(i = 0; i<SIZE; i++)
    {
      if((D.v[i] == 'A') || (D.v[i] == 'a')) D.NumLetteraA++;
    }
    write(descriptor, &D, sizeof(D));
  }
  
  printf("Il client si e' disconnesso...\n");
  close(descriptor);
  free(arg);
  pthread_exit(NULL);
}
