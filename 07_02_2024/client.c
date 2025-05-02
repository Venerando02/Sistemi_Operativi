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

typedef struct DatiCondivisiS
{
  float a, b, prodotto;
} DatiCondivisiT;

int main(void)
{
  int sockfd, server_len, val;
  struct sockaddr_in server_address;
  DatiCondivisiT D;
  
  printf("CLIENT : PID = %d.\n", getpid());
  
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
  
  printf("Invio richiesta di connessione...\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata..\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata...\n");
  
  while(1)
  {
    printf("Inserisci il primo valore: ");
    scanf("%f", &D.a);
    while(getchar() != '\n');
    printf("Inserisci il secondo valore: ");
    scanf("%f", &D.b);
    while(getchar() != '\n');
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il prodotto tra %.2f e %.2f e' %.2f.\n", D.a, D.b, D.prodotto);
    
    printf("Vuoi continuare (0: no, 1: si)? ");
    scanf("%d", &val);
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione avvenuta correttamente.\n");
  exit(EXIT_SUCCESS);
}
  
