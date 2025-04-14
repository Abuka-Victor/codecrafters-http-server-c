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

struct http_request requestParser(void *buffer);

#endif