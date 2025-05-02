/*
Sviluppare un’applicazione client/server, con implementazione multi-thread nel server. Si supponga che il
processo client, una volta connesso, invii al server una struct con due campi: una stringa e un numero intero. Il
client inserisce il campo stringa mentre il server calcola la lunghezza della stringa e invia al client, come risposta,
la struct riempiendo il valore calcolato nel campo intero. Si assuma di non supporre limiti al numero di scambi
tra client e server (lo scambio dati tra ogni client e il server continua fino a quando il client non si disconnette).
Non appena il client si disconnette, il server deve accorgersene automaticamente e chiudere la connessione con
il client.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define STR_LEN 128

typedef struct DatiCondivisiS
{
  char stringa[STR_LEN];
  int lunghezza;
} DatiCondivisiT;


int main(void)
{
  int sockfd, server_len, val, i;
  struct sockaddr_in server_address;
  DatiCondivisiT D;
  
  printf("Processo 'CLIENT' con PID = %d.\n", getpid());
  
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
  
  printf("Invio richiesta di connessione al SERVER...\n");
  
  if(connect(sockfd, (struct sockaddr*)&server_address, server_len) == -1)
  {
    fprintf(stderr, "OPPS: connessione rifiutata.\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Connessione accettata.\n");
  
  while(1)
  {
    printf("Inserisci una stringa: ");
    fgets(D.stringa, STR_LEN, stdin);
    for(i = 0; D.stringa[i] != '\0'; i++) if(D.stringa[i] == '\n') D.stringa[i] = '\0';
    
    printf("Hai inserito la stringa -> %s.\n", D.stringa);
    
    printf("Invio dei dati.\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D)); 
    
    printf("La lunghezza della stringa e' %d.\n", D.lunghezza);
    
    printf("Vuoi continuare? (0 se vuoi terminare) ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;  
  }
  
  printf("Disconnessione eseguita.\n");
  close(sockfd);
  exit(EXIT_SUCCESS);
}
