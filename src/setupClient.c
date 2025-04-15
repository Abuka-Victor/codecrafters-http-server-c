#include "header.h"


void setup_new_client(int server, fd_set *clients, int *fd_max){
  // Accept a new client connection
  int client;
  socklen_t client_addr_len;
	struct sockaddr_in client_addr;
  client_addr_len = sizeof(client_addr);

  client = accept(server, (struct sockaddr *) &client_addr, &client_addr_len);
  if (client < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
  // Add the new client to the set of clients
  FD_SET(client, clients);
  if (client > *fd_max) {
    *fd_max = client;
  }
  
  printf("Client connected: %d\n", client);

  // Give first response to the client
  respond_client(client, clients);
}