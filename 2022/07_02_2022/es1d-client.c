/*
Sviluppare un’applicazione client/server, realizzando due programmi diversi (uno client e l’altro server)
caratterizzati dal seguente scambio di dati. Il processo client invia un vettore (la cui dimensione è a scelta dello
studente) di valori interi (riempito casualmente). Il server fornisce al client un valore float corrispondente alla
media degli elementi del vettore. Lo scambio dati tra ogni client e il server continua fino a quando il client non
si disconnette. Si supponga di utilizzare i thread nel Server per la gestione di più client.
Nota: per realizzare lo scambio dati tra client e server, lo studente potrebbe considerare uno struct contenente
due campi: il vettore di interi e il valore float corrispondete alla media.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 10
#define LIM 100

typedef struct DatiCondivisiS
{
  int vettore[SIZE];
  float media;
} DatiCondivisiT;

void* ConnectionHandler(void* clientfd);

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
    printf("Riempio casualmente il vettore.\n");
    
    for(i = 0; i<SIZE; i++) D.vettore[i] = rand() % LIM;
    
    printf("---------\n");
    for(i = 0; i<SIZE; i++) printf("v[%d] = %d.\n", i, D.vettore[i]);
    printf("---------\n\n");
    printf("Invio i dati.\n");
    
    write(sockfd, &D, sizeof(D));
    read(sockfd, &D, sizeof(D));
    
    printf("la media del vettore e' %.2f.\n\n", D.media);
    
    printf("Vuoi continuare (digita 0 per concludere)? ");
    scanf("%d", &val);
    while(getchar() != '\n');
    if(val == 0) break;
  }
  
  close(sockfd);
  printf("Disconnessione eseguita.\n");
  exit(EXIT_SUCCESS);
}

