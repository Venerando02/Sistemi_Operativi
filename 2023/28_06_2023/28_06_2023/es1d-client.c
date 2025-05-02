/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un vettore di 10 float, riempito casualmente.
Il server fornisce al client il valore medio del contenuto del vettore.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 10
#define LIM 100

typedef struct DatiCondivisiS
{
  float vettore[SIZE];
  float ValoreMedio;
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
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  printf("Richiesta di connessione accettata...\n");
  
  while(1)
  {
    printf("Riempio il vettore casualmente.\n");
    
    for(i = 0; i<SIZE; i++) D.vettore[i] = (rand() % LIM);
    
    printf("----------------\n");
    for(i = 0; i<SIZE; i++) printf("v[%d] = %.2f \n", i, D.vettore[i]);
    printf("----------------\n");
    
    printf("Invio dei dati..\n");
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il valore medio del vettore e' %.2f \n", D.ValoreMedio);
    
    do
    {
      printf("Inserisci 1 se vuoi continuare, 0 per terminare: ");
      scanf("%d", &val);
      while(getchar() != '\n');
    } while((val != 0) && (val != 1));
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}


