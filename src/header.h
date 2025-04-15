#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>

struct http_header
{
  char host[256];
  char user_agent[500];
  char accept[500];
};

struct http_request
{
  char method[8];
  char path[256];
  char version[16];
  struct http_header header;
};

int createServer(void);
void setup_new_client(int server, fd_set *clients, int *fd_max);
void respond_client(int client, fd_set *all_clients);
struct http_request requestParser(void *buffer);

#endif