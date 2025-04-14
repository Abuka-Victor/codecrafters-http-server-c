#include "header.h"

struct http_request requestParser(void *buffer)
{
    struct http_request request;
    char *request_line = strtok(buffer, "\r\n");
    if (request_line != NULL) {
        sscanf(request_line, "%s %s %s", request.method, request.path, request.version);
    }

    request_line = strtok(NULL, "\r\n");
    while (request_line != NULL && strlen(request_line) > 0) {
        if (strstr(request_line, "Host:") == request_line) {
            sscanf(request_line, "Host: %s", request.header.host);
        } else if (strstr(request_line, "User-Agent:") == request_line) {
            sscanf(request_line, "User-Agent: %s", request.header.user_agent);
        } else if (strstr(request_line, "Accept:") == request_line) {
            sscanf(request_line, "Accept: %s", request.header.accept);
        }
        request_line = strtok(NULL, "\r\n");
    }

    return request;
}
