/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia una stringa di dimensione scelta a piacere dallo studente, il cui valore viene assegnato mediante
lettura da tastiera. Il server fornisce al client la stringa capovolta. Esempio: se il client invia “ciao”, il server
dovrà rispondere con “oaic”.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define SSIZE 64

void* ConnectionHandler(void* clientfd);

int main(void)
{
  int sockfd, server_len, clientfd, client_len, res;
  pthread_t tid;
  int* sockval;
  struct sockaddr_in server_address, client_address;
  char IndirizzoIP[20];
  
  printf("SERVER - PID = %d.\n", getpid());
  
  sockfd = socket(AF_INET, SOCK_STREAM , 0);
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
  
  printf("In attesa di richieste di connessione dal client.\n");
  
  while(1)
  {
    client_len = sizeof(client_address);
    if((clientfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len)) == -1)
    {
      fprintf(stderr, "OPPS: connessione non accettata.\n");
      exit(EXIT_FAILURE);
    } 
    inet_ntop(AF_INET, &client_address.sin_addr, IndirizzoIP, 20);
    
    printf("CONNESSIONE ACCETTATA.\n");
    printf("INDIRIZZO IP: %s.\n", IndirizzoIP);
    printf("NUMERO DI PORTA: %d.\n", ntohs(client_address.sin_port));
    
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
  char * StringaCapovolta = NULL;
  char StringaLetta[SSIZE];
  int i, j;
  
  while(read(descrittore, StringaLetta, SSIZE) > 0)
  {
    StringaCapovolta = (char*)malloc(sizeof(char)*SSIZE);
    j = 0;
    for(i = (strlen(StringaLetta)-1); i >= 0; i--)
    {
      StringaCapovolta[j++] = StringaLetta[i];
    }
    StringaCapovolta[j] = '\0';
    write(descrittore, StringaCapovolta, SSIZE);
  }
  
  printf("Il client si e' disconnesso.\n");
  close(descrittore);
  free(clientfd);
  pthread_exit(NULL);
}

