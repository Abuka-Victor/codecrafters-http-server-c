#include "header.h"

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	int server_fd, fd_max, status;
  struct timeval tv;

  fd_set all_clients;
  fd_set read_fds;
  FD_ZERO(&all_clients);
  FD_ZERO(&read_fds);
	
	server_fd = createServer();
  if(server_fd == -1){
    return 1;
  }
  FD_SET(server_fd, &all_clients);
  fd_max = server_fd;
  

	int connection_backlog = 10;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	printf("Server is listening on port 4221\n");
  
  while(1){
    read_fds = all_clients;
    tv.tv_sec = 5; // Timeout of 5 seconds
    tv.tv_usec = 0;

    status = select(fd_max + 1, &read_fds, NULL, NULL, &tv);

    if (status < 0) {
      perror("select");
      return 1;
    } else if(status == 0) {
      printf("Server Waiting...\n");
    }

    for (int i = 0; i <= fd_max; i++) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == server_fd) {
          setup_new_client(server_fd, &all_clients, &fd_max);
        } else {
          respond_client(i, &all_clients);
        }
      }
    }
  }

  close(server_fd);
	return 0;
}