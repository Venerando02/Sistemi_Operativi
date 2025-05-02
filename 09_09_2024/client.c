/*
Sviluppare un programma client e un programma server caratterizzati dal seguente scambio di dati. Il processo
client invia una stringa di dimensione scelta a piacere dallo studente, il cui valore viene assegnato mediante
lettura da tastiera. Il server fornisce al client la stringa capovolta. Esempio: se il client invia “ciao”, il server
dovrà rispondere con “oaic”.
Lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette. Si supponga
di utilizzare l’implementazione multi-thread nel Server per la gestione di più client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define STR_LEN 128

int main(void)
{
  int sockfd, server_len, val, i;
  struct sockaddr_in server_address;
  char StringaInput[STR_LEN];
  
  printf("CLIENT - PID = %d.\n", getpid());
  
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
  
  printf("Invio richiesta di connessione al server...\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("La connessione e' stata accettata.\n");
  
  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(StringaInput, STR_LEN, stdin);
    for(i = 0; StringaInput[i] != '\0'; i++) if(StringaInput[i] == '\n') StringaInput[i] = '\0';
    
    printf("Hai inviato la stringa = %s.\n", StringaInput);
    
    write(sockfd, StringaInput, STR_LEN);
    read(sockfd, StringaInput, STR_LEN);
    
    printf("Il contrario e' la stringa = %s.\n", StringaInput);
    
    printf("Vuoi continuare (0: per uscire dal ciclo)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}


