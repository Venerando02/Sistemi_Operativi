/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia una stringa di dimensione scelta a piacere dallo studente, il cui valore viene assegnato mediante
lettura da tastiera. Il server fornisce al client il numero di caratteri contenuti nella stringa.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define SIZE 128

typedef struct DatiCondivisiS
{
  char stringa[SIZE];
  int NumCaratteri;
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
  
  inet_aton("127.0.0.1", &server_address.sin_addr);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9000);
  
  server_len = sizeof(server_address);
  
  printf("Invio richiesta di connessione...\n");
  
  if((connect(sockfd, (struct sockaddr*)&server_address, server_len)) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata...\n");
  
  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(D.stringa, SIZE, stdin);
    for(i = 0; D.stringa[i] != '\0'; i++) if(D.stringa[i] == '\n') D.stringa[i] = '\0';
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("Il numero di caratteri di %s e' %d.\n", D.stringa, D.NumCaratteri);
    
    printf("Vuoi continuare (0 per terminare)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}


