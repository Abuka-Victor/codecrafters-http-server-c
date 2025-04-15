#include "header.h"

void respond_client(int client, fd_set *all_clients) {
  char buffer[30000];
  char response[1024];
  struct http_request request;
  size_t bytes_read;

  memset(buffer, 0, sizeof(buffer));

  bytes_read = read(client, buffer, sizeof(buffer));
  printf("Bytes read: %zu\n", bytes_read);
  if (bytes_read <= 0) {
    perror("read");
    close(client);
    FD_CLR(client, all_clients);
    return;
  }
  
  char *ok_response = "HTTP/1.1 200 OK\r\n\r\n";
  char *not_found_response = "HTTP/1.1 404 Not Found\r\n\r\n";

  buffer[bytes_read] = '\0'; // Null-terminate the buffer
  request = requestParser(buffer);
  char *path = strtok(request.path, "/");

  memset(response, 0, sizeof(response));

  if(path == NULL){ // path: /
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    write(client, response, strlen(response));
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
}