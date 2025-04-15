#include "header.h"

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	int server_fd;
  socklen_t client_addr_len;
	struct sockaddr_in client_addr;
  struct http_request request;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}
	
	// Since the tester restarts your program quite often, setting SO_REUSEADDR
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}
	
	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									};
	
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}
	
	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	
	client_addr_len = sizeof(client_addr);
  
  char *ok_response = "HTTP/1.1 200 OK\r\n\r\n";
  // char ok_response[10000] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n";
  char *not_found_response = "HTTP/1.1 404 Not Found\r\n\r\n";
  int client;
  char buffer[30000];
  char response[1024];
  
  while(1){
    memset(response, 0, sizeof(response));
    printf("Waiting for a client to connect...\n");
    client = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    printf("Client connected\n");
    read(client, buffer, sizeof(buffer));
    request = requestParser(buffer);
    char *path = strtok(request.path, "/");

    if(strcmp(request.path, "/") == 0){ // path: /
      write(client, ok_response, strlen(ok_response));
    }else if(strcmp(path, "echo") == 0){ // path: /echo
      path = strtok(NULL, "/");
      int path_length = strlen(path);
      sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s", path_length, path);
      write(client, response, strlen(response));
    }else if(strcmp(path, "user-agent") == 0){ // path: /user-agent
      sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(request.header.user_agent), request.header.user_agent);
      write(client, response, strlen(response));
    } else { // path: /not-found or any other path at all
      write(client, not_found_response, strlen(not_found_response));
    }

    printf("Response sent\n");
    close(client);
  }

  close(server_fd);
	return 0;
}
